#ifndef FABLE_ENGINE_CTHINGVILLAGE_H
#define FABLE_ENGINE_CTHINGVILLAGE_H
#include "engine/CThingGameObject.h"
#ifdef FABLE_RETAIL_CTHINGVILLAGE_DEFINITION
// Retail opt-in (agentNF9): see the class body. Needs FABLE_RETAIL_CTHING_MOREACCESS, the CGameDefinitionManager chain and the
// CAppearanceDef sub-def (FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY).
#include "engine/CDefPointer.h"
#include "engine/CEngineGraphic.h"
#include "engine/CThingVillageDef.h"
#include "engine/CGameDefinitionManager.h"
#endif
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// CEngineGraphic CurrentGraphic +d0; native allocation52ac70.
class CThingVillage:public CThingGameObject {
protected:
#ifdef FABLE_RETAIL_CTHINGVILLAGE_DEFINITION
    // The Ego_r.pdb member type (14-byte packed CEngineGraphic, same offset); `mutable` because the PDB's const
    // GetCurrentGraphic (IBE) caches into it (0x0083B520 stores the 14 bytes at +0xD0 and returns its address).
    mutable CEngineGraphic CurrentGraphic; // +d0
#else
    unsigned char CurrentGraphic[14]; // +d0 CEngineGraphic, size0e; ctor5299ec..529a04
#endif
    unsigned char _pad_0xde[2];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGVILLAGE_DEFINITION
    // Retail opt-in (agentNF9, row 0x0083B520). Ego_r.pdb ?PeekDefinition@CThingVillage@@IBE?AV?$CDefPointer@$$CBVCThingVillageDef@@@@XZ
    // (protected const, the def by value; no out-of-line copy in Fable.exe: the row expands it, add-ref and release sharing one null
    // test) and ?GetCurrentGraphic@CThingVillage@@IBEPBVCEngineGraphic@@XZ (protected const, 0x0083B520).
protected:
    CDefPointer<const CThingVillageDef> PeekDefinition() const
    {
        CDefPointer<const CThingVillageDef> def;
        PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
        return def;
    }
    const CEngineGraphic* GetCurrentGraphic() const;                     // 0x0083B520
public:
#endif
#ifdef FABLE_RETAIL_CTHINGVILLAGE_STATGETTYPE
    // Nested opt-in (chunk 853, row 0x0050A650): FableWin.pdb ?StatGetType@CThingVillage@@SIJXZ (inline); retail GetAllLeafThings<CThingVillage>
    // 0x005166B0 scans type list 4 (`push 4 ; call PeekTypeList`). No layout change.
    static long __fastcall StatGetType() { return 4; }
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingVillage)==0xe0);
#endif
