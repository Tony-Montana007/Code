// olcGame3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "olcPixelGameEngine.h"
using namespace std;

struct vec3d
{
    float x, y, z;      
};

struct triangle
{
    vec3d  p[3];
};

struct mesh
{
    vector<triangle>tris;
};


struct mat4x4
{
    float m[4][4] = { 0 };
};

class olcEngine3D : public olc::PixelGameEngine
{
public:
    olcEngine3D()
    {
        sAppName = "3D Demo";
    }

private: 
    mesh meshCube;
    mat4x4 matProj;

    void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m)

    {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {

            o.x /= w; o.y /= w; o.z /= w;
        }
    }
public:
    bool OnUserCreate() override
    {
        meshCube.tris = {
            // SOUTH
            {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f},
            { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

            //EAST
            {1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

            //NORTH
            {1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

            //WEST
            {0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

            //TOP
            {0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

            //BOTTOM
            {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f },

        };



        //Projection Matrix
        float fNear = 0.1f;
        float fFar = 1000.0f;
        float fFov = 90.0f;
        float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
        float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

        matProj.m[0][0] = fAspectRatio * fFovRad;
        matProj.m[1][1] = fFovRad;
        matProj.m[2][2] = fFar / (fFar - fNear);
        matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matProj.m[2][3] = 1.0f;
        matProj.m[3][3] = 0.0f;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {   
        Clear(olc::BLACK);

        //Draw Triangles
        for (auto tri : meshCube.tris)
        {
            triangle triProjected;
            MultiplyMatrixVector(tri.p[0], triProjected.p[0], matProj);
            MultiplyMatrixVector(tri.p[1], triProjected.p[1], matProj);
            MultiplyMatrixVector(tri.p[2], triProjected.p[2], matProj);

            // Scale into view
            triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
            triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
            triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

            triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
            triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
            triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
            triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
            triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
            triProjected.p[2].y *= 0.5f * (float)ScreenHeight();



            DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                triProjected.p[1].x, triProjected.p[1].y,
                triProjected.p[2].x, triProjected.p[2].y,
                olc::WHITE);

        }
        
        
        return true;
    }

};
int main()
{
    olcEngine3D demo;
    if (demo.Construct(256, 240, 4, 4))
        demo.Start();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
