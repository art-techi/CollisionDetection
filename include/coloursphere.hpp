#ifndef COLOURSPHERE_HPP
#define COLOURSPHERE_HPP
#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/utils/Mesh.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <string>
#include "colobject.hpp"

namespace personalProject
{
    namespace math=atlas::math;
    namespace gl=atlas::gl;

    class ColourSphere : public ColObject
    {
    public:
        ColourSphere(float mass, float rad, atlas::math::Vector position,
             atlas::math::Vector velocity, atlas::math::Vector force, math::Vector colour);

        ~ColourSphere() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);
        void updateGeometry(atlas::core::Time<> const& time) override;
        void setPosition(atlas::math::Vector position);
        void setVelocity(atlas::math::Vector velocity);
        void addForce(atlas::math::Vector force);
        void setInitDis(float distance);
        void setNeighbour(std::shared_ptr<ColourSphere> neighbour);
        atlas::math::Vector getPosition();
        atlas::math::Vector getVelocity();
        float getRad();
        float getMass();
        std::shared_ptr<ColourSphere> getNeighbour();
        float getDistance(std::shared_ptr<ColObject> object, float dT);
        atlas::math::Vector getNormal(std::shared_ptr<ColObject> object);

    private:
        float mMass;
        float mRad;
        float mLastD;
        atlas::math::Vector mPosition;
        atlas::math::Vector mVelocity;
        atlas::math::Vector mForce;
        atlas::math::Vector mColour;
        std::shared_ptr<ColourSphere> mNeighbour;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;
        atlas::gl::Buffer mIndexBuff;
        atlas::gl::Buffer mNormals;
        atlas::gl::Buffer mUV;
        int mNumVertices;
        float mInitDis;
        float mAngVelocity;
        atlas::math::Vector mCurrPos;
        float mTheta = 0;
        float mThetaDot = 0;

        //GLuint mTex;
    };
}

#endif // COLOURSPHERE_HPP
