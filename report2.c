#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

#define imageWidth  16
#define imageHeight 16

unsigned char texImage[2][imageHeight][imageWidth][4];  // テクスチャのイメージデータ

unsigned char mouseFlag = GL_FALSE;
int xStart, yStart;                 // マウス操作時の始点の座標
int xDist, yDist;                   // マウス操作時の移動距離の座標
double xAngle = 0.0, yAngle = 0.0;  //  X軸とY軸周りの回転角度

static double forefoot_right = 0.0, forefoot_left = 0.0;  // 右前脚と左前脚の回転角度
static double hindfoot_right = 0.0, hindfoot_left = 0.0;  // 右後ろ脚と左後ろ脚の回転角度

static double xTranslation = 0.0;   //  X軸方向の移動量
static int color_of_horse = 0;      // 馬の色を示す
static int size_of_horse = 1;       // 馬の大きさを示す
static int walking_dir;             // 歩行方向

/*  1次元目（0-2）：馬のサイズに対応した3つのサイズオプション
    2次元目（0-7）：馬の各部位に対応した8つの部位
    3次元目（0-2）：各部位のX、Y、Z座標値
*/
static double glt[3][8][3] = {
    {
        { 1.0000,  0.0000, -0.3750},  // 体パーツの座標
        { 1.2500,  0.2500,  0.0000},  // 尾パーツの座標
        {-0.7500,  0.7500,  0.0000},  // 首パーツの座標
        {-1.1750,  1.0625,  0.0000},  // 頭パーツの座標
        {-0.7500, -0.5000, -0.1875},  // 右前脚パーツの座標
        {-0.7500, -0.5000,  0.1875},  // 左前脚パーツの座標
        { 0.7500, -0.5000, -0.1875},  // 後右脚パーツの座標
        { 0.7500, -0.5000,  0.1875}   // 後左脚パーツの座標
    },
    {
        { 1.0000,  0.0000, -0.3750},
        { 2.5000,  0.2500,  0.0000},
        {-1.5000,  1.5000,  0.0000},
        {-2.3750,  2.1250,  0.0000},
        {-1.5000, -1.0000, -0.3750},
        {-1.5000, -1.0000,  0.3750},
        { 1.5000, -1.0000, -0.3750},
        { 1.5000, -1.0000,  0.3750}
    },
    {
        { 1.0000,  0.0000, -0.7500},
        { 5.0000,  0.2500,  0.0000},
        {-3.0000,  3.0000,  0.0000},
        {-4.7500,  4.2500,  0.0000},
        {-3.0000, -2.0000, -0.7500},
        {-3.0000, -2.0000,  0.7500},
        { 3.0000, -2.0000, -0.7500},
        { 3.0000, -2.0000,  0.7500}
    }
};

/*  1次元目（0-2）：馬のサイズに対応した3つのサイズオプション
    2次元目（0-7）：馬の各部位に対応した8つの部位
    3次元目（0-2）：各部位のスケーリング係数（X、Y、Z方向のスケール値）
*/
static double gls[3][8][3] = {
    {
        {2.000, 0.750, 0.750},  // 体パーツのスケール
        {0.500, 0.100, 0.100},  // 尾パーツのスケール
        {0.375, 1.000, 0.375},  // 首パーツのスケール
        {0.500, 0.375, 0.375},  // 頭パーツのスケール
        {0.250, 1.000, 0.250},  // 右前脚パーツのスケール
        {0.250, 1.000, 0.250},  // 左前脚パーツのスケール
        {0.250, 1.000, 0.250},  // 後右脚パーツのスケール
        {0.250, 1.000, 0.250}   // 後左脚パーツのスケール
    },
    {
        {4.000, 1.500, 1.500},
        {1.000, 0.200, 0.200},
        {0.750, 2.000, 0.750},
        {1.000, 0.750, 0.750},
        {0.500, 2.000, 0.500},
        {0.500, 2.000, 0.500},
        {0.500, 2.000, 0.500},
        {0.500, 2.000, 0.500}
    },
    {
        {8.000, 3.000, 3.000},
        {2.000, 0.400, 0.400},
        {1.500, 4.000, 1.500},
        {2.000, 1.500, 1.500},
        {1.000, 4.000, 1.000},
        {1.000, 4.000, 1.000},
        {1.000, 4.000, 1.000},
        {1.000, 4.000, 1.000}
    }
};

// : ライトの設定を行う関数
void mySetLight() {
    float light0_position[] = {100.0, 150.0, 100.0, 1.0};   // 位置を指定
    float light0_diffuse[]  = {1.0, 1.0, 1.0, 1.0};         // 拡散反射光の色を指定
    float light0_specular[] = {1.0, 1.0, 1.0, 1.0};         // 鏡面反射光の色を指定

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

// テクスチャのイメージデータを生成する関数
void makeTexImage(void) {
    int zebra;
    for (int i = 0; i < imageHeight; i++) {
        for (int j = 0; j < imageWidth; j++) {
            switch (color_of_horse) {
            case 0:
                zebra = (i % 2 == 0) ? 0 : 255;
                texImage[color_of_horse][i][j][0] = (unsigned char)zebra;
                texImage[color_of_horse][i][j][1] = (unsigned char)zebra;
                texImage[color_of_horse][i][j][2] = (unsigned char)zebra;
                break;
            case 1:
                texImage[color_of_horse][i][j][0] = (unsigned char)255;
                texImage[color_of_horse][i][j][1] = (unsigned char)255;
                texImage[color_of_horse][i][j][2] = (unsigned char)0;
                break;
            case 2:
                texImage[color_of_horse][i][j][0] = (unsigned char)50;
                texImage[color_of_horse][i][j][1] = (unsigned char)255;
                texImage[color_of_horse][i][j][2] = (unsigned char)100;
                break;
            case 3:
                texImage[color_of_horse][i][j][0] = (unsigned char)0;
                texImage[color_of_horse][i][j][1] = (unsigned char)0;
                texImage[color_of_horse][i][j][2] = (unsigned char)254;
                break;
            }
            texImage[color_of_horse][i][j][3] = (unsigned char)255;
        }
    }
}

// テクスチャの設定を行う関数
void setupTextures(void) {
    makeTexImage();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, texImage[color_of_horse]);
    /* automatic mapping */
    glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
}

// ウィンドウの初期化を行う関数
void myInit(char* progname) {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(progname);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
}

// 描画を行う関数
void myDisplay(void) {
    float ambient[]  = {1.0, 1.0, 1.0, 1.0};
    float diffuse[]  = {0.5, 1.0, 0.5, 1.0};
    float specular[] = {0.5, 1.0, 0.5, 1.0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialf(GL_FRONT, GL_SHININESS, 64.0);

    glPushMatrix();
    glRotated(xAngle, 1.0, 0.0, 0.0);
    glRotated(yAngle, 0.0, 1.0, 0.0);

    glTranslated(xTranslation, 0.0, 0.0);

    // 体パーツの描画
    glRotated(0.0, 0.0, 0.0, 0.0);
    glTranslated(glt[size_of_horse][0][0], glt[size_of_horse][0][1], glt[size_of_horse][0][2]);
    glPushMatrix();
    glScaled(gls[size_of_horse][0][0], gls[size_of_horse][0][1], gls[size_of_horse][0][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    // 尾パーツの描画
    glPushMatrix();
    glTranslated(glt[size_of_horse][1][0], glt[size_of_horse][1][1], glt[size_of_horse][1][2]);
    glRotated(0.0, 0.0, 0.0, 0.0);
    glPushMatrix();
    glScaled(gls[size_of_horse][1][0], gls[size_of_horse][1][1], gls[size_of_horse][1][2]);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    // 首パーツの描画
    glPushMatrix();
    glTranslated(glt[size_of_horse][2][0], glt[size_of_horse][2][1], glt[size_of_horse][2][2]);
    glRotated(0.0, 0.0, 0.0, 0.0);
    glPushMatrix();
    glScaled(gls[size_of_horse][2][0], gls[size_of_horse][2][1], gls[size_of_horse][2][2]);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    // 頭パーツの描画
    glPushMatrix();
    glTranslated(glt[size_of_horse][3][0], glt[size_of_horse][3][1], glt[size_of_horse][3][2]);
    glRotated(0.0, 0.0, 0.0, 0.0);
    glPushMatrix();
    glScaled(gls[size_of_horse][3][0], gls[size_of_horse][3][1], gls[size_of_horse][3][2]);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    // 右前脚パーツの描画
    glPushMatrix();
    glRotated((double)forefoot_right, 0.0, 0.0, 1.0);
    glTranslated(glt[size_of_horse][4][0], glt[size_of_horse][4][1], glt[size_of_horse][4][2]);
    glScaled(gls[size_of_horse][4][0], gls[size_of_horse][4][1], gls[size_of_horse][4][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    // 左前脚パーツの描画
    glPushMatrix();
    glRotated((double)forefoot_left, 0.0, 0.0, 1.0);
    glTranslated(glt[size_of_horse][5][0], glt[size_of_horse][5][1], glt[size_of_horse][5][2]);
    glScaled(gls[size_of_horse][5][0], gls[size_of_horse][5][1], gls[size_of_horse][5][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    // 右後ろ脚パーツの描画
    glPushMatrix();
    glRotated((double)hindfoot_right, 0.0, 0.0, 1.0);
    glTranslated(glt[size_of_horse][6][0], glt[size_of_horse][6][1], glt[size_of_horse][6][2]);
    glScaled(gls[size_of_horse][6][0], gls[size_of_horse][6][1], gls[size_of_horse][6][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    // 左後ろ脚パーツの描画
    glPushMatrix();
    glRotated((double)hindfoot_left, 0.0, 0.0, 1.0);
    glTranslated(glt[size_of_horse][7][0], glt[size_of_horse][7][1], glt[size_of_horse][7][2]);
    glScaled(gls[size_of_horse][7][0], gls[size_of_horse][7][1], gls[size_of_horse][7][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    glFlush();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
}

// ウィンドウのサイズ変更時に呼び出される関数
void myReshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-5.0, 5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// アニメーションの更新を行う関数
void updateAnimation(int value) {
    static int walk_count = 0, frame = 50;  // 歩行のカウントとフレーム数を定義
    int how2walk[50][4];                    // 歩行パターンを格納する
    srand(time(NULL));

    // -15°から15°までの角度でランダムに生成
    for (int i = 0; i < frame; i++) {
        for (int j = 0; j < 4; j++) {
            how2walk[i][j] = rand() % 31 - 15;
        }
    }

    // 歩く処理
    if (walk_count < frame) {
        forefoot_right = -how2walk[abs(walk_count)][0];
        forefoot_left = -how2walk[abs(walk_count)][1];
        hindfoot_right = -how2walk[abs(walk_count)][2];
        hindfoot_left = -how2walk[abs(walk_count)][3];
        walk_count += 1;
        xTranslation = xTranslation + walking_dir * 0.2;
    }

    // 馬の位置をリセット
    else {
        walk_count = -frame;
        xTranslation = (walking_dir == -1) ? 10.0 : -10.0;
    }

    glutPostRedisplay();
    glutTimerFunc(100, updateAnimation, value);
}

// キーボードイベントを処理する関数
void myKeyboard(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);
    else if (key == 'f') {
        walking_dir = -1;
        updateAnimation(walking_dir);
    }
    else if (key == 'b') {
        walking_dir = 1;
        updateAnimation(walking_dir);
    }
}

// マウスの移動イベントを処理する関数
void myMouseMotion(int x, int y) {
    xDist = x - xStart;
    yDist = y - yStart;

    xAngle += (double)yDist * 0.3;
    yAngle += (double)xDist * 0.3;

    xStart = x;
    yStart = y;
    glutPostRedisplay();
}

// マウスのボタンイベントを処理する関数
void myMouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        xStart = x;
        yStart = y;
    }
}

// ポップアップメニューの選択イベントを処理する関数
void myMenu(int value) {
    switch (value) {
    case 0:
        size_of_horse = 0;
        break;
    case 1:
        size_of_horse = 1;
        break;
    case 2:
        size_of_horse = 2;
        break;
    case 3:
        color_of_horse = 0;
        break;
    case 4:
        color_of_horse = 1;
        break;
    case 5:
        color_of_horse = 2;
        break;
    case 6:
        color_of_horse = 3;
        break;
    default:
        break;
    }
    setupTextures();
    glutPostRedisplay();
}

// ポップアップメニューを設定する関数
void mySetMenu() {
    int menu = glutCreateMenu(myMenu);
    glutAddMenuEntry("x 0.5", 0);
    glutAddMenuEntry("x 1.0", 1);
    glutAddMenuEntry("x 2.0", 2);
    glutAddMenuEntry("color: zebra", 3);
    glutAddMenuEntry("color: blue", 4);
    glutAddMenuEntry("color: red", 5);
    glutAddMenuEntry("color: yello", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    myInit(argv[0]);
    setupTextures();
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouseFunc);
    glutMotionFunc(myMouseMotion);
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    mySetLight();
    mySetMenu();
    glutMainLoop();
    return 0;
}
