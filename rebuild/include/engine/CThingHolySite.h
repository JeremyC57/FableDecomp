#ifndef FABLE_ENGINE_CTHINGHOLYSITE_H
#define FABLE_ENGINE_CTHINGHOLYSITE_H
#include "engine/CThingGameObject.h"
#ifdef FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION
// Retail opt-in (agentNF9): see the class body. Needs FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CTHING_MOREACCESS and the
// CGameDefinitionManager chain.
#include "engine/CDefPointer.h"
#include "engine/CThingHolySiteDef.h"
#include "engine/CGameDefinitionManager.h"
class CPersistContext;
#endif
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// CDefPointer<const CThingHolySiteDef> Def +d0, PlayerStart +d4; native allocation52ac90.
class CThingHolySite:public CThingGameObject {
private:
#ifdef FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION
    CDefPointer<const CThingHolySiteDef> Def; // +d0 (Ego_r.pdb member type), ctor52accd
#else
    unsigned char Def[4]; // +d0 CDefPointer<const CThingHolySiteDef>, ctor52accd
#endif
    unsigned char PlayerStart; // +d4, original sole trailing member; native totald8
    unsigned char _pad_0xd5[3];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION
    // Retail opt-in (agentNF9, rows 0x0083BAB0 / 0x0083BB40 / 0x0083BBA0). Spellings: Ego_r.pdb / FableWin.pdb `@CThingHolySite@@`;
    // CThingHolySite vtable 0x012452DC slot 3 SerialiseGameState 0x0083BB40, slot 4 ConstructFromLevelScript 0x0083BBA0 (the catalog's
    // vfunc_3 / vfunc_4). PeekDefinition (private, ?PeekDefinition@CThingHolySite@@ABEABVCThingHolySiteDef@@XZ) has no out-of-line
    // copy in Fable.exe: `return *Def;`. Declarations only otherwise.
private:
    const CThingHolySiteDef& PeekDefinition() const { return *Def; }
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, bool playerStart);   // 0x0083BAB0 ?ConstructFromParams@CThingHolySite@@IAE_NJABVC3DVector@@J_N@Z
public:
    virtual bool SerialiseGameState(CPersistContext& context);           // 0x0083BB40 ?SerialiseGameState@CThingHolySite@@UAE_NAAVCPersistContext@@@Z
    virtual bool ConstructFromLevelScript(const char* script);           // 0x0083BBA0 ?ConstructFromLevelScript@CThingHolySite@@UAE_NPBD@Z
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingHolySite)==0xd8);
#endif
