// CCreatureAction_SummonerLightningOrbInto::CCreatureAction_SummonerLightningOrbInto(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, thing, 150, zeroed CSoundPair), own vtable, an empty orb pointer, then the name "LIGHTNING_ORB_INTO" through the setter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CBD90, length 116 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (the member at +0x114; the inline CIntelligentPointer constructor then stores vtable 0x01238C6C), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 (literal 0x012830D0 "LIGHTNING_ORB_INTO"), CCombatActionBase::SetName(const CCharString&) @ 0x008574D0 (landed naming; the manifest
// "CTCInGameMenu::SetPDef" label is propagated), CCharString::~CCharString @ 0x0099EAE0. Own vtable 0x012830EC (rtti-vtables.tsv).
// Shape: `ret 8`; the landed 0x008CBCF0 constructor shape (sound-pair temporary zeroed from eax, a scoped named CCharString in the 8-byte frame)
// plus the default-constructed intelligent pointer member, whose vtable store is scheduled after the string constructor's pushes.
// The PDB header (CCreatureAction_SummonerLightningOrbInto.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
#include "fable_string.h"
struct CThingCreatureBase;
struct CThing;
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetName(const CCharString& name);  // 0x008574D0
};
struct CCreatureAction_SummonerLightningOrbInto : CCombatActionBase {   // vtable 0x012830EC
    CIntelligentPointer POrb;               // +0x114 CIntelligentPointer<CThing>
    virtual void v00();
    CCreatureAction_SummonerLightningOrbInto(CThingCreatureBase* owner, CThing* thing);
};

CCreatureAction_SummonerLightningOrbInto::CCreatureAction_SummonerLightningOrbInto(CThingCreatureBase* owner, CThing* thing)
    : CCombatActionBase(owner, thing, 150, CSoundPair())
{
    {
        CCharString name("LIGHTNING_ORB_INTO", -1);   // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetName(name);
    }
}
