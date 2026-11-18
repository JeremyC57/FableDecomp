// NScript::CV_BodyGuardScript::CHenchmanComment::Main() -- body-guard henchman chatter: collects the "BodyGuard" things and notes which one is the CREATURE_BODY_GUARD itself; then every frame, while any other henchman is within 10 units and more than 6 seconds have passed, asks the body-guard script for a comment (stopping once one is made); the timer restarts whenever no henchman is near
// Leaf: ?Main@CHenchmanComment@CV_BodyGuardScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountBlock (= CCPPointerInfo) members References->RefCount are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5F2B0, length 783 bytes (+ int3 padding).
// Callees: IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString(const
//   char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, ~CScriptThing 0x004AA840,
//   CV_BodyGuardScript::MakeHenchmanComment 0x00E5EBD0; CScriptThing slots 2 GetDefName / 3 GetDataString,
//   Compare 0x00411570 (the inlined CCharString ==), CCounter operator> 0x00D90670 / operator=(float) 0x00D90550 and its inline
//   ctor (pDSTGame 0x0143E8F8 slot 653 GetWorldFrame); CGameScriptInterfaceBase slots
//   7 NewScriptFrame, 75 GetAllThingsWithScriptName.
// Data: "BodyGuard" 0x012CB3F0, "CREATURE_BODY_GUARD" 0x012578FC, "" 0x0122D70E.
// Layout: Ego_r.pdb CHenchmanComment (40 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, HenchmanVector +0x1c (retail STLport vector, 12 bytes).
// Shape: the comment call sits behind `counter > 6.0f &&`, so its two temporaries carry construction flags; the counter
//   lives in its own block (so the spilled `this` of the first loop can share its slot) and one function-scope index
//   serves both loops (a per-loop index swaps the index/offset frame words).
// Naming wave 35: identity reg 00e5d59f "HenchmanComment" in 00e5d4f0; Alloc 00e5f1c0; vtable 012e70c8 slot 1; PDB ?Main@CHenchmanComment@CV_BodyGuardScript@NScript@@; refs=1; was under-qualified CHenchmanComment::Main (wave3/labels_alloc.tsv); PDB ?Main@CHenchmanComment@CV_BodyGuardScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CHenchmanComment@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes. Also: global 0x01375748 respelled NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations.
#include "fable_compiler.h"
class CCharString;
class C3DVector;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C {vptr, PImp_Data, PImp_Info}
 virtual void v01();
 virtual CCharString GetDefName() const;                 // slot 2 (+0x08) 0x004AA8D0
 virtual CCharString GetDataString() const;              // slot 3 (+0x0c) 0x004AA900
 virtual void v04(); virtual void v05();
 virtual const C3DVector& GetPos() const;               // slot 6 (+0x18)
 virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
 virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54) 0x004AAB30
 virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
 virtual void SetFriendsWithEverythingFlag(bool on);       // slot 67 (+0x10c) 0x004AB060
 virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
 virtual bool IsAlive() const;                            // slot 75 (+0x12c) 0x004AB130
 CCountedPointer<CScriptThing> PImp;                                  // +0x04 Data, +0x08 Info
 struct InlineCopy { };                                 // tag: the inline copy retail expands at by-value call sites
 CScriptThing() { }
 CScriptThing(const CScriptThing& other);               // 0x004ABE90 (the out-of-line copy)
 CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
 virtual ~CScriptThing();                                       // 0x004AA840
 CScriptThing& operator=(const CScriptThing& other);    // 0x008AB980
};
// NF78 (round 7, q10795; NF70 blocked/port): 0x0143E8F8 is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS), reached
//   through the shared engine/CGameScriptInterfaceBase.h (FABLE_RETAIL_NSCRIPT, as the landed 0x00CBE716 / 0x00D9E930); its slot 653 is
//   GetWorldFrame (the CCounter ctor). The local GameCalls (845 slots) and CThingManager_Calls (654 slots) interface models and their
//   forward declarations are gone.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase { public:virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
class CIsThingAlive:public CPredicateBase { public:
 CScriptThing Thing;
 FABLE_FORCEINLINE CIsThingAlive(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const;virtual CPredicateBase* Clone()const;
 ~CIsThingAlive(){}
};
}
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
 for (;;) {
  char ca = *a;
  if (ca == 0 && *b == 0)
   return 0;
  char cb = *b;
  if (ca < cb)
   return -1;
  if (ca > cb)
   return 1;
  ++a;
  ++b;
 }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                            // CBasicString<char>, 13 bytes
 char*         PData;                                // +0x00
 unsigned long StringLength;                         // +0x04
 unsigned long DataLength : 31;                      // +0x08
 bool          UseFastExtend : 1;                    // +0x0c
 bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                             // 17 bytes
 CBasicStringChar Data;                              // +0x00
 long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
class CCharString { public:
 CCharStringData* PStringData;
 CCharString(const char* text, long length = -1);   // 0x0099EBF0
 CCharString(const CCharString& other);             // 0x0099EC30
 ~CCharString();                                    // 0x0099EAE0
 FABLE_FORCEINLINE bool operator==(const char* text) const
 {
  if (PStringData == 0) {
   if (strcmp(text, "") == 0)
    return true;
   return false;
  }
  return PStringData->Data == text;
 }
};
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748 (established spelling; naming wave 35: was g_bScriptAnimationFlag)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
namespace std {
template <> class vector<CScriptThing, allocator<CScriptThing> > {  // STLport vector (retail 12 bytes)
public:
 CScriptThing* _M_start;
 CScriptThing* _M_finish;
 CScriptThing* _M_end_of_storage;
 unsigned int size() const { return _M_finish - _M_start; }
 CScriptThing* begin() { return _M_start; }
 CScriptThing& operator[](unsigned int n) { return *(begin() + n); }
};
}
namespace NScript {
class CV_BodyGuardScript { public:                     // NScript script class (both PDBs)
 enum EHENCHMAN_COMMENT { };                           // nested comment kind (Ego_r / FableWin W4EHENCHMAN_COMMENT@CV_BodyGuardScript@NScript@@)
 bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT type, const CScriptThing& target);   // 0x00E5EBD0
public:
    class CHenchmanComment;          // the entity class below (naming wave 35)
};
}
using NScript::CV_BodyGuardScript;
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
class CCounter { public:                                    // Ego_r.pdb CCounter { long RelevantWorldFrame }
 long RelevantWorldFrame;
 CCounter() : RelevantWorldFrame(pDSTGame->GetWorldFrame()) { }   // out of line 0x00CD4490 (slot 653)
 bool operator>(float seconds) const;               // 0x00D90670
 CCounter& operator=(float seconds);                // 0x00D90550
};
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CV_BodyGuardScript::CHenchmanComment : public CEntityScriptBase {
public:
  CGameScriptInterfaceBase* Game; CScriptThing Me;   // Ego_r.pdb CEntityScriptBase::Game (CGameScriptInterfaceBase*)
 CV_BodyGuardScript* ParentClass;                    // +0x14
 void* PMasterData;                                  // +0x18
 std::vector<CScriptThing, std::allocator<CScriptThing> > HenchmanVector;   // +0x1c
private:
    virtual void Main();                          // PDB ?Main@CHenchmanComment@CV_BodyGuardScript@NScript@@EAEXXZ
};
}
void NScript::CV_BodyGuardScript::CHenchmanComment::Main()
{
 Game->GetAllThingsWithScriptName("BodyGuard", HenchmanVector);
 if (HenchmanVector.size() == 0) {
  IsActiveThreadTerminating();
  return;
 }
 unsigned int bodyGuard = (unsigned int)-1;
 unsigned int i;
 for (i = 0; i < HenchmanVector.size(); ++i) {
  if (IsActiveThreadTerminating()) return;
  if (HenchmanVector[i].GetDefName() == "CREATURE_BODY_GUARD") {
   if (IsActiveThreadTerminating()) return;
   bodyGuard = i;
  }
 }
 if (IsActiveThreadTerminating()) return;
 {
 CCounter sinceComment;
 while (!IsActiveThreadTerminating()) {
  bool anyNear = false;
  for (i = 0; i < HenchmanVector.size(); ++i) {
   if (IsActiveThreadTerminating()) return;
   if (bodyGuard != i && IsDistanceBetweenThingsUnder(Me, HenchmanVector[i], 10.0f)) {
    if (IsActiveThreadTerminating()) return;
    anyNear = true;
    if (sinceComment > 6.0f && ParentClass->MakeHenchmanComment(Me.GetDataString(), 2, (CV_BodyGuardScript::EHENCHMAN_COMMENT)2, CScriptThing())) {
     IsActiveThreadTerminating();
     return;
    }
   }
  }
  if (IsActiveThreadTerminating()) return;
  if (!anyNear) {
   if (IsActiveThreadTerminating()) return;
   sinceComment = 0.0f;
  }
  Game->NewScriptFrame();
 }
 }
}