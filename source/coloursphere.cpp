#include "AssignmentPaths.hpp"
#include "coloursphere.hpp"
#include <atlas/core/ImGUI.hpp>
#include <iostream>
#include <math.h>
#include <stb/stb_image.h>
#include <vector>
#include <stdio.h>

# define M_PI           3.14159265358979323846  /* pi */

//https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c

namespace personalProject
{
    ColourSphere::ColourSphere(float mass, float rad, math::Vector position, math::Vector velocity, math::Vector force, math::Vector colour):
        mMass(mass),
        mRad(rad),
        mPosition(position),
        mVelocity(velocity),
        mForce(force),
        mColour(colour),
        mBuff(GL_ARRAY_BUFFER),
        mIndexBuff(GL_ELEMENT_ARRAY_BUFFER),
        mNormals(GL_ARRAY_BUFFER),
        mUV(GL_ARRAY_BUFFER)

    {
        //isDynamic = true;
        /* Spherical coordinates
         * x=rcos(theta)sin(phi)
         * y=rsin(theta)sin(phi)
         * z=rcos(phi)
         */

        mTheta=glm::radians(45.0f);
        mThetaDot = 0;

        std::vector<math::Vector> vertices;
        std::vector<math::Vector> normals;
        std::vector<math::Vector2> uvs;


        float R=mRad;

        //IMPORTANTE: https://gamedev.stackexchange.com/questions/16585/how-do-you-programmatically-generate-a-sphere
        int numStacks=20;
        int numSlices=30;
        for(int i=0; i<numStacks; i++){
            float theta1=((float) i/numStacks)*M_PI;
            float theta2=((float) (i+1)/numStacks)*M_PI;
            for(int j=0; j<numSlices; j++){
                float phi1=((float)j/numSlices)*2*M_PI;
                float phi2=((float)(j+1)/numSlices)*2*M_PI;

                /*General: x = rad * sin(theta) * sin(phi)
                           y = rad * cos(theta)
                           z = rad * sin(theta) * cos(phi)
                */
                math::Vector vertex1= math::Vector(R*sin(theta1)*sin(phi1),R*cos(theta1), R*sin(theta1)*cos(phi1));
                math::Vector vertex2= math::Vector(R*sin(theta1)*sin(phi2),R*cos(theta1), R*sin(theta1)*cos(phi2));
                math::Vector vertex3= math::Vector(R*sin(theta2)*sin(phi2),R*cos(theta2), R*sin(theta2)*cos(phi2));
                math::Vector vertex4= math::Vector(R*sin(theta2)*sin(phi1),R*cos(theta2), R*sin(theta2)*cos(phi1));

                //x and z componenets of the vertices
                math::Vector2 uv1(vertex1.x, vertex1.z);
                math::Vector2 uv2(vertex2.x, vertex2.z);
                math::Vector2 uv3(vertex3.x, vertex3.z);
                math::Vector2 uv4(vertex4.x, vertex4.z);

                if(i==0){
                    vertices.push_back(vertex1);
                    vertices.push_back(vertex3);
                    vertices.push_back(vertex4);

                    normals.push_back(vertex1);
                    normals.push_back(vertex3);
                    normals.push_back(vertex4);

                    uvs.push_back(uv1);
                    uvs.push_back(uv3);
                    uvs.push_back(uv4);

                }else if((i+1)==numStacks){
                    vertices.push_back(vertex3);
                    vertices.push_back(vertex1);
                    vertices.push_back(vertex2);

                    normals.push_back(vertex3);
                    normals.push_back(vertex1);
                    normals.push_back(vertex2);

                    uvs.push_back(uv3);
                    uvs.push_back(uv1);
                    uvs.push_back(uv2);

                }else{
                    vertices.push_back(vertex1);
                    vertices.push_back(vertex2);
                    vertices.push_back(vertex4);
                    vertices.push_back(vertex2);
                    vertices.push_back(vertex3);
                    vertices.push_back(vertex4);

                    normals.push_back(vertex1);
                    normals.push_back(vertex2);
                    normals.push_back(vertex4);
                    normals.push_back(vertex2);
                    normals.push_back(vertex3);
                    normals.push_back(vertex4);

                    uvs.push_back(uv1);
                    uvs.push_back(uv2);
                    uvs.push_back(uv4);
                    uvs.push_back(uv2);
                    uvs.push_back(uv3);
                    uvs.push_back(uv4);
                }
            }
        }

        mNumVertices=vertices.size();

        mVAO.bindVertexArray();
        mBuff.bindBuffer();
        mBuff.bufferData(gl::size<math::Point>(vertices.size()),
            vertices.data(), GL_STATIC_DRAW);
        mBuff.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<math::Vector>(0));
        mVAO.enableVertexAttribArray(0);
        mBuff.unBindBuffer();

        mNormals.bindBuffer();
        mNormals.bufferData(gl::size<math::Point>(normals.size()),
            normals.data(), GL_STATIC_DRAW);
        mNormals.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<math::Vector>(0));
        mVAO.enableVertexAttribArray(1);
        mNormals.unBindBuffer();

        mUV.bindBuffer();
        mUV.bufferData(gl::size<math::Point>(uvs.size()),
            uvs.data(), GL_STATIC_DRAW);
        mUV.vertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<math::Vector2>(0));
        mVAO.enableVertexAttribArray(2);
        mUV.unBindBuffer();

        mVAO.unBindVertexArray();

        std::vector<gl::ShaderUnit> shaders
        {
            { getShaderDirectory() + "ColourSphere.vs.glsl", GL_VERTEX_SHADER },
            { getShaderDirectory() + "ColourSphere.fs.glsl", GL_FRAGMENT_SHADER }
        };

        mShaders.push_back(gl::Shader(shaders));
        //mShaders[0].setShaderIncludeDir(getShaderDirectory());
        mShaders[0].compileShaders();
        mShaders[0].linkShaders();

        auto var = mShaders[0].getUniformVariable("model");
        mUniforms.insert(UniformKey("model", var));

        var = mShaders[0].getUniformVariable("projection");
        mUniforms.insert(UniformKey("projection", var));

        var = mShaders[0].getUniformVariable("view");
        mUniforms.insert(UniformKey("view", var));

        var = mShaders[0].getUniformVariable("colour");
        mUniforms.insert(UniformKey("colour", var));

        var = mShaders[0].getUniformVariable("lightPos");
        mUniforms.insert(UniformKey("lightPos", var));

        var = mShaders[0].getUniformVariable("ambient");
        mUniforms.insert(UniformKey("ambient", var));

        mVAO.unBindVertexArray();

        mShaders[0].disableShaders();
    }

    void ColourSphere::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view){
        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        mModel = math::Matrix4(1);
        mModel = glm::translate(mModel, mPosition);
        if(glm::length(mVelocity)!=0){
        //rotation
        math::Vector axis = glm::normalize(glm::cross(math::Vector(0,1,0), mVelocity));
        mModel = glm::rotate(mModel, mTheta, axis);
        }

        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);

        glUniform3f(mUniforms["colour"], mColour.x, mColour.y, mColour.z);
        glUniform3f(mUniforms["lightPos"],1,1,1);
        math::Vector ambient=mColour*0.9f;
        glUniform3f(mUniforms["ambient"], ambient.x, ambient.y, ambient.z);

        mVAO.bindVertexArray();
        glDrawArrays(GL_TRIANGLES, 0, mNumVertices);

        mVAO.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void ColourSphere::updateGeometry(atlas::core::Time<> const& time){
        //spin
        mThetaDot = glm::length(mVelocity)/mRad;
        mTheta += mThetaDot * time.deltaTime;

    }

    void ColourSphere::setPosition(atlas::math::Vector position){
        mPosition=position;
    }

    void ColourSphere::setVelocity(atlas::math::Vector velocity){
        mVelocity=velocity;
    }

    void ColourSphere::addForce(atlas::math::Vector force){
        mForce=force;
    }

    void ColourSphere::setInitDis(float distance){
        mInitDis=distance;
    }

    void ColourSphere::setNeighbour(std::shared_ptr<ColourSphere> neighbour){
        mNeighbour=neighbour;
    }

    atlas::math::Vector ColourSphere::getPosition(){
        return mPosition;
    }

    atlas::math::Vector ColourSphere::getVelocity(){
        return mVelocity;
    }

    float ColourSphere::getMass(){
        return mMass;
    }

    std::shared_ptr<ColourSphere> ColourSphere::getNeighbour(){
        return mNeighbour;
    }

    float ColourSphere::getRad(){
        return mRad;
    }

    float ColourSphere::getDistance(std::shared_ptr<ColObject> object, float dT){
        std::shared_ptr<ColourSphere> sphere = std::dynamic_pointer_cast<ColourSphere> (object);

        math::Vector normal = glm::normalize(mPosition - sphere->getPosition()); // normal to the object
        math::Vector p = sphere->getPosition() + normal * sphere->getRad(); // collision point

        float dBall = glm::length(mPosition - p); //current distance
        float d0 = dBall + (dBall - mRad > 0 ? -mRad : mRad);

        math::Vector nextPosition = mPosition + mVelocity * dT;
        float dBallNext = glm::length(nextPosition - p); //next distance
        float d1 = dBallNext + (dBallNext - mRad < 0 ? -mRad : mRad);

        if (d0 * d1 < 0) {
            return d0 / (d0 - d1);
        } else {
            return 2;
        }
    }

    atlas::math::Vector ColourSphere::getNormal(std::shared_ptr<ColObject> object) {
        std::shared_ptr<ColourSphere> sphere = std::dynamic_pointer_cast<ColourSphere> (object);
        //std::cout<<"position: "<< mPosition.x <<" "<< mPosition.y<<" "<<mPosition.z << " sphere Pos: " << sphere->getPosition().x<<" "<< sphere->getPosition().y<<" "<<sphere->getPosition().z<<std::endl;
        return glm::normalize(mPosition - sphere->getPosition());
    }
}

