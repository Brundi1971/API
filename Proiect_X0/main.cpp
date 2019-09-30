#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include "Project_X0.hpp"
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <math.h>
#include <thread>
#include <sstream>



using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void AddControlsAndInitVars(HWND hwnd);
void redrawTheGame(HWND hwnd);
void draw_grid(HWND hwnd);
void gestioneaza_lbmousedown(HWND hwnd);
void gestioneaza_lbmouseup(HWND hwnd);
void reset_or_exit(HWND hwnd, int item);
void redrawAllObjects(HWND hwnd);
void reset_game(HWND hwnd);
void InfoGameOver(void);


struct sCaroiaj{
    HWND hPictogramaX0;
    int val;
    bool bWinFigure;
    POINT pLeftTop;
    POINT pRightDown;
};

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("X and 0 Game");
HWND                    hwnd, hTabla;
HBRUSH                  grayBrush;
int                     MainWinWidth, MainWinHeight, MainWinWidthEx, MainWinHeightEx, intlGrid, intlTabla, noOfPictogrameX0;
int                     xInitPos, yInitPos, xPos, yPos, xDelta, yDelta, xPosCareu, yPosCareu;
bool                    bRedrawTheGrid;
tagMINMAXINFO           * tagInfo;
sCaroiaj                M[NO_LINII][NO_COLOANE];
bool                    triggerXand0, bGameOver;
HFONT                   hfX, hf0;
std::thread             tThread;
char                    cWinner;



int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    //HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("X and 0 Game"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           300,       /* Windows decides the position */
           100,       /* where the window ends up on the screen */
           WIN_MIN_W,                 /* The programs width */
           WIN_MIN_H,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    bRedrawTheGrid = false;

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
        if (bRedrawTheGrid) {draw_grid(hwnd);}
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i,j;


    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            AddControlsAndInitVars(hwnd);
            break;
        case WM_LBUTTONDOWN:
            xInitPos = GET_X_LPARAM(lParam);
            yInitPos = GET_Y_LPARAM(lParam);
            gestioneaza_lbmousedown(hwnd);
            break;
        case WM_LBUTTONUP:
            xInitPos = GET_X_LPARAM(lParam);
            yInitPos = GET_Y_LPARAM(lParam);
            gestioneaza_lbmouseup(hwnd);
            break;
        case WM_COMMAND:
            reset_or_exit(hwnd, LOWORD(wParam));
            break;
        case WM_SIZE:
            MainWinWidth = LOWORD(lParam);
            MainWinHeight = HIWORD(lParam);
            redrawTheGame(hwnd);
            break;
        case WM_CTLCOLORSTATIC:
            //cout << "WM_CTLCOLORSTATIC: " << (HWND)lParam << endl;
            if (hTabla == (HWND)lParam){bRedrawTheGrid = true;}
            for (i=0;i<NO_LINII;i++){
                for (j=0;j<NO_COLOANE;j++){
                    if ((M[i][j].hPictogramaX0 == (HWND)lParam) && (M[i][j].bWinFigure)) {
                        SetBkMode( (HDC)wParam, TRANSPARENT );
                        SetTextColor((HDC)wParam, COLOR_X0_GAMEOVER);
                    }else if((M[i][j].hPictogramaX0 == (HWND)lParam) && (!M[i][j].bWinFigure)){
                        SetBkMode( (HDC)wParam, TRANSPARENT );
                        if  (M[i][j].val==1){SetTextColor((HDC)wParam, COLOR_X);}
                        else {SetTextColor((HDC)wParam, COLOR_0);}
                    }
                }
            }
            return (INT_PTR)grayBrush;
            break;
        case WM_ERASEBKGND:
            //cout << "WM_ERASEBKGND" << endl;
            return DefWindowProc (hwnd, message, wParam, lParam);
            break;
        case WM_GETMINMAXINFO:
            tagInfo = (tagMINMAXINFO*)lParam;
            //tagInfo->ptMaxSize.x = 1000;
            //tagInfo->ptMaxSize.y = 500;
            //tagInfo->ptMaxPosition.x = 400;
            //tagInfo->ptMaxPosition.y = 200;
            //tagInfo->ptMaxTrackSize.x = 1000;
            //tagInfo->ptMaxTrackSize.y = 500;
            tagInfo->ptMinTrackSize.x = WIN_MIN_W;
            tagInfo->ptMinTrackSize.y = WIN_MIN_H;
            return DefWindowProc (hwnd, message, wParam, lParam);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}


void AddControlsAndInitVars(HWND hwnd){

    hTabla = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, 0, 0, (int)(WIN_W/FACTOR_SCALARE), (int)(WIN_W/FACTOR_SCALARE), hwnd, NULL, NULL, NULL);//| SS_BITMAP
    grayBrush = CreateSolidBrush(COLOR_FUNDAL);
    noOfPictogrameX0 = 0;
    triggerXand0 = true;

    HMENU hMenubar = CreateMenu();

    AppendMenu(hMenubar, MF_STRING, (UINT_PTR)1, "Reset");
    AppendMenu(hMenubar, MF_STRING, (UINT_PTR)2, "Exit");

    SetMenu(hwnd, hMenubar);

    //hfX = CreateFont(70, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
    //hf0 = CreateFont(70, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");

    reset_game(hwnd);
    redrawTheGame(hwnd);
}


void reset_game(HWND hwnd){
    int i,j;

    for (i=0;i<NO_LINII;i++){
        for (j=0;j<NO_COLOANE;j++){
            DestroyWindow(M[i][j].hPictogramaX0);
            M[i][j].hPictogramaX0 = 0;
            M[i][j].val = 0;
            M[i][j].bWinFigure = false;
    }}
    triggerXand0 = true;
    bGameOver = false;
    InvalidateRect(hwnd,NULL,true);
    //UpdateWindow(hwnd);
}


void reset_or_exit(HWND hwnd, int item){
    if(item == 1){reset_game(hwnd);}
    if(item == 2){DestroyWindow(hwnd);}
}


void redrawAllObjects(HWND hwnd){

    int i, j;

    for (i=0;i<NO_LINII;i++){
        for (j=0;j<NO_COLOANE;j++){
            if ((M[i][j].val==1)&&(M[i][j].hPictogramaX0==0)){
                M[i][j].hPictogramaX0 = CreateWindow(TEXT("STATIC"), TEXT("X"), WS_VISIBLE | WS_CHILD , M[i][j].pLeftTop.x+2, M[i][j].pLeftTop.y+2, intlGrid-3, intlGrid-3, hwnd, NULL, NULL, NULL);
                SendMessage(M[i][j].hPictogramaX0, WM_SETFONT, (WPARAM)hfX, true);
                InvalidateRect(M[i][j].hPictogramaX0,NULL,true);
                //SetWindowPos(M[i][j].hPictogramaX0, HWND_TOP, MCorners[i][j].LeftTop.x+2, MCorners[i][j].LeftTop.y+2, (int)(intlCareu/3-4), (int)(intlCareu/3-8), SWP_SHOWWINDOW | SWP_NOZORDER);

            }else if ((M[i][j].val==2)&&(M[i][j].hPictogramaX0==0)){
                M[i][j].hPictogramaX0 = CreateWindow(TEXT("STATIC"), TEXT("0"), WS_VISIBLE | WS_CHILD , M[i][j].pLeftTop.x+2, M[i][j].pLeftTop.y+2, intlGrid-3, intlGrid-3, hwnd, NULL, NULL, NULL);
                SendMessage(M[i][j].hPictogramaX0, WM_SETFONT, (WPARAM)hf0, true);
                InvalidateRect(M[i][j].hPictogramaX0,NULL,true);
                //SetWindowPos(M[i][j].hPictogramaX0, HWND_TOP, MCorners[i][j].LeftTop.x+2, MCorners[i][j].LeftTop.y+2, (int)(intlCareu/3-4), (int)(intlCareu/3-8), SWP_SHOWWINDOW | SWP_NOZORDER);
            }
        }
    }
}

void gestioneaza_lbmousedown(HWND hwnd){

    int i, j;

    if (bGameOver) return;

    for (i=0;i<NO_LINII;i++){
        for (j=0;j<NO_COLOANE;j++){
            if ((M[i][j].pLeftTop.x < xInitPos) && (xInitPos < M[i][j].pRightDown.x) &&
                (M[i][j].pLeftTop.y < yInitPos) && (yInitPos < M[i][j].pRightDown.y)){
                if ((M[i][j].val==0)&&(triggerXand0)) {M[i][j].val=1;triggerXand0 = !triggerXand0;}
                else if ((M[i][j].val==0)&&(!(triggerXand0))) {M[i][j].val=2;triggerXand0 = !triggerXand0;}
            }
        }
        //cout << "[" << i << "][" << j << "] = " << MCorners[i][j].LeftTop.x << " : " << MCorners[i][j].LeftTop.y << endl;
        //cout << "[" << i << "][" << j << "] = " << MCorners[i][j].RightDown.x << " : " << MCorners[i][j].RightDown.y << endl << endl;
    }
    redrawAllObjects(hwnd);
}

void gestioneaza_lbmouseup(HWND hwnd){

    int i,j;

    if(bGameOver) return;

    for (i=0;i<NO_LINII;i++){
       if ((M[i][0].val == M[i][1].val) && (M[i][0].val == M[i][2].val) && (M[i][0].val != 0)){
            M[i][0].bWinFigure=true;
            M[i][1].bWinFigure=true;
            M[i][2].bWinFigure=true;
            bGameOver=true;
            if (M[i][0].val==1){cWinner = 'X';} else{cWinner = '0';}
        }
    }
    for (j=0;j<NO_COLOANE;j++){
       if ((M[0][j].val == M[1][j].val) && (M[0][j].val == M[2][j].val) && (M[0][j].val != 0)){
            M[0][j].bWinFigure=true;
            M[1][j].bWinFigure=true;
            M[2][j].bWinFigure=true;
            bGameOver=true;
            if (M[0][j].val==1){cWinner = 'X';} else{cWinner = '0';}

       }
    }
    if ((M[0][0].val == M[1][1].val) && (M[0][0].val == M[2][2].val) && (M[0][0].val != 0)){
            M[0][0].bWinFigure=true;
            M[1][1].bWinFigure=true;
            M[2][2].bWinFigure=true;
            bGameOver=true;
            if (M[0][0].val==1){cWinner = 'X';} else{cWinner = '0';}
    }
    if ((M[0][2].val == M[1][1].val) && (M[0][2].val == M[2][0].val) && (M[0][2].val != 0)){
            M[0][2].bWinFigure=true;
            M[1][1].bWinFigure=true;
            M[2][0].bWinFigure=true;
            bGameOver=true;
            if (M[0][0].val==1){cWinner = 'X';} else{cWinner = '0';}
    }

    if(bGameOver){
            InvalidateRect(hwnd,NULL,true);
            tThread = thread(InfoGameOver);
            tThread.detach();
    }

/*

    for (i=0;i<NO_LINII;i++){
        for (j=0;j<NO_COLOANE;j++){
        cout << "[" << i << "][" << j << "] = " << M[i][j].val << " : " << M[i][j].hPictogramaX0 << " : " << M[i][j].bGameOver <<endl;
    }}
    cout << endl;
*/
}

void InfoGameOver(void){

    std::ostringstream      msg;

    msg.clear();
    msg << "GAME OVER !\nThe Winner of Game is " << cWinner << " !";
    MessageBox(hwnd, &msg.str()[0],"INFO !",MB_OK | MB_ICONINFORMATION);
}


void redrawTheGame(HWND hwnd){

    int i, j, intFontHeight, intFontWidthX, intFontWidth0;
    double k1,k2,k3;

    DestroyWindow(hTabla);

    if (MainWinWidth/FACTOR_SCALARE < MainWinHeight) {intlTabla = (int)(MainWinWidth/FACTOR_SCALARE);}
    else{intlTabla = MainWinHeight;}

    xPosCareu = (MainWinWidth - intlTabla)/2;
    yPosCareu = (MainWinHeight - intlTabla)/2;

    intlGrid = (int)(intlTabla/3);

    for (i=0;i<NO_LINII;i++){
      for (j=0;j<NO_COLOANE;j++){
        M[i][j].pLeftTop.x=xPosCareu+j*intlGrid;
        M[i][j].pLeftTop.y=yPosCareu+i*intlGrid;
        M[i][j].pRightDown.x=xPosCareu+(j+1)*intlGrid;
        M[i][j].pRightDown.y=yPosCareu+(i+1)*intlGrid;
        //cout << "[" << i << "][" << j << "] = " << MCorners[i][j].LeftTop.x << " : " << MCorners[i][j].LeftTop.y << endl;
        //cout << "[" << i << "][" << j << "] = " << MCorners[i][j].RightDown.x << " : " << MCorners[i][j].RightDown.y << endl << endl;
      }
    }
    hTabla = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, xPosCareu, yPosCareu, intlTabla, intlTabla, hwnd, NULL, NULL, NULL);

    for (i=0;i<NO_LINII;i++){
        for (j=0;j<NO_COLOANE;j++){
            DestroyWindow(M[i][j].hPictogramaX0);
            M[i][j].hPictogramaX0 = 0;
            //M[i][j].val = 0;
            //M[i][j].bGameOver = false;
    }}

    DeleteObject(hfX);
    DeleteObject(hf0);

    k1=1;
    k2=0.65;
    k3=0.8;
    intFontHeight = trunc((double)k1*intlGrid);
    intFontWidthX = trunc((double)k2*intlGrid);
    intFontWidth0 = trunc((double)k3*intlGrid);

    hfX = CreateFont(intFontHeight, intFontWidthX, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
    hf0 = CreateFont(intFontHeight, intFontWidth0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
    redrawAllObjects(hwnd);
}



void draw_grid(HWND hwnd){

    int i, j, x, y;
    HDC hdc;
    COLORREF color = COLOR_GRID;

    hdc = GetDC(hTabla);

    for (i=0;i<2;i++){
        for (x=0;x<intlTabla;x++){
            SetPixel(hdc,x,(i+1)*intlGrid,color);
            SetPixel(hdc,x,(i+1)*intlGrid-1,color);
            SetPixel(hdc,x,(i+1)*intlGrid+1,color);
    }}


    for (j=0;j<2;j++){
        for (y=0;y<intlTabla;y++){
            SetPixel(hdc,(j+1)*intlGrid,y,color);
            SetPixel(hdc,(j+1)*intlGrid-1,y,color);
            SetPixel(hdc,(j+1)*intlGrid+1,y,color);
    }}

    bRedrawTheGrid=false;
}

