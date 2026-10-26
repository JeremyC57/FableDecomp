// Coin::Coin(const Coin& other) -- copy constructor: Mass base copied word by word, then Owned, the Thing intelligent pointer (re-registered through SetPItem), Radius, Owner, Sound
// Leaf: ??0Coin@@QAE@ABV0@@Z
// Retail: Fable.exe @ 0x008EFF40, 131 bytes, `__fastcall`, `ret 4` (Ego_r.pdb ??0Coin@@QAE@ABV0@@Z). Called by the vector<Coin>
// placement copies (AddNewCoin 0x008EFA00 and the STL helpers).
// Layout (Ego_r.pdb Coin = 60 bytes, matches retail): Mass base (vtable 0x01288230: +4 Pos, +0xC Vel, +0x14 IMass, +0x18 APos,
// +0x1C AVel, +0x20 AIMass), +0x24 bool Owned, +0x25 CIntelligentPointer<CThing> Thing (8 bytes at an ODD offset: the
// intelligent-pointer classes are `pack(1)`, Coin itself is not, so Radius re-aligns to +0x30), +0x30 Radius, +0x34 Owner,
// +0x38 Sound. Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10;
// CIntelligentPointer<CThing> vtable 0x01238C6C. Used by vector<Coin> in CTCTavernGameCoinBase (landed AddNewCoin 0x008EFA00).
// Shape: Mass's implicit copy (its vtable store, then the eight words) inlined; Coin's vtable store follows; the
// pointer's inline copy constructor runs the out-of-line base ctor, stores its own vtable, then SetPItem(other.GetPItem()).
// Init-list in declaration order.
class CThing;
class CBaseObject;
extern unsigned long g_InvalidSoundHandle;               // 0x0129D25C (0xFFFFFFFF)

class C2DVector { public:
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
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

Coin::Coin(const Coin& other)
    : Mass(other), Owned(other.Owned), Thing(other.Thing), Radius(other.Radius), Owner(other.Owner), Sound(other.Sound)
{
}