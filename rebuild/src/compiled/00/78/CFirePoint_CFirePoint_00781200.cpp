// CFirePoint::CFirePoint(CThing* parentThing, const CCharString& helperName, long helperIndex, long effectIndex, long resistance) -- copy the helper name, store the indices, resistance and parent, clear Burning and FireDamageAfterFrame, construct the two effect pointers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00781200, length 89 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (out of line: fable_string.h
// would inline it, so the string is modelled locally), CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (x2). Vtable 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Identity: no vtable store and `ret 14h` = a five-argument constructor of a plain record; Ego_r.pdb
// CFirePoint (44 bytes) fits every store: HelperName +0, HelperIndex +4, EffectIndex +8, Resistance
// +0xc, Burning +0x10, FireDamageAfterFrame +0x14, PParentThing +0x18, PEffect +0x1c, PSingeEffect +0x24.
// The caller (0x00780B6E, inside the fire-point collection loop beginning 0x007808B0) builds one in a
// frame slot and pushes it into a vector of 0x2c-byte elements. The manifest label
// "CTCAvoidRegionExit::CTCAvoidRegionExit" is propagated.
// Shape: the init list in declaration order; VC interleaves the argument loads with the stores and
// shares one `xor eax,eax` between Burning and FireDamageAfterFrame.
class CThing;
class CBaseObject;

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CFirePoint {
    CCharString         HelperName;                         // +0x00
    long                HelperIndex;                        // +0x04
    long                EffectIndex;                        // +0x08
    long                Resistance;                         // +0x0c
    bool                Burning;                            // +0x10
    long                FireDamageAfterFrame;               // +0x14
    CThing*             PParentThing;                       // +0x18
    CIntelligentPointer<CThing> PEffect;                            // +0x1c
    CIntelligentPointer<CThing> PSingeEffect;                       // +0x24
    CFirePoint(CThing* parentThing, const CCharString& helperName, long helperIndex, long effectIndex, long resistance);
};

CFirePoint::CFirePoint(CThing* parentThing, const CCharString& helperName, long helperIndex, long effectIndex, long resistance)
    : HelperName(helperName)
    , HelperIndex(helperIndex)
    , EffectIndex(effectIndex)
    , Resistance(resistance)
    , Burning(false)
    , FireDamageAfterFrame(0)
    , PParentThing(parentThing)
{
}