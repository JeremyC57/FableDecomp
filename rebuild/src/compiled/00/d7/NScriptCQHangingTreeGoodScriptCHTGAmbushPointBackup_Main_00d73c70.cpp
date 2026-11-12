// NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPointBackup::Main() -- Hanging Tree (good) backup ambush point: wait until the quest's tracked thing comes within 5 units, find the nearest "HTG_AmbushPointCond" point and the one nearest to it, then once a second swap the two while the first is on screen; when it is off screen have the quest spawn four enemies there (with the point's data string) and remove both points
// Leaf: ?Main@CHTG_AmbushPointBackup@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D73C70, length 707 bytes (+ int3 padding).
// Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30,
//   CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0,
//   CBaseClass() 0x0099A2D0, ~CScriptThing 0x004AA840, operator delete 0x00BFE9BC, CQ_HangingTreeGoodScript::SpawnEnemies
//   0x00D736B0; CGameScriptInterface slots 7 NewScriptFrame, 77 GetNearestWithScriptName, 108 RemoveThing, 376 Pause,
//   421 IsCameraPosOnScreen; the thing impl's slots 3 GetDataString and 6 GetPosition.
// Data: "HTG_AmbushPointCond" 0x012D26C0; the zero vector 0x0143E8E0 and empty string 0x0143E8EC.
// Layout: as the landed sibling CHTG_AmbushPointRear::Main 0x00D73AE0 (CEntityScriptBase {vptr, Game +4, Me +8},
//   ParentClass +0x14 whose tracked script thing sits at +0x68).
// Shape: CScriptThing's GetPos / GetDataString are inline virtuals (devirtualised on the locals, so expanded inline);
//   the swap runs through a default-constructed temporary declared before the loop with the inline CCountedPointer
//   copy-assign.
// Naming wave 35: identity reg 00d6f78b "HTG_AmbushPointBackup" in 00d6f4b0; Alloc 00d70010; vtable 012d2770 slot 1; PDB ?Main@CHTG_AmbushPointBackup@CQ_HangingTreeGoodScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CHTG_AmbushPointBackup::Main (wave3/labels_alloc.tsv); PDB ?Main@CHTG_AmbushPointBackup@CQ_HangingTreeGoodScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CHTG_AmbushPointBackup@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CQ_HangingTreeGoodScript moved into NScript); same bytes.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (?NullString@CScriptThing@@2VCCharString@@A, the
//   engine/CScriptThing.h spelling; Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09); was g_EmptyString.
namespace NScript { class CPredicateBase; }
#include "engine/C3DVector.h"
class CCharString { public:
 void* PStringData;
 CCharString(const char* text, long length = -1);   // 0x0099EBF0
 CCharString(const CCharString& other);             // 0x0099EC30
 ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                      // 0x00BFE9BC (MSVCR71)
struct CScriptThingImp {
 virtual void i0(); virtual void i1(); virtual void i2();
 virtual CCharString GetDataString() const;         // slot 3 (+0x0c)
 virtual void i4(); virtual void i5();
 virtual const C3DVector& GetPosition() const;      // slot 6 (+0x18)
};
class CCPPointerInfo { public:
 long RefCount;
 void (__fastcall* DeleteFunc)(void* data);
 void* Data;
};
template <class T> class CCountedPointer { public:
 CScriptThingImp* Data;
 CCPPointerInfo*  Info;
 CCountedPointer() : Data(0), Info(0) { }
 CCountedPointer& operator=(const CCountedPointer& other)
 {
  CCPPointerInfo* info = other.Info;
  CScriptThingImp* data = other.Data;
  if (Info != info) {
   if (Info) {
    --Info->RefCount;
    if (Info->RefCount == 0) {
     Info->DeleteFunc(Info->Data);
     delete Info;
    }
   }
   Data = data;
   Info = info;
   if (info)
    ++info->RefCount;
  }
  return *this;
 }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass { public:                  // vtable 0x01238C8C
protected:
    static C3DVector NullCoord;                        // 0x0143E8E0 ?NullCoord@CScriptThing@@1VC3DVector@@A (Ego_r.pdb; engine/CScriptThing.h)
public:
 CCountedPointer<CScriptThing> PImp;
 CScriptThing() { }
 virtual ~CScriptThing();                                   // 0x004AA840
 CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
 virtual void v1(); virtual void v2();
 virtual CCharString GetDataString() const { if (PImp.Data == 0) return CScriptThing::NullString; return PImp.Data->GetDataString(); }   // slot 3
 virtual void v4(); virtual void v5();
 virtual const C3DVector& GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPosition(); }      // slot 6
public:
    static CCharString NullString;                  // 0x0143E8EC ?NullString@CScriptThing@@2VCCharString@@A
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_HangingTreeGoodScript {
public:
 unsigned char _pad[0x68];
 CScriptThing  TrackedThing;                        // +0x68
 void SpawnEnemies(const C3DVector& position, CCharString name, long count);   // 0x00D736B0
public:
    class CHTG_AmbushPointBackup;    // the entity class below (naming wave 35)
};
}
using NScript::CQ_HangingTreeGoodScript;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_HangingTreeGoodScript::CHTG_AmbushPointBackup : public CEntityScriptBase {
public:
  CGameScriptInterface* Game;                        // +0x04
 CScriptThing Me;                                   // +0x08
 CQ_HangingTreeGoodScript* ParentClass;             // +0x14
 void* PMasterData;                                 // +0x18
private:
    virtual void Main();                          // PDB ?Main@CHTG_AmbushPointBackup@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
};
}
void NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPointBackup::Main()
{
 bool done = false;
 while (!IsDistanceBetweenThingsUnder(Me, ParentClass->TrackedThing, 5.0f)) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
 }
 if (IsActiveThreadTerminating()) return;
 CScriptThing point = Game->GetNearestWithScriptName(Me, "HTG_AmbushPointCond");
 CScriptThing other = Game->GetNearestWithScriptName(point, "HTG_AmbushPointCond");
 CScriptThing swap;
 do {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
  if (Game->IsCameraPosOnScreen(point.GetPos())) {
   if (IsActiveThreadTerminating()) return;
   swap = point;
   point = other;
   other = swap;
  } else {
   if (IsActiveThreadTerminating()) return;
   ParentClass->SpawnEnemies(point.GetPos(), point.GetDataString(), 4);
   done = true;
   Game->RemoveThing(point, false, true);
   Game->RemoveThing(other, false, true);
  }
  Game->Pause(1.0f);
 } while (!done);
 IsActiveThreadTerminating();
}