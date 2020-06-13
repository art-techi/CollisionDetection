/*Basic collision detection between multiple planes represented as quads
 * Chapter 3 from "Foundations of Physically Based Modeling & Animation
 */

#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>
#include "staticcollision.hpp"
#include "AssignmentPaths.hpp"

#include <iostream>

namespace personalProject {
    StaticCollision::StaticCollision() {
        //colour
        math::Vector white(248.0f/255.0f, 255.0f/255.0f, 178.0f/255.0f);

        //ball
        std::shared_ptr<ColourSphere> cue(new ColourSphere(1, 1, math::Vector(1,0,0), math::Vector(1,0,-2) * 5.0f, math::Vector(0,0,0), white));

        //quads
        std::shared_ptr<Quad> quad(new Quad(math::Vector(0,0,-5), math::Vector(0,0,1), math::Vector(1,0,-5)));
        std::shared_ptr<Quad> quad2(new Quad(math::Vector(0,0,5), math::Vector(0,0,-1), math::Vector(1,0,5)));
        std::shared_ptr<Quad> quad3(new Quad(math::Vector(-5,0,0), math::Vector(1,0,0), math::Vector(-5,0,1)));
        std::shared_ptr<Quad> quad4(new Quad(math::Vector(5,0,0), math::Vector(-1,0,0), math::Vector(5,0,1)));

        mSpheres.push_back(cue);
        mQuads.push_back(quad);
        mQuads.push_back(quad2);
        mQuads.push_back(quad3);
        mQuads.push_back(quad4);
    }

    void StaticCollision::renderScene(){
        atlas::utils::Gui::getInstance().newFrame();
        float col = 100.0f/255.0f;
        glClearColor(col, col, col, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mProjection=glm::perspective(glm::radians(mCamera.getCameraFOV()),
                                       (float) mWidth/mHeight, 1.0f, 1000.0f);

        mUniformMatrixBuffer.bindBuffer();
        mUniformMatrixBuffer.bufferSubData(0, sizeof(atlas::math::Matrix4),
                                           &mProjection[0][0]);
        mView=mCamera.getCameraMatrix();
        mUniformMatrixBuffer.bufferSubData(sizeof(atlas::math::Matrix4),
                                           sizeof(atlas::math::Matrix4), &mView[0][0]);
        mUniformMatrixBuffer.bindBufferBase(0);
        mUniformMatrixBuffer.unBindBuffer();

        if(mShowGrid){
            mGrid.renderGeometry(mProjection, mView);
        }

        for (auto& sphere : mSpheres) {
            sphere->renderGeometry(mProjection, mView);
        }

        for (auto& quad : mQuads){
            quad->renderGeometry(mProjection, mView);
        }

        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Global HUD");
        ImGui::Checkbox("Show grid", &mShowGrid);
        if(ImGui::Button("Reset Camera")){
            mCamera.resetCamera();
        }

        ImGui::Text("%.3f ms/frame", 1000.0f/ImGui::GetIO().Framerate);
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
    }

    void StaticCollision::updateScene(double time){
        ModellingScene::updateScene(time);

        for(auto& sphere : mSpheres){
            float fLeft = doPlaneCollisions(sphere, 1.0f);

            // integrate for the rest of f
            sphere->setPosition(sphere->getPosition() + sphere->getVelocity() * mTime.deltaTime * fLeft);

            //std::cout<<sphere->getPosition().x<<" "<<sphere->getPosition().y<<" "<<sphere->getPosition().z<<std::endl;
        }
    }

    float StaticCollision::doPlaneCollisions(std::shared_ptr<ColourSphere> sphere, float fLeft){
        if (fLeft <= 0) { //base case
            return 0;
        }

        std::map<float, std::shared_ptr<Quad>> sorted;
        for(auto& q : mQuads){
            math::Vector normal = q->getNormal(sphere); // normal to the plane
            math::Vector p = q->getP(); // arbitrary point on the plane
            float dT = mTime.deltaTime;
            float rad = sphere->getRad();
            math::Vector vel = sphere->getVelocity();

            math::Vector curr = sphere->getPosition();
            float dBall = glm::dot((curr - p), normal); //d = (x-p).n from slides
            float d0 = dBall + (dBall > 0 ? -rad : rad); //+/- radius

            math::Vector next = curr + vel * dT;
            float dBallNext = glm::dot((next - p), normal);
            float d1 = dBallNext + (dBallNext > 0 ? -rad : rad);

            if (d0 * d1 < 0) { // collision will occur between this time step and next
                float f = d0 / (d0 - d1);
                sorted[f] = q;
            }
        }

        if (sorted.size() == 0) {
            return fLeft;
        }

        std::map<float, std::shared_ptr<Quad>>::iterator q = sorted.begin();
        float fUsed = std::min(q->first, fLeft);
        std::shared_ptr<Quad> quad = q->second;

        // integrate until first quad in map
        sphere->setPosition(sphere->getPosition() + sphere->getVelocity() * mTime.deltaTime * fUsed);

        //reflect velocity vector
        math::Vector normal = quad->getNormal(sphere);
        math::Vector vel = sphere->getVelocity();
        sphere->setVelocity(vel - 2 * dot(vel, normal) * normal); //reflection

        return doPlaneCollisions(sphere, fLeft - fUsed);
    }
}
