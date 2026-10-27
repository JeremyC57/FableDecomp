// Coin::~Coin() -- destroy the CIntelligentPointer member, then the Mass base (which stores its own vtable)
// Leaf: ??1Coin@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EFD50, 19 bytes, thiscall, plain `ret`.
// Identity (probable): it stores the Mass vtable 0x01288230 after destroying a CIntelligentPointer<CThing> at the odd offset +0x25: Ego_r.pdb Coin (the tavern coin-game class, next to Coin::Coin 0x008EFF40 / 0x008F0080) is Mass + Owned (+36) + Thing (+37); no direct callers (its scalar deleting destructor is not in a listed vtable).
// Callees: the member destructor 0x00A01C10.
// Shape (family M034, representative 0x00453E1A): an empty destructor of a `__declspec(novtable)` class (no own vtable store,
//   as in retail) over a base whose inline destructor stores the base vtable; the member destructor runs first.
// Header debt: the class chain is modelled locally (no engine/ header carries the destructors).
class CThing;
template <class T> class CIntelligentPointer {
public:
    virtual ~CIntelligentPointer();                       // 0x00A01C10
    T* PItem;
    void* Next;
};
class C2DVector { public: float X; float Y; };
class Mass {                                              // Ego_r.pdb: vptr + Pos, Vel, IMass, APos, AVel, AIMass (0x24 bytes)
public:
    virtual ~Mass() { }
    C2DVector Pos;
    C2DVector Vel;
    float IMass;
    float APos;
    float AVel;
    float AIMass;
};
#pragma pack(push, 1)
class __declspec(novtable) Coin : public Mass {
public:
    virtual ~Coin();
    bool Owned;                                           // +0x24
    CIntelligentPointer<CThing> Thing;                    // +0x25 (Ego_r.pdb offset 37)
    float Radius;
    int Owner;
    unsigned long Sound;
};
#pragma pack(pop)
Coin::~Coin()
{
}