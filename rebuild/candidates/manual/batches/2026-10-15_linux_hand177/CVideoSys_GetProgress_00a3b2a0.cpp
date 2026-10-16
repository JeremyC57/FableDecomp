// CVideoSys::GetProgress() -- playback progress as current position / duration (0 when no movie or a zero length)
// Leaf: ?GetProgress@CVideoSys@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3B2A0, length 128 bytes.
// Callees: IMediaPosition::get_CurrentPosition (slot 9, +0x24) and get_Duration (slot 7, +0x1c) through the inlined
// CVideoSys::GetCurTime (0x00A3B260) and GetLength (0x00A3B220). Data: 0.0 @ 0x0122ED70 (qword), 0.0f @ 0x0122DEDC.
// Model: engine/CVideoSys.h layout with stdcall COM vtables (landed CVideoSys rows).
// Shape: both accessors defined in the class so they inline; the length stays an 80-bit value for the zero test and the divide.
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
float CVideoSys::GetProgress()
{
    float time = GetCurTime();
    float length = GetLength();
    if (length == 0.0f)
        return 0.0f;
    return time / length;
}
