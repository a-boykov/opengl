#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <GL/glu.h>
//#include <GL/gl.h>
#include <math.h>
#include <time.h>
#include <FreeImage.h>
#include "Shader.h"
//#include <iostream>

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;
const int INSCRIPTION_WIDTH = 400;
const int INSCRIPTION_HEIGHT = 70;
const int INSCRIPTION_X1 = 320;
const int INSCRIPTION_Y1 = 5;
const int INSCRIPTION_X2 = 740;
const int INSCRIPTION_Y2 = 5;

GLenum image1;
GLenum image2;
GLenum image3;
GLenum background;
Shader shader1;
Shader shader2;
Shader shader3;

void TimeEvent(int te);
void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);

GLenum loadTexture(const char *textureFile);

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (1400, 900);
   glutInitWindowPosition (10, 10);
   glutCreateWindow (argv[0]);
   glutSetWindowTitle("Shader test");
   glewInit();
   init();

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   glutTimerFunc( 10, TimeEvent, 1);
   glutMainLoop();

   return 0;
}

void TimeEvent(int )
{
    static float time1 = 0;
    static float time2 = 0;
    static float delta1 = 0.005;
    static float delta2 = 0.02;

    glutPostRedisplay();
    shader1.Use(true);
    shader1.SetTime(time1);
    shader2.Use(true);
    shader2.SetTime(time2);
    shader3.Use(true);
    shader3.SetTime(time2);
    glutTimerFunc( 10, TimeEvent, 1);

    time1 += delta1;
    time2 += delta2;

    if(time1 > 1.0f)
      time1 = 0.0f;


    if(time2 > 1.0f || time2 < 0)
      delta2 = -delta2;
}

void init(void)
{
    image1 = loadTexture("pick_01.png");
    image2 = loadTexture("pick_02.png");
    image3 = loadTexture("mask006.png");
    background = loadTexture("screen.png");
    shader1.InitializeProgram("glow.vert","glow.frag", true);
    shader1.Use(true);
    shader1.SetWidth(2);
    shader1.SetHeight(1.0);

    shader2.InitializeProgram("color.vert","color.frag", false);

    shader3.InitializeProgram("grayscale.vert","grayscale.frag");

    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glEnable(GL_BLEND);

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, -SCREEN_HEIGHT/2, 1.0, 30.0);
}

void display(void)
{
  glEnable(GL_BLEND);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, -SCREEN_HEIGHT/2, 1.0, 30.0);
  glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 1.0, 30.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Put view state on screen
  glColor3f( 1.0, 1.0, 1.0);

  glPushMatrix();
  glTranslatef(0,0,-10);

  glEnable(GL_TEXTURE_2D);

  shader1.Use(false);
  shader2.Use(false);
  shader3.Use(false);


  shader3.Use(true);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, background);
  glBegin(GL_QUADS);

  glMultiTexCoord2f(GL_TEXTURE0,0,0);
  glVertex3f(0,0,0);

  glMultiTexCoord2f(GL_TEXTURE0,1,0);
  glVertex3f( SCREEN_WIDTH,0,0);

  glMultiTexCoord2f(GL_TEXTURE0,1,1);
  glVertex3f( SCREEN_WIDTH, SCREEN_HEIGHT,0);

  glMultiTexCoord2f(GL_TEXTURE0,0,1);
  glVertex3f(0, SCREEN_HEIGHT, 0);

  glEnd();
  shader3.Use(false);


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);


  shader1.Use(true);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, image1);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, image2);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, image3);

  glBegin(GL_QUADS);
  glMultiTexCoord2f(GL_TEXTURE0,0,0);
  glMultiTexCoord2f(GL_TEXTURE1,0,0);
  glMultiTexCoord2f(GL_TEXTURE2,0,0);
  glVertex3f(INSCRIPTION_X1,INSCRIPTION_Y1,1);

  glMultiTexCoord2f(GL_TEXTURE0,1,0);
  glMultiTexCoord2f(GL_TEXTURE1,1,0);
  glMultiTexCoord2f(GL_TEXTURE2,1,0);
  glVertex3f(INSCRIPTION_X1+INSCRIPTION_WIDTH,INSCRIPTION_Y1,1);

  glMultiTexCoord2f(GL_TEXTURE0,1,1);
  glMultiTexCoord2f(GL_TEXTURE1,1,1);
  glMultiTexCoord2f(GL_TEXTURE2,1,1);
  glVertex3f(INSCRIPTION_X1+INSCRIPTION_WIDTH,INSCRIPTION_Y1 + INSCRIPTION_HEIGHT,1);

  glMultiTexCoord2f(GL_TEXTURE0,0,1);
  glMultiTexCoord2f(GL_TEXTURE1,0,1);
  glMultiTexCoord2f(GL_TEXTURE2,0,1);
  glVertex3f(INSCRIPTION_X1,INSCRIPTION_Y1 + INSCRIPTION_HEIGHT,1);

  glEnd();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, 0);

  shader1.Use(false);
  shader2.Use(true);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, image1);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, image2);

  glBegin(GL_QUADS);
  glMultiTexCoord2f(GL_TEXTURE0,0,0);
  glMultiTexCoord2f(GL_TEXTURE1,0,0);
  glVertex3f(INSCRIPTION_X2,INSCRIPTION_Y2,1);

  glMultiTexCoord2f(GL_TEXTURE0,1,0);
  glMultiTexCoord2f(GL_TEXTURE1,1,0);
  glVertex3f(INSCRIPTION_X2+INSCRIPTION_WIDTH,INSCRIPTION_Y2,1);

  glMultiTexCoord2f(GL_TEXTURE0,1,1);
  glMultiTexCoord2f(GL_TEXTURE1,1,1);
  glVertex3f(INSCRIPTION_X2+INSCRIPTION_WIDTH,INSCRIPTION_Y2 + INSCRIPTION_HEIGHT,1);

  glMultiTexCoord2f(GL_TEXTURE0,0,1);
  glMultiTexCoord2f(GL_TEXTURE1,0,1);
  glVertex3f(INSCRIPTION_X2,INSCRIPTION_Y2 + INSCRIPTION_HEIGHT,1);

  glEnd();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);

  shader2.Use(false);

  glPopMatrix();

  glDisable(GL_BLEND);


  glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
}

void keyboard (unsigned char key,int x,int y)
{
  switch (key)
  {
    case 27:
        glutLeaveMainLoop();
        return;
  }

}

GLenum loadTexture(const char * textureFile)
{
    FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(textureFile,0);//Automatocally detects the format(from over 20 formats!)
    FIBITMAP* imagen = FreeImage_Load(formato, textureFile);

    FIBITMAP* temp = imagen;
    imagen = FreeImage_ConvertTo32Bits(imagen);
    FreeImage_Unload(temp);

    int w = FreeImage_GetWidth(imagen);
    int h = FreeImage_GetHeight(imagen);

    GLubyte* textura = new GLubyte[4*w*h];
    char* pixeles = (char*)FreeImage_GetBits(imagen);
    //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

    for(int j= 0; j<w*h; j++)
    {
        textura[j*4+0]= pixeles[j*4+2];
        textura[j*4+1]= pixeles[j*4+1];
        textura[j*4+2]= pixeles[j*4+0];
        textura[j*4+3]= pixeles[j*4+3];
        //cout<<j<<": "<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<endl;
    }

    //Now generate the OpenGL texture object

    GLenum texturaID;

    glGenTextures(1,&texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)textura );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLenum huboError = glGetError();
    if(huboError)
        return 0;

    return texturaID;
}
