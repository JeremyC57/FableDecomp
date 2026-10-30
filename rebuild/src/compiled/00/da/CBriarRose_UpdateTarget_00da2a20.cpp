// CBriarRose::UpdateTarget() -- minion-camp Briar Rose: keep the current target while it is still a live, non-turncoated
// thing; otherwise gather every "MC_MinionGuard" thing, pick the nearest live one to the hero (and the nearest live one that is
// not turncoated), prefer the non-turncoated pick, store it as Target and hand it to GiveThingBestEnemyTarget; returns
// true when a target is (still) set, false when none is found or the thread is terminating
// Leaf: UpdateTarget
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA2A20, 638 bytes (640 with padding).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, vector<CScriptThing> dtor 0x008AC970 (out-of-line COMDAT),
//   CScriptThing::operator= 0x008AB980, CScriptThing slots 75 IsAlive (0x004AB130) / 77 IsNull (0x004AB150),
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, the __fastcall thing-to-thing distance helper 0x00CBE583,
//   CGameScriptInterface slots 70 GetHero, 75 GetAllThingsWithScriptName, 451 GiveThingBestEnemyTarget,
//   748 IsThingTurncoated (ghidra_out/cgsi_slots_aligned.tsv); -1.0f pool constant 0x0122DEE0.
// Layout: Ego_r.pdb CBriarRose (80 bytes, the CQ_MinionCampScript one): CEntityScriptBase {vptr, Game +4, Me +8},
//   ParentClass +0x14, PMasterData +0x18, seh_me +0x1c, MyDestination +0x2c, Target +0x38, NumHitsBetweenForcePushes +0x44,
//   SpeechTimer +0x48, IsPlayingChantAnim +0x4c.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                     // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                         // 0x008AC970 (out-of-line COMDAT)
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(_M_start + i); }
};
}
using std::vector;
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CBriarRose : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CQ_MinionCampScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    char  seh_me[0x10];                                // +0x1c CScriptGameResourceObjectScriptedThingBase
    char  MyDestination[0xc];                          // +0x2c CScriptThing
    CScriptThing Target;                               // +0x38
    long  NumHitsBetweenForcePushes;                   // +0x44
    char  SpeechTimer[4];                              // +0x48 CTimer
    bool  IsPlayingChantAnim;                          // +0x4c
    bool UpdateTarget();
};

bool CBriarRose::UpdateTarget()
{
    vector<CScriptThing> things;
    Game->GetAllThingsWithScriptName("MC_MinionGuard", things);
    unsigned int count = things.size();
    if (count == 0) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    if (!Target.IsNull() && Target.IsAlive() && !Game->IsThingTurncoated(Target))
        return true;
    if (IsActiveThreadTerminating())
        return false;
    CScriptThing* nearest = 0;
    CScriptThing* nearestLoyal = 0;
    float nearestDistance = -1.0f;
    float nearestLoyalDistance = -1.0f;
    for (unsigned int i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return false;
        if (things[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return false;
            float distance = GetSquaredDistanceBetweenThings(Game->GetHero(), things[i]);
            if (distance < nearestDistance || nearestDistance == -1.0f) {
                if (IsActiveThreadTerminating())
                    return false;
                nearestDistance = distance;
                nearest = &things[i];
            }
            if (!Game->IsThingTurncoated(things[i])) {
                if (distance < nearestLoyalDistance || nearestLoyalDistance == -1.0f) {
                    if (IsActiveThreadTerminating())
                        return false;
                    nearestLoyalDistance = distance;
                    nearestLoyal = &things[i];
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return false;
    if (nearestLoyal) {
        if (IsActiveThreadTerminating())
            return false;
        Target = *nearestLoyal;
        Game->GiveThingBestEnemyTarget(Me, Target);
        return true;
    }
    if (nearest) {
        if (IsActiveThreadTerminating())
            return false;
        Target = *nearest;
        Game->GiveThingBestEnemyTarget(Me, Target);
        return true;
    }
    if (IsActiveThreadTerminating())
        return false;
    return false;
}