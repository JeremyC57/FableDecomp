// CTCSoundPlayer::CTCSoundPlayer(CThing* parent) -- thing-component base, null attached-emitter pointer, two empty strings, zero clip distance and material index
// Leaf: CTCSoundPlayer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066AC80, length 81 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CCharString::CCharString() @ 0x0099E4B0 (twice).
// Vtables: 0x0125B1C4 = CTCSoundPlayer, 0x0125B1B8 = CIntelligentPointer (rtti-vtables.tsv).
// Layout: Ego_r.pdb CTCSoundPlayer (0x24 bytes): PAttachedEmitter (CIntelligentPointer<CThingSoundEmitter>) +0x0c,
// VoiceName +0x14, ExtraTag +0x18, AnimCriteriaClipDistance (short) +0x1c, LastMaterialIndex +0x20.
// Shape: the derived vtable store comes before the member constructors; the intelligent pointer is built by an
// inline converting constructor from a null pointer (base constructor, own vtable, SetPItem(0)).
struct CThing;
struct CBaseObject;
struct CThingSoundEmitter;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* parent);                                // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThingSoundEmitter>, vtable 0x0125B1B8
    virtual void v00();
    CIntelligentPointer(CThingSoundEmitter* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CCharString {
    void* m_pData;
    CCharString();                                          // 0x0099E4B0
};

struct CTCSoundPlayer : CTCBase {
    CIntelligentPointer PAttachedEmitter;                   // +0x0c
    CCharString         VoiceName;                          // +0x14
    CCharString         ExtraTag;                           // +0x18
    short               AnimCriteriaClipDistance;           // +0x1c
    long                LastMaterialIndex;                  // +0x20
    virtual void v00();                                     // own vtable 0x0125B1C4
    CTCSoundPlayer(CThing* parent);
};

CTCSoundPlayer::CTCSoundPlayer(CThing* parent)
    : CTCBase(parent)
    , PAttachedEmitter(0)
    , AnimCriteriaClipDistance(0)
    , LastMaterialIndex(0)
{
}
