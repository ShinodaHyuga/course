#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>

double year = 0.0, day = 0.0;
int samplingTime = 50;

//	太陽,		水星,		金星,		地球,		火星,		木星,		土星,		天王星,		海王星,		冥王星
// 惑星ごとの半径 [10^5 km]
double planet_radius[] = {
	6.95700,	0.02440,	0.06052,	0.06371,	0.03390,	0.69911,	0.58232,	0.25362,	0.24622,	0.01173
};
// 惑星ごとの太陽からの距離 [10^9 km]
double planet_distance[] = {
	0.00000,	0.05791,	0.10821,	0.14960,	0.22794,	0.77834,	1.42667,	2.87066,	4.49840,	5.90638
};
// 惑星ごとの公転周期 [年]
double planet_revolution[] = {
	0.000,		0.241,		0.615,		1.000,		1.881,		11.862,		29.457,		84.011,		164.790,	248.090
};
// 惑星ごとの公転軌道傾斜角 [°]
double planet_revolution_inclination[] = {
	0.00,		7.00,		3.39,		0.0,		1.85,		1.31,		2.48,		0.77,		1.77,		17.140
};
// 惑星ごとの自転周期 [日]
double planet_rotation[] = {
	25.380,		58.650,		-243.020,	0.997,		1.026,		0.414,		0.444,		-0.718,		0.671,		-6.387
};
// 惑星ごとの自転軌道傾斜角 [°]
double planet_rotation_inclination[] = {
	0.000,		2.110,		2.640,		23.440,		25.190,		3.130,		26.730,		97.770,		28.320,		122.530
};

//	月,			フォボス,	ダイモス,	イオ,		エウロパ,	ガニメデ,	カリスト
// 衛星ごとの半径 [km]
double satellite_radius[] = {
	1737.1,		11.1,		6.2,		1821.6,		1560.8,		2634.1,		2410.3
};
// 衛星ごとの惑星からの距離 [10^6 km] 
double satellite_distance[] = {
	38.440,		0.938,		2.346,		42.180,		67.110,		107.040,	188.270
};
// 衛生ごとの公転周期 [年]
double satellite_revolution[] = {
	0.08,		0.32,		1.26,		1.77,		3.55,		7.16,		16.69
};
// 衛星ごとの公転軌道傾斜角 [°]
double satellite_revolution_inclination[] = {
	5.145,		1.080,		0.930,		0.040,		0.470,		0.200,		0.330
};
// 衛星ごとの自転周期 [日]
double satellite_rotation[] = {
	0.0370,	0.3189,		1.2624,		1.7691,		3.5512,		7.1550,		16.6890
};
// 衛星ごとの自転軌道傾斜角 [°]
double satellite_rotation_inclination[] = {
	6.687,		1.080,		0.930,		0.040,		0.470,		0.200,		0.330
};

void drawOrbit(double distance, double inclination) {
	glBegin(GL_LINE_LOOP);
	double pi = 3.14159;
	for (int i = 0; i < 360; i++) {
		double angle = i * pi / 180.0;
		double x = distance * cos(angle);
		double y = distance * sin(angle) * cos(inclination * pi / 180.0);
		double z = distance * sin(angle) * sin(inclination * pi / 180.0);
	}
	glEnd();
}

void myInit(char* progname)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(2000, 2000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(progname);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// 太陽
	glColor3d(1.00, 0.65, 0.15);
	glutWireSphere(planet_radius[0], 20, 16);

	//  水星
	glPushMatrix();
		glColor3d(0.50, 0.50, 0.50);										// 惑星の色を設定
		drawOrbit(planet_distance[1], planet_revolution_inclination[1]);	// 楕円軌道を設定
		glRotated((double)year / planet_revolution[1], 0.0, 1.0, 0.0);		// 公転周期を設定
		glRotated(planet_revolution_inclination[1], 1.0, 0.0, 0.0);			// 公転軌道を設定
		glTranslated(planet_distance[1], 0.0, 0.0);							// 太陽からの距離を設定
		glRotated((double)day / planet_rotation[1], 0.0, 1.0, 0.0);			// 自転周期を設定
		glRotated(planet_rotation_inclination[1], 1.0, 0.0, 0.0);			// 自転軌道を設定
		glutWireSphere(planet_radius[1], 10, 8);							// 惑星の大きさを設定
	glPopMatrix();

	// 金星
	glPushMatrix();
		glColor3d(1.00, 0.96, 0.70);
		drawOrbit(planet_distance[2], planet_revolution_inclination[2]);
		glRotated((double)year / planet_revolution[2], 0.0, 1.0, 0.0);
		glTranslated(planet_distance[2], 0.0, 0.0);
		glRotated(planet_revolution_inclination[2], 1.0, 0.0, 0.0);
		glRotated((double)day / planet_rotation[2], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[2], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[2], 10, 8);
	glPopMatrix();

	// 地球
	glPushMatrix();
		glColor3d(0.00, 0.56, 0.58);
		drawOrbit(planet_distance[3], planet_revolution_inclination[3]);
		glRotated((double)year / planet_revolution[3], 0.0, 1.0, 0.0);
		glTranslated(planet_distance[3], 0.0, 0.0);
		glRotated(planet_revolution_inclination[3], 1.0, 0.0, 0.0);
		glRotated((double)day / planet_rotation[3], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[3], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[3], 10, 8);
		// 月
		glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated((double)year / satellite_revolution[0], 0.0, 1.0, 0.0);
			glTranslated(satellite_distance[0], 0.0, 0.0);
			glRotated((double)day / satellite_rotation[0], 0.0, 1.0, 0.0);
			glRotated(satellite_rotation_inclination[0], 1.0, 0.0, 0.0);
			glutWireSphere(satellite_radius[0], 10, 8);
		glPopMatrix();
	glPopMatrix();

	// 火星
	glPushMatrix();
		glColor3d(1.00, 0.00, 0.00);
		drawOrbit(planet_distance[4], planet_revolution_inclination[4]);
		glRotated((double)year / planet_revolution[4], 0.0, 1.0, 0.0);
		glRotated(planet_revolution_inclination[4], 1.0, 0.0, 0.0);
		glTranslated(planet_distance[4], 0.0, 0.0);
		glRotated((double)day / planet_rotation[4], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[4], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[4], 10, 8);
		// フォボス
		glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated((double)year / satellite_revolution[1], 0.0, 1.0, 0.0);
			glRotated((double)day / satellite_rotation[1], 0.0, 1.0, 0.0);
			glRotated(satellite_rotation_inclination[1], 1.0, 0.0, 0.0);
			glTranslated(satellite_distance[1], 0.0, 0.0);
			glutWireSphere(satellite_radius[1], 10, 8);
		glPopMatrix();
		// ダイモス
		glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated((double)year / satellite_revolution[2], 0.0, 1.0, 0.0);
			glTranslated(satellite_distance[2], 0.0, 0.0);
			glRotated((double)day / satellite_rotation[2], 0.0, 1.0, 0.0);
			glRotated(satellite_rotation_inclination[2], 1.0, 0.0, 0.0);
			glutWireSphere(satellite_radius[2], 10, 8);
		glPopMatrix();
	glPopMatrix();

	// 木星
	glPushMatrix();
		glColor3d(0.80, 0.56, 0.20);
		drawOrbit(planet_distance[5], planet_revolution_inclination[5]);
		glRotated((double)year / planet_revolution[5], 0.0, 1.0, 0.0);
		glRotated(planet_revolution_inclination[5], 1.0, 0.0, 0.0);
		glTranslated(planet_distance[5], 0.0, 0.0);
		glRotated((double)day / planet_rotation[5], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[5], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[5], 10, 8);
		// イオ
		glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated((double)year / satellite_revolution[3], 0.0, 1.0, 0.0);
			glTranslated(satellite_distance[3], 0.0, 0.0);
			glRotated((double)day / satellite_rotation[3], 0.0, 1.0, 0.0);
			glRotated(satellite_rotation_inclination[3], 1.0, 0.0, 0.0);
			glutWireSphere(satellite_radius[3], 10, 8);
		glPopMatrix();
		// エウロパ
		glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated((double)year / satellite_revolution[4], 0.0, 1.0, 0.0);
			glTranslated(satellite_distance[4], 0.0, 0.0);
			glRotated((double)day / satellite_rotation[4], 0.0, 1.0, 0.0);
			glRotated(satellite_rotation_inclination[4], 1.0, 0.0, 0.0);
			glutWireSphere(satellite_radius[4], 10, 8);
		glPopMatrix();
		// ガニメデ
		glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated((double)year / satellite_revolution[5], 0.0, 1.0, 0.0);
			glTranslated(satellite_distance[5], 0.0, 0.0);
			glRotated((double)day / satellite_rotation[5], 0.0, 1.0, 0.0);
			glRotated(satellite_rotation_inclination[5], 1.0, 0.0, 0.0);
			glutWireSphere(satellite_radius[5], 10, 8);
		glPopMatrix();
		// カリスト
		glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated((double)year / satellite_revolution[6], 0.0, 1.0, 0.0);
			glTranslated(satellite_distance[6], 0.0, 0.0);
			glRotated((double)day / satellite_rotation[6], 0.0, 1.0, 0.0);
			glRotated(satellite_rotation_inclination[6], 1.0, 0.0, 0.0);
			glutWireSphere(satellite_radius[6], 10, 8);
		glPopMatrix();
	glPopMatrix();

	// 土星
	glPushMatrix();
		glColor3d(1.00, 1.00, 0.88);
		drawOrbit(planet_distance[6], planet_revolution_inclination[6]);
		glRotated((double)year / planet_revolution[6], 0.0, 1.0, 0.0);
		glRotated(planet_revolution_inclination[6], 1.0, 0.0, 0.0);
		glTranslated(planet_distance[6], 0.0, 0.0);
		glRotated((double)day / planet_rotation[6], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[6], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[6], 10, 8);
	//　土星の環
		glPushMatrix();
			glRotated(90.0, 1.0, 0.0, 0.0);
			glutSolidTorus(planet_radius[6] / 1.5, planet_radius[6] * 2, 2, 64);
		glPopMatrix();
	glPopMatrix();

	// 天王星
	glPushMatrix();
		glColor3d(0.68, 0.84, 0.85);
		drawOrbit(planet_distance[7], planet_revolution_inclination[7]);
		glRotated((double)year / planet_revolution[7], 0.0, 1.0, 0.0);
		glRotated(planet_revolution_inclination[7], 1.0, 0.0, 0.0);
		glTranslated(planet_distance[7], 0.0, 0.0);
		glRotated((double)day / planet_rotation[7], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[7], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[7], 10, 8);
	glPopMatrix();

	// 海王星
	glPushMatrix();
		glColor3d(0.48, 0.58, 0.80);
		drawOrbit(planet_distance[8], planet_revolution_inclination[8]);
		glRotated((double)year / planet_revolution[8], 0.0, 1.0, 0.0);
		glRotated(planet_revolution_inclination[8], 1.0, 0.0, 0.0);
		glTranslated(planet_distance[8], 0.0, 0.0);
		glRotated((double)day / planet_rotation[8], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[8], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[8], 10, 8);
	glPopMatrix();

	// 冥王星
	glPushMatrix();
		glColor3d(0.52, 0.33, 0.29);
		drawOrbit(planet_distance[9], planet_revolution_inclination[9]);
		glRotated((double)year / planet_revolution[9], 0.0, 1.0, 0.0);
		glRotated(planet_revolution_inclination[9], 1.0, 0.0, 0.0);
		glTranslated(planet_distance[9], 0.0, 0.0);
		glRotated((double)day / planet_rotation[9], 0.0, 1.0, 0.0);
		glRotated(planet_rotation_inclination[9], 1.0, 0.0, 0.0);
		glutWireSphere(planet_radius[9], 10, 8);
	glPopMatrix();

	glutSwapBuffers();
}

void myReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width / (double)height, 0.5, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// gluLookAt(0.0, 8.0, 16.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // default
	gluLookAt(0.0, 0.0, 16.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // 真横から
	// gluLookAt(2.0, 16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // 真上から
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'z':
		day = 0.0;
		year = 0.0;
		glutPostRedisplay();
		break;
	case 'y':
		year += 60.0;
		glutPostRedisplay();
		break;
	case 'Y':
		year -= 60.0;
		glutPostRedisplay();
		break;
	case 'd':
		day += 60.0;
		glutPostRedisplay();
		break;
	case 'D':
		day += 60.0;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void myTimer(int value)
{
	if (value == 1)
	{
		glutTimerFunc(samplingTime, myTimer, 1);
		year += 1.0;
		day  += 1.0;

		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	int simulation_mode = 0;  // この数値を変更することでモードを切り替える
	int planet_num = sizeof(planet_radius) / sizeof(planet_radius[0]);
	int satellite_num = sizeof(satellite_radius) / sizeof(satellite_radius[0]);
	switch (simulation_mode)
	{
		// すべてを表示
	case 0:
		planet_radius[0] /= 100.0;
		for (int i = 0; i < planet_num; i++)
		{
			planet_radius[i]   *= 0.9;
			planet_distance[i] *= 3.8;
		}
		for (int i = 0; i < satellite_num; i++)
		{
			satellite_radius[i]   *= (1.2 / 100000);
			satellite_distance[i] *= (3.4 / 1000);
		}
		break;
		// 火星まで表示
	case 1:
		planet_radius[0] /= 50;
		for (int i = 0; i < planet_num; i++)
		{
			if (i < 5)
			{
				planet_radius[i]   *= 10.0;
				planet_distance[i] *= 60.0;
			}
			else
			{
				planet_radius[i]   *= 0.0;
				planet_distance[i] *= 0.0;
			}
		}

		for (int i = 0; i < satellite_num; i++)
		{
			if (i < 3)
			{
				// 惑星と衛星のスケールを合わせるため、で10^5、10^3で割る
				satellite_radius[i]   *= (10.0 / 100000);
				satellite_distance[i] *= (60.0 / 1000);
			}
			else
			{
				satellite_radius[i]   *= 0.0;
				satellite_distance[i] *= 0.0;
			}
		}
		break;
		// 衛星を非表示
	case 2:
		planet_radius[0] /= 100;
		for (int i = 0; i < planet_num; i++)
		{
			planet_radius[i]   *= 0.9;
			planet_distance[i] *= 3.8;
		}
		for (int i = 0; i < satellite_num; i++)
		{
			satellite_radius[i] *= 0.0;
		}
		break;
	}
	// 天王星の距離のスケールを調整する
	planet_distance[7] *= 0.8;
	// 海王星の距離のスケールを調整する
	planet_distance[8] *= 0.7;
	// 冥王星の距離のスケールを調整する
	planet_distance[9] *= 0.6;
	// フォボス、ダイモスの大きさと距離のスケールを調整する
	satellite_radius[1] *= 100.00;
	satellite_distance[1] *= 10.00;
	satellite_radius[2] *= 100.00;
	satellite_distance[2] *= 10.00;
	// 木星の衛星の距離のスケールを調整する
	satellite_distance[3] *= 7.00;
	satellite_distance[4] *= 5.00;
	satellite_distance[5] *= 3.00;
	satellite_distance[6] *= 2.00;

	glutInit(&argc, argv);
	myInit(argv[0]);
	glutKeyboardFunc(myKeyboard);
	glutReshapeFunc(myReshape);
	glutTimerFunc(samplingTime, myTimer, 1);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}