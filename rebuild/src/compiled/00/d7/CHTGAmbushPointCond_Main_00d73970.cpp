// CHTG_AmbushPointCond::Main() -- Hanging Tree (good) conditional ambush point: wait until the hero is within 6 units, find the nearest "HTG_AmbushPointCond" marker, wait until it is off screen, pause a second, have the quest spawn four enemies at the marker with its data string and remove both the marker and this point (any wait bails out when the script is done)
// Leaf: Main
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
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
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
struct CQ_HangingTreeGoodScript {
    void SpawnEnemies(const C3DVector& position, CCharString name, long count);   // 0x00D736B0
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CHTG_AmbushPointCond : public CEntityScriptBase {
public:
    void*                     __vftable;           // +0x00
    CGameScriptInterface*     Game;                // +0x04
    CScriptThing              Me;                  // +0x08
    CQ_HangingTreeGoodScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
    void Main();
};

void CHTG_AmbushPointCond::Main()
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