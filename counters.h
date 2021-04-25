#ifndef __PDH_H__
#define __PDH_H__

#include <windows.h>
#include "pdh.h"

enum {kHistory = 1024};

class PDH
{
        HQUERY query;
        HCOUNTER counter;
        TCHAR counterpath[256];
        long history[kHistory];
        long historyPos;

    public:
        PDH()
        {
            query = NULL;
            counter = NULL;
            historyPos = 0;
            PdhOpenQuery(NULL, 0, &query);
            for (int i = 0; i < kHistory; i++)
            {
                history[i] = 0;
            }
        }
        ~PDH()
        {
            PdhCloseQuery(query);
        }
        long get(int idx)
        {
            return history[(kHistory + kHistory + historyPos - 1 - idx) % kHistory];
        }
        void setCounter(char* path)
        {
            if (counter)
            {
                PdhRemoveCounter(counter);
            }
            PdhAddCounter(query, path, 0, &counter);
            strcpy_s(counterpath, path);
        }
        void tick()
        {
            PDH_FMT_COUNTERVALUE fmtValue;
            if (!counter)
            {
                return;
            }
            PdhCollectQueryData(query);
            PdhGetFormattedCounterValue(counter, PDH_FMT_LONG | PDH_FMT_NOSCALE, 0, &fmtValue);
            history[historyPos] = fmtValue.longValue;
            historyPos = (historyPos + 1) % kHistory;
        }
};

#endif // __PDH_H__
