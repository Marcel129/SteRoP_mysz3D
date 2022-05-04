#include <GL/glut.h>

float angle = 0;
float posx = 0, posz = 10;

void drawCube() {
	glColor3f(1, 0.5 , 0.5);
	glutSolidCube(2);
}

void resizeMe(int w, int h)
{
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45 ,ratio, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void renderMe(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set camera
	glLoadIdentity();
	gluLookAt(
           0, 1, 4,
           0, 1, 5,
           0, 1, 0);

    //Draw Ground
	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUADS);
		glVertex3f(-100, 0, -100);
		glVertex3f(-100, 0,  100);
		glVertex3f( 100, 0,  100);
		glVertex3f( 100, 0, -100);
	glEnd();

	//Drow and move cube
	glPushMatrix();
    glTranslatef(posx, 1, posz);
    glRotatef(angle, 0, 1, 0);
    drawCube();
    glPopMatrix();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'a':
        posx += 0.01;
        break;

    case 'd':
        posx -= 0.01;
        break;

    case 'w':
        posz += 0.01;
        break;

    case 's':
        posz -= 0.01;
        break;

    case 'q':
        angle += 0.5;
        break;

    case 'e':
        angle -= 0.5;
        break;
    }

}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(800, 200);
	glutInitWindowSize(700, 700);
	glutCreateWindow("SR App");

	glutDisplayFunc(renderMe);
	glutReshapeFunc(resizeMe);
	glutIdleFunc(renderMe);

	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;
}
