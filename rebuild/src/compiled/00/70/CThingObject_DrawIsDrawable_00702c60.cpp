// CThingObject::DrawIsDrawable() -- base CThing test; a dying-off object blinks (drawn on odd timer ticks) once its remaining time, scaled by the game speed, is under 5
// Grade: RELOCATION_MATCH -- the blink parity is `return (DieOffTimer & 1) ? true : false;`, which narrows to the byte load `mov al,[esi+0D4h] ; and al,1`; `(bool)(x & 1)` and `(x & 1) != 0` widen through eax
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00702C60, length 75 bytes (vtable 0x01264DCC slot 27).
// Callees: CThing::DrawIsDrawable() @ 0x004C7790 (non-virtual), GetDisplayEngine() @ 0x004C7A70 (the thing manager's +0x1c
// object), DivideByGameSpeed(float) @ 0x00415FD3 (`fild [0x01375550] ; fdivr [esp+4] ; ret 4`: never reads ecx).
// Layout: DieOffTimer +0xd4, DiesOff +0xd8. The float constant at 0x012462A8 is 5.0f.
// Shape: DivideByGameSpeed is a static `__stdcall` member reached through the engine object:
// the object expression is still evaluated (the 0x004C7A70 call with its result unused) but ecx
// is never loaded; `(float)DieOffTimer` is the `fild` into the argument slot; `x < 5.0f` is
// `fcomp ; test ah,5 ; jp`; retail narrows the parity to the byte load `mov al ; and al,1`
// (the `? true : false` select); the base
// test wraps the body (`if (base) { ... } return false;` keeps retail's `xor al,al` tail).
struct CDisplayEngine {
    static float __stdcall DivideByGameSpeed(float value);   // 0x00415FD3
};
struct CThing {
    virtual void v00();
    CDisplayEngine* GetDisplayEngine();              // 0x004C7A70
    virtual bool DrawIsDrawable();                   // 0x004C7790 (called non-virtually)
};
struct CThingObject : CThing {
    unsigned char _pad04[0xd0];
    long          DieOffTimer;                       // +0xd4
    bool          DiesOff;                           // +0xd8
    virtual bool DrawIsDrawable();
};

bool CThingObject::DrawIsDrawable()
{
    if (CThing::DrawIsDrawable()) {
        if (DiesOff && GetDisplayEngine()->DivideByGameSpeed((float)DieOffTimer) < 5.0f)
            return (DieOffTimer & 1) ? true : false;
        return true;
    }
    return false;
}