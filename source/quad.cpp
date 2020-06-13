#include "AssignmentPaths.hpp"
#include "quad.hpp"
#include <atlas/core/ImGUI.hpp>
#include <iostream>
#include <math.h>
#include <stb/stb_image.h>

# define M_PI           3.14159265358979323846  /* pi */

//https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c

namespace personalProject
{
    Quad::Quad(math::Vector position, math::Vector normal, math::Vector p):
        mPosition(position),
        mNormal(normal),
        mP(p),
        mBuff(GL_ARRAY_BUFFER),
        mColour(math::Vector(0, 0.5, 0.3))
    {

        std::vector<math::Vector> positions = {
            math::Vector(-0.5f, 0.5f, 0.0f),
            math::Vector(-0.5f, -0.5f, 0.0f),
            math::Vector(0.5f, 0.5f, 0.0f),

            math::Vector(0.5f, 0.5f, 0.0f),
            math::Vector(-0.5f, -0.5f, 0.0f),
            math::Vector(0.5f, -0.5f, 0.0f)
        };

        mVAO.bindVertexArray();
        mBuff.bindBuffer();
        mBuff.bufferData(gl::size<math::Point>(positions.size()),
            positions.data(), GL_STATIC_DRAW);
        mBuff.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
            gl::bufferOffset<math::Vector>(0));
        mVAO.enableVertexAttribArray(0);
        mBuff.unBindBuffer();

        std::vector<gl::ShaderUnit> shaders
        {
            { getShaderDirectory() + "Quad.vs.glsl", GL_VERTEX_SHADER },
            { getShaderDirectory() + "Quad.fs.glsl", GL_FRAGMENT_SHADER }
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

    void Quad::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view){
        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        mModel = math::Matrix4(1);
        mModel = glm::translate(mModel, mPosition);
        math::Vector m = math::Vector(0, 0, 1);
        math::Vector n = mNormal;
        math::Vector axis = glm::normalize(glm::cross(m , n));

        float theta = acos(glm::dot(m, n)/(glm::length(m)*glm::length(n)));

        mModel = glm::scale(mModel, math::Vector(5, 5, 5));
        if (glm::length(axis) > 0) {
            mModel = glm::rotate(mModel, theta, axis);
        }

        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);

        glUniform4f(mUniforms["colour"], mColour.x, mColour.y, mColour.z , 1);
        glUniform3f(mUniforms["lightPos"],1,1,1);
        math::Vector ambient=mColour*0.9f;
        glUniform3f(mUniforms["ambient"], ambient.x, ambient.y, ambient.z);

        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, mTex);

        mVAO.bindVertexArray();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
        mVAO.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void Quad::setPosition(atlas::math::Vector position){
        mPosition=position;
    }

    atlas::math::Vector Quad::getPosition(){
        return mPosition;
    }

    atlas::math::Vector Quad::getNormal(std::shared_ptr<ColObject> object){
        return mNormal;
    }

    atlas::math::Vector Quad::getP(){
        return mP;
    }


    float Quad::getDistance(std::shared_ptr<ColObject> object, float dT){
        std::shared_ptr<ColourSphere> sphere = std::dynamic_pointer_cast<ColourSphere> (object);

        math::Vector normal = mNormal; // normal to the plane
        math::Vector p = mP; // arbitrary point on the plane
        float rad = sphere->getRad();
        math::Vector vel = sphere->getVelocity();

        math::Vector curr = sphere->getPosition();
        float dBall = glm::dot((curr - p), normal); //d = (x-p).n
        float d0 = dBall + (dBall > 0 ? -rad : rad);

        math::Vector next = curr + vel * dT;
        float dBallNext = glm::dot((next - p), normal);
        float d1 = dBallNext + (dBallNext > 0 ? -rad : rad);

        if (d0 * d1 < 0) {
            return d0 / (d0 - d1);
        } else {
            return 2;
        }
    }
}
