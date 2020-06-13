#include "AssignmentPaths.hpp"
#include "sphere.hpp"
#include <atlas/core/ImGUI.hpp>
#include <iostream>
#include <math.h>
#include <stb/stb_image.h>
# define M_PI           3.14159265358979323846  /* pi */

//https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c

namespace personalProject
{
    Sphere::Sphere(float mass, float rad, math::Vector position, math::Vector velocity, math::Vector force, std::string texture_filename):
        mMass(mass),
        mRad(rad),
        mPosition(position),
        mVelocity(velocity),
        mForce(force),
        //mColour(colour),
        mBuff(GL_ARRAY_BUFFER),
        mIndexBuff(GL_ELEMENT_ARRAY_BUFFER),
        mNormals(GL_ARRAY_BUFFER),
        mUV(GL_ARRAY_BUFFER)

    {

        /* Spherical coordinates
         * x=rcos(theta)sin(blah)
         * y=rsin(theta)sin(blah)
         * z=rcos(blah)
         */
        std::vector<math::Vector> vertices;
        std::vector<math::Vector> normals;
        std::vector<math::Vector2> uvs;

        float height;
        float R=mRad;
        float theta;
        float phi;
        float x, y, z;

        //IMPORTANTE: https://gamedev.stackexchange.com/questions/16585/how-do-you-programmatically-generate-a-sphere
        int numStacks=20;
        int numSlices=30;
        for(int i=0; i<numStacks; i++){
            float theta1=((float) i/numStacks)*M_PI;
            float theta2=((float) (i+1)/numStacks)*M_PI;
            for(int j=0; j<numSlices; j++){
                float phi1=((float)j/numSlices)*2*M_PI;
                float phi2=((float)(j+1)/numSlices)*2*M_PI;

                math::Vector vertex1= math::Vector(R*sin(theta1)*sin(phi1),R*cos(theta1), R*sin(theta1)*cos(phi1));
                math::Vector vertex2= math::Vector(R*sin(theta1)*sin(phi2),R*cos(theta1), R*sin(theta1)*cos(phi2));
                math::Vector vertex3= math::Vector(R*sin(theta2)*sin(phi2),R*cos(theta2), R*sin(theta2)*cos(phi2));
                math::Vector vertex4= math::Vector(R*sin(theta2)*sin(phi1),R*cos(theta2), R*sin(theta2)*cos(phi1));

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
            { getShaderDirectory() + "Sphere.vs.glsl", GL_VERTEX_SHADER },
            { getShaderDirectory() + "Sphere.fs.glsl", GL_FRAGMENT_SHADER }
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

        //var = mShaders[0].getUniformVariable("colour");
        //mUniforms.insert(UniformKey("colour", var));

        var = mShaders[0].getUniformVariable("lightPos");
        mUniforms.insert(UniformKey("lightPos", var));

        var = mShaders[0].getUniformVariable("ambient");
        mUniforms.insert(UniformKey("ambient", var));

        mVAO.unBindVertexArray();

        glGenTextures(1, &mTex);
        glBindTexture(GL_TEXTURE_2D, mTex);

        int imageWidth, imageHeight, comp;
        std::cout << "texture_filename: " << texture_filename << std::endl;
        unsigned char* image = stbi_load(texture_filename.c_str(), &imageWidth, &imageHeight, &comp, 0);

        if(image==nullptr){
            std::cout<<stbi_failure_reason() << std::endl;
            std::cout << "Failed to load texture" << std::endl;
            throw(std::string("Failed to load texture"));
        }

        //decodePNG(image, w, h, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());

        //OpenGP::EigenImage<vec3> image;
        //OpenGP::imread(texture_filename, image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        if(comp == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if(comp == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        var = mShaders[0].getUniformVariable("tex");
        mUniforms.insert(UniformKey("tex", var));

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image);

        mShaders[0].disableShaders();
    }

    void Sphere::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view){
        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        mModel = math::Matrix4(1);
        mModel = glm::translate(mModel, mPosition);

        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);

        //glUniform4f(mUniforms["colour"], mColour.x, mColour.y, mColour.z , 1);
        glUniform3f(mUniforms["lightPos"],1,1,1);
        math::Vector ambient=mColour*0.9f;
        glUniform3f(mUniforms["ambient"], ambient.x, ambient.y, ambient.z);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTex);

        mVAO.bindVertexArray();
        glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
        //glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
        mVAO.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void Sphere::updateGeometry(atlas::core::Time<> const& time){

    }

    void Sphere::setPosition(atlas::math::Vector position){
        mPosition=position;
    }

    void Sphere::setVelocity(atlas::math::Vector velocity){
        mVelocity=velocity;
    }

    void Sphere::addForce(atlas::math::Vector force){
        mForce=force;
    }

    void Sphere::setInitDis(float distance){
        mInitDis=distance;
    }

    void Sphere::setNeighbour(std::shared_ptr<Sphere> neighbour){
        mNeighbour=neighbour;
    }

    atlas::math::Vector Sphere::getPosition(){
        return mPosition;
    }

    atlas::math::Vector Sphere::getVelocity(){
        return mVelocity;
    }

    float Sphere::getMass(){
        return mMass;
    }

    float Sphere::getDis(){
        return mInitDis;
    }

    std::shared_ptr<Sphere> Sphere::getNeighbour(){
        return mNeighbour;
    }

    float Sphere::getRad(){
        return mRad;
    }

    float Sphere::getDistance(std::shared_ptr<ColObject> object, float dT){
        std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere> (object);

        math::Vector normal = glm::normalize(mPosition - sphere->getPosition()); // normal to the plane
        math::Vector p = sphere->getPosition() + normal * sphere->getRad(); // collision point

        float dBall = glm::length(mPosition - p);//glm::dot((curr - p), normal);
        float d0 = dBall + (dBall - mRad > 0 ? -mRad : mRad);

        math::Vector nextPosition = mPosition + mVelocity * dT;
        float dBallNext = glm::length(nextPosition - p);
        float d1 = dBallNext + (dBallNext - mRad < 0 ? -mRad : mRad);

        if (d0 * d1 < 0) {
            return d0 / (d0 - d1);
        } else {
            return 2;
        }
    }

    atlas::math::Vector Sphere::getNormal(std::shared_ptr<ColObject> object) {
        std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere> (object);
        //std::cout<<"position: "<< mPosition.x <<" "<< mPosition.y<<" "<<mPosition.z << " sphere Pos: " << sphere->getPosition().x<<" "<< sphere->getPosition().y<<" "<<sphere->getPosition().z<<std::endl;
        return glm::normalize(mPosition - sphere->getPosition());
    }
}

