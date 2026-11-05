// NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::OnPersist(CPersistContext&) -- empty override (the entity persists nothing of its own)
// Leaf: ?OnPersist@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members PObject->Data, PCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEC30, 3 bytes: ret 4.
// Identity (naming wave 35, family ent): Anniversary wave 2026-11-04 (high): vtable 0x012C3208 slot 4; Anniversary RTTI NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost (string-literal match, validated 257/258); PDB name set has the OnPersist member of NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost; slot layout from named TLC siblings; function referenced by this vtable only.
//   ?OnPersist@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00CDEB40 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScriptCArcheryCompetitionSignpost_Alloc_00cdeb40.cpp); the PDB member declaration added; the Alloc body (if any) is not part of this graded body.
class CGameScriptInterfaceBase;
class CScriptDataBase;
class CPersistContext;                                  // the OnPersist parameter (declaration only)

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_GlobalThingsScript {
public:
    unsigned char _scriptBase[0x40];
    CGameScriptInterfaceBase* Game;                     // +0x40
    class CArcheryCompetitionSignpost;                  // the entity class below (naming wave 35)
};
}
using NScript::CPersonalScript_GlobalThingsScript;

class CEntityScriptRoot {
public:
    CEntityScriptRoot(CGameScriptInterfaceBase* game) : Game(game) {}
    virtual ~CEntityScriptRoot();
    CGameScriptInterfaceBase* Game;                     // +4
};

class CEntityScriptBase : public CEntityScriptRoot {
public:
    CEntityScriptBase(CGameScriptInterfaceBase* game, const CScriptThing& me) : CEntityScriptRoot(game), Me(me) {}
    virtual ~CEntityScriptBase();
    CScriptThing Me;                                    // +8
};


namespace NScript {
class CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost : public CEntityScriptBase {
public:
    CArcheryCompetitionSignpost(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
        : CEntityScriptBase(parent.Game, me), ParentClass(&parent), PMasterData((CQ_SunnyvaleMasterData*)data) {}
    virtual ~CArcheryCompetitionSignpost();
    static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me);
    CPersonalScript_GlobalThingsScript* ParentClass;    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
CCountedPointer<CEntityScriptBase> __fastcall CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Alloc(CPersonalScript_GlobalThingsScript& parent,
                                                                               CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CArcheryCompetitionSignpost(parent, data, me));
}
}

void NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::OnPersist(CPersistContext&)
{
}
