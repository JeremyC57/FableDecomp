// CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase*, CThing*, const CCharString&, bool) -- base CCreatureActionBase(owner, zeroed CSoundPair, 0, 0), own vtable (0x012752C4), ScriptName copied from the string, LoadResources from the flag, PObjectToUse set to the thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843F50, length 109 bytes (+ int3 padding).
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long) @ 0x00693C70,
// CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the sound pair is zeroed through one register (eax, which also supplies the two `push 0`), the base arguments
// go right to left (0, 0, the pair, the owner); then the init-list members in declaration order: the string copy at
// +0xa8 (vtable stored just before its call), the flag byte at +0xac, and the CIntelligentPointer<CThing> at +0xad
// (base ctor, vtable 0x01238C6C, SetPItem(thing)). The arguments are read from their slots after the base call
// (name = arg 2, flag = arg 3, thing = arg 1). `ret 10h`: four arguments.
// Layout: byte-packed as the PDB header (engine/CCreatureAction_ActivateQuest.h: ScriptName +0xb4, LoadResources +0xb8,
// PObjectToUse +0xb9 -- 12 bytes high, the retail CCreatureActionBase being 0xa8 bytes), so `this` is modelled locally
// under #pragma pack(1) with the retail offsets +0xa8 / +0xac / +0xad.
struct CThingCreatureBase;
struct CThing;
struct CBaseObject;

struct CCharString {                        // one pointer (PStringData)
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed frame stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long c, long d);   // 0x00693C70
};

#pragma pack(push, 1)
struct CCreatureAction_ActivateQuest : CCreatureActionBase {   // vtable 0x012752C4
    CCharString         ScriptName;         // +0xa8 (PDB +0xb4)
    bool                LoadResources;      // +0xac (PDB +0xb8)
    CIntelligentPointer PObjectToUse;       // +0xad (PDB +0xb9), unaligned
    virtual void v00();
    CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources);
};
#pragma pack(pop)

CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources)
    : CCreatureActionBase(owner, CSoundPair(), 0, 0)
    , ScriptName(scriptName)
    , LoadResources(loadResources)
    , PObjectToUse(objectToUse)
{
}