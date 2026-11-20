// CGameScriptInterface::AddCreatureScriptedMode(const CScriptThing& thing, const CCharString& scriptName) const -- gives a valid creature that has a mode manager (kind 0x31) a new scripted creature mode running the named script
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A1A10, length 157 bytes (padded to 160).
// Callees: CScriptThing::IsValid (slot 75) and GetThing (slot 11), the component-map LowerBound @ 0x0040F020 (kind 0x31),
//          operator new @ 0x00BFEA1A (0x74 bytes), CModeStandBase::CModeStandBase(CThing&) @ 0x00861E00 (manifest
//          "CTCCreatureNavigation" ctor is propagated), CAnimFilterOfGroup(const CCharString&) @ 0x0073E8F0 at +0x68 (manifest
//          "CConsole::SendMessageToUser" is propagated), CCountedPointer<CModeBase>::CCountedPointer(T*) @ 0x00718A00,
//          CTCCreatureModeManager::AddMode @ 0x0073ACC0.
// Vtable: NCreatureMode::CModeScripted 0x01267670.
// Shape: one GetThing for the mask test and the lookup; the mode manager is a named local (ebx) kept across the
// allocation; the mode is an inline `new CModeScripted(thing.GetThing(), scriptName)` (base, vtable store, member);
// the counted pointer is a by-value argument built in the argument area (`sub esp,8 ; mov ecx,esp`).
// Leaf: ?AddCreatureScriptedMode@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
#include "fable_compiler.h"
// Naming (NF147, 2026-11-20; q6408): the local NCreatureMode::CModeBase stand-in (= CModeStandBase), the CScriptedModeInfo member stand-in
//   (= CAnimFilterOfGroup), the local CCharString, the empty ETCInterfaceType, the local std::pair / CVectorMap forward templates and
//   CTCCreatureModeManager_Retail replaced by fable_string.h, engine/CModeStandBase.h, engine/CAnimFilterOfGroup.h (NF147 overlay opt-in
//   FABLE_RETAIL_CANIMFILTEROFGROUP_CTOR), engine/CVectorMap.h, engine/ETCInterfaceType.h, engine/CTCCreatureModeManager.h (ADDMODEPOINTER) and
//   engine/CCountedPointer.h (CALLED_CTOR). The header-less NCreatureMode::CModeScripted keeps its token on the shared CModeStandBase.
//   0x0073ACC0 binds the Ego_r.pdb AddMode, 0x00718A00 the FableWin CCountedPointer<CModeBase> ctor, 0x0073E8F0 ??0CAnimFilterOfGroup@@QAE@ABVCCharString@@@Z,
//   0x00861E00 ??0CModeStandBase@@QAE@AAVCThing@@@Z. The row's local CScriptThing still returns its CThing_Retail stand-in, so the ctor argument
//   is `*(CThing*)thing.GetThing()` until those models move onto engine/CThing.h / CScriptThing.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_CTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "fable_string.h"
struct CTCBase_Retail;
class CTCBase;
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    const STCEntry_Retail* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0 (the const instantiation)
    FABLE_FORCEINLINE CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
    FABLE_FORCEINLINE CTCBase_Retail* Find(long kind) const
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
};
class CThingPlayerCreature : public CThing_Retail { };
FABLE_FORCEINLINE CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
FABLE_FORCEINLINE CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;       // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
    CScriptThing();                                // 0x008CFF40 (the null script thing)
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CThingSearchTools;
FABLE_FORCEINLINE CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
FABLE_FORCEINLINE CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> FABLE_FORCEINLINE bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)GetTC(thing, kind);
        return true;
    }
    return false;
}
FABLE_FORCEINLINE bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = thing.GetThing();
    return t && !t->m_dead;
}
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
struct CModeScripted : CModeStandBase {           // 0x74 bytes, vtable 0x01267670
    CAnimFilterOfGroup m_info;                     // +0x68 (0x0073E8F0 = CAnimFilterOfGroup(const CCharString&))
    CModeScripted(CThing& thing, const CCharString& name) : CModeStandBase(thing), m_info(name) {}
    virtual unsigned long GetType() const;
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::AddCreatureScriptedMode(const CScriptThing& thing, const CCharString& scriptName) const
{
    if (!thing.IsValid())
        return;
    CThing_Retail* t = thing.GetThing();
    if (!t->HasTC(0x31))
        return;
    CTCCreatureModeManager* modes = (CTCCreatureModeManager*)GetTC(t, 0x31);
    modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeScripted(*(CThing*)thing.GetThing(), scriptName)));
}