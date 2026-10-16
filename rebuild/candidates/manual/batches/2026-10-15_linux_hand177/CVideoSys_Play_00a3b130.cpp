// CVideoSys::Play() -- restart a loaded movie: rewind (retrying put_CurrentPosition(0) until it succeeds), try IMediaControl::Run up to 50 times, then wait for State_Running and record state 1
// Leaf: ?Play@CVideoSys@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3B130, length 112 bytes.
// Callees: IMediaPosition::put_CurrentPosition (slot 8, +0x20, a double pushed as a qword), IMediaControl::Run (slot 7, +0x1c),
// CVideoSys::WaitForState(_FilterState) @ 0x00A3B0F0. Data: 0.0 @ 0x0122ED70 (qword).
// Model: engine/CVideoSys.h layout (DirectShow interface pointers +0..+0x1c, PlayMode +0x24, State +0x28) with the COM
// interfaces as stdcall vtables, as the landed CVideoSys rows (0x00A3B1C0 Pause, 0x00A3B260 GetCurTime).
// Shape: the top-tested retry loop `while (tries < 50 && (++tries, Run() != 0))` (a for/while with `if (...) break` is rotated
// to a bottom test); the counter is compared with 50 after the loop.
struct IDispatchRetail {
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetTypeInfoCount(unsigned int* count);
    virtual long __stdcall GetTypeInfo(unsigned int index, unsigned long lcid, void** info);
    virtual long __stdcall GetIDsOfNames(void* iid, void* names, unsigned int count, unsigned long lcid, long* ids);
    virtual long __stdcall Invoke(long id, void* iid, unsigned long lcid, unsigned short flags, void* params, void* result, void* excep, unsigned int* err);
};
struct IMediaControl : IDispatchRetail {
    virtual long __stdcall Run();                                   // slot 7  (+0x1c)
    virtual long __stdcall Pause();                                 // slot 8  (+0x20)
    virtual long __stdcall Stop();                                  // slot 9  (+0x24)
    virtual long __stdcall GetState(long timeout, long* state);     // slot 10 (+0x28)
};
struct IMediaPosition : IDispatchRetail {
    virtual long __stdcall get_Duration(double* length);            // slot 7  (+0x1c)
    virtual long __stdcall put_CurrentPosition(double time);        // slot 8  (+0x20)
    virtual long __stdcall get_CurrentPosition(double* time);       // slot 9  (+0x24)
};
struct IMediaEvent : IDispatchRetail {
    virtual long __stdcall GetEventHandle(long* handle);            // slot 7  (+0x1c)
    virtual long __stdcall GetEvent(long* code, long* param1, long* param2, long timeout);   // slot 8 (+0x20)
};
enum _FilterState { State_Stopped = 0, State_Paused = 1, State_Running = 2 };
struct CVideoSys {                                 // engine/CVideoSys.h layout (no vptr), DirectShow graph wrapper
    void*           GBuilder;                      // +0x00 IGraphBuilder*
    IMediaControl*  MControl;                      // +0x04
    IMediaPosition* MPosition;                     // +0x08
    void*           MSeeking;                      // +0x0c
    IMediaEvent*    MEvent;                        // +0x10
    void*           BasicAudio;                    // +0x14
    void*           PAsfReader;                    // +0x18
    void*           PFileSource;                   // +0x1c
    void*           TexRenderer;                   // +0x20
    long            PlayMode;                      // +0x24 PTYPE (0 = play once, else loop/stop)
    long            State;                         // +0x28 (0 = nothing loaded, 1 playing, 2 paused, 3 stopped)
    void WaitForState(_FilterState state);         // 0x00A3B0F0
    bool Play();                                   // 0x00A3B130
    unsigned char Stop();                          // 0x00A3B1F0
    float GetCurTime()                             // 0x00A3B260 (landed; inline here)
    {
        if (MPosition == 0)
            return 0.0f;
        double time = 0.0;
        MPosition->get_CurrentPosition(&time);
        return (float)time;
    }
    float GetLength()                              // 0x00A3B220 (landed; inline here)
    {
        if (MPosition == 0)
            return 0.0f;
        double length = 0.0;
        MPosition->get_Duration(&length);
        return (float)length;
    }
    float GetProgress();
    bool Process();
    void TogglePause(bool pause);
};
bool CVideoSys::Play()
{
    if (State != 0 && MControl != 0 && MPosition != 0) {
        while (MPosition->put_CurrentPosition(0.0) != 0)
            ;
        long tries = 0;
        while (tries < 50 && (++tries, MControl->Run() != 0))
            ;
        if (tries != 50) {
            WaitForState(State_Running);
            State = 1;
            return true;
        }
    }
    return false;
}
