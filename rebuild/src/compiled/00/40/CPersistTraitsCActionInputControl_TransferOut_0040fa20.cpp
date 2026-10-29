// CPersistTraits<CActionInputControl>::TransferOut(CPersistContext* context, CCharString* out, CActionInputControl* value) -- text persist write of a key binding as "CActionInputControl(action,controller,key[,(x,y)])": the key field chosen by the controller type, the direction only for the two analogue actions
// Leaf: ?TransferOut@?$CPersistTraits@VCActionInputControl@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040FA20, 287 bytes, `ret 0Ch` (traits `this` unused).
// Identity: called by the landed CPersistContext_TransferableVector<CActionInputControl>::TransferOut 0x0040F9D0 (its
//   model names this address); Ego_r.pdb CActionInputControl (0x1C bytes) = { EGameAction GameAction +0, EControllerType
//   ControllerType +4, EInputKey KeyboardKey +8, EXboxControllerButton XboxButton +0xC, EMouseButtonControl MouseButton
//   +0x10, C2DVector ControlDirection +0x14 }.
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 ("CActionInputControl" 0x0122E02C, "(" 0x0122E028,
//   "," 0x0122E024, ")" 0x0122E020), TransferOutLong @ 0x009E2560 (`__fastcall` context, out, &copy), CPersistTraits<C2DVector>
//   ::TransferOut @ 0x009E7F00 (thiscall on a zero-defaulted traits temporary). Float 0x0129BA3C (1e-4f).
// Shape: the landed 0x0057F78F text writer: each long through an inline CPersistTraits<long>::TransferOut (copy into the
//   dead `out` argument slot, then TransferOutLong); the controller switch (1 Xbox button, 2 keyboard key, 3 mouse button)
//   is a `dec eax ; je` chain with one merged call; the direction is written when its squared length beats eps*eps and the
//   action is 0x3C or 0x72 (the float test first). Speed-optimised (default) TU.
class CPersistContext;
class CCharString {
public:
    class CCharStringData* PStringData;
    const CCharString& operator+=(const char* text);         // 0x0099F100
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

extern const float g_Epsilon_0129ba3c;                 // 0x0129BA3C (1e-4f)

class C2DVector {
public:
    float X;
    float Y;
    C2DVector() : X(0.0f), Y(0.0f) {}
    float GetLengthSquared() const { return X * X + Y * Y; }
};

class CActionInputControl {                            // 0x1C bytes
public:
    long      GameAction;                              // +0x00 EGameAction
    long      ControllerType;                          // +0x04 EControllerType
    long      KeyboardKey;                             // +0x08 EInputKey
    long      XboxButton;                              // +0x0C EXboxControllerButton
    long      MouseButton;                             // +0x10 EMouseButtonControl
    C2DVector ControlDirection;                        // +0x14
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const long* value) const
    {
        long copy = *value;
        TransferOutLong(context, out, &copy);
    }
};
template <> struct CPersistTraits<C2DVector> {
    C2DVector DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, C2DVector* value);   // 0x009E7F00
};
template <> struct CPersistTraits<CActionInputControl> {
    CActionInputControl DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, CActionInputControl* value);
};

void CPersistTraits<CActionInputControl>::TransferOut(CPersistContext* context, CCharString* out, CActionInputControl* value)
{
    *out += "CActionInputControl";
    *out += "(";
    CPersistTraits<long> longTraits;
    longTraits.TransferOut(context, out, &value->GameAction);
    *out += ",";
    longTraits.TransferOut(context, out, &value->ControllerType);
    *out += ",";
    switch (value->ControllerType) {
    case 1:
        longTraits.TransferOut(context, out, &value->XboxButton);
        break;
    case 2:
        longTraits.TransferOut(context, out, &value->KeyboardKey);
        break;
    case 3:
        longTraits.TransferOut(context, out, &value->MouseButton);
        break;
    }
    if (value->ControlDirection.GetLengthSquared() > g_Epsilon_0129ba3c * g_Epsilon_0129ba3c &&
        (value->GameAction == 0x3C || value->GameAction == 0x72)) {
        *out += ",";
        CPersistTraits<C2DVector>().TransferOut(context, out, &value->ControlDirection);
    }
    *out += ")";
}