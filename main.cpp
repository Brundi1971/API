#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "GUI_TurnurileDinHanoi.hpp"

using namespace std;


typedef struct Disc{
    HWND    href;
    POINT   Pos;
    POINT   Dim;
    bool    bIsDragAndDrop;
};

/*  Make the class name into a global variable  */
TCHAR                               szClassName[ ] = _T("TurnurileDinHanoiApp");
HWND                                hwnd, hDiscBig, hDiscMid, hDiscLit, hBtn, hBase, hBat1, hBat2, hBat3, hText, hText1;
int                                 xInitPos, yInitPos, xPos, yPos, xDelta, yDelta;
int                                 MainWinWidth, MainWinHeight, BazaWidth, BazaHeight, BazaTop, BazaLeft,
                                    BatLeft[NO_BETE], BatTop, BatHeight, DiscWidth[NO_DISCURI], DiscHeight,
                                    proximityWidth, discClearence;
HBRUSH                              redBrush, greenBrush, blueBrush, blackBrush, discBrush[NO_MAX_DISCURI];
int                                 M[NO_BETE][NO_MAX_DISCURI], no_discuri;
HWND                                hBat[NO_BETE];
Disc                                Discuri[NO_MAX_DISCURI];
bool                                MPrev[NO_MAX_DISCURI];
HBITMAP                             hIcon;
tagMINMAXINFO                       * tagInfo;
HMENU                               hMeniu;



/**
*
*       semnificatia indexului matricei Discuri
*
*       0 - discul cu latimea cea mai mica
*       n - discul cu latimea cea mai mare
*
*
**/


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
void CALLBACK catchInt(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime);
void AddControls(HWND hwnd);
void initialize(void);
void calculate_position_of_the_disc_accordingTo_M(void);
void calculate_the_proximity(void);
void verify_if_can_start_the_Drag_of_Disc(void);
void gestioneaza_lbmousedown(HWND hwnd);
void gestioneaza_lbmousemove(HWND hwnd);
void gestioneaza_lbmouseup(HWND hwnd);
void SetHook();
void display_matrix(void);
void calculate_identity(void);
void GeneratorSignal(void);
void resize_windows(HWND hwnd);
void calculate_element_dimensions_based_on_MainWin_dimension(void);
void create_discs(HWND hwnd);
void rebuild_the_game(HWND hwnd, int item);
void distroy_discs(void);


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    //hIcon = LoadImage("dbgrun.png");
    hIcon = (HBITMAP)::LoadImage(NULL, "turn.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    //if (hIcon == 0){std::cout  << "Nu s-a incarcat icon-ul!\n";}
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;// | WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = (HICON)hIcon;//LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = (HICON)hIcon; //LoadIcon (NULL, IDI_APPLICATION);
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
           0,//WS_OVERLAPPEDWINDOW&~WS_THICKFRAME,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Turnurile din Hanoi"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           WIN_L,               /* Windows decides the position */
           WIN_T,               /* where the window ends up on the screen */
           WIN_W,               /* The programs width */
           WIN_H,               /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    SetHook();
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            AddControls(hwnd);
            break;
        case WM_LBUTTONDOWN:
            xInitPos = GET_X_LPARAM(lParam);
            yInitPos = GET_Y_LPARAM(lParam);
            gestioneaza_lbmousedown(hwnd);
            break;
        case WM_LBUTTONUP:
        case WM_NCLBUTTONUP:
            xInitPos = GET_X_LPARAM(lParam);
            yInitPos = GET_Y_LPARAM(lParam);
            gestioneaza_lbmouseup(hwnd);
            break;
        case WM_MOUSEMOVE:
            xPos = GET_X_LPARAM(lParam);
            yPos = GET_Y_LPARAM(lParam);
            xDelta = xPos - xInitPos;
            yDelta = yPos - yInitPos;
            gestioneaza_lbmousemove(hwnd);
            xInitPos = xPos;
            yInitPos = yPos;
            break;
        case WM_CTLCOLORSTATIC:
            for(size_t i=0; i<no_discuri; i++){
                if (Discuri[i].href == (HWND)lParam) return (INT_PTR)discBrush[i];}
            for(size_t i=0; i<NO_BETE; i++){
                if(hBat[i] == (HWND)lParam) return (INT_PTR)blackBrush;}
            if (hBase == (HWND)lParam) {return (INT_PTR)blackBrush;}
            break;
        case WM_SIZE:
            MainWinWidth = LOWORD(lParam);
            MainWinHeight = HIWORD(lParam);
            resize_windows(hwnd);
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
        case WM_COMMAND:
            //cout << LOWORD(wParam) <<endl;
            rebuild_the_game(hwnd, LOWORD(wParam));
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}


void AddControls(HWND hwnd){

    no_discuri = NO_DISCURI;
    initialize();
    MainWinWidth = WIN_W-16;
    MainWinHeight = WIN_H-40;
    //resize_windows(hwnd);
    calculate_element_dimensions_based_on_MainWin_dimension();
    calculate_position_of_the_disc_accordingTo_M();


    hBase = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, BazaLeft, BazaTop, BazaWidth, BazaHeight, hwnd, NULL, NULL, NULL);
    for(size_t i=0; i<NO_BETE; i++){
        hBat[i] = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, BatLeft[i], BatTop, BAT_W, BatHeight, hwnd, NULL, NULL, NULL);}
    blackBrush = CreateSolidBrush(RGB(50,50,50));
    create_discs(hwnd);

    //hMeniu = CreateMenu();

    HMENU hMenubar = CreateMenu();
    HMENU hMenu = CreateMenu();

    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "Optiuni");
    AppendMenu(hMenu, MF_STRING, 3, "3 Discuri");
    AppendMenu(hMenu, MF_STRING, 4, "4 Discuri");
    AppendMenu(hMenu, MF_STRING, 5, "5 Discuri");
    AppendMenu(hMenu, MF_STRING, 6, "6 Discuri");
    AppendMenu(hMenu, MF_STRING, 7, "7 Discuri");
    AppendMenu(hMenu, MF_STRING, 8, "8 Discuri");

    SetMenu(hwnd, hMenubar);
}


void rebuild_the_game(HWND hwnd, int item){

    distroy_discs();
    no_discuri = item;
    initialize();
    calculate_element_dimensions_based_on_MainWin_dimension();
    calculate_position_of_the_disc_accordingTo_M();
    create_discs(hwnd);
//cout << item << endl;
}

void distroy_discs(void){

    for(size_t i=0; i<no_discuri; i++){DestroyWindow(Discuri[i].href);}
    for(size_t i=0; i<no_discuri; i++){CloseHandle(discBrush[i]);}
}


void create_discs(HWND hwnd){

    for(size_t i=0; i<no_discuri; i++){
        Discuri[i].href = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, Discuri[i].Pos.x, Discuri[i].Pos.y, Discuri[i].Dim.x, Discuri[i].Dim.y, hwnd, NULL, NULL, NULL);}
    for(size_t i=0; i<no_discuri; i++){
        discBrush[i] = CreateSolidBrush(RGB((((i+1)*COEF_CULOARE)%255),(((i+2)*COEF_CULOARE)%255),(((i+3)*COEF_CULOARE)%255)));}
}


void gestioneaza_lbmousedown(HWND hwnd){
    verify_if_can_start_the_Drag_of_Disc();
    //display_matrix();
}

void gestioneaza_lbmousemove(HWND hwnd){
    for(size_t i=0; i<no_discuri; i++){
        if(Discuri[i].bIsDragAndDrop){
            Discuri[i].Pos.x = Discuri[i].Pos.x + xDelta;
            Discuri[i].Pos.y = Discuri[i].Pos.y + yDelta;
            SetWindowPos(Discuri[i].href, HWND_TOP, Discuri[i].Pos.x, Discuri[i].Pos.y, Discuri[i].Dim.x, Discuri[i].Dim.y, SWP_SHOWWINDOW | SWP_NOZORDER);
            InvalidateRect(Discuri[i].href,NULL,true);
            break;
        }
    }
}

void gestioneaza_lbmouseup(HWND hwnd){

    calculate_the_proximity();
    calculate_position_of_the_disc_accordingTo_M();

    //display_matrix();

    for(size_t i=0; i<no_discuri; i++){
        if(Discuri[i].bIsDragAndDrop){
            SetWindowPos(Discuri[i].href, HWND_TOP, Discuri[i].Pos.x, Discuri[i].Pos.y, Discuri[i].Dim.x, Discuri[i].Dim.y, SWP_SHOWWINDOW | SWP_NOZORDER);
            InvalidateRect(Discuri[i].href,NULL,true);
            Discuri[i].bIsDragAndDrop = false;
        }
    }
    //display_matrix();
    calculate_identity();

    //RECT rect;
    //GetClientRect(hwnd,&rect);
    //std::cout << "w:" << rect.bottom-rect.top << endl;
}

void initialize(void){
    for (size_t i=0;i<no_discuri;i++){
        MPrev[i]=false;
        Discuri[i].bIsDragAndDrop=false;
        Discuri[i].Pos.x=0;
        Discuri[i].Pos.y=0;
        Discuri[i].Dim.x=DISC_W(i);
        Discuri[i].Dim.y=DISC_H;
    }
    for (size_t i=0;i<NO_BETE;i++){
        for (size_t j=0;j<no_discuri;j++){
            if (i==0){M[i][j] = no_discuri-j;}
            else M[i][j] = 0;
        }
    }
    MPrev[0]=true;
}

void resize_windows(HWND hwnd){

    calculate_element_dimensions_based_on_MainWin_dimension();
    calculate_position_of_the_disc_accordingTo_M();

    InvalidateRect(hBase,NULL,true);
    SetWindowPos(hBase, HWND_TOP, BazaLeft, BazaTop, BazaWidth, BazaHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
    for(size_t i=0; i<NO_BETE; i++){
        InvalidateRect(hBat[i],NULL,true);
        SetWindowPos(hBat[i], HWND_TOP, BatLeft[i], BatTop, BAT_W, BatHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
    }
    for(size_t i=0; i<no_discuri; i++){
        InvalidateRect(Discuri[i].href,NULL,true);
        SetWindowPos(Discuri[i].href, HWND_TOP, Discuri[i].Pos.x, Discuri[i].Pos.y, Discuri[i].Dim.x, Discuri[i].Dim.y, SWP_SHOWWINDOW | SWP_NOZORDER);
    }
    //RECT rect;
    //GetClientRect(hwnd, &rect);
    //std::cout << "W: " << rect.right-rect.left << endl;
    //std::cout << "H: " << rect.bottom-rect.top << endl;
}

void calculate_element_dimensions_based_on_MainWin_dimension(void){
    BazaTop = static_cast<int>((MainWinHeight*8)/10);
    BazaLeft = static_cast<int>((MainWinWidth)/10);
    BazaWidth = static_cast<int>((MainWinWidth*8)/10);
    BazaHeight = static_cast<int>(MainWinHeight/20);

    BatHeight = static_cast<int>((MainWinHeight*5)/10);
    BatTop = BazaTop - BatHeight;

    for (size_t i=0;i<NO_BETE;i++){
        BatLeft[i] = BazaLeft + static_cast<int>((i+1)*(BazaWidth/(NO_BETE+1)));
    }

    DiscHeight = static_cast<int>((8*BatHeight)/(10*no_discuri));
    discClearence=static_cast<int>(BazaWidth/((no_discuri+1)*(NO_BETE+1)));

    for (size_t i=0;i<no_discuri;i++){
        Discuri[i].Dim.x = static_cast<int>(BazaWidth/(NO_BETE+1) - (no_discuri-i-1)*discClearence);
        Discuri[i].Dim.y = DiscHeight;
    }
    proximityWidth = static_cast<int>(BazaWidth/8 - 10);
}


void calculate_position_of_the_disc_accordingTo_M(void){

    size_t xref;

    for (size_t i=0;i<NO_BETE;i++){
        for (size_t j=0;j<no_discuri;j++){
            xref = BatLeft[i];
            if (M[i][j] != 0){
                Discuri[M[i][j]-1].Pos.x = (int)(xref - Discuri[M[i][j]-1].Dim.x/2 + BAT_W/2);
                Discuri[M[i][j]-1].Pos.y = (int)(BazaTop - (j+1)*DiscHeight);
            }
        }
    }
}

void calculate_the_proximity(void){
    size_t i,j,k,l, intNextBat, intPrevBat;

    for (k=0;k<NO_BETE;k++){
        if (((xInitPos >= BatLeft[k] - proximityWidth) && (xInitPos <= BatLeft[k] + proximityWidth)) &&
            ((yInitPos >= BatTop) && (yInitPos <= BatTop + BatHeight)))break;
    }
    intNextBat = k;
    if (intNextBat<NO_BETE){
        for (l=0;l<no_discuri;l++){
            if(Discuri[l].bIsDragAndDrop) break;
        }
        if (l<no_discuri){
            for (i=0;i<NO_BETE;i++){
                for (j=0;j<no_discuri;j++){
                    if (M[i][j] == l+1) goto LABEL_PROXIMITY;
                }
            }
LABEL_PROXIMITY:
        intPrevBat = i;
            if (intPrevBat!=intNextBat){
                //M[i][j] = 0;
                for (k=0;k<no_discuri;k++){
                    if (M[intNextBat][k] == 0) {
                        if (k>0) {
                            if (M[intNextBat][k-1]>l+1) {
                                M[i][j] = 0;
                                M[intNextBat][k]=l+1;
                            }
                        }else{
                            M[i][j] = 0;
                            M[intNextBat][k]=l+1;
                        }
                    }
                }
            }
        }
    }
}


void verify_if_can_start_the_Drag_of_Disc(void){
    size_t i,j,k;

    for(k=0; k<no_discuri; k++){
        if (((xInitPos >= Discuri[k].Pos.x) && (xInitPos <= Discuri[k].Pos.x+Discuri[k].Dim.x)) && ((yInitPos >= Discuri[k].Pos.y) && (yInitPos <= Discuri[k].Pos.y+Discuri[k].Dim.y))){
            if (k==0){Discuri[k].bIsDragAndDrop=true;break;}
            for (i=0;i<NO_BETE;i++){
                for (j=0;j<no_discuri;j++){
                    if (M[i][j] == k+1){
                        if (M[i][j+1] == 0) Discuri[k].bIsDragAndDrop = true;
                    }
                }
            }
        }
    }
}


void calculate_identity(void){

    size_t i,j,intTag=0;

    for (i=0;i<NO_BETE;i++){
        for ( j=0;j<no_discuri;j++){
            if (M[i][j]==no_discuri-j){
                intTag++;
                if (intTag==no_discuri && !MPrev[i]) goto LABEL_CALCULATE;
            }else{intTag=0;}
        }
    }

    return;

LABEL_CALCULATE:
    //MessageBox(hwnd,"Joc finalizat","Mesaj", MB_OK | MB_ICONINFORMATION);
    //std::cout << "joc finalizat " << endl;
    //std::cout << "MPrev[i]: " << MPrev[i] << endl;
    GeneratorSignal();
    for (size_t j=0;j<no_discuri;j++){MPrev[j]=false;}
    MPrev[i]=true;

}


void display_matrix(void){

    for(size_t i=0; i<NO_BETE; i++){
        for(size_t j=0; j<no_discuri; j++){
            std::cout << "M[" << i << "]" << "[" << no_discuri-1-j << "] = " << M[i][no_discuri-1-j] << "\n";
        }
        std::cout << "\n";
    }
}




void GeneratorSignal(void){
    PlaySound(TEXT("Kiss - I Was Made For Lovin' You.wav"),NULL,SND_ASYNC);
    //cout << "Apasati orice tasta pentru a anula melodia ! \n";
    //bTriggerSignal = true;
    //while(!kbhit()){;}
    //PlaySound(NULL, 0, 0);
    //bTriggerSignal = false;
}


HHOOK _hook;
 void SetHook(void){
    if (!(_hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallback, NULL, 0)))
        {MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);}
}

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam){
    if (nCode >= 0){
        if (wParam == WM_LBUTTONUP){gestioneaza_lbmouseup(hwnd);}
    }
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}
