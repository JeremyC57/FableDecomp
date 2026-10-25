// CMsgThread::DefaultThreadProc(void* pParam) -- DirectShow msgthrd.cpp worker thread entry: CoInitialize, OnThreadInit, then GetThreadMsg/ThreadMessageProc until a non-zero result, CoUninitialize
// Leaf: DefaultThreadProc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0FC0, 109 bytes (+ int3 padding). WINAPI (__stdcall) static
// member, `ret 4`; its address is taken once (0x00CB1125, CMsgThread::CreateThread's CreateThread argument).
// Library: DirectShow base classes (strmbase msgthrd.cpp). The manifest label "COM_CoInitializeMainLoop" is
// descriptive.
// Callees: ole32 CoInitialize (IAT 0x01440644), CoUninitialize (IAT 0x01440648); virtual CMsgThread::GetThreadMsg
// (slot 0), OnThreadInit (slot 1), ThreadMessageProc (slot 2).
// Shape: the pre-2005 msgthrd.h, whose ~CMsgThread is NOT virtual, so GetThreadMsg is vtable slot 0. CMsg's
// default constructor zeroes its four members (one zero register shared with the CoInitialize(NULL) push).
extern "C" __declspec(dllimport) long __stdcall CoInitialize(void* pvReserved);         // IAT 0x01440644
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();                        // IAT 0x01440648

class CAMEvent;

class CMsg {
public:
    unsigned int  uMsg;                             // +0x00
    unsigned long dwFlags;                          // +0x04
    void*         lpParam;                          // +0x08
    CAMEvent*     pEvent;                           // +0x0c
    CMsg() : uMsg(0), dwFlags(0L), lpParam(0), pEvent(0) {}
};

class CMsgThread {
private:
    static unsigned long __stdcall DefaultThreadProc(void* lpParam);
    unsigned long m_ThreadId;
    void*         m_hThread;
public:
    virtual void GetThreadMsg(CMsg* msg);           // slot 0
    virtual void OnThreadInit() {}                  // slot 1
    virtual long ThreadMessageProc(unsigned int uMsg, unsigned long dwFlags,
                                   void* lpParam, CAMEvent* pEvent) = 0;   // slot 2
};

unsigned long __stdcall CMsgThread::DefaultThreadProc(void* pParam)
{
    CMsgThread* lpThis = (CMsgThread*)pParam;
    CMsg msg;
    long lResult;

    CoInitialize(0);

    lpThis->OnThreadInit();

    do {
        lpThis->GetThreadMsg(&msg);
        lResult = lpThis->ThreadMessageProc(msg.uMsg, msg.dwFlags, msg.lpParam, msg.pEvent);
    } while (lResult == 0L);

    CoUninitialize();

    return (unsigned long)lResult;
}