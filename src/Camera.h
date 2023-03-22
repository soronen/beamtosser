#pragma once

#include "RTWeek.h"

class Camera
{
public:
    Camera() {
        double aspectRatio = 16.0 / 9.0;
        double viewportHeight = 2.0;
        double viewportWidth = aspectRatio * viewportHeight;
        double focalLength = 1.0;

        m_Origin = Point3(0, 0, 0);
        m_Horizontal = Vec3(viewportWidth, 0.0, 0.0);
        m_Vertical = Vec3(0.0, viewportHeight, 0.0);
        m_LowerLeftCorner = m_Origin - m_Horizontal / 2 - m_Vertical / 2 - Vec3(0, 0, focalLength);
    }

    Ray GetRay(double u, double v) const {
        return Ray(m_Origin, m_LowerLeftCorner + u * m_Horizontal + v * m_Vertical - m_Origin);
    }

private:
    Point3 m_Origin;
    Point3 m_LowerLeftCorner;
    Vec3 m_Horizontal;
    Vec3 m_Vertical;
};