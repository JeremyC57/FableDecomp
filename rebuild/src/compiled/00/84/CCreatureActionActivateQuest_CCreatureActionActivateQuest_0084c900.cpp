// CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest&) -- copy constructor: base CCreatureActionBase(other), own vtable (0x012752C4), ScriptName string copy, LoadResources byte, PObjectToUse copied through GetPItem/SetPItem
// Leaf: ??0CCreatureAction_ActivateQuest@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C900, length 97 bytes (+ int3 padding).
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CCharString::CCharString(const CCharString&)
// @ 0x0099EC30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: `other` rides in edi; the base copy constructor runs out of line, the vtable is stored before the string copy's call,
// the flag byte is copied through cl, then the CIntelligentPointer<CThing> member's inline copy constructor (base ctor, its
// vtable 0x01238C6C, SetPItem(other.GetPItem()) -- the landed 0x0084C070 family). `ret 4`.
// Layout: byte-packed as the PDB header (engine/CCreatureAction_ActivateQuest.h: ScriptName +0xb4, LoadResources +0xb8,
// PObjectToUse +0xb9 -- 12 bytes high, the retail CCreatureActionBase being 0xa8 bytes), so `this` is modelled locally under
// #pragma pack(1) with the retail offsets +0xa8 / +0xac / +0xad.
class CBaseObject;

class CCharString { public:                        // one pointer (PStringData)
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};

struct CBaseIntelligentPointer {            // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};

#pragma pack(push, 1)
class CCreatureAction_ActivateQuest : public CCreatureActionBase { public:   // vtable 0x012752C4
    CCharString         ScriptName;         // +0xa8 (PDB +0xb4)
    bool                LoadResources;      // +0xac (PDB +0xb8)
    CIntelligentPointer PObjectToUse;       // +0xad (PDB +0xb9), unaligned
    virtual void v00();
    CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest& other);
};
#pragma pack(pop)

CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest& other)
    : CCreatureActionBase(other)
    , ScriptName(other.ScriptName)
    , LoadResources(other.LoadResources)
    , PObjectToUse(other.PObjectToUse)
{
}