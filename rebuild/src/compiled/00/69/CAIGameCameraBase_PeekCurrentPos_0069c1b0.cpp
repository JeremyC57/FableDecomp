// CAIGameCameraBase::PeekCurrentPos() -- returns the camera position evaluated for a full blend (1.0f) into a scratch camera state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069C1B0, length 46 bytes. No direct callees
// (slot-1 virtual on `this`: `call [eax+4]` with a 0x38-byte uninitialised stack state and 1.0f).
// The C3DVector result comes back through the hidden pointer ([esp+3Ch] after the frame);
// the virtual returns a `const C3DVector&` whose three floats are copied one register per dword
// (edx, edx, eax: the memberwise user copy constructor; a POD copy would use three registers), then `mov eax,ecx` returns the hidden pointer and the
// 0x38-byte state is dropped (`add esp,38h`). The vtable is fetched before the frame is built.
class C3DVector { public:                          // three floats; moved as dwords by the memberwise copy
    unsigned long X;                        // (a float member would route the copy through the FPU,
    unsigned long Y;                        //  GOTCHAS wave 19; retail moves each dword with `mov`)
    unsigned long Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

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