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

using namespace glm;

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
    setupOpenGL();
    init_timer();

    glutDisplayFunc(display); // frame_timer.cpp 의 display 사용
    glutMainLoop();

    return 0;
}

void setupOpenGL()
{
    // 뎁스 버퍼 사용
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // 컬링 비활성화 (슬라이드에서 백페이스 컬링 금지)
    glDisable(GL_CULL_FACE);

    // 뷰포트 설정
    glViewport(0, 0, 1280, 1280);

    // 투영 행렬 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    // 모델뷰 행렬 설정
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 카메라가 e = (0, 0, 0), w = (0, 0, 1) 이므로 look-at 없이 identity로 둠
    
    // 모델 변환: scale → translate 순서
    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    // --- 조명 설정 ---

    // --- 재질 설정 ---
    GLfloat ka_kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // ambient & diffuse
    GLfloat ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // no specular
    GLfloat shininess = 0.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ka_kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // --- 전역 환경광 설정 ---
    GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    // --- 조명 시스템 활성화 ---
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    // --- 조명 위치 설정 ---
    GLfloat light_dir[] = { 1.0f, 1.0f, 1.0f, 0.0f };  // 방향벡터 (w = 0)
    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };     // 없음
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };     // 흰색 diffuse
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };     // 없음

    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}