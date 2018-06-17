#include "TXLib.h"

const COLORREF SINCOLOR = RGB (0, 255, 0);
const COLORREF COSCOLOR = RGB (255, 0, 0);
const int arrayAmount = 8;

enum BUTTONHIT {MISS = 0, HIT = 1};
enum BUTTONTYPE {EXIT = 0, FUNC = 1, ERASE = 2, OVERFUNC = 3}; //overfunc работает с функцией(рисованием)



struct Button
    {
    int x;
    int y;
    int sizeX;
    int sizeY;
    const char* funcName;
    double (*func) (double x, const std::vector <double>* parameters);
    int paramAmount;
    COLORREF Color;
    BUTTONTYPE type;



    void draw();
    };


double invertProportionality (double x, const std::vector <double>* parameters);
double square                (double x, const std::vector <double>* parameters);
double sqrt1                 (double x, const std::vector <double>* parameters);
double line                  (double x, const std::vector <double>* parameters);
double sin1                  (double x, const std::vector <double>* parameters);
double cos1                  (double x, const std::vector <double>* parameters);


void DoFunc(double *x, double *y);
//net
void netDraw();
//Checking buttons
BUTTONHIT checkMouse (Button button);
//drawing
int drawAndPushButtons (Button buttons[], const int arrayAmount);

int main()
    {
    _fpreset();
    //txCreateWindow (800, 600);
    txCreateWindow (1900, 1000);

    txBegin();

    txSetFillColor(RGB (0, 64, 0));
    txSetColor(RGB (0, 255, 0));


    txRectangle(50, 50, txGetExtentX() - 50, txGetExtentY() - 50);
    netDraw();

    const char* rp1text = "Redraw first Parameter";  //rp stands for redraw parameter
    const char* rp2text = "Redraw second Parameter";
    const char* rp3text = "Redraw third  Parameter";


    int prevParamAmount = 0;


    Button buttons [arrayAmount] = {
                         {10,   10, 90,  20,   "sin",                      &sin1,                  0,               RGB (255, 0, 0)    , FUNC},
                         {110,  10, 90,  20,   "cos",                      &cos1,                  0,               RGB (0, 255, 0)    , FUNC},
                         {210,  10, 90,  20,   "invProp",                  &invertProportionality, 2,               RGB (255, 255, 255), FUNC},
                         {310,  10, 90,  20,   "line",                     &line,                  2,               RGB (0, 255, 255)  , FUNC},
                         {410,  10, 90,  20,   "x^2",                      &square,                3,               RGB (255, 255, 0)  , FUNC},
                         {510,  10, 90,  20,   "sqrt",                     &sqrt1,                 3,               RGB (150, 150, 0)  , FUNC},
                         {610,  10, 90,  20,   "Erase",                    NULL,                   0,               RGB (150, 150, 0)  , ERASE},
                         {710,  10, 90,  20,   "Exit",                     NULL,                   0,               RGB (150, 150, 0)  , EXIT},
                         };
//перевести на вектора

                                  //массив линий по x и y тонкие, толстая в середине


    while (true)
        {
        //
        int returnedValue = drawAndPushButtons (buttons, arrayAmount);

        //printf ("returnedValue = %d\n", returnedValue);
        if (returnedValue == 0)
            return 0;

        //if (returnedValue == 2)
       //

        if (GetAsyncKeyState (VK_ESCAPE))
            break;

        txSleep(100);
        }

    return 0;
    }


void drawGraph (/*double (*func) (double x, double param1, double param2, double param3),*/ Button button)
    {
    double xE1 =  -18;
    double xE2 =   18;

    double x = 0;
    double y = 0;

    const std::vector <double> parameters = {1, 0, 0};


    if (button.paramAmount == 3)
        {
        sscanf (txInputBox ("3 param"),   "%lg %lg %lg",   parameters.at (0), parameters.at (1), parameters.at (2));
        }

    if (button.paramAmount == 2)
        {
        sscanf (txInputBox ("2 param"),   "%lg %lg",       parameters.at (0), parameters.at (1));
        }

    if (button.paramAmount == 1)
        {
        sscanf (txInputBox ("1 param"),   "%lg",           parameters.at (0));
        }

    if (button.paramAmount == 0)
        {//принимаем значение коэффициентов, не используем в синусоиде, косинусоиде
        }

    for (x = xE1; x <= xE2; x += 0.02)
        {
        //printf ("I do work!\n");
        y = button.func(x, &parameters); //вызов функции


        if (std::isfinite(y))
            {
            txSetColor (RGB (255, 255, 255));
            }


        else
            {
            txSetColor (RGB (0, 0, 255));
            y = 0;
            }

        //printf ("x = %lg\n", x);

        DoFunc(&x, &y);
        }
    }

BUTTONHIT checkMouse (Button button)
    {
    if (txMouseX() > button.x && txMouseX() < (button.x + button.sizeX) &&
        txMouseY() > button.y && txMouseY() < (button.y + button.sizeY) &&
        GetAsyncKeyState (MK_LBUTTON))
        {
        return HIT;
        }

    return MISS;
    }

void DoFunc (double *x, double *y)
    {
    double x1 =  (*x)*50 + txGetExtentX()/2;
    double y1 = -(*y)*50 + txGetExtentY()/2;

    if (x1 >= 50 && x1 <= 1850 && y1 >= 50 && y1 <= 950)
        {
        txCircle (x1, y1, 1);
        }
    }

//void Button draw()
    //{
    //txCircle (x*50 + txGetExtentX()/2, -y*50 + txGetExtentY()/2, 1);
    //}

double invertProportionality (double x, const std::vector <double>* parameters)
    {
    double k = parameters -> at (0);
    double b = parameters -> at (1);

    double x1 = k/(x + b);
    return x1;
    }

double square (double x, const std::vector <double>* parameters)
    {
    double a = parameters -> at (0);
    double b = parameters -> at (1);
    double c = parameters -> at (2);

    double x2 = a*x*x + b*x + c;
    return x2;
    }

double line (double x, const std::vector <double>* parameters)
    {
    double k = parameters -> at (0);
    double b = parameters -> at (1);

    //scanf ("%lg", k);eters -> at (0) //sprintf

    //cin << k;
    //cin << b;

    double x3 = k*x + b;

    return x3;
    }

double sqrt1 (double x, const std::vector <double>* parameters)
    {
    double a = parameters -> at (0);
    double b = parameters -> at (1);
    double c = parameters -> at (2);
    double x4 = sqrt((a*x + b)/c);

    return x4;
    }

double cos1 (double x, const std::vector <double>* parameters)
    {
    double x5 = cos(x);

    return x5;
    }

double sin1 (double x, const std::vector <double>* parameters)
    {
    double x6 = sin(x);

    return x6;
    }

int drawAndPushButtons (Button buttons [], const int arrayAmount)
    {
    for (int i = 0; i < arrayAmount; i++)
        {
        txSetColor (RGB (0, 0, 0));
        txSetFillColor (RGB (255, 255, 255));
        txRectangle (buttons[i].x, buttons[i].y, buttons[i].x + buttons[i].sizeX, buttons[i].y + buttons[i].sizeY);
        txDrawText (buttons[i].x, buttons[i].y, buttons[i].x + buttons[i].sizeX, buttons[i].y + buttons[i].sizeY, buttons[i].funcName);
        }

    for (int i = 0; i < arrayAmount; i++)
        {
        //printf ("i = %d", i);
        BUTTONHIT checkResult = checkMouse(buttons[i]);

        int checkResultUsed = int (checkResult);

        //printf("checkMouse = %d\n", checkResultUsed);

        if (checkResult == HIT && buttons[i].type == FUNC)      //////
            {
            //printf ("I do work!\n");
            txSetColor (buttons[i].Color);

            if (buttons[i].func != NULL)
                {
                drawGraph (buttons[i]);
                }

            return 1;
            }

        //printf (("buttons[i].x = %lg", buttons[i].x));
        //printf (("buttons[i].y = %lg", buttons[i].y));
        //printf (("buttons[i].func = %lg", buttons[i].func));

        if (buttons[i].type == ERASE && checkResult == HIT)
            {
            txSetFillColor(RGB (0, 0, 0));

            txClear();

            txSetFillColor(RGB (0, 64, 0));
            txSetColor(RGB (0, 255, 0));
            txRectangle(50, 50, txGetExtentX() - 50, txGetExtentY() - 50);
            netDraw();

            printf ("erasing");

            return 2;
            }

        if (buttons[i].type == EXIT && checkResult == HIT)
            {
            txMessageBox ("Программа Завершена", "graph.exe сообщает");
            printf ("exiting");
            return 0;
            }


        else
            {
            continue;
            }
        }

    return 1;
    }

void netDraw()
    {
    //printf ("I do work!\n");

    txSetColor (RGB (255, 255, 255), 1);

    int xNet = txGetExtentX()/2;
    int yNet = txGetExtentY()/2;

    while (xNet <= txGetExtentX() - 50)
        {
        //
        if (xNet == txGetExtentX()/2)
            txSetColor(RGB (0, 128, 0), 3);
        else
            txSetColor(RGB (0, 128, 0), 1);
        //

        txLine (xNet, txGetExtentY() - 50, xNet, 50);
        xNet += 50;

        }

    while (xNet >= 50)
        {
        txLine (xNet, txGetExtentY() - 50, xNet, 50);
        xNet -= 50;
        }

    while (yNet <= txGetExtentY() - 50)
        {
        //
        if (yNet == txGetExtentY()/2)
            txSetColor(RGB (0, 128, 0), 3);
        else
            txSetColor(RGB (0, 128, 0), 1);
        //

        txLine (txGetExtentX() - 50, yNet, 50, yNet);
        yNet += 50;
        }

    while (yNet >= 50)
        {
        txLine (txGetExtentX() - 50, yNet, 50, yNet);
        yNet -= 50;
        }

    txSetColor (RGB (255, 255, 255), 3);
    txLine(0, yNet/2.0, txGetExtentX(), yNet/2.0);
    txSetColor (RGB (255, 255, 255), 1);
    }
