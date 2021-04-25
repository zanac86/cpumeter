#ifndef SINGLE_INSTANCE_HEADER
#define SINGLE_INSTANCE_HEADER

#include <windows.h>

class SingleInstance
{

    protected:
        DWORD lastError;
        HANDLE mutex;

    public:
        SingleInstance()
        {
            mutex = CreateMutex(NULL, FALSE, "{0E248886-25F9-45ce-9333-15E55EBD133E}----vb");
            lastError = GetLastError();
        }

        ~SingleInstance()
        {
            if (mutex)
            {
                CloseHandle(mutex);
                mutex = NULL;
            }
        }

        bool isAnotherInstanceRunning()
        {
            return (ERROR_ALREADY_EXISTS == lastError);
        }
};

#endif
