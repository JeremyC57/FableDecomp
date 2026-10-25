// _global::AsyncThreadProc(void* launcher) -- the async launcher's worker-thread procedure: call the launcher's virtual slot 1 (one unit of work, nonzero when finished) until it reports done, then release the launcher's completion semaphore (+0x04) and return 0
// Leaf: AsyncThreadProc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A210, length 32 bytes (30 + int3 padding), __stdcall (`ret 4`, ecx unused).
// Reference: the only use is CAsyncThreadLauncher::StartThread @ 0x00C2A270 (landed), which passes it to CreateThread with
//   the launcher as the parameter (`push 0x00C2A210` at 0x00C2A27B); the landed source declares it as this free function.
// Callees: the launcher's virtual slot 1 (+0x04, an int result), the semaphore handle's Release() @ 0x00C36170 (a bare
//   ReleaseSemaphore(handle, 1, NULL) on +0x04).
// Name: no PDB covers libpk; the names are descriptive (the catalog's `SpinWaitUntilCondition` describes the loop).
// Shape: the launcher pointer lives in esi; the empty-bodied `while (!launcher->Process());` loop is bottom-tested on the
//   virtual call's result.
class CSemaphoreHandle {                    // a Win32 semaphore handle word
public:
    void Release();                         // 0x00C36170
private:
    void* m_handle;
};

class CAsyncThreadLauncher {
public:
    virtual void v00();
    virtual int  Process();                 // slot 1 (+0x04): run one unit, nonzero when finished
    CSemaphoreHandle m_finished;            // +0x04 released when the worker loop ends
};

unsigned long __stdcall AsyncThreadProc(void* parameter)
{
    CAsyncThreadLauncher* launcher = (CAsyncThreadLauncher*)parameter;
    while (!launcher->Process())
        ;
    launcher->m_finished.Release();
    return 0;
}