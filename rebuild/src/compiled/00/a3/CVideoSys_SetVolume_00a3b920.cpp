// CVideoSys::SetVolume(float volume) -- when the graph has audio, clamp the linear volume to >= 0.001 and set IBasicAudio::put_Volume to 500 * ln(volume)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3B920, length 80 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (the long conversion), IBasicAudio::put_Volume (slot 7, +0x1c).
// Constants: 0x01266300 = 0.001f (the clamp floor), 0x3A83126F = 0.001f written into the argument slot,
// 0x01237584 = 500.0f.
// Shape: `fld arg ; fcomp [floor] ; fnstsw ; test ah,5 ; jp` is `volume < 0.001f` (C0|C2 parity), the
// clamp stores the immediate into the parameter slot; `fldln2 ; fxch ; fyl2x` is the `log` intrinsic on
// the re-read parameter, `fmul [500.0f]`, __ftol2, then the COM call with BasicAudio re-read into esi
// and its vtable in edi; `ret 4`.
#include <math.h>
#pragma intrinsic(log)
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
    void*           MSeeking;                 // +0x0c
    void*           MEvent;                   // +0x10
    IBasicAudio*    BasicAudio;               // +0x14
    void SetVolume(float volume);
};

void CVideoSys::SetVolume(float volume)
{
    if (BasicAudio) {
        if (volume < 0.001f)
            volume = 0.001f;
        BasicAudio->put_Volume((long)(log(volume) * 500.0f));
    }
}