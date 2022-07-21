#include<GL/glut.h>
#include<windows.h>
#include<mmsystem.h>
#include<stdio.h>
#include<string>
#include <math.h>

void init2();
void display();
void display0();
void display1();
void display3();
void display4();
void display5();
void display6();
void astronaut();
void reshape(int,int);
float dist=0,dist2=0,dist3=0,y=0,y3=0,y2=0,down=0,fire=0,fire1=0,fall=0,fallx=0,rang=0,dx=0,dy=0,dz=0,c=0,down1=0,lx=0,ly=0,lz=0;
int x=8;
void timer2(int);
void timer3(int);
void update(int);

int winWidth = 700, winHeight = 500;
float posX = 4, posY = 6, posZ = 5, targetX = 0, targetY = 0, targetZ = 0, upX = 0, upY = 1, upZ = 0;
float rot = 1;
GLuint texture[4];      // storage for textures
GLUquadricObj *sphere = NULL;


/* Image type - contains height, width, and data */
struct Image
{
      unsigned long sizeX;
      unsigned long sizeY;
      GLubyte *data;
};
static unsigned int getint(FILE *fp)
{
   unsigned int c, c1, c2, c3;
   c  = ((unsigned int)getc(fp));  // get 4 bytes
   c1 = ((unsigned int)getc(fp)) << 8;
   c2 = ((unsigned int)getc(fp)) << 16;
   c3 = ((unsigned int)getc(fp)) << 24;
   return c | c1 | c2 | c3;
}

// Ensure that Little Endian shorts are read into memory correctly on Big Endian platforms
static unsigned short getshort(FILE* fp)
{
   unsigned short c, c1;
   //get 2 bytes
   c  = ((unsigned short)getc(fp));
   c1 = ((unsigned short)getc(fp)) << 8;
   return c | c1;
}

// Quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.
bool ImageLoad(char *filename, Image *image)
{
   FILE *file;
   unsigned long size;          // size of the image in bytes.
   size_t i,j,k, linediff;		     // standard counter.
   unsigned short int planes;   // number of planes in image (must be 1)
   unsigned short int bpp;      // number of bits per pixel (must be 24)
   char temp;                   // temporary storage for bgr-rgb conversion.

   // Make sure the file is there.
   if ((file = fopen(filename, "rb"))==NULL) {
      printf("File Not Found : %s\n",filename);
      return false;
   }

   // Seek through the bmp header, up to the width/height:
   fseek(file, 18, SEEK_CUR);

   // Read the width
   image->sizeX = getint (file);
   printf("Width of %s: %lu\n", filename, image->sizeX);

   // Read the height
   image->sizeY = getint (file);
   printf("Height of %s: %lu\n", filename, image->sizeY);

   // Calculate the size (assuming 24 bits or 3 bytes per pixel).
   // BMP lines are padded to the nearest double word boundary.
   // fortunat
   size = 4.0*ceil(image->sizeX*24.0/32.0) * image->sizeY ;

   // Read the planes
   planes = getshort(file);
   if (planes != 1){
      printf("Planes from %s is not 1: %u\n", filename, planes);
      return false;
   }

   // Read the bpp
   bpp = getshort(file);
   if (bpp != 24) {
      printf("Bpp from %s is not 24: %u\n", filename, bpp);
      return 0;
   }

   // Seek past the rest of the bitmap header.
   fseek(file, 24, SEEK_CUR);

   // Allocate space for the data.
   image->data = new GLubyte[size];
   if (image->data == NULL) {
      printf("Error allocating memory for color-corrected image data");
      return false;
   }

   // Read the data
   i = fread(image->data, size, 1, file);
   if (i != 1) {
      printf("Error reading image data from %s.\n", filename);
      return false;
   }

   // Reverse all of the colors (bgr -> rgb)
   // Calculate distance to 4 byte boundary for each line
   // if this distance is not 0, then there will be a color reversal error
   //  unless we correct for the distance on each line.
   linediff = 4.0*ceil(image->sizeX*24.0/32.0) - image->sizeX*3.0;
   k = 0;
   for (j=0;j<image->sizeY;j++) {
      for (i=0;i<image->sizeX;i++) {
        temp = image->data[k];
        image->data[k] = image->data[k+2];
        image->data[k+2] = temp;
        k+=3;
      }
      k+= linediff;
   }
   return true;
}

// Load Bitmaps And Convert To Textures
void LoadGLTextures() {
   // Load Texture
   Image *image1;

   // Allocate space for texture
   image1 = new Image();
   if (image1 == NULL) {
      printf("Error allocating space for image");
      exit(0);
   }

   // Load picture from file
   if (!ImageLoad("C:/Users/Raghavendra/earthday.bmp", image1)) {
      exit(1);
   }


   // Create Texture Name and Bind it as current
   glGenTextures(1, &texture[0]);
   glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)



   // Set Texture Parameters
   //  Scale linearly when image bigger than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   //  Scale linearly when image smaller than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
   // Load texture into OpenGL RC
   glTexImage2D(GL_TEXTURE_2D,     // 2D texture
        0,                  // level of detail 0 (normal)
        3,	                // 3 color components
        image1->sizeX,      // x size from image
        image1->sizeY,      // y size from image
        0,	                // border 0 (normal)
        GL_RGB,             // rgb color data order
        GL_UNSIGNED_BYTE,   // color component types
        image1->data        // image data itself
      );
Image *image2;
image2 = new Image();
   if (image2 == NULL) {
      printf("Error allocating space for image");
      exit(0);
   }

   // Load picture from file
   if (!ImageLoad("C:/Users/Raghavendra/jupiter1.bmp", image2)) {
      exit(1);
   }
   glGenTextures(1, &texture[1]);
   glBindTexture(GL_TEXTURE_2D, texture[1]);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   //  Scale linearly when image smaller than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      glTexImage2D(GL_TEXTURE_2D,     // 2D texture
        0,                  // level of detail 0 (normal)
        3,	                // 3 color components
        image2->sizeX,      // x size from image
        image2->sizeY,      // y size from image
        0,	                // border 0 (normal)
        GL_RGB,             // rgb color data order
        GL_UNSIGNED_BYTE,   // color component types
        image2->data        // image data itself
      );

      Image *image3;
    image3 = new Image();
   if (image3 == NULL) {
      printf("Error allocating space for image");
      exit(0);
   }

   // Load picture from file
   if (!ImageLoad("C:/Users/Raghavendra/stars1.bmp", image3)) {
      exit(1);
   }
   glGenTextures(1, &texture[2]);
   glBindTexture(GL_TEXTURE_2D, texture[2]);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   //  Scale linearly when image smaller than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      glTexImage2D(GL_TEXTURE_2D,     // 2D texture
        0,                  // level of detail 0 (normal)
        3,	                // 3 color components
        image3->sizeX,      // x size from image
        image3->sizeY,      // y size from image
        0,	                // border 0 (normal)
        GL_RGB,             // rgb color data order
        GL_UNSIGNED_BYTE,   // color component types
        image3->data        // image data itself
      );
};



void init() {
    LoadGLTextures();
   // LoadGLTextures1();	                 // Load The Texture(s)
    glEnable(GL_TEXTURE_2D);              // Enable Texture Mapping
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear The Background Color To Blue
    glClearDepth(1.0);                    // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                 // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);	             // Enables Depth Testing
    glShadeModel(GL_SMOOTH);              // Enables Smooth Color Shading

    sphere = gluNewQuadric();             //Instatiate our sphere
    gluQuadricDrawStyle(sphere, GLU_FILL);//Following three method calls instantiate the texturing style
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,4.0/3.0,1,40);    //Tells the program how the camera should display the world

    glMatrixMode(GL_MODELVIEW);
}



void mySpecialFunc(int,int, int);
void display2();



int scene=1;

void keys(unsigned char key,int x,int y ){
    if(key==13){
    switch(scene){
    case 0: glutDisplayFunc(display0);
                 break;
    case 1: glutDisplayFunc(display1);
                 break;
    case 2: glutDisplayFunc(display);
                break;
    case 3: glutDisplayFunc(display2);
                break;
    case 4: PlaySound(TEXT("launch.wav"),NULL,SND_ASYNC);
        glutDisplayFunc(display3);
            glutPostRedisplay();
            glutTimerFunc(0,timer2,0);
                break;
    case 5:PlaySound(TEXT("space2.wav"),NULL,SND_ASYNC);
        glutDisplayFunc(display4);
                init();
             glutPostRedisplay();
             glutTimerFunc(0,update,0);
                break;
    case 6: PlaySound(TEXT("firststep1.wav"),NULL,SND_ASYNC);
        glutDisplayFunc(display5);
                init2();
            glutPostRedisplay();
            glutTimerFunc(0,timer3,0);
                break;
    case 7:glutDisplayFunc(display6);
            glutPostRedisplay();
            break;
    default:exit(0);
    }
    scene++;
    glutPostRedisplay();
     }
     else if(key==32){
    scene!=0?scene--:1;
    switch(scene){
    case 0: glutDisplayFunc(display0);
                 break;
    case 1: glutDisplayFunc(display1);
                 break;
    case 2: glutDisplayFunc(display);
                break;
    case 3: glutDisplayFunc(display2);
                break;
    case 4: glutDisplayFunc(display3);
        break;
    case 5: glutDisplayFunc(display4);
        break;
    case 6:glutDisplayFunc(display5);
        break;
    case 7:glutDisplayFunc(display6);
    break;
    default:exit(0);
     }
     glutPostRedisplay();
}
}




float angle=0;

void objectaxis(){                      // show the axes for the object
    glPushMatrix();                     //reference axis
        glColor3f(1.0,0.0,0.0); // red x
        glBegin(GL_LINES);
        glVertex3f(-4.0, 0.0f, 0.0f);
        glVertex3f(4.0, 0.0f, 0.0f);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.0,1.0,0.0); // green y
        glBegin(GL_LINES);
        glVertex3f(0.0, -4.0f, 0.0f);
        glVertex3f(0.0, 4.0f, 0.0f);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.0,0.0,1.0); // blue z
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0f ,-4.0f );
        glVertex3f(0.0, 0.0f ,4.0f );
        glEnd();
    glPopMatrix();
}

GLUquadricObj *p = gluNewQuadric(); //cylindepointer


void mySpecialFunc(int key, int x, int y){
	switch (key) {
    case GLUT_KEY_RIGHT:
        angle += 1;
        if (angle > 360) angle = 0.0;
		break;
    case GLUT_KEY_LEFT:
        angle -= 1;
        if (angle > 360) angle = 0.0;
	    break;
	}
	glutPostRedisplay();
}

void output(const char *text,int length,int x,int y,void* font){
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX,matrix);
    glLoadIdentity();
    glOrtho(0,600,0,600,-5,5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(x,y);
    for(int i=0;i<length;i++){
        glutBitmapCharacter(font,(int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}
void display0(){
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
        glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(0,0,-41);
            glBegin(GL_QUADS);
            glVertex3f(-50,50,0);
            glVertex3f(50,50,0);
            //glColor3f(0.882, 0, 1);
            glVertex3f(50,-50,0);
            glVertex3f(-50,-50,0);
        glEnd();
        glPopMatrix();
    glColor3f(1,1,1);

std::string t1,t2,t3,t4,t5,t6,t7,t8,t9,t10;
t1= "GALILEO MISSION";
output(t1.data(), t1.size(), 220,450,GLUT_BITMAP_TIMES_ROMAN_24);
glColor4f(0,0.7,0.8,0);
t2= "NAMES-  PRERANA R BHAT     SANGHAMITRA PHUKAN      MOHAMMAD FUZAIL MALIK";
output(t2.data(), t2.size(), 100,370,GLUT_BITMAP_HELVETICA_18);
t3= " ";
output(t3.data(), t3.size(), 350,370,GLUT_BITMAP_HELVETICA_18);
t4= "USN-      1BY19CS217                   1BY19CS134                                1BY19CS082";
output(t4.data(), t4.size(), 100,340,GLUT_BITMAP_HELVETICA_18);
t5= " ";
output(t5.data(), t5.size(), 350,340,GLUT_BITMAP_HELVETICA_18);
glColor3f(0.7,0.8,0);
t6= "UNDER THE GUIDANCE OF";
output(t6.data(), t6.size(), 210,250,GLUT_BITMAP_HELVETICA_18);
t10= "PROF. MUNESHWARA M S";
output(t10.data(), t10.size(), 215,230,GLUT_BITMAP_HELVETICA_18);
t8= "Assistant Prof.";
output(t8.data(), t8.size(), 215,210,GLUT_BITMAP_HELVETICA_18);
t9= "Department of CSE, BMSIT&M";
output(t9.data(), t9.size(), 210,190,GLUT_BITMAP_HELVETICA_18);
glColor3f(1,1,1);
t7= "Press 'Enter' To Launch";
output(t7.data(), t7.size(), 230,170,GLUT_BITMAP_HELVETICA_12);


glutSwapBuffers();
}

void display1(){
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
        glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(0,0,-41);
            glBegin(GL_QUADS);
            glVertex3f(-50,50,0);
            glVertex3f(50,50,0);
            //glColor3f(1, 1, 1);
            glVertex3f(50,-50,0);
            glVertex3f(-50,-50,0);
        glEnd();
        glPopMatrix();
    glColor3f(1,1,1);
std::string t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11;
t1= "GALILEO MISSION";
output(t1.data(), t1.size(), 250,500,GLUT_BITMAP_TIMES_ROMAN_24);
glColor4f(0.6,0.7,0.8,0);
t2= "\"We choose to go to Jupiter in this decade and do the other things, not because they are easy, but because they are hard\"";
output(t2.data(), t2.size(), 75,450,GLUT_BITMAP_HELVETICA_18);
glColor4f(0,0.7,0.8,0);
t3= "The Space Race was a 20th-century competition between two Cold War adversaries, the Soviet Union and the United States, to achieve";
output(t3.data(), t3.size(), 50,400,GLUT_BITMAP_HELVETICA_18);
t4= "superior spaceflight capability. It had its origins in the ballistic missile-based nuclear arms race between the two nations following the";
output(t4.data(), t4.size(), 50,380,GLUT_BITMAP_HELVETICA_18);
t5= "World War II. The technological advantage demonstrated by spaceflight achievement was seen as necessary for national security, and";
output(t5.data(), t5.size(), 50,360,GLUT_BITMAP_HELVETICA_18);
t6= "became part of the symbolism and ideology of the time. The Space Race brought pioneering launches of artificial satellites, robotic ";
output(t6.data(), t6.size(), 50,340,GLUT_BITMAP_HELVETICA_18);
t7= "space probes to the Moon, Venus, and Mars, and human spaceflight in low Earth orbit.";
output(t7.data(), t7.size(), 50,320,GLUT_BITMAP_HELVETICA_18);
t8= "Galileo was an American robotic space probe that studied the planet Jupiter and its moons,It was delivered into Earth orbit on October 18,  ";
output(t8.data(), t8.size(), 50,280,GLUT_BITMAP_HELVETICA_18);
t9= "1989 by Space Shuttle Atlantis. Galileo arrived at Jupiter on December 7, 1995, after gravitational assist flybys of Venus and Earth,";
output(t9.data(), t9.size(), 50,260,GLUT_BITMAP_HELVETICA_18);
t10= "and became the first spacecraft to orbit an outer planet";
output(t10.data(), t10.size(), 50,240,GLUT_BITMAP_HELVETICA_18);


glutSwapBuffers();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0,0.0,-15.0);
    glPushMatrix();                                     //sky
        glColor3f(0.156, 0.176, 0.223);
        glTranslatef(0,0,-41);
        glBegin(GL_QUADS);
            glVertex3f(-50,50,0);
            glVertex3f(50,50,0);
            glVertex3f(50,-50,0);
            glVertex3f(-50,-50,0);
        glEnd();
    glPopMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    glPushMatrix();                                     //grass
        glColor3f(0.333, 0.486, 0.333);
        glBegin(GL_POLYGON);
        glVertex3f(-50,-1,-50);
        glVertex3f(50,-1,-50);
        glVertex3f(50,-1,50);
        glVertex3f(-50,-1,50);

        glEnd();
        //objectaxis();
    glPopMatrix();
    glPushMatrix();                                     //pitch
        glColor3f(0.901, 0.811, 0.576);
        glBegin(GL_POLYGON);
        glVertex3f(-5,-0.9,-5);
        glVertex3f(5,-0.9,-5);
        glVertex3f(5,-0.9,10);
        glVertex3f(-5,-0.9,10);
        glEnd();
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //c1(from bottom)
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,-1);
        glColor3f(0.9, 0.9, 1);
        gluCylinder(p, 1.4, 0.9, 2, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //c2
        glRotatef(90,1,0,0);
        glTranslatef(0,0,-7);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.9, 7, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //c3
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,7);
        glColor3f(0.2, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.05, 2, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s11(r)
        glRotatef(-90,1,0,0);
        glTranslatef(1.3,0,1);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.5, 0.4, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s12
        glRotatef(-90,1,0,0);
        glTranslatef(1.3,0,2);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.4, 3, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s13
        glRotatef(-90,1,0,0);
        glTranslatef(1.3,0,5);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.05, 1.5, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s21(l)
        glRotatef(-90,1,0,0);
        glTranslatef(-1.3,0,1);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.5, 0.4, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s22
        glRotatef(-90,1,0,0);
        glTranslatef(-1.3,0,2);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.4, 3, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s23
        glRotatef(-90,1,0,0);
        glTranslatef(-1.3,0,5);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.05, 1.5, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //stairs
        glColor3f(0.478, 0.423, 0.423);
        glBegin(GL_POLYGON);
        glVertex3f(-0.9,1.5,0);
        glVertex3f(0.9,1.5,0);
        glVertex3f(0.9,-0.8,5);
        glVertex3f(-0.9,-0.8,5);
        glEnd();
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //door
        glColor3f(0.968, 0.952, 0.952);
        glBegin(GL_POLYGON);
        glVertex3f(-0.4,3.3,0.9);
        glVertex3f(0.4,3.3,0.9);
        glVertex3f(0.4,1,0.9);
        glVertex3f(-0.4,1,0.9);
        glEnd();
        //objectaxis();
    glPopMatrix();

    //astronaut1
    glPushMatrix();                                     //astronaut body
        glRotatef(90,1,0,0);
        glTranslatef(0,8-dist,-1-y);
        glScalef(1,0.5,1);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.3, 0.3, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut hand1
        glTranslatef(0.3,1+y,8-dist);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut hand2
        glTranslatef(-0.3,1+y,8-dist);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut leg1
        glTranslatef(-0.15,0+y,8-dist);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut leg2
        glTranslatef(0.15,0+y,8-dist);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut helmet
        glTranslatef(0,1.2+y,8-dist);
        glRotatef(90,1,0,0);
        glScalef(0.8,0.8,1.5);
        glColor3f(0.9, 0.9, 0.9);
        gluSphere(p,0.2,50,10);
        //gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    //astronaut2
    glPushMatrix();                                     //astronaut body
        glRotatef(90,1,0,0);
        glTranslatef(0,10-dist2,-1-y3);
        glScalef(1,0.5,1);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.3, 0.3, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut hand1
        glTranslatef(0.3,1+y3,10-dist2);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut hand2
        glTranslatef(-0.3,1+y3,10-dist2);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut leg1
        glTranslatef(-0.15,0+y3,10-dist2);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut leg2
        glTranslatef(0.15,0+y3,10-dist2);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut helmet
        glTranslatef(0,1.2+y3,10-dist2);
        glRotatef(90,1,0,0);
        glScalef(0.8,0.8,1.5);
        glColor3f(0.9, 0.9, 0.9);
        gluSphere(p,0.2,50,10);
        //gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    //astronaut3
    glPushMatrix();                                     //astronaut body
        glRotatef(90,1,0,0);
        glTranslatef(0,12-dist3,-1-y2);
        glScalef(1,0.5,1);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.3, 0.3, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut hand1
        glTranslatef(0.3,1+y2,12-dist3);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut hand2
        glTranslatef(-0.3,1+y2,12-dist3);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut leg1
        glTranslatef(-0.15,0+y2,12-dist3);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut leg2
        glTranslatef(0.15,0+y2,12-dist3);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut helmet
        glTranslatef(0,1.2+y2,12-dist3);
        glRotatef(90,1,0,0);
        glScalef(0.8,0.8,1.5);
        glColor3f(0.9, 0.9, 0.9);
        gluSphere(p,0.2,50,10);
        //gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();





    glutSwapBuffers();

}

void display2(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glutSpecialFunc(mySpecialFunc);
//    glScalef(1,1.5,1);
    glTranslatef(0.0,0.0,-8.0);
    glPushMatrix();
    glColor3f(0.156, 0.176, 0.223);
        glTranslatef(0,0,-45);
        glBegin(GL_QUADS);
            glVertex3f(-35,35,0);
            glVertex3f(35,35,0);
            glVertex3f(35,-35,0);
            glVertex3f(-35,-35,0);
        glEnd();
    glPopMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    glPushMatrix();
        glTranslatef(0,0,-40);
        glColor3f(0.2,0.6,1);       //cockpit glass
        glBegin(GL_QUADS);
            glVertex3f(-20,20,1);
            glVertex3f(20,20,1);
            glVertex3f(20,0,1);
            glVertex3f(-20,0,1);
        glEnd();
        glColor3f(0, 0.176, 0.223);
        glBegin(GL_QUADS);          //glass bg
            glVertex3f(-27,27,0);
            glVertex3f(27,27,0);
            glVertex3f(27,0,0);
            glVertex3f(-27,0,0);
        glEnd();
        glColor3f(0, 0.176, 0.223);
        glBegin(GL_QUADS);
            glVertex3f(-27,0,0);
            glVertex3f(27,0,0);
            glVertex3f(27,-27,0);
            glVertex3f(-27,-27,0);
        glEnd();
        glPushMatrix();
        glPushMatrix();
            glTranslatef(0,-7.6,0);
            glScalef(4.5,1.5,1);
            glColor3f(0.1, 0.2, 0.3);
            glutSolidCube(10);
            //objectaxis();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-22,-1,5);
            glScalef(0.1,0.1,0.1);

            glColor3f(1, 0, 0);
            for(int i=0;i<21;i++){
                    glTranslatef(20,0,0);
                    glPushMatrix();
                    for(int j=0;j<10;j++){
                    glTranslatef(0,-15,0);
                    gluSphere(p,5,30,30);}
        glPopMatrix();
                    }
           // objectaxis();
        glPopMatrix();
        glPopMatrix();
        glPushMatrix();                                     //ground
            glColor3f(0.333, 0.486, 0.333);
            glBegin(GL_POLYGON);
            glVertex3f(-30,-15,-50);
            glVertex3f(30,-15,-50);
            glVertex3f(30,-15,50);
            glVertex3f(-30,-15,50);
            glEnd();
        //objectaxis();
        glPopMatrix();
        glPushMatrix();         //chair1
            glTranslatef(-8,-7.6,20);
            glScalef(0.5,0.7,0.5);
            glColor3f(0.2, 0.3, 0.3);
            glutSolidCube(10);
            //objectaxis();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-8,-3,21.9);
            glScalef(0.49,0.7,0.1);
            glColor3f(0.2, 0.3, 0.3);
            glutSolidCube(10);
            //objectaxis();
        glPopMatrix();
        glPushMatrix();         //chair2
            glTranslatef(7,-7.6,20);
            glScalef(0.5,0.7,0.5);
            glColor3f(0.2, 0.3, 0.3);
            glutSolidCube(10);
            //objectaxis();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(7,-3,21.9);
            glScalef(0.49,0.7,0.1);
            glColor3f(0.2, 0.3, 0.3);
            glutSolidCube(10);
            //objectaxis();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(7,-3,-15);
            glScalef(4.5,4.5,4.5);
            astronaut();
            //objectaxis();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-8,-3,-15);
            glScalef(4.5,4.5,4.5);
            astronaut();
            objectaxis();
            glPopMatrix();
    glPopMatrix();



    glutSwapBuffers();
}

void astronaut()
{
    glPushMatrix();                                     //astronaut body
        glRotatef(90,1,0,0);
        glTranslatef(0,8,-1);
        glScalef(1,0.5,1);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.3, 0.3, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut hand1
        glTranslatef(0.3,1,8);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut hand2
        glTranslatef(-0.3,1,8);
        glRotatef(180,0,1,0);
        glRotatef(70,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.09, 0.09, 0.8, 20, 10);
        //objectaxis();
    glPopMatrix();

      glPushMatrix();                                     //astronaut leg1
        glTranslatef(-0.15,0,8);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut leg2
        glTranslatef(0.15,0,8);
        glRotatef(90,1,0,0);
        glScalef(0.8,1,1);
        glColor3f(0.9, 0.9, 0.9);
        gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //astronaut helmet
        glTranslatef(0,1.2,8);
        glRotatef(90,1,0,0);
        glScalef(0.8,0.8,1.5);
        glColor3f(0.9, 0.9, 0.9);
        gluSphere(p,0.2,50,10);
        //gluCylinder(p, 0.1, 0.1, 1, 20, 10);
        //objectaxis();
    glPopMatrix();
}

void display3(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
//    glScalef(1,1.5,1);
    glTranslatef(0.0,0.0,-15.0);
    glPushMatrix();                                     //sky
        glColor3f(0.01, 0.01, 0.01);
        glTranslatef(0,0,-41);
        glBegin(GL_QUADS);
            glVertex3f(-50,25,0);
            glVertex3f(50,25,0);
            glColor3f(0.256, 0.176, 0.223);
            glVertex3f(50,-65,0);
            glVertex3f(-50,-65,0);
        glEnd();
    glPopMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    glPushMatrix();                                     //grass
        glColor3f(0.333, 0.486, 0.333);
        glTranslatef(0,1-down,0);
        glBegin(GL_POLYGON);
        glVertex3f(-50,-1,-50);
        glVertex3f(50,-1,-50);
        glVertex3f(50,-1,50);
        glVertex3f(-50,-1,50);
        glEnd();
        //objectaxis();
    glPopMatrix();
    glPushMatrix();                                     //pitch
        glColor3f(0.901, 0.811, 0.576);
        glTranslatef(0,1-down,0);
        glBegin(GL_POLYGON);
        glVertex3f(-5,-0.9,-5);
        glVertex3f(5,-0.9,-5);
        glVertex3f(5,-0.9,10);
        glVertex3f(-5,-0.9,10);
        glEnd();
        //objectaxis();

    glPopMatrix();

    glPushMatrix();
    glRotatef(rang,0,0,1);
    glPushMatrix();                                     //c1(from bottom)
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,-1);
        glColor3f(0.9, 0.9, 1);
        gluCylinder(p, 1.4, 0.9, 2, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPushMatrix();                                     //c1-fire
        //glRotatef(-90,1,0,0);
        glTranslatef(0,0,0);
        glScalef(0.6,2,0.6);
        glColor3f(1, 0, 0);
        gluSphere(p,fire,50,20);
        //objectaxis();
    glPopMatrix();


    glPushMatrix();                                     //c2
        glRotatef(90,1,0,0);
        glTranslatef(0,0,-7);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.9, 7, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //c3
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,7);
        glColor3f(0.2, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.05, 2, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    //glTranslatef(0,-fall,0);
    glPushMatrix();
    glTranslatef(fallx,-fall,0);
    glPushMatrix();                                     //s11(r)
        glRotatef(-90,1,0,0);
        glTranslatef(1.3,0,1);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.5, 0.4, 1, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPushMatrix();                                     //s1-fire
        //glRotatef(-90,1,0,0);
        glTranslatef(1.3,1,0);
        glScalef(0.6,2,0.6);
        glColor3f(1, 0, 0);
        gluSphere(p,fire1,50,20);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s12
        glRotatef(-90,1,0,0);
        glTranslatef(1.3,0,2);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.4, 3, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s13
        glRotatef(-90,1,0,0);
        glTranslatef(1.3,0,5);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.05, 1.5, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-fallx,-fall,0);
    glPushMatrix();                                     //s21(l)
        glRotatef(-90,1,0,0);
        glTranslatef(-1.3,0,1);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.5, 0.4, 1, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPushMatrix();                                     //s1-fire
        //glRotatef(-90,1,0,0);
        glTranslatef(-1.3,1,0);
        glScalef(0.6,2,0.6);
        glColor3f(1, 0, 0);
        gluSphere(p,fire1,50,20);
        //objectaxis();
    glPopMatrix();


    glPushMatrix();                                     //s22
        glRotatef(-90,1,0,0);
        glTranslatef(-1.3,0,2);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.4, 3, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //s23
        glRotatef(-90,1,0,0);
        glTranslatef(-1.3,0,5);
        glColor3f(0.933, 0.925, 0.925);
        gluCylinder(p, 0.4, 0.05, 1.5, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();




    glutSwapBuffers();


}
void timer2(int){
    glutPostRedisplay();
    glutTimerFunc(1000/60,timer2,0);
    c=c+1;
    if(c>=1750){
    if(down<=450){
        down+=0.1;
        if(fire1>0)
            down+=0.3;
        if(down>=.5)
            fire=1.4;
        if(down>=12)
            while(fire1<=0.4)
                fire1+=0.05;
        if(down>20&&down<=125)
            angle += 0.7;
            if (angle > 360) angle = 0.0;
        if(down>=125){
                fall+=.05;
                if(fallx<=5){
                fallx+=0.02;}
        if(down>=175){
            if(rang<30)
            {rang+=0.02;}
        }
        }
    }
    }








}
void display4()
{
     // Clear The Screen And The Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Moves the camera according to user input
    gluLookAt(posX,posY,posZ,targetX,targetY,targetZ,upX,upY,upZ);

    //Draws the sphere and textures it
    glPushMatrix();
        glColor3f(1,1,1);       //earth
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_COLOR);
        glTranslatef(-11,-7,-1);
        glRotatef(250,1,0,0);
        glRotatef(90,0,0,1);
        glRotatef(rot,0,0,1);
        glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
        gluSphere(sphere,2.4,100,100);
        //objectaxis();
        glEnable(GL_COLOR);
    glPopMatrix();

    glPushMatrix();         //moon
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_COLOR);
        glTranslatef(3,0,-2);
        glRotatef(250,1,0,0);
        glRotatef(90,0,0,1);
        glRotatef(rot/2,0,0,1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);   // choose the texture to use.
        gluSphere(sphere,0.5,100,100);
        //objectaxis();
        glEnable(GL_COLOR);
    glPopMatrix();

    /*glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_COLOR);
        glTranslatef(30,35,30);
        glScalef(10,10,10);
            glColor3f(1,1,1);
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            glutSolidCube(10);
            glEnable(GL_COLOR);
    glPopMatrix();*/

    glPushMatrix();
        if(dx<=150)
        glScalef(0.05,0.05,0.05);
        else
        glScalef(0,0,0);
        glTranslatef(-80+dx,0+dy,12+dz);
        glRotatef(-80,1,0,0);
        glPushMatrix();                                     //c1(from bottom)
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,-1);
        glColor3f(0.9, 0.9, 1);
        gluCylinder(p, 1.4, 0.9, 2, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPushMatrix();                                     //c1-fire
        //glRotatef(-90,1,0,0);
        glTranslatef(0,0,0);
        glScalef(0.6,2,0.6);
        glColor3f(1, 0, 0);
        gluSphere(p,fire,50,20);
        //objectaxis();
    glPopMatrix();


    glPushMatrix();                                     //c2
        glRotatef(90,1,0,0);
        glTranslatef(0,0,-7);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.9, 7, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //c3
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,7);
        glColor3f(0.2, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.05, 2, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPopMatrix();


	glutSwapBuffers();
}
void update (int value) {
    //This method changes the rotation amount of the sphere.
     rot += 0.5f;
    if (rot > 360) {
       rot -= 360;
    }

    if(dx<=150)
        dx+=0.3;
        dy+=0.1;
        if(dx==150){
            dz=-10;
            dx=0;
            dy=0;}
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}


void display5(){
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
//    glScalef(1,1.5,1);
    glTranslatef(0.0,0.0,-15.0);
    glPushMatrix();                                     //sky
        glColor3f(0.01, 0.01, 0.01);
        glTranslatef(0,0,-41);
        glBegin(GL_QUADS);
            glVertex3f(-50,25,0);
            glVertex3f(50,25,0);
            glColor3f(0.256, 0.176, 0.223);
            glVertex3f(50,-65,0);
            glVertex3f(-50,-65,0);
        glEnd();
    glPopMatrix();
    glRotatef(0, 0.0, 1.0, 0.0);
    glPushMatrix();                                     //grass
        glColor3f(0.65,0.65,0.631);
        glTranslatef(0,-50+down1,0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBegin(GL_POLYGON);
        glVertex3f(-50,-1,-50);
        glVertex3f(50,-1,-50);
        glVertex3f(50,-1,50);
        glVertex3f(-50,-1,50);
        glEnd();
        //objectaxis();
    glPopMatrix();

    glPushMatrix();
    //glRotatef(rang,0,0,1);
    glPushMatrix();                                     //c1(from bottom)
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,-1);
        glColor3f(0.9, 0.9, 1);
        gluCylinder(p, 1.4, 0.9, 2, 20, 10);
        //objectaxis();
    glPopMatrix();
    glPushMatrix();                                     //c1-fire
        //glRotatef(-90,1,0,0);
        glTranslatef(0,0,0);
        glScalef(0.6,2,0.6);
        glColor3f(1, 0, 0);
        gluSphere(p,fire,50,20);
        //objectaxis();
    glPopMatrix();


    glPushMatrix();                                     //c2
        glRotatef(90,1,0,0);
        glTranslatef(0,0,-7);
        glColor3f(0.5, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.9, 7, 20, 10);
        //objectaxis();
    glPopMatrix();

    glPushMatrix();                                     //c3
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,7);
        glColor3f(0.2, 0.6, 0.6);
        gluCylinder(p, 0.9, 0.05, 2, 20, 10);
        //objectaxis();
    glPopMatrix();

    if(down1>=50){
            glPushMatrix();                                     //stairs
        glColor3f(0.478, 0.423, 0.423);
        glBegin(GL_POLYGON);
        glVertex3f(-0.9,1.5,0);
        glVertex3f(0.9,1.5,0);
        glVertex3f(0.9,-0.8,5);
        glVertex3f(-0.9,-0.8,5);
        glEnd();
        //objectaxis();
        glPopMatrix();

    glPushMatrix();                                     //door
        glColor3f(0.968, 0.952, 0.952);
        glBegin(GL_POLYGON);
        glVertex3f(-0.4,3.3,0.9);
        glVertex3f(0.4,3.3,0.9);
        glVertex3f(0.4,1,0.9);
        glVertex3f(-0.4,1,0.9);
        glEnd();
        //objectaxis();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef(0,2,-10);
    glRotatef(90,0,1,0);
    glTranslatef(-10+lx,0+ly,-8+lz);
        glPushMatrix();
        glTranslatef(0.3,1.5,7);
        glColor3f(0,0,0);
        glScalef(0.05,2,0.1);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.15,2.1,7);
        glColor3f(0,0,1);
        glScalef(1,0.8,0.1);
        glutSolidCube(1);
        glPopMatrix();
        astronaut();
        //objectaxis();
    glPopMatrix();
        }
        glPopMatrix();

    glutSwapBuffers();
}
void init2()
{//LoadGLTextures();
//glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1.0,70.0);
    glRotatef(10,1,0,0);
    glTranslatef(0,-5,0);
    glMatrixMode(GL_MODELVIEW);
}
void timer3(int){
        glutPostRedisplay();
        glutTimerFunc(1000/60,timer3,0);

        fire=1.4;
        if(down1<=50)
            down1+=0.1;
        if(down1>=50){

            if(lz<=5&&lz>-7){
                lz-=0.05;
                ly-=0.015;}

        }

}
void display6()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
        glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(0,0,-41);
            glBegin(GL_QUADS);
            glVertex3f(-50,50,0);
            glVertex3f(50,50,0);
            //glColor3f(0.882, 0, 1);
            glVertex3f(50,-50,0);
            glVertex3f(-50,-50,0);
        glEnd();
        glPopMatrix();
    glColor3f(1,1,1);

std::string t1,t2;
t1= "GALILEO MISSION";
output(t1.data(), t1.size(), 250,450,GLUT_BITMAP_TIMES_ROMAN_24);
t1= "THANKYOU!";
output(t1.data(), t1.size(), 260,250,GLUT_BITMAP_TIMES_ROMAN_24);
glutSwapBuffers();
}

void reshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1.0,70.0);
    glRotatef(10,1,0,0);
    glTranslatef(0,-5,0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int){
    glutPostRedisplay();
        glutTimerFunc(1000/60,timer,0);

    /*switch(x){
    case 8:if(dist<3)
                dist+=0.03;
            else{
                //if(dist<8 && dist<3){
            dist+=0.03;
            y+=0.017;
            x=10;
        //}
            }
            break;
    case 10:if(dist2<10 && dist<5)
                dist2+=0.03;
            else{
                //if(dist2<10)
                {dist2+=0.03;
                y1+=0.03;
                x=12;
                }
            }
            break;
    case 12:if(dist3<12 && dist<7)
                dist3+=0.03;
            else{
                //if(dist3<12)
                    {dist3+=0.03;
                    y2+=0.03;}
            //}
        }
}*/

    if(dist<3)
            y=0;
    if(dist2<5)
                y3=0;
    if(dist3<7)
                y2=0;
    if(dist<8)
        dist+=0.03;
    else
        if(dist2<=10)
            dist2+=0.03;
        else
            if(dist3<=12)
                dist3+=0.03;


        if(dist<8&&dist>3){
            dist+=0.03;
            y+=0.017;
        }
        else
            if(dist2<10&&dist2>5)
                {dist2+=0.03;
                y3+=0.03;}
            else
                if(dist3<12&&dist3>7)
                    {dist3+=0.03;
                    y2+=0.03;}

    /*switch(dir){
        case 1: if(angle<60)
                        angle+=1;
                    else  dir=-1;
        break;
        case -1:
            if(angle>0)
                        angle-=1;
                    else dir=1;
        break;
}*/
    }
    int main(int argc,char**argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1200,700);
    //init();
    glutCreateWindow("Galileo Mission");

    glutReshapeFunc(reshape);
    glutSpecialFunc(mySpecialFunc);
    glutDisplayFunc(display0);
    glutKeyboardFunc(keys);
    glutTimerFunc(0,timer,0);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
