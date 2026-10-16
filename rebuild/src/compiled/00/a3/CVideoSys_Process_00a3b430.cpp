// CVideoSys::Process() -- poll the graph's event queue once; on EC_COMPLETE restart the movie (PlayMode 0) or stop it
// Leaf: ?Process@CVideoSys@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3B430, length 100 bytes (padded to 112).
// Callees: IMediaEvent::GetEvent (slot 8, +0x20; code, param1, param2, timeout 0), CVideoSys::Play @ 0x00A3B130,
// CVideoSys::Stop @ 0x00A3B1F0.
// Model: engine/CVideoSys.h layout with stdcall COM vtables (landed CVideoSys rows); MEvent +0x10, PlayMode +0x24.
// Shape: three zeroed event words in the frame; both arms return true.
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
bool CVideoSys::Process()
{
    long code = 0;
    long param1 = 0;
    long param2 = 0;
    if (MEvent != 0) {
        MEvent->GetEvent(&code, &param1, &param2, 0);
        if (code == 1) {                           // EC_COMPLETE
            if (PlayMode == 0)
                Play();
            else
                Stop();
        }
    }
    return true;
}