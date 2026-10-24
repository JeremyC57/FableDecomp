// Coin::Coin(float radius, float density, CThing* thing, bool owned) -- Mass base zeroed, Owned and the Thing pointer set, Radius stored, IMass = density / (radius^2 * 2pi), AIMass = IMass * 2.5 / radius^2, Sound invalid
// Grade: DIFFER(ours v retail) -- 223v231: retail loads radius and density onto the x87 stack up front (`fld r ; fld d ; fld st(1) ; fmul st(2)`, later `fstp st(1)`) and recomputes radius^2 from memory for AIMass; VC7.1 here CSEs radius*radius across both statements (`fld r ; fmul r` ... `fdivrp`)
// Leaf: ??0Coin@@QAE@MMPAVCThing@@_N@Z
// Retail: Fable.exe @ 0x008EF200, 231 bytes, `__thiscall`, `ret 10h` (Ego_r.pdb ??0Coin@@QAE@MMPAVCThing@@_N@Z). Constants: 6.283185307
// (double 0x01288248), 2.5 (double 0x01288240), g_InvalidSoundHandle 0x0129D25C.
// Layout (Ego_r.pdb Coin = 60 bytes, matches retail): Mass base (vtable 0x01288230: +4 Pos, +0xC Vel, +0x14 IMass, +0x18 APos,
// +0x1C AVel, +0x20 AIMass), +0x24 bool Owned, +0x25 CIntelligentPointer<CThing> Thing (8 bytes at an ODD offset: the
// intelligent-pointer classes are `pack(1)`, Coin itself is not, so Radius re-aligns to +0x30), +0x30 Radius, +0x34 Owner,
// +0x38 Sound. Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10;
// CIntelligentPointer<CThing> vtable 0x01238C6C. Used by vector<Coin> in CTCTavernGameCoinBase (landed AddNewCoin 0x008EFA00).
// Shape: the inline Mass() of the landed 0x008EE9B0 (C2DVector temporaries assigned); Mass's vtable store is dead and
// dropped; Owned and Thing are assigned in the body (the member pointer is default-built first, then a temporary
// CIntelligentPointer<CThing>(thing) is assigned and destroyed).
class CThing;
class CBaseObject;
extern unsigned long g_InvalidSoundHandle;               // 0x0129D25C (0xFFFFFFFF)

struct C2DVector {
    float X;                                 // +0x00
    float Y;                                 // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

class Mass {                                 // vtable 0x01288230, 0x24 bytes (landed Mass::Mass 0x008EE9B0)
public:
    C2DVector Pos;                           // +0x04
    C2DVector Vel;                           // +0x0C
    float IMass;                             // +0x14
    float APos;                              // +0x18
    float AVel;                              // +0x1C
    float AIMass;                            // +0x20
    Mass()
    {
        Pos = C2DVector(0.0f, 0.0f);
        Vel = C2DVector(0.0f, 0.0f);
        APos = 0.0f;
        AVel = 0.0f;
    }
    virtual ~Mass() { }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {             // 8 bytes, byte-aligned (Coin::Thing sits at the odd offset +0x25)
    void* m_pItem;                           // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    virtual void Touch() { }
};
#pragma pack(pop)

class Coin : public Mass {                   // vtable 0x01288238, 0x3C bytes
public:
    bool                        Owned;       // +0x24
    CIntelligentPointer<CThing> Thing;       // +0x25
    float                       Radius;      // +0x30
    int                         Owner;       // +0x34
    unsigned long               Sound;       // +0x38
    Coin(float radius, float density, CThing* thing, bool owned);
    Coin(const Coin& other);
    Coin& operator=(const Coin& other);
};

Coin::Coin(float radius, float density, CThing* thing, bool owned)
{
    Owned = owned;
    Thing = CIntelligentPointer<CThing>(thing);
    Radius = radius;
    IMass = density / (radius * radius * 6.283185307);
    AIMass = IMass * 2.5 / (radius * radius);
    Sound = g_InvalidSoundHandle;
}
