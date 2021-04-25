#ifndef CPU_METER_SETTINGS_HEADER
#define CPU_METER_SETTINGS_HEADER

#include <windows.h>

#define MY_REG_KEY "Software\\vb\\CpuMeter"
#define MY_REG_POS_LEFT "pos_left"
#define MY_REG_POS_TOP "pos_top"
#define MY_REG_GRAPH_WIDTH "graph_width"
#define MY_REG_GRAPH_HEIGHT "graph_height"
#define MY_REG_COLOR_FRAME "color_frame"
#define MY_REG_COLOR_BACKGROUND "color_background"
#define MY_REG_COLOR_GRAPH "color_graph"
#define MY_REG_COLOR_GRAPH_DARK "color_graph_dark"

class Settings
{
    public:
        Settings()
        {
            graphMargin = 1;
            graphWidth = 72;
            graphHeight = 14;
            graphPosLeft = 180;
            graphPosTop = 0;
            colorFrame = 0xffffff;
            colorBackground = 0x000000;
            colorGraph = 0x00ff00;
            colorGraphDark = 0x00aa00;
        }

        void load()
        {
            HKEY key;
            DWORD w;
            DWORD wLen = 4;
            DWORD wType = REG_DWORD;
            RegOpenKeyEx(HKEY_CURRENT_USER, MY_REG_KEY, 0, KEY_QUERY_VALUE, &key);
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_POS_LEFT, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                graphPosLeft = (w > 2500) ? graphPosLeft : w;
            }
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_POS_TOP, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                graphPosTop = (w > 2048) ? graphPosTop : w;
            }
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_GRAPH_WIDTH, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                graphWidth = (w < 5) ? graphWidth : w;
            }
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_GRAPH_HEIGHT, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                graphHeight = (w < 5) ? graphHeight : w;
            }
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_COLOR_FRAME, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                colorFrame = w;
            }
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_COLOR_BACKGROUND, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                colorBackground = w;
            }
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_COLOR_GRAPH, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                colorGraph = w;
            }
            if (ERROR_SUCCESS == RegQueryValueEx(key, MY_REG_COLOR_GRAPH_DARK, NULL, &wType, (BYTE*)(&w), &wLen))
            {
                colorGraphDark = w;
            }
            RegCloseKey(key);
        }

        void save(HWND* hwnd)
        {
            RECT rect;
            GetWindowRect(*hwnd, &rect);
            HKEY key;
            DWORD w;
            DWORD wLen = 4;
            DWORD wType = REG_DWORD;
            if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, MY_REG_KEY, 0, KEY_SET_VALUE, &key))
            {
                RegCreateKey(HKEY_CURRENT_USER, MY_REG_KEY, &key);
            }
            w = rect.left;
            RegSetValueEx(key, MY_REG_POS_LEFT, NULL, wType, (BYTE*)(&w), wLen);
            w = rect.top;
            RegSetValueEx(key, MY_REG_POS_TOP, NULL, wType, (BYTE*)(&w), wLen);

            w = graphWidth;
            RegSetValueEx(key, MY_REG_GRAPH_WIDTH, NULL, wType, (BYTE*)(&w), wLen);
            w = graphHeight;
            RegSetValueEx(key, MY_REG_GRAPH_HEIGHT, NULL, wType, (BYTE*)(&w), wLen);

            w = colorFrame;
            RegSetValueEx(key, MY_REG_COLOR_FRAME, NULL, wType, (BYTE*)(&w), wLen);
            w = colorBackground;
            RegSetValueEx(key, MY_REG_COLOR_BACKGROUND, NULL, wType, (BYTE*)(&w), wLen);
            w = colorGraph;
            RegSetValueEx(key, MY_REG_COLOR_GRAPH, NULL, wType, (BYTE*)(&w), wLen);
            w = colorGraphDark;
            RegSetValueEx(key, MY_REG_COLOR_GRAPH_DARK, NULL, wType, (BYTE*)(&w), wLen);
            RegCloseKey(key);
        }

        int graphMargin;
        int graphWidth;
        int graphHeight;
        int graphPosLeft;
        int graphPosTop;
        unsigned long colorFrame;
        unsigned long colorBackground;
        unsigned long colorGraph;
        unsigned long colorGraphDark;

};

#endif
