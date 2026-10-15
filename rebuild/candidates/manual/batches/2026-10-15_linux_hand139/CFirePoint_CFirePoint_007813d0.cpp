// CFirePoint::CFirePoint(const CFirePoint&) -- copy constructor: HelperName by the out-of-line CCharString copy, the scalar fields word by word, the two effect intelligent pointers re-pointed at the source's items
// Leaf: ??0CFirePoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007813D0, length 123 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (out of line, so the string is modelled locally),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90 (each x2);
// vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Layout: Ego_r.pdb CFirePoint (44 bytes), as the landed CFirePoint constructor 0x00781200.
// Shape: the implicit-style member-wise copy in declaration order; the CIntelligentPointer copy constructor is inline
// (base ctor, vtable store, SetPItem(other.GetPItem())), its member address held in ebx for both calls.
struct CThing;

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                        // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CFirePoint {
    CCharString         HelperName;                         // +0x00
    long                HelperIndex;                        // +0x04
    long                EffectIndex;                        // +0x08
    long                Resistance;                         // +0x0c
    bool                Burning;                            // +0x10
    long                FireDamageAfterFrame;               // +0x14
    CThing*             PParentThing;                       // +0x18
    CIntelligentPointer PEffect;                            // +0x1c
    CIntelligentPointer PSingeEffect;                       // +0x24
    CFirePoint(const CFirePoint& other);
};

CFirePoint::CFirePoint(const CFirePoint& other)
    : HelperName(other.HelperName)
    , HelperIndex(other.HelperIndex)
    , EffectIndex(other.EffectIndex)
    , Resistance(other.Resistance)
    , Burning(other.Burning)
    , FireDamageAfterFrame(other.FireDamageAfterFrame)
    , PParentThing(other.PParentThing)
    , PEffect(other.PEffect)
    , PSingeEffect(other.PSingeEffect)
{
}
