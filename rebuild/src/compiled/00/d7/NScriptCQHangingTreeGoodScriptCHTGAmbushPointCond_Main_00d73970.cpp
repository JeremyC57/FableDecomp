// NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPointCond::Main() -- Hanging Tree (good) conditional ambush point: wait until the hero is within 6 units, find the nearest "HTG_AmbushPointCond" marker, wait until it is off screen, pause a second, have the quest spawn four enemies at the marker with its data string and remove both the marker and this point (any wait bails out when the script is done)
// Leaf: ?Main@CHTG_AmbushPointCond@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D73970, length 354 bytes.
// Callees: IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), CEntityScriptBase::IsDone @ 0x00F35B30,
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("HTG_AmbushPointCond" @ 0x012D26C0),
//   CScriptThing::GetDataString @ 0x004AA900 (direct on the local) / ~CScriptThing @ 0x004AA840 (manifest
//   "~C3DClothPrimitive" is propagated), CQ_HangingTreeGoodScript::SpawnEnemies @ 0x00D736B0; CGameScriptInterface
//   (vtable 0x01260F0C) slots 7 NewScriptFrame, 70 GetHero, 77 GetNearestWithScriptName, 108 RemoveThing, 376 Pause,
//   421 IsCameraPosOnScreen. Zero vector @ 0x0143E8E0.
// Layout: as the landed sibling CHTG_AmbushPoint::Main 0x00D73610 (Game +4, Me +8, ParentClass +0x14); CScriptThing is
//   engine/CScriptThing.h's pimpl handle {vptr, PImp_Data +4, PImp_Info +8}, modelled locally because its virtuals are
//   called (GetPos inlined and devirtualised on the by-value local, as the landed CScriptThing::GetPos 0x004AA980).
// Shape: the marker name is an implicit CCharString conversion temporary built before the other arguments; the
//   marker's position is taken before its data string for SpawnEnemies.
// Naming wave 35: identity reg 00d6f727 "HTG_AmbushPointCond" in 00d6f4b0; Alloc 00d6fe50; vtable 012d2738 slot 1; PDB ?Main@CHTG_AmbushPointCond@CQ_HangingTreeGoodScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CHTG_AmbushPointCond::Main (wave3/labels_alloc.tsv); PDB ?Main@CHTG_AmbushPointCond@CQ_HangingTreeGoodScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CHTG_AmbushPointCond@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CQ_HangingTreeGoodScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/C3DVector.h"
extern C3DVector g_ZeroVector;                     // 0x0143E8E0
class CCPPointerInfo;
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CCharString GetDataString() const;     // slot 3 (+0x0c), 0x004AA900
    virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const        // slot 6 (+0x18), 0x004AA980
    {
        if (!PImp_Data)
            return g_ZeroVector;
        return PImp_Data->GetPos();
    }
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    virtual ~CScriptThing();                               // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    void SpawnEnemies(const C3DVector& position, CCharString name, long count);   // 0x00D736B0
public:
    class CHTG_AmbushPointCond;      // the entity class below (naming wave 35)
};
}
using NScript::CQ_HangingTreeGoodScript;
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_HangingTreeGoodScript::CHTG_AmbushPointCond : public CEntityScriptBase {
public:
        CGameScriptInterface*     Game;                // +0x04
    CScriptThing              Me;                  // +0x08
    CQ_HangingTreeGoodScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
private:
    virtual void Main();                          // PDB ?Main@CHTG_AmbushPointCond@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPointCond::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 6.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing marker = Game->GetNearestWithScriptName(Me, "HTG_AmbushPointCond");
    while (Game->IsCameraPosOnScreen(marker.GetPos())) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    ParentClass->SpawnEnemies(marker.GetPos(), marker.GetDataString(), 4);
    Game->RemoveThing(marker, false, true);
    Game->RemoveThing(Me, false, true);
}