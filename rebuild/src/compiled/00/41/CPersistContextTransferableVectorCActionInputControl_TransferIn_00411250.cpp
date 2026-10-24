// CPersistContext_TransferableVector<CActionInputControl>::TransferIn(CPersistContext* context, CStringParser* parser, long index) -- parses key binding `index` through a default CPersistTraits<CActionInputControl>
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00411250, 72 bytes (+8 int3 padding), `ret 0Ch`.
// Callee: 0x004112A0 CPersistTraits<CActionInputControl>::TransferIn(CPersistContext*, CStringParser*,
// CActionInputControl*) (thiscall, `ret 0Ch`; its manifest label `CPersistTraits<CSimVoice>` is propagated).
// Persist READ hook (vtable 0x0122E010 slot 3; slot 2 is TransferOut 0x0040F9D0, slot 1 Resize 0x00412C10, slot 0
// the count 0x0040F9B0) of CPersistContext_TransferableVector<CActionInputControl> { vptr; vector<T>* TransVector; }.
// The manifest's `<long>` is propagated: the 28-byte stride (`imul ecx,ecx,1Ch`) and the traits callee name the type
// (Ego_r.pdb CActionInputControl, 0x1c bytes: five enums and a C2DVector ControlDirection). The traits object
// ({ CActionInputControl DefaultVal; }, the `sub esp,1Ch` frame) is zeroed member by member through one register
// by the element's inline default constructor, ahead of the argument loads; no destructor runs.
struct CPersistContext;
struct CStringParser;
struct CCharString;

class C2DVector {
public:
    float X;
    float Y;
};

struct CActionInputControl {
    long      GameAction;                          // +0x00 EGameAction
    long      ControllerType;                      // +0x04 EControllerType
    long      KeyboardKey;                         // +0x08 EInputKey
    long      XboxButton;                          // +0x0c EXboxControllerButton
    long      MouseButton;                         // +0x10 EMouseButtonControl
    C2DVector ControlDirection;                    // +0x14
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0)
    {
        ControlDirection.X = 0.0f;
        ControlDirection.Y = 0.0f;
    }
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);       // 0x0040FA20 for CActionInputControl
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x004112A0 for CActionInputControl
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CActionInputControl>* TransVector;   // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CActionInputControl> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}