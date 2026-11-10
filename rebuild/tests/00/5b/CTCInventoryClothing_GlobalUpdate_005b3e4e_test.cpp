// Behaviour test for 005b3e4e CTCInventoryClothing::GlobalUpdate: the body of src/005b3e4e.cpp over minimal stand-ins of its
// callees (def lookup, animation bank, appearance modifiers, animation component). Checks: the base update always runs; nothing
// else happens while the inventory is closed or without a mannequin; the mannequin switches to EMBARRASSED_TWIRL when its
// covering-clothes list is empty and back to DEFAULT otherwise (also when it has no appearance-modifier component), keeping the
// current frame and the entry's transition time; an animation already playing is not restarted; every call advances the
// animator by 1/ConstantFPS.
#include <cstdio>
#include <list>
#include <string>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
enum EAnimUpdateMode { ANIM_UPDATE_NONE = 0, ANIM_UPDATE_FROM_SPEED = 1 };
enum EHeroAttachableAppearanceModifierType { APPEARANCE_HAIR = 0, APPEARANCE_HORN = 1, APPEARANCE_CLOTHING = 2 };
static long gConstantFPS = 30;
static int gBaseUpdates = 0;
class CAnimationEntry { public:
    long BankIndex; unsigned long Flags; long Transition;
    unsigned long GetAnimateThingFlags() const { return Flags; }
    long GetTransitionTime() const { return Transition; }
};
static CAnimationEntry gDefault = { 11, 0x40, 5 };
static CAnimationEntry gTwirl = { 22, 0x80, 7 };
struct CAnimFilterNoGroup { };
class CAnimationSet { public:
    const CAnimationEntry& GetAnim(const std::string& name, long, const CAnimFilterNoGroup&) const
    { return name == "DEFAULT" ? gDefault : gTwirl; }
};
class CAppearanceDef { public: CAnimationSet Animation; };
static CAppearanceDef gAppearance;
template <class T> class CDefPointer { public: const T* Object; CDefPointer() : Object(0) {} const T* operator->() const { return Object; } };
class CThingBaseDef { public: bool GetSubDef(CDefPointer<const CAppearanceDef>* out) const { out->Object = &gAppearance; return true; } };
static CThingBaseDef gBaseDef;
class CAnimFrameInfo { public: float Current, Last; CAnimFrameInfo() : Current(0), Last(0) {} CAnimFrameInfo(float c, float l) : Current(c), Last(l) {} };
class CAnimInit { public:
    unsigned char Type; unsigned long TypeFlags; long Anim; EAnimUpdateMode UpdateMode; bool Looping; unsigned long Flags; long Priority;
    CAnimFrameInfo Frame;
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode mode, bool looping, unsigned long flags, long priority)
        : Type(type), TypeFlags(typeFlags), Anim(anim), UpdateMode(mode), Looping(looping), Flags(flags), Priority(priority) {}
};
class CAnimSet { public:
    short Index; float Current, Last;
    long GetAnimIndex() const { return Index; }
    CAnimFrameInfo GetFrame() const { return CAnimFrameInfo(Current, Last); }
};
class CAnimateThingUpdate { public: float TimePassed, DistanceMoved, PercentageComplete;
    CAnimateThingUpdate(float t, float d) : TimePassed(t), DistanceMoved(d), PercentageComplete(0.0f) {} };
class CAnimateThing { public:
    CAnimSet Main; int SetCount; CAnimInit LastInit; long LastTransition; float LastTime; int Updates;
    CAnimateThing() : SetCount(0), LastInit(0, 0, 0, ANIM_UPDATE_NONE, false, 0, 0), LastTransition(0), LastTime(0), Updates(0) {}
    CAnimSet& GetMainAnim() { return Main; }
    void SetMainAnim(const CAnimInit& init, long transition, long) { ++SetCount; LastInit = init; LastTransition = transition; Main.Index = (short)init.Anim; }
    void Update(const CAnimateThingUpdate& u) { ++Updates; LastTime = u.TimePassed; }
};
class CTCAnimationComplex { public: CAnimateThing Animator; CAnimateThing& GetAnimation() { return Animator; } };
class CTCHeroAttachableAppearanceModifiers { public:
    bool Covered; int Edits;
    std::list<long> GetAppearanceModifierListWithCoversBodyAreaFlags(long, EHeroAttachableAppearanceModifierType) const
    { std::list<long> l; if (Covered) l.push_back(3); return l; }
    void EditFrameUpdate() { ++Edits; }
};
class CThing { public:
    CTCHeroAttachableAppearanceModifiers* Modifiers; CTCAnimationComplex* Complex;
    const CThingBaseDef* GetPBaseDef() const { return &gBaseDef; }
    bool GetTC(CTCHeroAttachableAppearanceModifiers** out) { if (!Modifiers) return false; *out = Modifiers; return true; }
    bool GetTC(CTCAnimationComplex** out) { if (!Complex) return false; *out = Complex; return true; }
};
class CMainGameComponent { public: static long GetConstantFPS() { return gConstantFPS; } };
struct CTCInventoryBase { void GlobalUpdate(bool) { ++gBaseUpdates; } };
class CTCInventoryClothing : public CTCInventoryBase { public:
    bool Open; CThing* PMannequin;
    bool IsInventoryOpen() const { return Open; }
    void GlobalUpdate(bool paused);
};
// ---- source body (verbatim) ----
void CTCInventoryClothing::GlobalUpdate(bool paused)
{
    CTCInventoryBase::GlobalUpdate(paused);
    if (IsInventoryOpen() && PMannequin) {
        CDefPointer<const CAppearanceDef> appearance;
        PMannequin->GetPBaseDef()->GetSubDef(&appearance);
        const CAnimationEntry& defaultAnim = appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
        const CAnimationEntry& twirlAnim = appearance->Animation.GetAnim("EMBARRASSED_TWIRL", 0, CAnimFilterNoGroup());
        const CAnimationEntry* anim = &defaultAnim;
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (PMannequin->GetTC(&modifiers)) {
            if (modifiers->GetAppearanceModifierListWithCoversBodyAreaFlags(4, APPEARANCE_CLOTHING).empty())
                anim = &twirlAnim;
            modifiers->EditFrameUpdate();
        }
        CTCAnimationComplex* complex = 0;
        PMannequin->GetTC(&complex);
        CAnimateThing& animator = complex->GetAnimation();
        if (animator.GetMainAnim().GetAnimIndex() != anim->BankIndex) {
            CAnimInit init(6, 0, anim->BankIndex, ANIM_UPDATE_FROM_SPEED, true, anim->GetAnimateThingFlags(), 100);
            init.Frame = animator.GetMainAnim().GetFrame();
            animator.SetMainAnim(init, anim->GetTransitionTime(), 0);
        }
        CAnimateThingUpdate update(1.0f / CMainGameComponent::GetConstantFPS(), 0.0f);
        animator.Update(update);
    }
}
static bool Near(float a, float b) { return a - b < 0.0001f && b - a < 0.0001f; }
int main()
{
    CTCHeroAttachableAppearanceModifiers mods = { false, 0 };
    CTCAnimationComplex complex;
    complex.Animator.Main.Index = 11; complex.Animator.Main.Current = 0.25f; complex.Animator.Main.Last = 0.2f;
    CThing mannequin = { &mods, &complex };
    CTCInventoryClothing inv; inv.Open = false; inv.PMannequin = &mannequin;
    inv.GlobalUpdate(false);                                 // closed: base only
    CHECK(gBaseUpdates == 1 && complex.Animator.Updates == 0 && mods.Edits == 0);
    inv.Open = true; inv.PMannequin = 0;
    inv.GlobalUpdate(false);                                 // no mannequin
    CHECK(gBaseUpdates == 2 && complex.Animator.Updates == 0);
    inv.PMannequin = &mannequin;
    inv.GlobalUpdate(false);                                 // uncovered: twirl starts, keeps the frame
    CHECK(gBaseUpdates == 3 && mods.Edits == 1 && complex.Animator.SetCount == 1);
    CHECK(complex.Animator.LastInit.Anim == 22 && complex.Animator.LastInit.Type == 6 && complex.Animator.LastInit.TypeFlags == 0);
    CHECK(complex.Animator.LastInit.UpdateMode == ANIM_UPDATE_FROM_SPEED && complex.Animator.LastInit.Looping);
    CHECK(complex.Animator.LastInit.Flags == 0x80 && complex.Animator.LastInit.Priority == 100 && complex.Animator.LastTransition == 7);
    CHECK(Near(complex.Animator.LastInit.Frame.Current, 0.25f) && Near(complex.Animator.LastInit.Frame.Last, 0.2f));
    CHECK(complex.Animator.Updates == 1 && Near(complex.Animator.LastTime, 1.0f / 30.0f));
    inv.GlobalUpdate(false);                                 // still uncovered: already twirling, not restarted
    CHECK(complex.Animator.SetCount == 1 && complex.Animator.Updates == 2 && mods.Edits == 2);
    mods.Covered = true; gConstantFPS = 25;
    inv.GlobalUpdate(true);                                  // dressed: back to DEFAULT
    CHECK(complex.Animator.SetCount == 2 && complex.Animator.LastInit.Anim == 11 && complex.Animator.LastInit.Flags == 0x40);
    CHECK(complex.Animator.LastTransition == 5 && Near(complex.Animator.LastTime, 1.0f / 25.0f));
    complex.Animator.Main.Index = 22; mannequin.Modifiers = 0;
    inv.GlobalUpdate(false);                                 // no modifier component: DEFAULT
    CHECK(complex.Animator.SetCount == 3 && complex.Animator.LastInit.Anim == 11 && mods.Edits == 3);
    if (fails) { std::printf("%d failures\n", fails); return 1; }
    std::printf("005b3e4e_TEST PASS\n");
    return 0;
}