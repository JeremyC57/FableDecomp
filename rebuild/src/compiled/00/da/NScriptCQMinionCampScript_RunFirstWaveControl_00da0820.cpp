// CQ_MinionCampScript::RunFirstWaveControl() -- during the first wave, whenever neither hero attacker is alive, pick the two minion guards nearest the hero that are not already attacking and assign them as the new hero attackers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0820, length 776 bytes; spawned by CQ_MinionCampScript::PhaseOne 0x00DA05D0.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, IsAssignedAsHeroAttacker 0x00D9EC50, GetSquaredDistanceBetweenThings 0x00CBE583 (__fastcall), AddNewHeroAttacker
//   0x00DA0B30, free 0x00BFEA14 (inline vector destructor copies), ~vector<CScriptThing> 0x008AC970 (the out-of-line copy);
//   CScriptThing slots 0 (virtual destructor), 75 IsAlive and 77 IsNull; through the interface (+0x40): NewScriptFrame (7),
//   GetHero (70), GetAllThingsWithScriptName (75).
// Strings: "MC_MinionGuard" 0x012D6CF4.
// Layout (retail = Ego_r.pdb - 0x14, as the landed ctor 0x00D9E210): CurrentStage +0x74, HeroAttackerOne +0x78,
//   HeroAttackerTwo +0x84.
// Shape: vector model of the landed CQ_BountyHuntScript::WatchForAllLeaderChargeGruntsKilled 0x00D1CAC0; `#pragma
//   inline_depth(0)` regions pick the return sites that call the out-of-line vector destructor.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline void Destroy(CScriptThing* first, CScriptThing* last)
{
    for (; first != last; ++first)
        first->~CScriptThing();
}
struct CScriptThingVector {                             // STLport vector<CScriptThing>, 12 bytes in retail
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~CScriptThingVector()
    {
        Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CScriptThing& operator[](unsigned int n) { return _M_start[n]; }
};
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
class CGameScriptInterface { public:                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) GetHero
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const;   // slot 75 (+0x12c) GetAllThingsWithScriptName
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
enum MissionStage { MISSION_STAGE_0 };
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x74 - 0x48];
    long                  CurrentStage;                 // +0x74
    CScriptThing          HeroAttackerOne;              // +0x78
    CScriptThing          HeroAttackerTwo;              // +0x84
    void RunFirstWaveControl();
    bool IsAssignedAsHeroAttacker(const CScriptThing& thing);   // 0x00D9EC50
    void AddNewHeroAttacker(const CScriptThing& thing);         // 0x00DA0B30
};
void CQ_MinionCampScript::RunFirstWaveControl()
{
    while (CurrentStage == 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if ((HeroAttackerOne.IsNull() || !HeroAttackerOne.IsAlive()) && (HeroAttackerTwo.IsNull() || !HeroAttackerTwo.IsAlive())) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThingVector guards;
            Game->GetAllThingsWithScriptName("MC_MinionGuard", guards);
            unsigned int count = guards.size();
            CScriptThing* nearest = 0;
            CScriptThing* second = 0;
            float nearestDistance = -1.0f;
            float secondDistance = -1.0f;
            for (unsigned int i = 0; i < count; ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (IsAssignedAsHeroAttacker(guards[i])) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                } else {
                    float distance = GetSquaredDistanceBetweenThings(guards[i], Game->GetHero());
                    if (distance < nearestDistance || nearestDistance == -1.0f) {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        second = nearest;
                        secondDistance = nearestDistance;
                        nearest = &guards[i];
                        nearestDistance = distance;
                    } else if (distance < secondDistance || secondDistance == -1.0f) {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        second = &guards[i];
                        secondDistance = distance;
                    }
                }
            }
            if (IsActiveThreadTerminating())
                return;
            if (nearest) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                AddNewHeroAttacker(*nearest);
            }
            if (second) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                AddNewHeroAttacker(*second);
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}