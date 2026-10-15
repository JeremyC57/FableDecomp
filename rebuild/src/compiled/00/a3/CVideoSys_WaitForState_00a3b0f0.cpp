// CVideoSys::WaitForState(long state) -- poll IMediaControl::GetState (10 ms timeout) up to 100 times until the graph reports the wanted state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3B0F0, length 64 bytes. No direct callees (one COM call).
// Shape: the wanted state in ebx, the counter in esi, the out word in the frame (`lea edx,[esp+10h]`);
// `do { GetState(10, &current); ++i; } while (current != state && i < 100)` gives the aligned loop head
// (`jmp` over the `lea ecx,[ecx]` pad), the compare `cmp ebx,eax` with the argument on the left, and
// `cmp esi,64h ; jl` for a signed counter; `ret 4`.
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
    IMediaPosition* MPosition;                // +0x08
    void WaitForState(long state);
};

void CVideoSys::WaitForState(long state)
{
    long current;
    long i = 0;
    do {
        MControl->GetState(10, &current);
        ++i;
    } while (state != current && i < 100);
}