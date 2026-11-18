#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

typedef std::string CCharString;
const float DELTA_NOTIONAL_ZERO = 0.0001f;

enum EMovementType { AI_MOVEMENT_NULL };
enum EAnimUpdateMode { ANIM_UPDATE_NONE, ANIM_UPDATE_FROM_SPEED };
enum ComponentKind { TCI_CARRYING, TCI_WEAPON, TCI_SPECIAL_ABILITIES, TCI_ANIMATING_OBJECT };
struct CThing {
    bool dead;
    void* parts[4];
    CThing() : dead(false)
    {
        for (unsigned i=0;i<4;++i)
            parts[i]=0;
    }
    bool IsDead() const { return dead; }
    template<class T> bool GetTC(ComponentKind type,T** output)
    {
        *output=static_cast<T*>(parts[type]);
        return *output!=0;
    }
    template<class T> bool PeekTC(ComponentKind type,T** output) const
    {
        *output=static_cast<T*>(parts[type]);
        return *output!=0;
    }
};
struct CAnimationEntry {
    long BankIndex;
    explicit CAnimationEntry(long index=0) : BankIndex(index) { }
    long GetDelayTime() const { return 3; }
    long GetTimeToBlendIn(const CThing&) const { return 4; }
    unsigned long GetAnimateThingFlags() const { return 7; }
};
struct CCreatureActionBase {
    bool finished;
    unsigned group, priority;
    bool IsFinished() const { return finished; }
    unsigned GetInterruptionGroupID() const { return group; }
    unsigned GetInterruptionPriority() const { return priority; }
};
struct CThingCreatureBase : CThing {
    CCharString strafe_name;
    CAnimationEntry normal, ability;
    CCreatureActionBase* action;
    CThingCreatureBase() : strafe_name("strafe"),normal(10),ability(20),action(0) { }
    const CAnimationEntry& GetAnimFromName(const CCharString& name,long) const
    {
        return name=="ability" ? ability : normal;
    }
    const CCreatureActionBase* PeekPCurrentAction() const { return action; }
};
struct CTCCarrying {
    CThing* carried;
    CThing* GetThingInPrimarySlot() const { return carried; }
};
struct CTCWeapon {
    bool projectile, loaded, analogue;
    bool IsProjectileWeapon() const { return projectile; }
    bool IsProjectileWeaponLoaded() const { return loaded; }
    bool IsAnalogueLoading() const { return analogue; }
};
struct CTCSpecialAbilities {
    bool locked;
    CCharString name;
    bool GetAbilityMutexLock() const { return locked; }
    CCharString GetCurrentAbilityStrafeAnim() const { return name; }
};
struct CTCAnimatingObject {
    unsigned manual, updates;
    float fraction;
    CTCAnimatingObject() : manual(0),updates(0),fraction(-1) { }
    void SetCurrentAnimAsManualUpdate() { ++manual; }
    void SetFractionComplete(float x) { fraction=x; ++updates; }
};
namespace NAnimationEntryBodge {
struct CAnimEntryLite {
    long BankIndex, AnimFlags;
    long GetTimeToBlendIn(const CThing&) const { return 6; }
};
}
struct CAnimFrameInfo {
    float Current,Last;
    CAnimFrameInfo(float current,float last) : Current(current),Last(last) { }
};
struct CAnimInit {
    unsigned type, index, priority;
    CAnimInit() : type(0),index(0),priority(0) { }
    void Init(unsigned char t, unsigned long, long bank, EAnimUpdateMode, bool, unsigned long, long rank)
    {
        type=t; index=bank; priority=rank;
    }
    CAnimInit(unsigned char t,unsigned long,long bank,EAnimUpdateMode,bool,unsigned long,long priority)
        : type(t),index(bank),priority(priority) { }
};
struct CAnimSet {
    bool valid, finished, updated;
    unsigned type, index, priority;
    long blend, delay;
    float alpha, internal, current, last, change;
    CAnimSet() : valid(false),finished(false),updated(false),type(0),index(0),priority(0),
        blend(-1),delay(-1),alpha(0),internal(0),current(0),last(0),change(0) { }
    bool IsFinished() const { return !valid || finished; }
    long GetAnimIndex() const { return index; }
    float GetPercentageComplete() const { return current; }
    CAnimFrameInfo GetFrame() const { return CAnimFrameInfo(current,last); }
    void SetFrame(const CAnimFrameInfo& f) { current=f.Current; last=f.Last; }
    void SetAsUpdated(bool v) { updated=v; }
    void SetChangeAlphaBy(float x) { change=x; }
    void SetAlpha(float x) { alpha=x; }
    void SetInternalAlpha(long i,float x,bool b) { assert(!i && !b); internal=x; }
    void Use(const CAnimInit& init,long b,long d)
    {
        valid=true; finished=false; updated=false;
        type=init.type; index=init.index; priority=init.priority; blend=b; delay=d;
    }
};
struct CAnimateThing {
    CAnimSet main, parts[38];
    unsigned main_sets, additions[38], removals[38];
    CAnimateThing() : main_sets(0)
    {
        for (unsigned i=0;i<38;++i)
            additions[i]=removals[i]=0;
    }
    const CAnimSet& PeekMainAnim() const { return main; }
    unsigned GetMainAnimType() const { return main.type; }
    void SetMainAnim(const CAnimInit& init,long blend,long delay)
    {
        main.Use(init,blend,delay);
        ++main_sets;
    }
    void AddCombinationAnim(const CAnimInit& init,long blend,long delay,bool,bool)
    {
        parts[init.type].Use(init,blend,delay);
        ++additions[init.type];
    }
    void RemoveCombinationAnimsOfType(long type)
    {
        parts[type].valid=false;
        ++removals[type];
    }
    void RemoveCombinationAnim(const CAnimSet* item)
    {
        RemoveCombinationAnimsOfType(item->type);
    }
    CAnimSet* GetFirstCombinationAnimFromType(long type)
    {
        return parts[type].valid ? parts+type : 0;
    }
};
template<class T> struct CCountedPointer {
    T* Data;
    CCountedPointer() : Data(0) { }
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    bool IsNull() const { return Data==0; }
};

namespace NCreatureMode {
CCharString GetStrafingAnimationName(const CThing& thing, EMovementType)
{
    return static_cast<const CThingCreatureBase&>(thing).strafe_name;
}
bool GetAnimDistanceAndIndexFromAnimName(const CThingCreatureBase&,const CCharString&,long& index,float& distance)
{
    index=10;
    distance=2.0f;
    return true;
}
class CModeStrafe {
public:
    const CThing& ParentThing;
    CCharString StrafeAnimName;
    long AnimIndex;
    float AnimMovementDistance, RumbleParam, PullBackAmount;
    bool ForceAnimNoBlendIn, has_frame;
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileAim, PAnimProjectileBuildUp,
                                                        PAnimProjectilePullBack, PAnimStrafeUpperBody;
    explicit CModeStrafe(const CThing& thing) : ParentThing(thing),AnimIndex(-1),AnimMovementDistance(0),
        RumbleParam(0.25f),PullBackAmount(0.75f),ForceAnimNoBlendIn(true),has_frame(false) { }
    bool GetBestFrameInAimingAnim(float& frame) const
    {
        frame=0.25f;
        return has_frame;
    }
    void AnimUpdate(CAnimateThing& anim);
};

// Selected AnimUpdate body. The closing brace terminates NCreatureMode.

// The selected implementation is reproduced verbatim for this standalone fixture.
void CModeStrafe::AnimUpdate(CAnimateThing& anim)
{
    const CThingCreatureBase& creature = static_cast<const CThingCreatureBase&>(ParentThing);
    CCharString anim_name = GetStrafingAnimationName(creature, (EMovementType)0);
    if (anim_name != StrafeAnimName || anim.PeekMainAnim().IsFinished() || anim.GetMainAnimType() != 0x1b) {
        StrafeAnimName = anim_name;
        GetAnimDistanceAndIndexFromAnimName(creature, anim_name, AnimIndex, AnimMovementDistance);
        const CAnimationEntry& anim_entry = creature.GetAnimFromName(anim_name, 0);
        anim.SetMainAnim(CAnimInit(0x1b, 0, AnimIndex, ANIM_UPDATE_FROM_SPEED, true, anim_entry.GetAnimateThingFlags(), 0x5a),
            ForceAnimNoBlendIn ? 0 : anim_entry.GetTimeToBlendIn(ParentThing), ForceAnimNoBlendIn ? 0 : anim_entry.GetDelayTime());
    }
    CThing* pcarried = 0;
    const CTCWeapon* ptc_weapon = 0;
    bool add_bow_anims = false;
    const CTCCarrying* ptc_carrying;
    if (creature.PeekTC(TCI_CARRYING, &ptc_carrying)) {
        pcarried = ptc_carrying->GetThingInPrimarySlot();
        if (pcarried && pcarried->PeekTC(TCI_WEAPON, &ptc_weapon) && ptc_weapon->IsProjectileWeapon())
            add_bow_anims = true;
    }
    bool locked = false;
    const CTCSpecialAbilities* ptc_spec_abils;
    if (creature.PeekTC(TCI_SPECIAL_ABILITIES, &ptc_spec_abils) && ptc_spec_abils->GetAbilityMutexLock())
        locked = true;
    if (locked || !add_bow_anims) {
        anim.RemoveCombinationAnimsOfType(0x24);
        anim.RemoveCombinationAnimsOfType(0x25);
        anim.RemoveCombinationAnimsOfType(0x1f);
        CAnimSet* panim_set = anim.GetFirstCombinationAnimFromType(0x23);
        if (locked && ptc_spec_abils->GetCurrentAbilityStrafeAnim() != "") {
            CCharString spec_abil_animname = ptc_spec_abils->GetCurrentAbilityStrafeAnim();
            const CAnimationEntry& anim_entry = creature.GetAnimFromName(spec_abil_animname, 0);
            if (!panim_set || panim_set->GetAnimIndex() != anim_entry.BankIndex) {
                anim.RemoveCombinationAnimsOfType(0x23);
                CAnimInit anim_init(0x23, 0, anim_entry.BankIndex, ANIM_UPDATE_FROM_SPEED, true, anim_entry.GetAnimateThingFlags(), 0x5f);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : anim_entry.GetTimeToBlendIn(creature), 0, false, false);
            }
        } else if (const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimStrafeUpperBody.Data) {
            if (!panim_set || panim_set->GetAnimIndex() != lite->BankIndex) {
                anim.RemoveCombinationAnimsOfType(0x23);
                CAnimInit anim_init(0x23, 0, lite->BankIndex, ANIM_UPDATE_FROM_SPEED, true, lite->AnimFlags, 0x5f);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(creature), 0, false, false);
            }
        } else
            anim.RemoveCombinationAnimsOfType(0x23);
    } else {
        anim.RemoveCombinationAnimsOfType(0x23);
        if (PAnimProjectileAim.Data) {
            float aiming_frame;
            if (!GetBestFrameInAimingAnim(aiming_frame))
                aiming_frame = -1.0f;
            CAnimSet* panim_set = anim.GetFirstCombinationAnimFromType(0x24);
            if (!panim_set) {
                do {
                const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectileAim.Data;
                CAnimInit anim_init(0x24, 0, lite->BankIndex, ANIM_UPDATE_NONE, true, lite->AnimFlags, 0x5f);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(ParentThing), 0, false, false);
                } while (0);
                panim_set = anim.GetFirstCombinationAnimFromType(0x24);
                if (panim_set)
                    panim_set->SetFrame(CAnimFrameInfo(aiming_frame > -DELTA_NOTIONAL_ZERO ? aiming_frame : 0.5f, aiming_frame > -DELTA_NOTIONAL_ZERO ? aiming_frame : 0.5f));
            }
            if (panim_set) {
                float last_frame = panim_set->GetPercentageComplete();
                float cur_frame = last_frame;
                if (aiming_frame > -DELTA_NOTIONAL_ZERO)
                    cur_frame = aiming_frame;
                panim_set->SetFrame(CAnimFrameInfo(cur_frame, last_frame));
                panim_set->SetAsUpdated(true);
            }
        }
        if (PAnimProjectileBuildUp.Data) {
            CAnimSet* panim_set = anim.GetFirstCombinationAnimFromType(0x25);
            if (!panim_set || panim_set->IsFinished()) {
                if (panim_set)
                    anim.RemoveCombinationAnim(panim_set);
                const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectileBuildUp.Data;
                CAnimInit anim_init(0x25, 0, lite->BankIndex, ANIM_UPDATE_FROM_SPEED, true, lite->AnimFlags, 0x69);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(ParentThing), 0, false, false);
                panim_set = anim.GetFirstCombinationAnimFromType(0x25);
                if (panim_set) {
                    panim_set->SetAlpha(RumbleParam);
                    panim_set->SetChangeAlphaBy(0.0f);
                }
            }
            panim_set->SetAlpha(RumbleParam);
            panim_set->SetInternalAlpha(0, RumbleParam, false);
        }
        if (PAnimProjectilePullBack.Data && pcarried && !pcarried->IsDead() && ptc_weapon && ptc_weapon->IsAnalogueLoading()) {
            CAnimSet* panim_load = anim.GetFirstCombinationAnimFromType(0x1f);
            const CCreatureActionBase* paction = creature.PeekPCurrentAction();
            if (paction && !paction->IsFinished() && paction->GetInterruptionGroupID() == 100 && paction->GetInterruptionPriority() > 150) {
                if (panim_load)
                    anim.RemoveCombinationAnim(panim_load);
            } else if (!ptc_weapon->IsProjectileWeaponLoaded()) {
                if (panim_load)
                    anim.RemoveCombinationAnim(panim_load);
            } else {
                CTCAnimatingObject* ptc_anim_obj;
                if (!pcarried->GetTC(TCI_ANIMATING_OBJECT, &ptc_anim_obj))
                    ptc_anim_obj = 0;
                if (!panim_load) {
                    const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectilePullBack.Data;
                    CAnimInit anim_init(0x1f, 0, lite->BankIndex, ANIM_UPDATE_NONE, false, lite->AnimFlags, 0x6e);
                    anim.AddCombinationAnim(anim_init, 0, 0, false, false);
                    panim_load = anim.GetFirstCombinationAnimFromType(0x1f);
                    if (panim_load)
                        panim_load->SetFrame(CAnimFrameInfo(PullBackAmount, PullBackAmount));
                    if (ptc_anim_obj)
                        ptc_anim_obj->SetCurrentAnimAsManualUpdate();
                }
                panim_load->SetFrame(CAnimFrameInfo(PullBackAmount, panim_load->GetPercentageComplete()));
                panim_load->SetAsUpdated(true);
                if (ptc_anim_obj)
                    ptc_anim_obj->SetFractionComplete(PullBackAmount);
            }
        }
    }
    ForceAnimNoBlendIn = false;
}

}


int main()
{
    CThingCreatureBase creature;
    CThing carried;
    CTCCarrying carrying={&carried};
    CTCWeapon weapon={true,true,true};
    CTCSpecialAbilities abilities;
    abilities.locked=false;
    CTCAnimatingObject animated_object;
    CCreatureActionBase action={false,100,160};
    NAnimationEntryBodge::CAnimEntryLite upper={21,0}, aim={22,0}, charge={23,0}, pull={24,0};
    NCreatureMode::CModeStrafe state(creature);
    CAnimateThing anim;
    state.PAnimStrafeUpperBody.Data=&upper;
    state.AnimUpdate(anim);
    assert(!state.ForceAnimNoBlendIn && anim.main_sets==1 && anim.main.blend==0 && anim.main.delay==0);
    assert(anim.parts[0x23].valid && anim.parts[0x23].index==21);
    state.AnimUpdate(anim);
    assert(anim.main_sets==1 && anim.additions[0x23]==1);
    anim.main.finished=true;
    state.AnimUpdate(anim);
    assert(anim.main_sets==2 && anim.main.blend==4 && anim.main.delay==3);
    creature.parts[TCI_SPECIAL_ABILITIES]=&abilities;
    abilities.locked=true;
    abilities.name="ability";
    state.AnimUpdate(anim);
    assert(anim.parts[0x23].valid && anim.parts[0x23].index==20);
    abilities.name="";
    state.PAnimStrafeUpperBody.Data=0;
    state.AnimUpdate(anim);
    assert(!anim.parts[0x23].valid);
    creature.parts[TCI_CARRYING]=&carrying;
    carried.parts[TCI_WEAPON]=&weapon;
    carried.parts[TCI_ANIMATING_OBJECT]=&animated_object;
    abilities.locked=false;
    state.PAnimProjectileAim.Data=&aim;
    state.PAnimProjectileBuildUp.Data=&charge;
    state.PAnimProjectilePullBack.Data=&pull;
    state.AnimUpdate(anim);
    assert(anim.parts[0x24].current==0.5f && anim.parts[0x24].updated);
    assert(anim.parts[0x25].alpha==0.25f && anim.parts[0x25].internal==0.25f);
    assert(anim.parts[0x1f].current==0.75f && animated_object.manual==1 && animated_object.fraction==0.75f);
    state.has_frame=true;
    anim.parts[0x25].finished=true;
    creature.action=&action;
    state.AnimUpdate(anim);
    assert(anim.parts[0x24].current==0.25f && anim.parts[0x24].last==0.5f);
    assert(anim.additions[0x25]==2 && !anim.parts[0x1f].valid);
    action.finished=true;
    weapon.loaded=false;
    state.AnimUpdate(anim);
    assert(!anim.parts[0x1f].valid);
    weapon.loaded=true;
    carried.parts[TCI_ANIMATING_OBJECT]=0;
    state.AnimUpdate(anim);
    assert(anim.parts[0x1f].valid && animated_object.manual==1);
    carried.dead=true;
    state.AnimUpdate(anim);
    assert(animated_object.updates==1);
    puts("008e9b70_TEST PASS");
    return 0;
}
