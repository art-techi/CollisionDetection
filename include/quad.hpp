#ifndef QUAD_HPP
#define QUAD_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/utils/Mesh.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <string>
#include "colobject.hpp"
#include "coloursphere.hpp"

namespace personalProject
{
    namespace math=atlas::math;
    namespace gl=atlas::gl;

    class Quad : public ColObject
    {
    public:
        Quad(atlas::math::Vector position, atlas::math::Vector normal, atlas::math::Vector p);

        ~Quad() = default;

        void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view);

        void setPosition(atlas::math::Vector position);

        float getDistance(std::shared_ptr<ColObject> object, float dT);
        atlas::math::Vector getPosition();
        atlas::math::Vector getNormal(std::shared_ptr<ColObject> object) override;
        atlas::math::Vector getP();

    private:
        atlas::math::Vector mPosition;
        atlas::gl::VertexArrayObject mVAO;
        atlas::gl::Buffer mBuff;
        math::Vector mColour;
        atlas::math::Vector mNormal;
        atlas::math::Vector mP;
    };
}

#endif // QUAD_HPP
