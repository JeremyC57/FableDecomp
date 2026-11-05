// CAIGameCameraBase::PeekCurrentPos() -- returns the camera position evaluated for a full blend (1.0f) into a scratch camera state
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069C1B0, length 46 bytes. No direct callees
// (slot-1 virtual on `this`: `call [eax+4]` with a 0x38-byte uninitialised stack state and 1.0f).
// The C3DVector result comes back through the hidden pointer ([esp+3Ch] after the frame);
// the virtual returns a `const C3DVector&` whose three floats are copied one register per dword
// (edx, edx, eax: the memberwise user copy constructor; a POD copy would use three registers), then `mov eax,ecx` returns the hidden pointer and the
// 0x38-byte state is dropped (`add esp,38h`). The vtable is fetched before the frame is built.
#include "engine/C3DVector.h"

struct SCameraState {                       // 0x38 bytes, filled by the virtual
    unsigned char _raw[0x38];
};

class CAIGameCameraBase { public:
    virtual void v00();
    virtual const C3DVector& Evaluate(SCameraState& state, float blend);   // slot 1 (+0x04)
    C3DVector PeekCurrentPos();
};

C3DVector CAIGameCameraBase::PeekCurrentPos()
{
    SCameraState state;
    return Evaluate(state, 1.0f);
}