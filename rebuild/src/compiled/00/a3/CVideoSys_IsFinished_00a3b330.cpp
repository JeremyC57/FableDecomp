// CVideoSys::IsFinished() -- true unless the movie is playing or paused; a playing movie whose progress reached 1.0 is stopped first (Stop inlined) and reported as not finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3B330, length 80 bytes.
// Callees: CVideoSys::GetProgress @ 0x00A3B2A0 (float), CVideoSys::WaitForState @ 0x00A3B0F0,
// IMediaControl::Stop (slot 9). The inlined CVideoSys::Stop (0x00A3B1F0, same TU, defined above) folds its
// `State == 0` guard into `test eax,eax ; je` and drops its result.
// Shape: `cmp eax,1 ; je ; cmp eax,2 ; je ; mov al,1` for `State != 1 && State != 2`;
// `fcomp [1.0f] ; fnstsw ; test ah,1 ; jne` is `progress >= 1.0f` (C0 only; `!(p < 1.0f)` tests ah,5 with jnp).
// Model: engine/CVideoSys.h (PDB) has the DirectShow interface pointers at +0..+0x1c, the texture renderer
// at +0x20, PlayMode +0x24, State +0x28, RenderVideo +0x2c; the interfaces are modelled as COM vtables
// (`virtual __stdcall`, `this` pushed) in IDispatch slot order (IMediaControl: Run 7, Pause 8, Stop 9,
// GetState 10; IMediaPosition: get_Duration 7, get_CurrentPosition 9; IBasicAudio: put_Volume 7).
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
struct IBasicAudio : IDispatchRetail {
    virtual long __stdcall put_Volume(long volume);                 // slot 7  (+0x1c)
};
struct CVideoSys {                            // engine/CVideoSys.h layout (no vptr)
    void*           GBuilder;                 // +0x00
    IMediaControl*  MControl;                 // +0x04
    unsigned char   _pad08[0x20];             // +0x08..+0x27
    long            State;                    // +0x28 (0 = nothing loaded, 1 playing, 2 paused, 3 stopped)
    void WaitForState(long state);            // 0x00A3B0F0
    float GetProgress();                      // 0x00A3B2A0
    unsigned char Stop();                     // 0x00A3B1F0 (defined here so it is inlined)
    unsigned char IsFinished();
};

unsigned char CVideoSys::Stop()
{
    if (State == 0)
        return 0;
    MControl->Stop();
    WaitForState(0);
    State = 3;
    return 1;
}

unsigned char CVideoSys::IsFinished()
{
    if (State != 1 && State != 2)
        return 1;
    if (GetProgress() >= 1.0f)
        Stop();
    return 0;
}