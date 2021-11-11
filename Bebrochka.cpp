// Bebrochka.cpp : Определяет точку входа для приложения.
//

#include <Windows.h>
#include <WindowsX.h>
#include "framework.h"
#include "Bebrochka.h"
#include <commctrl.h>
#include <commdlg.h>
#include <WinUser.h>
#include <string.h>
#include <stdio.h>
#include <DLGS.H>
#include <ctime>

#define MAX_LOADSTRING 100
#define IDC_BUTTON1 2000
#define IDC_BUTTON2 2001
#define IDC_BUTTON3 2002
#define IDC_BUTTON4 2003
#define IDC_BUTTON5 2004
#define IDC_BUTTON6 2005
#define IDC_DATE 2006

// Глобальные переменные:
HINSTANCE hInst;                                //Указатель на экземпляр приложения
HWND hProgress;                                 //Дескриптор прогресс бара
DWORD IDC_TIMER;                                //Номер таймера, который мы создаем
DWORD nCounter;                                 // Счетчик
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hwndDP = NULL;
HWND hwndDlg = NULL;
FINDREPLACE findrep;
OPENFILENAME OpenFileName;
UINT uFindMsgString = 0;
HWND ReplDialog;
HWND hMainWnd;
TCHAR file[MAX_PATH];
HINSTANCE hInstance;
HWND Date;


//BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM);
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) 
//{
//    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DlgProc), 0);
//    return 0;
//}
//BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM uParam, LPARAM lParam) 
//{
//    switch (uMsg) 
//    {
//    case WM_INITDIALOG:
//        break;
//    case WM_COMMAND:
//        break;
//    case WM_CLOSE:
//        EndDialog(hwnd,0);
//        return false;
//    }
//    return false;
//}

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BEBROCHKA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BEBROCHKA));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BEBROCHKA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BEBROCHKA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   //Те самые ментовские закладки
   CreateWindowEx(0, TEXT("SysTabControl32"), TEXT("Вкуснятна"), WS_CHILD | WS_VISIBLE, 0,
       0, 700, 500, hWnd, NULL, hInstance, NULL);

   //Создаем диалоговое окно Find
   CreateWindowEx(0, TEXT("Button"), TEXT("Найти"), 
       BS_CENTER | WS_CHILD | WS_VISIBLE, 
       10, 150, 300, 40, hWnd, (HMENU)IDC_BUTTON1, hInstance, NULL);

   //Создаем диалоговое окно Open
   CreateWindowEx(0, TEXT("Button"), TEXT("Открыть"),
       BS_CENTER | WS_CHILD | WS_VISIBLE,
       10, 200, 300, 40, hWnd, (HMENU)IDC_BUTTON5, hInstance, NULL);

   //Кнопка таймера
   CreateWindowEx(0, TEXT("Button"), TEXT("Запустить таймер"),
       BS_CENTER | WS_CHILD | WS_VISIBLE,
       10, 300, 300, 40, hWnd, (HMENU)IDC_BUTTON6, hInstance, NULL);

   //Так называемый календарик
   Date = CreateWindowEx(0, DATETIMEPICK_CLASS, TEXT("DateTime"), WS_CHILD | WS_VISIBLE | WS_BORDER,
       10, 350, 200, 25, hWnd, (HMENU)IDC_DATE, hInstance, NULL);

   //Создание Радиокнопок
   CreateWindowEx(0, TEXT("Button"), TEXT("Занюхать"),
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 10, 95, 30, hWnd, (HMENU)IDC_BUTTON2, hInstance, NULL);
   CreateWindowEx(0, TEXT("Button"), TEXT("Погладить"),
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 30, 95, 30, hWnd, (HMENU)IDC_BUTTON3, hInstance, NULL);
   CreateWindowEx(0, TEXT("Button"), TEXT("Погулять"),
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 50, 95, 30, hWnd, (HMENU)IDC_BUTTON4, hInstance, NULL);
   //Создание Прогресс Бара
   hProgress = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
       WS_CHILD | WS_VISIBLE, 
       10, 90, 300, 30, hWnd, (HMENU)0,hInst, NULL);

   SendMessage(hProgress, PBM_SETRANGE, 0,MAKELPARAM(0,100)); //Диапазон ProgressBar
   SendMessage(hProgress, PBM_SETSTEP, (WPARAM)10,0); 
   //SetTimer(hWnd, IDC_TIMER,100,0); //Таймер 100 мс
   nCounter = 0;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    //Системные сообщения
        case WM_MOUSEMOVE:
    {
        SendMessage(hProgress, PBM_STEPIT, 0, 0);
        break;
    }

    case WM_RBUTTONDBLCLK:
    {
        SYSTEMTIME st{};
        DateTime_GetSystemtime(Date, &st);
        TCHAR buf[100]{};
        wsprintf(buf, L"%d-%d-%d", st.wYear, st.wMonth, st.wDay);
        MessageBox(hWnd, buf, L"Time", 0);
        break;
    }

    case WM_RBUTTONDOWN:
    {
        SetWindowText(hWnd, L"Вниз");
        break;
    }
    case WM_RBUTTONUP:
    {
        SetWindowText(hWnd, L"Вверх");
        break;
    }
    case WM_SIZE:
    {
        MessageBox(hWnd,L"Вы изменили размер окна", L"Системное сообщение", MB_OKCANCEL);
        break;
    }
    case WM_SYSCHAR:
    {
        unsigned int symbol = wParam;
        TCHAR buf[30]{};
        wsprintf(buf, L"Вы нажали символ:%c", symbol);
        MessageBox(hWnd, buf, L"Системное сообщение", MB_OKCANCEL);
        break;
    }
    case WM_SYSKEYDOWN:
    {
        TCHAR buf[30]{};
        wsprintf(buf, L"Вы нажали кнопку :)");
        SetWindowText(hWnd,buf);
        break;
    }
    case WM_TIMER:
    {
        nCounter++;
        //Передвигаем таймер на заданную позицию
        SendMessage(hProgress, PBM_STEPIT, 0, 0);
        if (nCounter == 10)
        {
            KillTimer(hWnd, IDC_TIMER);
        }
        break;
    }

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:

            switch (wmId)
            {// Нажатия на радиокнопки

            case IDC_BUTTON2:
            {
                if (LOWORD(wParam) == IDC_BUTTON2) { SetWindowText(hWnd, L"Занюхал"); break; }
            }

            case IDC_BUTTON3:
            {
                if (LOWORD(wParam) == IDC_BUTTON3) { SetWindowText(hWnd, L"Погладил"); break; }
            }

            case IDC_BUTTON4:
            {
                if (LOWORD(wParam) == IDC_BUTTON4) { SetWindowText(hWnd, L"Погулял"); break; }
            }

            //Структура Find
            case IDC_BUTTON1:
            {
                TCHAR WhatFind[100], WhatReplace[100] = TEXT("\0");
                memset(WhatFind, 0, _countof(WhatFind));
                if (uFindMsgString == 0) {
                    uFindMsgString = RegisterWindowMessage(FINDMSGSTRING);
                }
                if (IsWindow(ReplDialog) == false) {
                    findrep.lStructSize = sizeof(FINDREPLACE);
                    findrep.hInstance = hInst;
                    findrep.hwndOwner = hWnd;
                    findrep.lpstrFindWhat = WhatFind;
                    findrep.lpstrReplaceWith = WhatReplace;
                    findrep.wFindWhatLen = 80;
                    findrep.wReplaceWithLen = _countof(WhatReplace);
                    findrep.Flags = NULL;
                    ReplDialog = FindText(&findrep);

                }
                break;
            }

            //Структура Open
            case IDC_BUTTON5:
            {
                ZeroMemory(&OpenFileName, sizeof(OpenFileName));

                OpenFileName.lStructSize = sizeof(OpenFileName);
                OpenFileName.hwndOwner = hMainWnd;
                OpenFileName.lpstrFile = file;
                OpenFileName.nMaxFile = sizeof(*file) * MAX_PATH;
                OpenFileName.lpstrFilter = L"PNG (*.png)\0*.png\0BMP (*.bmp)\0*.bmp\0TGA (*tga)\0*.tga\0JPG (*.jpg)\0*.jpg\0Все файлы(*.*)\0*.*\0\0";
                OpenFileName.nFilterIndex = 1;
                OpenFileName.lpstrFileTitle = NULL;
                OpenFileName.nMaxFileTitle = NULL;
                OpenFileName.lpstrInitialDir = NULL;
                OpenFileName.lpstrTitle = L"Open...";
                OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                OpenFileName.hInstance = hInstance;
                GetOpenFileName(&OpenFileName);
                break;
            }
            case IDC_BUTTON6: 
            {
                nCounter = 0;
                SetTimer(hWnd, IDC_TIMER, 100, 0); //Таймер 100 мс
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {

    

    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
