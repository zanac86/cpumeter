#include <windows.h>
#include "resource.h"
#include "settings.h"
#include "counters.h"
#include "snapwindow.h"
#include "singleinstance.h"
#include <stdio.h>

SingleInstance app;
CSnapWindow snapHandler;//processes snapping

class CpuMeter
{
        HWND hwnd;
        HBITMAP hBitmap;

        long width, height;
        unsigned long* rawBits;

        Settings settings;

        int cpuCount;

        long* vs;
        long* maxval;

        PDH* pdh;

        void RegisterMyWindowClass(HINSTANCE hinst)
        {
            WNDCLASS wc;
            wc.style = 0;
            wc.lpfnWndProc = (WNDPROC)wproc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = hinst;
            wc.hIcon = LoadIcon((HINSTANCE) NULL, IDI_APPLICATION);
            wc.hCursor = LoadCursor((HINSTANCE) NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
            wc.lpszMenuName =  NULL;
            wc.lpszClassName = "CpuMeter";
            RegisterClass(&wc);
        }

        static void CALLBACK tproc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
        {
            CpuMeter* nm = (CpuMeter*)idEvent;
            nm->Timer();
        }

        int Initialize()
        {
            settings.load();
            vs = new long[settings.graphWidth * cpuCount];
            maxval = new long[cpuCount];
            for (int i = 0; i < cpuCount; i++)
            {
                maxval[i] = 0;
            }
            for (int i = 0; i < settings.graphWidth * cpuCount; i++)
            {
                vs[i] = 0;
            }

            width =  settings.graphMargin + cpuCount * (settings.graphWidth + settings.graphMargin);
            height = settings.graphMargin + settings.graphHeight + settings.graphMargin;
            HINSTANCE hi = GetModuleHandle(NULL);
            RegisterMyWindowClass(hi);
            hwnd = ::CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, "CpuMeter", "", WS_POPUP,
                                    settings.graphPosLeft, settings.graphPosTop, width, height, (HWND)NULL, (HMENU)NULL, hi, LPVOID(NULL));
            if (!hwnd)
            {
                return -1;
            }
            ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
            ::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wproc);
            BITMAPINFO bmi;
            int bmihsize = sizeof(BITMAPINFOHEADER);
            memset(&bmi, 0, bmihsize);
            BITMAPINFOHEADER& hBmp = bmi.bmiHeader;
            hBmp.biSize     = bmihsize;
            hBmp.biWidth        = width;
            hBmp.biHeight       = -height;
            hBmp.biPlanes       = 1;
            hBmp.biBitCount = 32;
            hBmp.biCompression  = BI_RGB;
            hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&rawBits, NULL, 0);
            if (!hBitmap)
            {
                return -1;
            }
            settings.save(&hwnd);
            ::ShowWindow((HWND)hwnd, SW_SHOWNORMAL);
            ::SetTimer(hwnd, (UINT_PTR)this, 250, tproc);
            return 0;
        }

        void ClearBackground()
        {
            for (int i = 0; i < width * height; i++)
            {
                rawBits[i] = settings.colorBackground;
            }
        }

        void UpdateValArray()
        {
            for (int c = 0; c < cpuCount; c++)
            {
                maxval[c] = 0;
                for (int i = 0; i < settings.graphWidth; i++)
                {
                    vs[c * settings.graphWidth + i] = pdh[c].get(i);
                    if (vs[c * settings.graphWidth + i] > maxval[c])
                    {
                        maxval[c] = vs[c * settings.graphWidth + i];
                    }
                }
                if (maxval[c] <= 10)
                {
                    maxval[c] = 50;
                }
                else
                {
                    if (maxval[c] <= 100)
                    {
                        maxval[c] = 100;
                    }
                }
                // if >100, without change
            }
        }

        void Normalize()
        {
            for (int c = 0; c < cpuCount; c++)
            {
                double m = (double)maxval[c];
                double w = (double)(height - 2);
                double k = w / m;
                for (int i = 0; i < settings.graphWidth; i++)
                {
                    double v = (double)vs[c * settings.graphWidth + i];
                    vs[c * settings.graphWidth + i] = (long)(v * k);
                }
            }
        }

        void DrawSamples()
        {
            int idx_pos_x = settings.graphMargin;
            DWORD c = 0;
            for (int cpu = 0; cpu < cpuCount; cpu++)
            {
                for (int i = 0; i < settings.graphWidth; i++)
                {
                    int idx = idx_pos_x + settings.graphHeight * width;
                    int h = vs[cpu * settings.graphWidth + (settings.graphWidth - 1 - i)];
                    if (h > (settings.graphHeight / 2))
                    {
                        c = settings.colorGraph;
                    }
                    else
                    {
                        c = settings.colorGraphDark;
                    }
                    for (int j = 0; j < h; j++)
                    {
                        rawBits[idx] = c;
                        idx -= width;
                    }
                    idx_pos_x++;
                }
                idx_pos_x += settings.graphMargin;
            }
        }

        int Redraw()
        {
            if (!rawBits)
            {
                return -1;
            }

            ClearBackground();
            UpdateValArray();
            Normalize();
            DrawSamples();
            DrawFrame();
            return Draw();
        }

        void DrawFrame()
        {
            for (int i = 0; i < width; i++)
            {
                rawBits[i] = settings.colorFrame;
                rawBits[width * height - 1 - i] = settings.colorFrame;
            }
            for (int j = 0; j <= cpuCount; j++)
            {
                for (int i = 0; i < height; i++)
                {
                    rawBits[j * (settings.graphWidth + settings.graphMargin) + i * width] = settings.colorFrame;
                }
            }
        }

        int Draw(HDC hdc = NULL)
        {

            HDC hcdc = CreateCompatibleDC(NULL);
            HDC dc = hdc;

            if (!dc)
            {
                dc = (HDC)GetDC(hwnd);
            }
            HBITMAP oldbitmap = (HBITMAP)SelectObject((HDC)hcdc, hBitmap);

            ::BitBlt(dc, 0, 0, width, height, (HDC)hcdc, 0, 0, SRCCOPY);

            SelectObject((HDC)hcdc, oldbitmap);
            DeleteDC(hcdc);
            DeleteDC(dc);
            ::GdiFlush();

            return 0;
        }

        static int __stdcall wproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            CpuMeter* nm = (CpuMeter*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);

            long rv = nm->WindowProc(uMsg, wParam, lParam);
            if (rv != -1)
            {
                return rv;
            }

            // -1 => default
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        void getCpuCount()
        {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            cpuCount = si.dwNumberOfProcessors;
        }

    public:

        /// Конструктор класса
        CpuMeter() : rawBits(NULL), hwnd(NULL), hBitmap(NULL)
        {
            getCpuCount();
            pdh = new PDH[cpuCount];
            for (int i = 0; i < cpuCount; i++)
            {
                char sn[1024];
                sprintf(sn, "\\Процессор(%d)\\%% загруженности процессора", i);
                pdh[i].setCounter(sn);
            }
            Initialize();
        }

        int Timer()
        {
            for (int i = 0; i < cpuCount; i++)
            {
                pdh[i].tick();
            }
            return Redraw();
        }

        int WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            switch (uMsg)
            {
                case WM_LBUTTONDOWN:
                    ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | 0x02, 0);
                    break;
                case WM_RBUTTONDOWN:
                    settings.save(&hwnd);
                    break;
                case WM_TIMER:
                    Timer();
                    break;
                case WM_ERASEBKGND:
                    return 0;
                case WM_PAINT:
                    {
                        PAINTSTRUCT ps;
                        HDC hdc = ::BeginPaint(hwnd, &ps);
                        if (hdc != NULL)
                        {
                            Draw(hdc);
                            ::EndPaint(hwnd, &ps);
                        }
                    }
                    break;
                case WM_CLOSE:
                    settings.save(&hwnd);
                    ::PostQuitMessage(0);
                    break;

                case WM_MOVING:
                    return snapHandler.OnSnapMoving(hwnd, uMsg, wParam, lParam);
                    break;
                case WM_ENTERSIZEMOVE:
                    return snapHandler.OnSnapEnterSizeMove(hwnd, uMsg, wParam, lParam);
                    break;
                default:
                    break;
            }
            return -1;
        }
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
//int main()
{
    if (app.isAnotherInstanceRunning())
    {
        return 0;
    }

    CpuMeter nm;
    MSG msg;

    while (::GetMessage(&msg, (HWND)NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
