// NScript::CQ_SecretPassageScript::CSP_UndeadTrigger::Main() -- the secret-passage undead trigger: spawn the parent quest's SwitchDoorsOnAndOff once, wait until the hero is within 2.5 units of Me, then turn every "SP_UndeadSpawnPoint<data>" marker into an undead soldier that hunts the hero (a "BlockADoor" marker also blocks the doors), wait until all of them are dead, unblock the doors and remove Me
// Leaf: ?Main@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountBlock (= CCPPointerInfo) members References->RefCount are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Revisit: inline-depth -- byte-exact, but reached with #pragma inline_depth regions retail most likely never wrote; redo pragma-free once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEF760, length 944 bytes, plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30; operator new 0x00BFEA1A; CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, operator+(const CCharString&, const CCharString&) 0x0099F570 and operator+(const char*, const
// CCharString&) 0x0099F690 (both __fastcall); CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012DDC54),
// CScriptBase::AddSpawnedFunction 0x00CB7E50, CQ_SecretPassageScript::SwitchDoorsOnAndOff 0x00DEFB10;
// IsDistanceBetweenThingsUnder 0x00CBE2FF / AreAllThingsInVectorDead 0x00CBED00 (__fastcall); ~CScriptThing
// 0x004AA840, CScriptThing::operator= 0x008AB980 (inline at its first use, out of line at the second), operator
// delete 0x00BFE9BC, CBasicString<char>::Compare 0x00411570 (the inlined CCharString == "BlockADoor"); CScriptThing slots
// 3 GetDataString, 6 GetPos; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 75 GetAllThingsWithScriptName,
// 91 CreateCreature, 108 RemoveThing, 451 GiveThingBestEnemyTarget.
// Literals "SwitchDoorsOnAndOff" 0x012DDF78, "ParentClass." 0x012C9934, "" 0x0122D70E, "SP_UndeadSpawnPoint"
// 0x012DDF64, "EnemySeekingHero" 0x012DDD98, "CREATURE_OK_UNDEAD_SOLDIER_SWORD" 0x0127D594, "BlockADoor" 0x012DDF58.
// Layout: Ego_r.pdb CSP_UndeadTrigger: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
// UndeadVec +0x1c (12-byte STLport vector in retail), TempCreature +0x28. CQ_SecretPassageScript is retail = PDB - 0x14:
// DoorsBlocked +0x4c, DoorsBeingDone +0x4d.
// Shape: CScriptThing::operator= and its CCountedPointer::operator= are __forceinline (the counted-pointer assignment
// caches the source Data/Info first, as the out-of-line 0x008AB980 body does); the second use, `UndeadVec[i] =
// TempCreature`, binds the element to a reference first and assigns under `#pragma inline_depth(0)` so only operator=
// stays out of line; the spawned function is the CUndeadRisingZombieAttacker::OnPredicateFail 0x00E0DE90 new-expression.
// Naming wave 35: identity Anniversary wave 2026-11-04 (high): vtable 0x012DDF3C slot 1; Anniversary RTTI NScript::CQ_SecretPassageScript::CSP_UndeadTrigger (string-literal match, validated 257/258); PDB name set has the Main member of NScript::CQ_SecretPassageScript::CSP_UndeadTrigger; slot layout from named TLC siblings; function referenced by this vtable only (2026-11-04_anniversary/labels_high.tsv); PDB ?Main@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CSP_UndeadTrigger@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
#include "fable_compiler.h"
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                              // CBasicString<char>, 13 bytes
    char*         PData;                               // +0x00
    unsigned long StringLength;                        // +0x04
    unsigned long DataLength : 31;                     // +0x08
    bool          UseFastExtend : 1;                   // +0x0c
};
class CCharStringData { public:                               // 17 bytes
    CBasicStringChar Data;                             // +0x00
    long             NoRefs;                           // +0x0d
};
#pragma pack(pop)
long __fastcall Compare(const char* a, const char* b); // 0x00411570 CBasicString<char>::Compare
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return Compare(PStringData->Data.PData, text) ? false : true;
    }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
#include "engine/C3DVector.h"
void operator delete(void* p);                         // 0x00BFE9BC
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
struct CScriptThingVector {                            // STLport vector<CScriptThing>
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CScriptThing& operator[](unsigned int n) { return _M_start[n]; }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall AreAllThingsInVectorDead(const CScriptThingVector& things);                                // 0x00CBED00
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptBase { public:                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
namespace NScript {
class CQ_SecretPassageScript : public CScriptBase { public:   // NScript script class (both PDBs: class, namespace NScript)
    unsigned char _pad40[0x4c - 0x40];
    bool DoorsBlocked;                                 // +0x4C
    bool DoorsBeingDone;                               // +0x4D
    void SwitchDoorsOnAndOff();                        // 0x00DEFB10
public:
    class CSP_UndeadTrigger;         // the entity class below (naming wave 35)
};
}
using NScript::CQ_SecretPassageScript;
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD2(qe)                              // slots 8..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD4(ra)                                                                        // slots 71..74
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const;   // slot 75 (+0x12c)
    VPAD8(sa) VPAD4(sb) VPAD2(sc) VPAD1(sd)                                          // slots 76..90
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& pos, const CCharString& scriptName, bool a) const;   // slot 91 (+0x16c)
    VPAD16(ta)                                                                       // slots 92..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;       // slot 108 (+0x1b0)
    VPAD256(ua) VPAD64(ub) VPAD16(uc) VPAD4(ud) VPAD2(ue)                            // slots 109..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
namespace NScript {
class CQ_SecretPassageScript::CSP_UndeadTrigger : public CEntityScriptBase {
public:
    CQ_SecretPassageScript* ParentClass;               // +0x14
    void*                   PMasterData;               // +0x18 CQ_SunnyvaleMasterData*
    CScriptThingVector      UndeadVec;                 // +0x1c
    CScriptThing            TempCreature;              // +0x28
private:
    virtual void Main();                          // PDB ?Main@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ
};
}

void NScript::CQ_SecretPassageScript::CSP_UndeadTrigger::Main()
{
    if (!ParentClass->DoorsBeingDone) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->AddSpawnedFunction(new CSpawnedFunc<CQ_SecretPassageScript>(CCharString("ParentClass.") + CCharString("SwitchDoorsOnAndOff"), 0, ParentClass, &CQ_SecretPassageScript::SwitchDoorsOnAndOff), CCharString(""));
        ParentClass->DoorsBeingDone = true;
    }
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.5f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CCharString spawnPointName = "SP_UndeadSpawnPoint" + Me.GetDataString();
    Game->GetAllThingsWithScriptName(spawnPointName, UndeadVec);
    for (unsigned int i = 0; i < UndeadVec.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        TempCreature = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD", UndeadVec[i].GetPos(), "EnemySeekingHero", true);
        Game->GiveThingBestEnemyTarget(TempCreature, Game->GetHero());
        if (UndeadVec[i].GetDataString() == "BlockADoor") {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->DoorsBlocked = true;
        }
        CScriptThing& undead = UndeadVec[i];
#pragma inline_depth(0)
        undead = TempCreature;
#pragma inline_depth()
    }
    if (IsActiveThreadTerminating())
        return;
    while (!AreAllThingsInVectorDead(UndeadVec)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->DoorsBlocked = false;
    Game->RemoveThing(Me, false, true);
}