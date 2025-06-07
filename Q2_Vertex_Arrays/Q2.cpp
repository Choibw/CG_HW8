#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <fstream>
#include <sstream>

#include "render_buffers.h"
#include "load_mesh.h"

using namespace glm;

GLuint gVAO;
GLuint gVBO_positions;
GLuint gVBO_normals;
GLuint gEBO;

std::vector<unsigned int> gIndices;

void load_mesh(std::string fileName);
void init_timer();
void setupOpenGL();
void display(); // frame_timer.cpp 에서 정의되어 있음

int windowWidth = 1280;
int windowHeight = 1280;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("OpenGL Bunny");

    // GLEW 초기화
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "GLEW init failed: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    load_mesh("bunny.obj");

    gIndices.reserve(gTriangles.size() * 3);
    for (const auto& tri : gTriangles) {
        gIndices.push_back(tri.indices[0]);
        gIndices.push_back(tri.indices[1]);
        gIndices.push_back(tri.indices[2]);
    }

    setupOpenGL();
    init_timer();

    glutDisplayFunc(display); // frame_timer.cpp 의 display 사용
    glutMainLoop();

    return 0;
}

void setupOpenGL()
{
    // 1) VAO 생성 & 바인드
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    // -------------------------------------------------
    //  2) 버퍼 업로드 (변경 없음)
    // -------------------------------------------------
    glGenBuffers(1, &gVBO_positions);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO_positions);
    glBufferData(
        GL_ARRAY_BUFFER,
        gPositions.size() * sizeof(Vector3),
        gPositions.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &gVBO_normals);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO_normals);
    glBufferData(
        GL_ARRAY_BUFFER,
        gNormals.size() * sizeof(Vector3),
        gNormals.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &gEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        gIndices.size() * sizeof(unsigned int),
        gIndices.data(),
        GL_STATIC_DRAW
    );

    // -------------------------------------------------
    //  3) 고정파이프라인 전용 배열 설정
    // -------------------------------------------------

    // Position 배열 활성화
    glBindBuffer(GL_ARRAY_BUFFER, gVBO_positions);
    glEnableClientState(GL_VERTEX_ARRAY);
    // stride = sizeof(Vector3), offset = 0
    glVertexPointer(
        3,               // vec3
        GL_FLOAT,
        sizeof(Vector3),
        (void*)0
    );

    // Normal 배열 활성화
    glBindBuffer(GL_ARRAY_BUFFER, gVBO_normals);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(
        GL_FLOAT,
        sizeof(Vector3),
        (void*)0
    );

    // 다시 VAO 언바인드 (선택)
    glBindVertexArray(0);

    // -------------------------------------------------
    //  4) Q1 코드에 있던 깊이테스트, 컬링, 매트릭스, 조명 세팅
    // -------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    // 재질
    GLfloat ka_kd[] = { 1,1,1,1 };
    GLfloat ks[] = { 0,0,0,1 };
    GLfloat shin = 0.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ka_kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shin);

    // 환경광
    GLfloat gamb[] = { 0.2f,0.2f,0.2f,1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gamb);

    // 조명 켜기
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    // 방향광 설정
    GLfloat lpos[] = { 1,1,1,0 };
    GLfloat lamb[] = { 0,0,0,1 };
    GLfloat ldiff[] = { 1,1,1,1 };
    GLfloat lspec[] = { 0,0,0,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);
}
