// project.cpp
//*********************************************
// Darrell Wright
// This program will display a comet hitting the earth.  Most controls are
// implemented through  the right mouse button.  However the following controls
// are not:
//
// move eye +x: <6>
// move eye -x: <4>
// move eye +y: <8>
// move eye -y: <2>
// move eye +z: <9>
// move eye -z: <1>
//
// move look +x: <RIGHT ARROW>
// move look -x: <LEFT ARROW>
// move look +y: <UP ARROW>
// move look -y: <DOWN ARROW>
// move look +z: <PG UP>
// move look -z: <PG DOWN>
//
// I recomend that you use the numeric keypad and numlock to use these features.
// The rest of the controls are labeled on the menu during execution.
//

//*********************************************
// Includes
#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
// Defines
#define STAGE1 60
#define STAGE2 80
#define STAGE3 100

//----------------------------------------------------------------
// Global Variables
GLuint callMe = 0;     // Hold value for start of call list
GLuint cometLists = 0; // Hold call lists for comet object

bool isLight = false;      // enable/disable lighting
bool isSmooth = true;      // enable/disable smooth shading, otherwise flat
bool isAtmosphere = false; // enable/disable atmosphere
bool isMoon = true;        // enable/disable moon
bool isGas = true;         // enable/disable comet gases

static GLfloat const position[] = = 0.0, 0.0, 20.0, 1.0;
GLfloat tempf = -1.0f;     // Just a temp float
GLfloat dMove = 1.0f;      // Distance to move view point
GLfloat spin = 0.0f;       // Angle to rotate stuff
GLfloat spin2 = 0.0f;      // Angle to rotate crater
GLfloat t = 0.0f;          // Program time.  Allows all movements of objects
                           // to be a function of time and thus allows for
                           // variable time movement, pauses, and reverse
                           // time.
GLfloat timeChange = 1.0f; // Value to increment t by
bool isNotPaused = true;   // Whether of not to pause program
GLfloat const eyeX = 15.0f;
GLfloat const eyeY = 0.0f;
GLfloat const eyeZ = 15.0f;
GLfloat const lookX = 0.0f;
GLfloat const lookY = 0.0f;
GLfloat const lookZ = 0.0f;
GLfloat eyeDX = 0.0f;
GLfloat eyeDY = 0.0f;
GLfloat eyeDZ = 0.0f;
GLfloat lookDX = 0.0f;
GLfloat lookDY = 0.0f;
GLfloat lookDZ = 0.0f;

GLfloat moveDX = 1.0f;
GLfloat moveDY = 1.0f;
GLfloat moveDZ = 1.0f;
GLfloat earthSpin = 0.0f;
GLfloat moonSpin = 0.0f;

GLfloat earthX = 0.0f;
GLfloat earthY = 0.0f;
GLfloat earthZ = 0.0f;

GLfloat moonX = -8.0f;
GLfloat moonY = 0.0f;
GLfloat moonZ = 0.0f;

GLfloat sunX = 0.0f;
GLfloat sunY = 0.0f;
GLfloat sunZ = 20.0f;

GLfloat cometX = 0.0f;
GLfloat cometY = 0.0f;
GLfloat cometZ = 0.0f;

GLUquadricObj *q = nullptr;

GLuint dispLists;

unsigned int texture[3];
GLfloat explosionX(0.0f);             // X position of explosion
GLfloat explosionY(0.0f);             // Y position of explosion
GLfloat explosionZ(4.0f);             // Z position of explosion
static GLfloat const cometSize(0.5f); // Radius of Comet

typedef struct {
  unsigned long sizeX;
  unsigned long sizeY;
  unsigned char *data;
} Image;

//----------------------------------------------------------------
// Functions Prototypes
inline static void drawEarth();
inline static void drawMoon();
inline static void drawComet();
inline static void drawSun();
inline static void utilSolidSphere(GLdouble radius, GLint slices, GLint stacks);
inline static int ImageLoad(char *filename, Image *image);
inline static void utilLoadTextures(char *fname, unsigned int *texture);
static void exit1();
inline static void checkError();
inline static const void lightMeUp(GLfloat *specular,
                                   GLfloat *shininess); // Common lighting calls
inline static void drawExplosion();  // Draw explosion on earth
inline static void drawSun();        // Draw the sun
inline static void drawMoon();       // Draw the moon
inline static void drawEarth();      // Draw the earth
inline static void drawCrater();     // Draw crater on earth after explosion
inline static void drawAtmosphere(); // Draw the earths atmosphere
inline static void drawComet();      // Draw the melting ice gasses around comet
inline static void
drawEntryBurn(); // Draw the glow of comet breaching the earths atmosphere
                 // Functions based on glut conterparts but modified
inline static void
utilSilhouetteCone(GLdouble base, GLdouble height, GLint slices,
                   GLint stacks); // Draw the silhouette of a cone
inline static void utilPointCone(GLdouble base, GLdouble height, GLint slices,
                                 GLint stacks); // Draw a cone with just dots
inline static void
utilSolidSphere(GLdouble radius, GLint slices,
                GLint stacks); // Draw a cyninder that is solid
inline static void
utilSolidCylinder(GLdouble base, GLdouble top, GLdouble height, GLint slices,
                  GLint stacks); // Draw a cyninder that is solid
                                 // inline static void
                                 // utilPrintString( void *font, char *string,
                                 // GLint X, GLint Y );	// Print text in
                                 // window at pos (X,Y)
inline static GLfloat
utilDistance(GLfloat x, GLfloat y,
             GLfloat z); // Determine distance from (x,y,z) -> (0.0,0.0,0.0)
inline static int ImageLoad(char *filename, Image *image);

//----------------------------------------------------------------
// Program Code

void display() { // Main display loop. This is basically where everything in
                 // OpenGL is called from
  glLoadIdentity();
  gluLookAt(eyeX + eyeDX, eyeY + eyeDY, eyeZ + eyeDZ, lookX + lookDX,
            lookY + lookDY, lookZ + lookDZ, 0.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawSun();
  if (isMoon) {
    drawMoon();
  }

  glPushMatrix(); // Draw Earth
  drawEarth();
  glPopMatrix();
  if (t < STAGE1) { // Comet stuff
    glPushMatrix(); // Draw comet on screen before inpact
    drawComet();
    glPopMatrix();
    if (isAtmosphere) {
      glPushMatrix(); // Draw the comet burning in earths atmosphere
      drawEntryBurn();
      glPopMatrix();
    }
  } else {
    glRotatef(spin2, 0, 1, 0);
    glPushMatrix(); // Draw crater from inpact
    drawCrater();
    glPopMatrix();
    if (t < STAGE2) {
      glPushMatrix(); // Draw the explosion of comet hitting earth
      drawExplosion();
      glPopMatrix();
    }
  }
  if (isAtmosphere) {
    glPushMatrix(); // Draw Atmosphere
    drawAtmosphere();
    glPopMatrix();
  }

  glutSwapBuffers(); // Double buffer
}

void idleMe() {    // Idle loop. This is where most of the calculations are done
  if (t <= STAGE1) // While where in stage one of destruction
  {
    cometX = 9 - pow((t / 20.0), 2); // move the commet to proper position
    cometY = 0.0;
    cometZ = 13 - pow((t / 20.0), 2);
  }
  earthSpin += 1.0 * timeChange * isNotPaused; // Used for angle of rotation of
                                               // earth about its axis and the
                                               // moon about the earth
  moonSpin += (1.0 / 28.0) * timeChange * isNotPaused; // Used for angle of
                                                       // rotation of earth
                                                       // about its axis and the
                                                       // moon about the earth

  if (t >= STAGE1) {
    spin2 +=
        1.0 * timeChange * isNotPaused; // Used for angle of rotation of crater
  }
  t += timeChange * isNotPaused; // Increment time
  glutPostRedisplay();           // Resdisplay the screen now
}

void init() // The initialization stuff
{
  q = gluNewQuadric();
  char earth_bmp[] = "textures/earth.bmp";
  char moon_bmp[] = "textures/moon.bmp";
  char sun_bmp[] = "textures/sun.bmp";
  char comet_bmp[] = "textures/comet.bmp";
  utilLoadTextures(earth_bmp, &(texture[0]));
  utilLoadTextures(moon_bmp, &(texture[1]));
  utilLoadTextures(sun_bmp, &(texture[2]));
  utilLoadTextures(comet_bmp, &(texture[3]));
  glEnable(GL_TEXTURE_2D);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
}
void menu(int key) {
  switch (key) {
  case 27:
    exit(0);
    break;
  case '5':
    eyeDX = 0.0f;
    eyeDY = 0.0f;
    eyeDZ = 0.0f;
    lookDX = 0.0f;
    lookDY = 0.0f;
    lookDZ = 0.0f;
    break;
  case '9':
    eyeDZ += moveDZ;
    break;
  case '1':
    eyeDZ -= moveDZ;
    break;
  case '6':
    eyeDX += moveDX;
    break;
  case '4':
    eyeDX -= moveDX;
    break;
  case '8':
    eyeDY += moveDY;
    break;
  case '2':
    eyeDY -= moveDY;
    break;
  case '-':
    timeChange -= 0.5;
    break;
  case '=':
    timeChange += 0.5;
    break;
  case '+':
    timeChange += 0.5;
    break;
  case 'p':
    if (isNotPaused) {
      isNotPaused = false;
    } else {
      isNotPaused = true;
    }
    break;
  case 'a':
    isAtmosphere = !isAtmosphere;
    break;
  case 'm':
    isMoon = !isMoon;
    break;
  case 'l':
    isLight = !isLight;
    break;
  case 's':
    isSmooth = !isSmooth;
    break;
  case 'c':
    isGas = !isGas;
    break;
  }
}
void keyboard(unsigned char key, int, int) { menu(key); }
static void specialkey(int key, int, int) {
  switch (key) {
  case GLUT_KEY_UP:
    lookDY += moveDY;
    break;
  case GLUT_KEY_DOWN:
    lookDY -= moveDY;
    break;
  case GLUT_KEY_LEFT:
    lookDX -= moveDX;
    break;
  case GLUT_KEY_RIGHT:
    lookDX += moveDX;
    break;
  case GLUT_KEY_END:
    lookDZ -= moveDZ;
    break;
  case GLUT_KEY_PAGE_UP:
    lookDZ += moveDZ;
    break;
  }
}
void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)(w) / (GLfloat)(h), 1.0, 80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char **argv) {
  atexit(exit1);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(400, 400); // Initial screen size
  glutInitWindowPosition(0, 0); // Initial window position
  glutCreateWindow("CS4471 - Project Death To Earth(Darrell Wright)");
  init();
  glutDisplayFunc(display);
  glutFullScreen();
  glutReshapeFunc(reshape);
  glutIdleFunc(idleMe);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialkey);
  glutCreateMenu(menu);
  glutAddMenuEntry("Quit - <ESC>", 27);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutMainLoop();
  return (0);
}
//----------------------------------------------------------------
// Program Functions
inline static const void lightMeUp(GLfloat *specular, GLfloat *shininess) {
  glEnable(GL_COLOR_MATERIAL);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}
inline static void drawExplosion() // Draw explosion when comet hits the earth
{
  if (isLight) {
    GLfloat specular[] = {1.0, 0.0, 0.0, 0.5} GLfloat shininess[] = {
        25.0} lightMeUp(specular, shininess);
  }
  glColor4f(1.0f, 0.45f, 0.15f, 0.5f);
  glTranslatef(cometX, cometY, cometZ);
  glutSolidSphere(2.0 - ((t - STAGE1) / 10), 40, 40);
}

inline static void drawCrater() // Draw the crater left by the comet
{
  glTranslatef(0.0f, 0.0f, 2.16f);
  glColor4f(0.50f, 0.26f, 0.0f, 1.0f);
  glutSolidSphere(2.0, 10, 10);
}
inline static void drawAtmosphere() // Draw Earth's Atmosphere
{
  if (isLight) {
    GLfloat specular[] = {0.9f, 0.9f, 0.9f, 0.1f} GLfloat shininess[] = {
        5.0f} lightMeUp(specular, shininess);
  }
  glColor4f(0.1f, 0.1f, 1.0f, 0.2f);
  glutSolidSphere(4.4, 40, 40);
}
inline static void drawComet() // Draw Comet Ball
{
  glPushMatrix();
  glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
  glTranslatef(cometX, cometY, cometZ);
  glBindTexture(GL_TEXTURE_2D, texture[3]);
  utilSolidSphere(cometSize, 20, 20);
  glPopMatrix();
}
inline static void drawEntryBurn() {
  GLfloat tempf = utilDistance(cometX, cometY, cometZ);
  if (tempf > 5.0 && tempf < 7.0) {
    glTranslatef(0.0f, 0.0f, 4.0f);
    glColor4f(1.0f, 0.0f, 0.0f, 0.2f);
    glutSolidTorus(cometSize * 0.95, cometSize * 1.02, 40,
                   40); // Draw entry burn
  }
}
//----------------------------------------------------------------
// Utility Functions
inline static void utilSilhouetteCone(GLdouble base, GLdouble height,
                                      GLint slices, GLint stacks) {
  GLUquadricObj *q = gluNewQuadric();
  gluQuadricDrawStyle(q, GLU_SILHOUETTE);
  if (isSmooth) {
    gluQuadricNormals(q, GLU_SMOOTH);
  } else {
    gluQuadricNormals(q, GLU_FLAT);
  }
  gluCylinder(q, base, 0.0, height, slices, stacks);
  gluDeleteQuadric(q);
}
inline static void utilPointCone(GLdouble base, GLdouble height, GLint slices,
                                 GLint stacks) {
  GLUquadricObj *q = gluNewQuadric();
  gluQuadricDrawStyle(q, GLU_POINT);
  if (isSmooth) {
    gluQuadricNormals(q, GLU_SMOOTH);
  } else {
    gluQuadricNormals(q, GLU_FLAT);
  }
  gluCylinder(q, base, 0.0, height, slices, stacks);
  gluDeleteQuadric(q);
}
inline static void utilSolidCylinder(GLdouble base, GLdouble top,
                                     GLdouble height, GLint slices,
                                     GLint stacks) {
  GLUquadricObj *q = gluNewQuadric();
  gluQuadricDrawStyle(q, GLU_FILL);
  if (isSmooth) {
    gluQuadricNormals(q, GLU_SMOOTH);
  } else {
    gluQuadricNormals(q, GLU_FLAT);
  }
  gluCylinder(q, base, top, height, slices, stacks);
  gluDeleteQuadric(q);
}

inline static void drawEarth() {
  glPushMatrix();
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glTranslatef(earthX, earthY, earthZ);
  glRotatef(earthSpin, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, 0.0f);
  glPushMatrix();
  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  utilSolidSphere(4.0f, 20, 20);
  glPopMatrix();
  glPopMatrix();
}

inline static void drawMoon() {
  glPushMatrix();
  glRotatef(moonSpin, 0.25f, 1.0f, 0.0f);
  glPushMatrix();
  glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
  glTranslatef(moonX, moonY, moonZ);
  glRotatef(moonSpin / 2.0f, 0.25f, 1.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, 0.0f);
  glPushMatrix();
  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  utilSolidSphere(1.5f, 20, 20);
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
}

inline static void drawSun() {
  glPushMatrix();
  glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
  glTranslatef(sunX, sunY, sunZ);
  glPushMatrix();
  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  utilSolidSphere(1.5f, 20, 20);
  glPopMatrix();
  glPopMatrix();
}

//*****************************************************************

inline static void utilSolidSphere(GLdouble radius, GLint slices,
                                   GLint stacks) {
  // Allows for a textured sphere
  gluQuadricDrawStyle(q, (GLenum)(GLU_FILL));
  gluQuadricTexture(q, GL_TRUE);
  gluSphere(q, radius, slices, stacks);
}

inline static int ImageLoad(char *filename, Image *image) {
  FILE *file = nullptr;
  unsigned long size { 0 }        // size of the image in bytes.
  unsigned long i { 0 }           // standard counter.
  unsigned short int planes { 0 } // number of planes in image {must be 1}
  unsigned short int bpp { 0 }    // number of bits per pixel {must be 24}
  unsigned char temp { 0 } // temporary color storage for bgr-rgb conversion.
  if ((fopen_s(&file, filename, "rb"))) {
    std::cerr << "File Not Found " << filename << '\n';
    exit(EXIT_FAILURE);
  }
  fseek(file, 18, SEEK_CUR);

  if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
    std::cerr << "Error reading width from " << filename << '\n';
    exit(EXIT_FAILURE);
  }

  if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
    std::cerr << "Error reading height from " << filename << '\n';
    exit(EXIT_FAILURE);
  }
  size = image->sizeX * image->sizeY * 3;
  if ((fread(&planes, 2, 1, file)) != 1) {
    std::cerr << "Error reading planes from " << filename << '\n';
    exit(EXIT_FAILURE);
  }
  if (planes != 1) {
    std::cerr << "Planes from " << filename << " is not 1: " << planes << '\n';
    exit(EXIT_FAILURE);
  }
  if ((i = fread(&bpp, 2, 1, file)) != 1) {
    std::cerr << "Error reading bpp from " << filename << '\n';
    exit(EXIT_FAILURE);
  }
  if (bpp != 24) {
    std::cerr << "Bpp from " << filename << " is not 24: " << bpp << '\n';
    exit(EXIT_FAILURE);
  }
  fseek(file, 24, SEEK_CUR);
  image->data = new unsigned char[size];
  if (image->data == nullptr) {
    std::cerr << "Error allocating memory for color-corrected image data"
              << '\n';
    exit(EXIT_FAILURE);
  }

  if ((i = fread(image->data, size, 1, file)) != 1) {
    std::cerr << "Error reading image data from " << filename << '\n';
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < size; i += 3) {
    temp = image->data[i];
    image->data[i] = image->data[i + 2];
    image->data[i + 2] = temp;
  }
  fclose(file);
  return (EXIT_SUCCESS);
}

inline static void utilLoadTextures(char *fname, unsigned int *texture) {
  Image *image = new Image;
  if (image == nullptr) {
    std::cerr << "Error allocating space for image" << '\n';
    exit(EXIT_FAILURE);
  }

  if (ImageLoad(fname, image)) {
    std::cerr << "Error loading image" << '\n';
    exit(EXIT_FAILURE);
  }
  glGenTextures(1, (unsigned int *)(texture));
  glBindTexture(GL_TEXTURE_2D, *texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, image->sizeX, image->sizeY, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image->data);
  delete[] image->data;
  delete image;
}

static void exit1() { gluDeleteQuadric(q); }

inline static void checkError() {
  GLenum errCode(glGetError());
  int isError(false);

  while (errCode != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << '\n' << gluErrorString(errCode) << '\n';
    isError = true;
    errCode = glGetError();
  }
  if (isError) {
    exit(EXIT_FAILURE);
  }
}

inline GLfloat utilDistance(GLfloat x, GLfloat y, GLfloat z) {
  return (sqrt(x * x + y * y + z * z));
}
