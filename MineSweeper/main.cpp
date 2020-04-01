#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <chrono>
#include <time.h>
#include <thread>
#include <conio.h>
#include <sys/types.h>
#include "Shapes.hpp"

struct scell{
    HWND        href;
    int         Img;
    bool        IsFlaged;
    bool        IsVisible;
    bool        IsToggled;
};

struct sFigure{
    HWND        href;
    int         fig;
};

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
void CALLBACK catchInt(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime);
void add_controls(HWND hwnd);
void gestioneaza_lbmousedown(void);
void gestioneaza_lbmouseup(void);
void gestioneaza_movemouse(void);
void gestioneaza_rbmousedown(void);
void generate_random(int no);
void reset_the_game(void);
void rebuild_the_game(HWND hwnd, int item);
void build_the_game(HWND hwnd);
uint64_t micros(void);
void about_message(HWND hwnd);
void functia_lui_Victor(void);
void functia_lui_Victor1(void);
void SetHook(void);
void display_content_M(void);

inline void show_the_matrix_and_evaluate_the_game(void);
inline bool get_cell(POINT& cell, const COORD coord);
inline bool get_smile(const COORD coord);
inline void cronometru(void);
inline void decrease_the_number_of_contor(sFigure (&figs)[3]);
inline void increase_the_number_of_contor(sFigure (&figs)[3]);
void getUTCDateAndTime(void);



/*  Make the class name into a global variable  */
TCHAR                               szClassName[ ] = _T("Mine Sweeper");
char                                szCurrentPath[MAX_PATH];
HWND                                hwnd, hBck, hStartBtn;
short                               intNoBombs, intNoRows, intNoColumns, intLeftSmile, intTopSmile, intLeftCell, intTopCell;
short                               intLeftContor, intTopContor, intLeftClock, intTopClock;
short                               x, y, xScreen, yScreen;
bool                                bStartButtonDown, bMouseLBDown, bGameOver, bGameWon, bGameFail, bToggleClock;
unsigned char                       SetContor[3];
CBitmap*                            BckBmp;
HBITMAP                             hbmp, hbmpBck, hbmpStartBtn;
HMENU                               hMenubar, hMenu;

scell                               cell;
sFigure                             Clock[3], Contor[3];
std::vector<scell>                  v_col;
std::vector<std::vector<scell>>     M;
std::vector<POINT>                  RAND;
std::vector<CBitmap>                Colectie, ColectieM, ColectieFigs;
std::vector<HBITMAP>                ColectieHBMP, ColectieHFigs;
std::vector<POINT>                  BckSize;
POINT                               pBckSize;
HBRUSH                              hBrush;
COORD                               coord;
POINT                               cell_prev;
std::string                         UTCDateAndTime;
std::string                         strCloneLog;
tm                                  tm_, * timeinfo;


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
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
           _T("Mine Sweeper"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           WIN_W,                 /* The programs width */
           WIN_H,                 /* and height in pixels */
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
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_CREATE:
            add_controls(hwnd);
            build_the_game(hwnd);
            break;
        case WM_COMMAND:
            if ((LOWORD(wParam) >= 1) && (LOWORD(wParam) <= 3)) {rebuild_the_game(hwnd, LOWORD(wParam));}
            if (LOWORD(wParam) == 5){about_message(hwnd);}
            break;
        case WM_RBUTTONDOWN:
            gestioneaza_rbmousedown();
            break;
        case WM_LBUTTONDOWN:
            gestioneaza_lbmousedown();
            break;
        case WM_LBUTTONUP:
        case WM_NCLBUTTONUP:
            break;
        case WM_MOUSEMOVE:
            coord.X = GET_X_LPARAM(lParam);
            coord.Y = GET_Y_LPARAM(lParam);
            gestioneaza_movemouse();
            break;
        case WM_MOUSEWHEEL:
            break;
        case WM_CTLCOLORSTATIC:
            return (INT_PTR)hBrush;
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

/**
colectia de handlere la bitmap-uri
======================================
    0  - empty
    1  - 1
    2  - 3
    3  - 3
    4  - 4
    5  - 5
    6  - 6
    7  - normal
    8  - stegulet
    9  - bomba
    10 - marcare gresita bomba
    11 - bomba explodata

    12 - mutunachi normal
    13 - mutunachi cu ochelari
    14 - mutunachi suparat
    15 - mutunachi apasat

    16 - Background Beginer
    17 - Background Intermediate
    18 - Background Expert
**/

 void add_controls(HWND hwnd){

    ZeroMemory(szCurrentPath,MAX_PATH);
    GetCurrentDirectory(MAX_PATH, szCurrentPath);
    bToggleClock = false;
    SetTimer(NULL, 0, 1000,(TIMERPROC) &catchInt);


    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "Optiuni");
    AppendMenu(hMenubar, MF_STRING, 5, "About");

    //SetMenuItemBitmaps(hMenubar,ITEM_REZOLVA, MF_BITMAP, hIcon,hIcon);

    AppendMenu(hMenu, MF_STRING, BEGINER, "Beginer");
    AppendMenu(hMenu, MF_STRING, INTERMEDIATE, "Intermediate");
    AppendMenu(hMenu, MF_STRING, EXPERT, "Expert");

    SetMenu(hwnd, hMenubar);
 }


  void about_message(HWND hwnd){
    MessageBox(hwnd,(LPSTR)"Mine Sweeper Game !", "About !", MB_OK | MB_ICONINFORMATION);
 }


 void build_the_game(HWND hwnd){

    std::string         szTag;

    /** incarca BMP-ul cu 12 bmp-uri pentru celule
        si le plaseaza in colectia de BMP-uri
     ================================================= **/
    szTag.assign(szCurrentPath);
    szTag.append("\\res\\ColectieBMP.bmp");
    CBitmap* Bitmap;
    Bitmap = new CBitmap(szTag);
    Bitmap->extract_bitmaps_into_collection_of_bitmaps_ext({0,0},24,12,1,Colectie);
    delete Bitmap;
    /** incarca BMP-ul cu Mutunachi
        si le plaseaza in colectia de BMP-uri
     ================================================= **/
    szTag.assign(szCurrentPath);
    szTag.append("\\res\\ColectieM.bmp");
    Bitmap = new CBitmap(szTag);
    Bitmap->extract_bitmaps_into_collection_of_bitmaps_ext({0,0},40,4,1,ColectieM);
    delete Bitmap;

    /** genereaza colectia cu hbmp-urile pentru colectia cu BMP-uri
     ================================================================== **/
    for (int i=0; i<Colectie.size(); i++){
        hbmp = Colectie[i].generate_HBITMAP();
        if (hbmp == NULL){cout << "Jocul nu poate fi creat!\n aparut o eroare interna!" << endl; return;}
        ColectieHBMP.push_back(hbmp);
    }

    /** genereaza colectia cu hbmp-urile pentru colectia cu BMP-uri
     ================================================================== **/
    for (int i=0; i<ColectieM.size(); i++){
        hbmp = ColectieM[i].generate_HBITMAP();
        if (hbmp == NULL){cout << "Jocul nu poate fi creat!\n aparut o eroare interna!" << endl; return;}
        ColectieHBMP.push_back(hbmp);
    }

    /** incarca background-ul pentru Beginer si
        il plaseaza in Colectie
     ================================================= **/
    szTag.assign(szCurrentPath);
    szTag.append("\\res\\backgroundB.bmp");
    BckBmp = new CBitmap(szTag);
    pBckSize.x=BckBmp->get_width();
    pBckSize.y=BckBmp->get_height();
    BckSize.push_back(pBckSize);
    hbmp = BckBmp->generate_HBITMAP();
    ColectieHBMP.push_back(hbmp);
    delete BckBmp;

    /** incarca background-ul pentru Intermediate si
        il plaseaza in Colectie
     ================================================= **/
    szTag.assign(szCurrentPath);
    szTag.append("\\res\\backgroundI.bmp");
    BckBmp = new CBitmap(szTag);
    pBckSize.x=BckBmp->get_width();
    pBckSize.y=BckBmp->get_height();
    BckSize.push_back(pBckSize);
    hbmp = BckBmp->generate_HBITMAP();
    ColectieHBMP.push_back(hbmp);
    delete BckBmp;

    /** incarca background-ul pentru Expert si
        il plaseaza in Colectie
     ================================================= **/
    szTag.assign(szCurrentPath);
    szTag.append("\\res\\backgroundE.bmp");
    BckBmp = new CBitmap(szTag);
    pBckSize.x=BckBmp->get_width();
    pBckSize.y=BckBmp->get_height();
    BckSize.push_back(pBckSize);
    hbmp = BckBmp->generate_HBITMAP();
    ColectieHBMP.push_back(hbmp);
    delete BckBmp;
    //intTypeOfGame = EXPERT;//BEGINER;//INTERMEDIATE;//

    /** incarca Fugures pentru contori
    ================================================= **/
    szTag.assign(szCurrentPath);
    szTag.append("\\res\\FiguresBMP_.bmp");
    Bitmap = new CBitmap(szTag);
    Bitmap->extract_bitmaps_into_collection_of_bitmapsE({0,0},19,35,11,1,ColectieFigs);

    for (int i=0; i<ColectieFigs.size(); i++){
        hbmp = ColectieFigs[i].generate_HBITMAP();
        if (hbmp == NULL){cout << "Jocul nu poate fi creat!\n aparut o eroare interna!" << endl; return;}
        ColectieHFigs.push_back(hbmp);
    }

    rebuild_the_game(hwnd, INTERMEDIATE);
 }


 void rebuild_the_game(HWND hwnd, int item){

    std::string         szTag;

    DestroyWindow(hBck);
    switch(item)
    {
        case BEGINER:
                hbmpBck = ColectieHBMP[INDEX_BCK_BEGINER];
                intNoRows = NO_RANDURI_GAME_EGINER;
                intNoColumns = NO_COLOANE_GAME_BEGINER;
                intNoBombs = NO_BOMBS_BEGINER;
                intLeftSmile = LEFT_SMILE_X_B;
                intTopSmile = TOP_SMILE_Y_B;
                intLeftCell = ORIGINE_CELLS_X_B;
                intTopCell = ORIGINE_CELLS_Y_B;
                intLeftContor = CONTOR_LEFT_POSITION;
                intTopContor = CONTOR_TOP_POSITION;
                intLeftClock = CLOCK_LEFT_POSITION_B;
                intTopClock = CLOCK_TOP_POSITION_B;
                SetContor[0]=static_cast<unsigned char>(NO_MAXIM_BOMBS_B>>16);
                SetContor[1]=static_cast<unsigned char>(NO_MAXIM_BOMBS_B>>8);
                SetContor[2]=static_cast<unsigned char>(NO_MAXIM_BOMBS_B);
            break;
        case INTERMEDIATE:
                hbmpBck = ColectieHBMP[INDEX_BCK_INTERMEDIATE];
                intNoRows = NO_RANDURI_GAME_INTERMEDIATE;
                intNoColumns = NO_COLOANE_GAME_INTERMEDIATE;
                intNoBombs = NO_BOMBS_INTERMEDIATE;
                intLeftSmile = LEFT_SMILE_X_I;
                intTopSmile = TOP_SMILE_Y_I;
                intLeftCell = ORIGINE_CELLS_X_I;
                intTopCell = ORIGINE_CELLS_Y_I;
                intLeftContor = CONTOR_LEFT_POSITION;
                intTopContor = CONTOR_TOP_POSITION;
                intLeftClock = CLOCK_LEFT_POSITION_I;
                intTopClock = CLOCK_TOP_POSITION_I;
                SetContor[0]=static_cast<unsigned char>(NO_MAXIM_BOMBS_I>>16);
                SetContor[1]=static_cast<unsigned char>(NO_MAXIM_BOMBS_I>>8);
                SetContor[2]=static_cast<unsigned char>(NO_MAXIM_BOMBS_I);
            break;
        case EXPERT:
                hbmpBck = ColectieHBMP[INDEX_BCK_EXPERT];
                intNoRows = NO_RANDURI_GAME_EXPERT;
                intNoColumns = NO_COLOANE_GAME_EXPERT;
                intNoBombs = NO_BOMBS_EXPERT;
                intLeftSmile = LEFT_SMILE_X_E;
                intTopSmile = TOP_SMILE_Y_E;
                intLeftCell = ORIGINE_CELLS_X_E;
                intTopCell = ORIGINE_CELLS_Y_E;
                intLeftContor = CONTOR_LEFT_POSITION;
                intTopContor = CONTOR_TOP_POSITION;
                intLeftClock = CLOCK_LEFT_POSITION_E;
                intTopClock = CLOCK_TOP_POSITION_E;
                SetContor[0]=static_cast<unsigned char>(NO_MAXIM_BOMBS_E>>16);
                SetContor[1]=static_cast<unsigned char>(NO_MAXIM_BOMBS_E>>8);
                SetContor[2]=static_cast<unsigned char>(NO_MAXIM_BOMBS_E);
            break;
    }

    xScreen = GetSystemMetrics(SM_CXSCREEN);
    yScreen = GetSystemMetrics(SM_CYSCREEN);
    xScreen = static_cast<int>((xScreen-BckSize[item-1].x)/2);
    yScreen = static_cast<int>((yScreen-BckSize[item-1].y)/2);
    InvalidateRect(hwnd,NULL,true);
    SetWindowPos(hwnd, HWND_TOP, xScreen, yScreen, BckSize[item-1].x+2*ORIGINE_BCK_X+15 ,BckSize[item-1].y+2*ORIGINE_BCK_Y+60, SWP_SHOWWINDOW | SWP_NOZORDER);

    hbmpStartBtn = ColectieHBMP[INDEX_MUT_NORMAL];
    hBck = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_BITMAP, ORIGINE_BCK_X, ORIGINE_BCK_Y, WIDTH_BCK_H, HEIGHT_BCK_V, hwnd, NULL, NULL, NULL);
    hStartBtn = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_BITMAP, intLeftSmile, intTopSmile, L_SMILE, L_SMILE, hBck, NULL, NULL, NULL);

    SendMessage(hBck, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)hbmpBck);
    SendMessage(hStartBtn, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)hbmpStartBtn);

    M.clear();

    for(int y=0; y<intNoRows; y++){
        for(int x=0; x<intNoColumns; x++){
            cell.href = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_BITMAP, intLeftCell+x*(L_CAREU), intTopCell+y*(L_CAREU), L_CAREU, L_CAREU, hBck, NULL, NULL, NULL);
            //cout << "No: "  << (y*intNoRows+x) << " |  cell.href: " <<  cell.href << endl;
            if (cell.href == NULL){cout << "EROARE: Nu s-a alocat handler pentru celula " << " y: " << y << " x: " << x << endl;}
            v_col.push_back(cell);
        }
        M.push_back(v_col);
        v_col.clear();
    }


    for (int i=0; i<3; i++){
        Contor[i].href = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_BITMAP, intLeftContor+i*19, intTopContor, L_FIGURE, H_FIGURE, hBck, NULL, NULL, NULL);
        if (Contor[i].href == NULL){cout << "EROARE: Nu s-a alocat handler-ul!" << endl;}
        Clock[i].href = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_BITMAP, intLeftClock+i*19, intTopClock, L_FIGURE, H_FIGURE, hBck, NULL, NULL, NULL);
        if (Clock[i].href == NULL){cout << "EROARE: Nu s-a alocat handler-ul!" << endl;}
    }

    reset_the_game();
 }

void reset_the_game(void){

    bStartButtonDown=false;
    bToggleClock=false;
    bGameOver=false;
    bGameWon=false;
    bGameFail=false;
    Clock[0].fig=0;
    Clock[1].fig=0;
    Clock[2].fig=0;

    generate_random(intNoBombs);

    Contor[0].fig=SetContor[0];
    Contor[1].fig=SetContor[1];
    Contor[2].fig=SetContor[2];

    for (int i=0; i<3; i++){
        SendMessage(Contor[i].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHFigs[Contor[i].fig]);
        SendMessage(Clock[i].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHFigs[Clock[i].fig]);
    }


    for(int y=0; y<intNoRows; y++){
        for(int x=0; x<intNoColumns; x++){
            M[y][x].Img=0;
            M[y][x].IsToggled=false;
            M[y][x].IsVisible=false;
            M[y][x].IsFlaged=false;
            for (int i=0; i<RAND.size(); i++){
                if ((x==RAND[i].x) && (y==RAND[i].y)) {M[y][x].Img=INDEX_BOMB;}
            }
        }
    }

    functia_lui_Victor();
    for(int y=0; y<intNoRows; y++){
        for(int x=0; x<intNoColumns; x++){
            SendMessage(M[y][x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_NORMAL]);
            InvalidateRect(M[y][x].href,NULL,true);
            SetWindowPos(M[y][x].href, HWND_TOP, intLeftCell+x*(L_CAREU), intTopCell+y*(L_CAREU), L_CAREU, L_CAREU, SWP_SHOWWINDOW | SWP_NOZORDER);
        }
    }
}

inline void increase_the_number_of_contor(sFigure (&figs)[3]){
    if (figs[0].fig==10){
        figs[2].fig--;
        if(figs[2].fig==0){
            figs[2].fig=9;
            figs[1].fig--;
            if(figs[1].fig==0){
                figs[1].fig=10;
                figs[0].fig=0;
            }
        }
    }else if(figs[1].fig==10){
        figs[2].fig--;
        if(figs[2].fig==0){
            figs[1].fig=0;
        }
    }else{
        if (figs[2].fig==9){
            figs[2].fig=0;
            if(figs[1].fig==9){
                figs[1].fig=0;
                figs[0].fig++;
            }else{
                figs[1].fig++;
            }
        }else{
            figs[2].fig++;
        }
    }

    for (int i=0; i<3; i++){
        SendMessage(figs[i].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHFigs[figs[i].fig]);
    }
}


inline void decrease_the_number_of_contor(sFigure (&figs)[3]){
    if ((figs[0].fig==10) || (figs[1].fig==10)){
        if(figs[2].fig==9){
            figs[2].fig=0;
            if (figs[1].fig==10){
                figs[1].fig=1;
                figs[0].fig=10;
            }else{
                figs[1].fig++;
            }
        }else{figs[2].fig++;}
    }else{
        if (figs[2].fig==0){
            if(figs[1].fig==0){
                if(figs[0].fig==0){
                    figs[2].fig=1;
                    figs[1].fig=10;
                }else{
                    figs[1].fig=9;
                    figs[0].fig--;
                }
            }else{
                figs[2].fig=9;
                figs[1].fig--;
            }
        }else{
            figs[2].fig--;
        }
    }

    for (int i=0; i<3; i++){
        SendMessage(figs[i].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHFigs[figs[i].fig]);
    }
}


void generate_random(int no){

    uint64_t        us;
    uint64_t        random_no1, random_no2, random_no, i=1;
    POINT           prand;

    RAND.clear();

LABEL_1:
    us = (uint64_t)(micros()/i);
    srand(us);
    random_no1 = rand();

    random_no = random_no1%(intNoRows*intNoColumns);

    prand.y = random_no/intNoColumns;
    prand.x = random_no-prand.y*intNoColumns;

    if (RAND.size()==0){RAND.push_back(prand);}
    i++;
    for (int i=0; i<RAND.size(); i++){
        if ((prand.x==RAND[i].x) && (prand.y==RAND[i].y)){goto LABEL_1;}
    }
    RAND.push_back(prand);

    if (RAND.size()!=no){goto LABEL_1;}
}


 inline bool get_cell(POINT& cell, const COORD coord){

    short           xdiff, ydiff;
    bool            bIsInCareu;

    cell.y = static_cast<int>(coord.Y-intTopCell-ORIGINE_BCK_Y)/L_CAREU;
    cell.x = static_cast<int>(coord.X-intLeftCell-ORIGINE_BCK_X)/L_CAREU;

    ydiff = static_cast<short>(coord.Y-intTopCell-ORIGINE_BCK_Y);
    xdiff = static_cast<short>(coord.X-intLeftCell-ORIGINE_BCK_X);

    bIsInCareu=false;
    if ((((cell.y>=0) && (cell.x>=0)) && ((cell.y<intNoRows) && (cell.x<intNoColumns))) && ((ydiff>=0) && (xdiff>=0))){
        bIsInCareu=true;
    }
    return bIsInCareu;
}


inline bool get_smile(const COORD coord){

    bool    bIsPushedStartButton;

    bIsPushedStartButton=false;
    if (((coord.X-ORIGINE_BCK_X>intLeftSmile) && (coord.X-ORIGINE_BCK_X<intLeftSmile+L_SMILE)) && ((coord.Y-ORIGINE_BCK_Y>intTopSmile) && (coord.Y-ORIGINE_BCK_Y<intTopSmile+L_SMILE))){
        bIsPushedStartButton = true;
    }
    return bIsPushedStartButton;
}


void gestioneaza_rbmousedown(){

    bool            bIsInCareu;
    POINT           cell;

    bIsInCareu = get_cell(cell, coord);

    if (bIsInCareu && (!bGameOver)){
        if (!M[cell.y][cell.x].IsVisible){
            if (!M[cell.y][cell.x].IsFlaged){
                SendMessage(M[cell.y][cell.x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_FLAG]);
            }else{
                SendMessage(M[cell.y][cell.x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_NORMAL]);
            }
            M[cell.y][cell.x].IsFlaged=!M[cell.y][cell.x].IsFlaged;
            if (M[cell.y][cell.x].IsFlaged){decrease_the_number_of_contor(Contor);}
            if (!M[cell.y][cell.x].IsFlaged){increase_the_number_of_contor(Contor);}
        }
        show_the_matrix_and_evaluate_the_game();
        if (bGameWon){
            SendMessage(hStartBtn, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_MUT_OCHELARI]);
            bToggleClock=false;
        }
    }
}


void gestioneaza_lbmousedown(){

    bool            bIsPushedStartButton;
    bool            bIsInCareu;
    POINT           cell;

    bMouseLBDown = true;
    bIsPushedStartButton = get_smile(coord);

    if (bIsPushedStartButton) {
        SendMessage(hStartBtn, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_MUT_APASAT]);
        bStartButtonDown = true;
    }

    bIsInCareu = get_cell(cell, coord);

    if ((bIsInCareu) && (!bGameOver)){
        if((!M[cell.y][cell.x].IsVisible) && (!M[cell.y][cell.x].IsFlaged)){
            SendMessage(M[cell.y][cell.x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_EMPTY]);
        }
        cell_prev.y=cell.y;
        cell_prev.x=cell.x;
    }
 }


void gestioneaza_movemouse(){

    bool        bIsInCareu;
    POINT       cell;

    bIsInCareu = get_cell(cell, coord);

    if (bMouseLBDown && (!bGameOver) && bIsInCareu){
        if ((cell.y!=cell_prev.y)||(cell.x!=cell_prev.x)){
            if((!M[cell.y][cell.x].IsVisible) && (!M[cell.y][cell.x].IsFlaged)){
                SendMessage(M[cell.y][cell.x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_EMPTY]);
            }
            if ((!M[cell_prev.y][cell_prev.x].IsVisible) && (!M[cell_prev.y][cell_prev.x].IsFlaged)){
                SendMessage(M[cell_prev.y][cell_prev.x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_NORMAL]);
            }
            cell_prev.y=cell.y;
            cell_prev.x=cell.x;
        }
    }else if (bMouseLBDown && (!bGameOver) && (!bIsInCareu)){
        if ((!M[cell_prev.y][cell_prev.x].IsVisible) && (!M[cell_prev.y][cell_prev.x].IsFlaged)){
            SendMessage(M[cell_prev.y][cell_prev.x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_NORMAL]);
        }
    }
}


void gestioneaza_lbmouseup(void){

    bool        bIsInCareu;
    POINT       cell;

    bIsInCareu = get_cell(cell, coord);

    if ((!bGameOver) && bIsInCareu){
        if(!(M[cell.y][cell.x].IsFlaged)){
            if(M[cell.y][cell.x].Img==INDEX_BOMB){
                M[cell.y][cell.x].Img=INDEX_EXPLODED_BOMB;
                M[cell.y][cell.x].IsToggled=true;
                bGameFail=true;
                bGameOver=true;
                for(int y=0; y<intNoRows; y++){
                    for(int x=0; x<intNoColumns; x++){
                        if ((M[y][x].IsFlaged) && (M[y][x].Img!=INDEX_BOMB)){
                            M[y][x].Img=INDEX_NO_BOMB;
                            M[y][x].IsToggled=true;
                        }
                        if (M[y][x].Img==INDEX_BOMB){
                            M[y][x].IsToggled=true;
                        }
                    }
                }
            }else if(M[cell.y][cell.x].Img==INDEX_EMPTY){
                M[cell.y][cell.x].IsToggled=true;
                functia_lui_Victor1();
            }else{M[cell.y][cell.x].IsToggled=true;}
            if(M[cell.y][cell.x].IsToggled){if (!bToggleClock){bToggleClock = true;}}
        }
        show_the_matrix_and_evaluate_the_game();
        if (bGameWon){
            SendMessage(hStartBtn, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_MUT_OCHELARI]);
            bToggleClock=false;
        }
        if(bGameFail){
            SendMessage(hStartBtn, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_MUT_SUPARAT]);
            bToggleClock=false;
        }
    }

    bMouseLBDown = false;
    if (bStartButtonDown){
        SendMessage(hStartBtn, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[INDEX_MUT_NORMAL]);
        reset_the_game();
        bStartButtonDown=false;
    }
}


 inline void show_the_matrix_and_evaluate_the_game(void){

    int         CntBombe;
    int         CntCellsVisible;

    CntBombe=0;
    CntCellsVisible=0;

    for(int y=0; y<intNoRows; y++){
        for(int x=0; x<intNoColumns; x++){
            if (M[y][x].IsToggled){
                SendMessage(M[y][x].href, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)ColectieHBMP[M[y][x].Img]);
                M[y][x].IsToggled=false;
                M[y][x].IsVisible=true;
            }
            if (M[y][x].IsFlaged && M[y][x].Img==INDEX_BOMB){
                CntBombe++;
            }
            if (M[y][x].IsVisible){
                CntCellsVisible++;
            }
        }
    }

    if ((CntBombe==intNoBombs) && (CntCellsVisible==(intNoRows*intNoColumns-intNoBombs))){bGameWon=true;bGameOver=true;}
}


/** functia care descopere celulele goale
in cazul in care se selecteaza o celula goala
================================================= **/
void functia_lui_Victor1(void) {

	while (true) {
		bool bTag = false;
		for (int y = 0; y < intNoRows; y++) {
			for (int x = 0; x < intNoColumns; x++) {
				if ((M[y][x].Img == INDEX_EMPTY) && (M[y][x].IsToggled)) {
					if (y == 0) {
						if (x == 0) {
							if (!(M[y + 1][x].IsToggled)) { M[y + 1][x].IsToggled = true; bTag = true; }
							if (!(M[y][x + 1].IsToggled)) { M[y][x + 1].IsToggled = true; bTag = true; }
							if (!(M[y + 1][x + 1].IsToggled)) { M[y + 1][x + 1].IsToggled = true; bTag = true; }
						}
						else if (x == intNoColumns - 1) {
							if (!(M[y + 1][x].IsToggled)) { M[y + 1][x].IsToggled = true; bTag = true; }
							if (!(M[y][x - 1].IsToggled)) { M[y][x - 1].IsToggled = true; bTag = true; }
							if (!(M[y + 1][x - 1].IsToggled)) { M[y][x - 1].IsToggled = true; bTag = true; }
						}
						else {
							if (!(M[y][x - 1].IsToggled)) { M[y][x - 1].IsToggled = true; bTag = true; }
							if (!(M[y][x + 1].IsToggled)) { M[y][x + 1].IsToggled = true; bTag = true; }
							if (!(M[y + 1][x - 1].IsToggled)) { M[y + 1][x - 1].IsToggled = true; bTag = true; }
							if (!(M[y + 1][x].IsToggled)) { M[y + 1][x].IsToggled = true; bTag = true; }
							if (!(M[y + 1][x + 1].IsToggled)) { M[y + 1][x + 1].IsToggled = true; bTag = true; }
						}
					}
					else if (y == intNoRows - 1) {
						if (x == 0) {
							if (!(M[y][x + 1].IsToggled)) { M[y][x + 1].IsToggled = true; bTag = true; }
							if (!(M[y - 1][x + 1].IsToggled)) { M[y - 1][x + 1].IsToggled = true; bTag = true; }
							if (!(M[y - 1][x].IsToggled)) { M[y - 1][x].IsToggled = true; bTag = true; }
						}
						else if (x == intNoColumns - 1) {
							if (!(M[y][x - 1].IsToggled)) { M[y][x - 1].IsToggled = true; bTag = true; }
							if (!(M[y - 1][x - 1].IsToggled)) { M[y - 1][x - 1].IsToggled = true; bTag = true; }
							if (!(M[y - 1][x].IsToggled)) { M[y - 1][x].IsToggled = true; bTag = true; }
						}
						else {
							if (!(M[y][x - 1].IsToggled)) { M[y][x - 1].IsToggled = true; bTag = true; }
							if (!(M[y][x + 1].IsToggled)) { M[y][x + 1].IsToggled = true; bTag = true; }
							if (!(M[y - 1][x - 1].IsToggled)) { M[y - 1][x - 1].IsToggled = true; bTag = true; }
							if (!(M[y - 1][x].IsToggled)) { M[y - 1][x].IsToggled = true; bTag = true; }
							if (!(M[y - 1][x + 1].IsToggled)) { M[y - 1][x + 1].IsToggled = true; bTag = true; }
						}
					}
					else if (((y > 0) && (y < intNoRows - 1)) && (x == 0)) {
						if (!M[y + 1][x].IsToggled) { M[y+1][x].IsToggled = true; bTag = true; }
						if (!M[y - 1][x].IsToggled) { M[y-1][x].IsToggled = true; bTag = true; }
						if (!M[y][x + 1].IsToggled) { M[y][x + 1].IsToggled = true; bTag = true; }
						if (!M[y + 1][x + 1].IsToggled) { M[y+1][x + 1].IsToggled = true; bTag = true; }
						if (!M[y - 1][x + 1].IsToggled) { M[y-1][x + 1].IsToggled = true; bTag = true; }
					}
					else if (((y > 0) && (y < intNoRows - 1)) && (x == intNoColumns - 1)) {
						if (!M[y + 1][x].IsToggled) { M[y+1][x].IsToggled = true; bTag = true; }
						if (!M[y - 1][x].IsToggled) { M[y-1][x].IsToggled = true; bTag = true; }
						if (!M[y][x - 1].IsToggled) { M[y][x - 1].IsToggled = true; bTag = true; }
						if (!M[y + 1][x - 1].IsToggled) { M[y+1][x - 1].IsToggled = true; bTag = true; }
						if (!M[y - 1][x - 1].IsToggled) { M[y-1][x - 1].IsToggled = true; bTag = true; }
					}

					else {
						for (int j = 0; j < 3; j++) {
							for (int i = 0; i < 3; i++) {
								if (!(M[y - 1 + j][x - 1 + i].IsToggled)) { M[y - 1 + j][x - 1 + i].IsToggled = true; bTag = true; }
							}
						}
					}
				}
			}
		}
		if (!bTag)break;
	}
}


 void functia_lui_Victor(){

    int nr_bombe;

    for(int y=0; y<intNoRows; y++){
        for(int x=0; x<intNoColumns; x++){
            nr_bombe=0;
            if(M[y][x].Img==INDEX_EMPTY){
                if(y==0){
                    if(x==0){
                        if(M[y+1][x].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y][x+1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y+1][x+1].Img==INDEX_BOMB)nr_bombe++;
                    }
                    else if(x==intNoColumns-1){
                        if(M[y+1][x].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y][x-1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y+1][x-1].Img==INDEX_BOMB)nr_bombe++;
                    }
                    else{
                        if(M[y][x-1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y][x+1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y+1][x-1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y+1][x].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y+1][x+1].Img==INDEX_BOMB)nr_bombe++;
                    }
                }
                else if(y==intNoRows-1){
                    if(x==0){
                        if(M[y][x+1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y-1][x+1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y-1][x].Img==INDEX_BOMB)nr_bombe++;
                    }
                    else if(x==intNoColumns-1){
                        if(M[y][x-1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y-1][x-1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y-1][x].Img==INDEX_BOMB)nr_bombe++;
                    }
                    else{
                        if(M[y][x-1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y][x+1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y-1][x-1].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y-1][x].Img==INDEX_BOMB)nr_bombe++;
                        if(M[y-1][x+1].Img==INDEX_BOMB)nr_bombe++;
                    }
                }
                else if(((y>0) && (y<intNoRows-1)) && (x==0)){
                    if(M[y+1][x].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y-1][x].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y][x+1].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y+1][x+1].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y-1][x+1].Img==INDEX_BOMB)nr_bombe++;
                }
                else if(((y>0) && (y<intNoRows-1)) && (x==intNoColumns-1)){
                    if(M[y+1][x].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y-1][x].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y][x-1].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y+1][x-1].Img==INDEX_BOMB)nr_bombe++;
                    if(M[y-1][x-1].Img==INDEX_BOMB)nr_bombe++;
                }else{
                    for(int j=0; j<3; j++){
                        for(int i=0; i<3; i++){
                            if(M[y-1+j][x-1+i].Img==INDEX_BOMB)nr_bombe++;
                        }
                    }
                }
            }
            if(nr_bombe!=INDEX_EMPTY)M[y][x].Img=nr_bombe;
        }
    }
 }


void display_content_M(void){
    for(int y=0; y<intNoRows; y++){
        for(int x=0; x<intNoColumns; x++){
            if (M[y][x].IsVisible){
                cout << "===============================" << endl;
                cout << "M[" << y << "][" << x << "].IsVisible: " << M[y][x].IsVisible << endl;
                cout << "===============================" << endl;
            }
        }
    }

}


inline void cronometru(void){
    increase_the_number_of_contor(Clock);
}


uint64_t micros(void)
{
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
                                                                        now().time_since_epoch()).count();
    return us;
}


HHOOK _hook;
 void SetHook(void){
    if (!(_hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallback, NULL, 0)))
        {MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);}
}


LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam){
    if (nCode >= 0){
        if (wParam == WM_LBUTTONUP){
            gestioneaza_lbmouseup();
        }
    }
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void CALLBACK catchInt(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime){if (bToggleClock) {cronometru();}}



void WriteLog(std::string strLog){

    FILE                            *pf;
    std::string                     strTemp, strFile, strDate;


    if (strCloneLog.compare(strLog.c_str()) == 0){goto NOT_WRITE_LOG;}

    strCloneLog = strLog;
    getUTCDateAndTime();


    strTemp.clear();
    strTemp.append(UTCDateAndTime.c_str());
    strTemp.append(" : ");
    strTemp.append(strLog);
    strTemp.append("\r\n");
    //strTemp.append(1,char(13));
    //strTemp.append(1,char(10));

    strDate = UTCDateAndTime.substr(0,10);
    strDate.replace(4,1,"");
    strDate.replace(6,1,"");
    strDate += "_";

    strFile.clear();
    strFile.append(szCurrentPath);
    strFile.append(SUBFOLDER_LOGS);
    strFile.append(strDate);
    strFile.append(FILE_LOGURI);

    pf = fopen((char*)strFile.c_str(),"a");
    if (pf == NULL) {perror ("Error opening file: ");goto NOT_WRITE_LOG;}

    fputs(&strTemp[0],pf);
    fclose(pf);

NOT_WRITE_LOG:;

}


void getUTCDateAndTime(void){

    time_t                               t;

    timeinfo=&tm_;
    t = time(&t);
    timeinfo = gmtime ( &t );
    UTCDateAndTime.clear();
    UTCDateAndTime.resize(21);
    strftime (&UTCDateAndTime[0],21,"%Y-%m-%dT%H:%M:%SZ",timeinfo);
}

