/*Basic collision detection between multiple planes represented as quads
 * Chapter 3 from "Foundations of Physically Based Modeling & Animation
 */

#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>
#include "movingcollision.hpp"
#include "AssignmentPaths.hpp"

#include <iostream>

namespace personalProject {
    MovingCollision::MovingCollision() {
        //colour
        math::Vector white(248.0f/255.0f, 255.0f/255.0f, 178.0f/255.0f);
        math::Vector yellow(255.0f/255.0f,200.0f/255.0f,23.0f/255.0f);
        math::Vector blue(11.0f/255.0f,23.0f/255.0f,185.0f/255.0f);
        math::Vector green(4.0f/255.0f,95.0f/255.0f,32.0f/255.0f);
        //ball
        std::shared_ptr<ColourSphere> cue(new ColourSphere(1, 1, math::Vector(-7,0,0), math::Vector(2.5,0,1.3) * 5.0f, math::Vector(0,0,0), white));
        std::shared_ptr<ColourSphere> ball1(new ColourSphere(1, 1, math::Vector(7,0,2), math::Vector(-5,0,0) * 5.0f, math::Vector(0,0,0), blue));
        std::shared_ptr<ColourSphere> ball2(new ColourSphere(1, 1, math::Vector(3,0,4), math::Vector(1,0,5) * 5.0f, math::Vector(0,0,0), green));
        std::shared_ptr<ColourSphere> ball3(new ColourSphere(1, 1, math::Vector(1,0,4), math::Vector(3,0,2) * 5.0f, math::Vector(0,0,0), yellow));
        //quads
        std::shared_ptr<Quad> quad(new Quad(math::Vector(0,0,-10), math::Vector(0,0,1), math::Vector(1,0,-10)));
        std::shared_ptr<Quad> quad2(new Quad(math::Vector(0,0,10), math::Vector(0,0,-1), math::Vector(1,0,10)));
        std::shared_ptr<Quad> quad3(new Quad(math::Vector(-10,0,0), math::Vector(1,0,0), math::Vector(-10,0,1)));
        std::shared_ptr<Quad> quad4(new Quad(math::Vector(10,0,0), math::Vector(-1,0,0), math::Vector(10,0,1)));

        mSpheres.push_back(cue);
        mSpheres.push_back(ball1);
        mSpheres.push_back(ball2);
        mSpheres.push_back(ball3);
        mQuads.push_back(quad);
        mQuads.push_back(quad2);
        mQuads.push_back(quad3);
        mQuads.push_back(quad4);

        mObjects.push_back(quad);
        mObjects.push_back(quad2);
        mObjects.push_back(quad3);
        mObjects.push_back(quad4);
        mObjects.push_back(cue);
        mObjects.push_back(ball1);
        mObjects.push_back(ball2);
        mObjects.push_back(ball3);
    }

    void MovingCollision::renderScene(){
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


        if (mWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (auto& sphere : mSpheres) {
            sphere->renderGeometry(mProjection, mView);
        }
        if (mWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
        ImGui::Checkbox("Wireframe Mode", &mWireframe);
        ImGui::End();

        ImGui::Render();
    }

    void MovingCollision::updateScene(double time){
        ModellingScene::updateScene(time);

        for(auto& sphere : mSpheres){
            sphere->updateGeometry(mTime); //spin
            float fLeft = 1.0f;
            while(fLeft > 0){
                float minDistance = 999999.0f;
                std::shared_ptr<ColObject> minObj;
                for(auto& obj : mObjects){
                    if (obj == sphere)
                        continue;

                    float dist = obj->getDistance(sphere, mTime.deltaTime);
                    if(dist < minDistance){ //find minDistance
                        minDistance = dist;
                        minObj = obj;
                    }
                }


                if(minDistance < 1.0f){
                    //integrate for sphere
                    sphere->setPosition(sphere->getPosition() + sphere->getVelocity() * mTime.deltaTime * minDistance);
                    fLeft -= minDistance;

                    if (std::shared_ptr<ColourSphere> otherSphere = std::dynamic_pointer_cast<ColourSphere>(minObj)) {

                        //get, set new velocity
                        math::Vector vel1 = sphere->getVelocity();
                        math::Vector vel2 = otherSphere->getVelocity();
                        float m1 = sphere->getMass();
                        float m2 = otherSphere->getMass();


                        math::Vector newVel1 = (vel1 * (m1-m2)+(2* m2*vel2))/(m1+m2);
                        math::Vector newVel2 = (vel2 * (m2-m1)+(2*m1*vel1))/(m1+m2);

                        sphere->setVelocity(newVel1);
                        otherSphere->setVelocity(newVel2);

                    } else {

                        math::Vector normal = minObj->getNormal(sphere);
                        //std::cout<< "normal: "<<glm::length(normal)<<std::endl;
                        math::Vector vel = sphere->getVelocity();
                        sphere->setVelocity(vel - 2 * dot(vel, normal) * normal); //reflection
                    }

                }else{
                    sphere->setPosition(sphere->getPosition() + sphere->getVelocity() * mTime.deltaTime * fLeft);
                    fLeft = 0;
                }
            }
            sphere->setVelocity(sphere->getVelocity() * 0.999f); //add slow down after 0.989
        }
    }
}
