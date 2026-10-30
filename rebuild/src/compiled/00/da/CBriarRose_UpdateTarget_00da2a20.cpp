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
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031();
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055();
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071();
    virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                      // slot 75 (+0x12c)
    virtual void t076();
    virtual bool IsNull() const;                       // slot 77 (+0x134)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
};
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
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
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