// Behaviour test for 0x005DD670 CAnimationEntry::StatRegisterClass: every one of the 13 prototypes is registered by its token, by its
// virtual type and as the default of its static type; the registries share the prototype (reference count 3 once the local is
// released), and the two vector maps are sorted by key exactly once at the end (Dirty cleared).
#include <cstdio>
#include <cstring>
enum EAnimComponent { ANIM_COMPONENT_FLAGS, ANIM_COMPONENT_TRANSITION_IN_TIME, ANIM_COMPONENT_DELAY_TIME, ANIM_COMPONENT_COMBAT_HANDEDNESS,
    ANIM_COMPONENT_COMBAT_COMBO_CHAIN, ANIM_COMPONENT_COMBAT_RECOIL, ANIM_COMPONENT_COMBAT_MISC, ANIM_COMPONENT_COMBAT_FLOURISH_TARGET_OFFSET,
    ANIM_COMPONENT_FOLLOWING_ON_ANIM, ANIM_COMPONENT_STRIKE_SPEED_MULTIPLIER, ANIM_COMPONENT_SET_CREATURE_MODE,
    ANIM_COMPONENT_FOLLOWING_ON_ANIM_FILTER, ANIM_COMPONENT_STRIKE_RESPONSE_ANIM };
static int g_deleted = 0;
class CAnimComponentBase {
public:
    CAnimComponentBase() : IVRefCount(1), Defaulted(false) { }
    virtual ~CAnimComponentBase() { }
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) { ++g_deleted; delete this; } }
    virtual EAnimComponent GetType() const = 0;
    virtual void SetDefaultValue() { Defaulted = true; }
    long IVRefCount; bool Defaulted;
};
class CPtr {   // CIVCountedPointer<CAnimComponentBase>: adopts from a raw pointer, counts its copies
public:
    explicit CPtr(CAnimComponentBase* p) : Object(p) { }
    CPtr(const CPtr& o) : Object(o.Object) { if (Object) Object->AddRef(); }
    ~CPtr() { if (Object) Object->Release(); }
    CAnimComponentBase* operator->() const { return Object; }
    CAnimComponentBase* Object;
};
struct TokenEntry { char Token[32]; CAnimComponentBase* Object; };
static TokenEntry g_byToken[32]; static int g_tokens = 0;
static bool TokenInsert(const char* token, const CPtr& p) {
    for (int i = 0; i < g_tokens; ++i) if (std::strcmp(g_byToken[i].Token, token) == 0) return false;
    std::strcpy(g_byToken[g_tokens].Token, token); g_byToken[g_tokens].Object = p.Object; p.Object->AddRef(); ++g_tokens; return true;
}
struct VectorMap {   // CVectorMap<EAnimComponent, CIVCountedPointer<...> >: push_back + Dirty, sorted by Update
    EAnimComponent Key[32]; CAnimComponentBase* Value[32]; int Count; bool Dirty; int Sorts;
    void Insert(EAnimComponent k, const CPtr& v) { Key[Count] = k; Value[Count] = v.Object; v.Object->AddRef(); ++Count; Dirty = true; }
    void Update() {
        if (!Dirty) return;
        for (int i = 1; i < Count; ++i) for (int j = i; j > 0 && Key[j] < Key[j - 1]; --j) {
            EAnimComponent k = Key[j]; Key[j] = Key[j - 1]; Key[j - 1] = k; CAnimComponentBase* v = Value[j]; Value[j] = Value[j - 1]; Value[j - 1] = v; }
        ++Sorts; Dirty = false;
    }
};
static VectorMap g_byType, g_defaults;
#define COMPONENT(T, TOKEN, KEY) class T : public CAnimComponentBase { public: \
    virtual EAnimComponent GetType() const { return KEY; } \
    static const char* StatGetTokenName() { return TOKEN; } static EAnimComponent StatGetType() { return KEY; } };
COMPONENT(CAnimComponentFlags, "Flags", ANIM_COMPONENT_FLAGS)
COMPONENT(CAnimComponentTransitionInTime, "TransTime", ANIM_COMPONENT_TRANSITION_IN_TIME)
COMPONENT(CAnimComponentDelayTime, "Delay", ANIM_COMPONENT_DELAY_TIME)
COMPONENT(CAnimComponentCombatHandedness, "Handedness", ANIM_COMPONENT_COMBAT_HANDEDNESS)
COMPONENT(CAnimComponentCombatComboChain, "Combo", ANIM_COMPONENT_COMBAT_COMBO_CHAIN)
COMPONENT(CAnimComponentCombatRecoil, "Recoil", ANIM_COMPONENT_COMBAT_RECOIL)
COMPONENT(CAnimComponentCombatMisc, "CombatMisc", ANIM_COMPONENT_COMBAT_MISC)
COMPONENT(CAnimComponentCombatFlourishTargetOffset, "TargetOffset", ANIM_COMPONENT_COMBAT_FLOURISH_TARGET_OFFSET)
COMPONENT(CAnimComponentFollowingOnAnim, "NextAnim", ANIM_COMPONENT_FOLLOWING_ON_ANIM)
COMPONENT(CAnimComponentStrikeSpeedMultiplier, "StrikeSpeedMultiplier", ANIM_COMPONENT_STRIKE_SPEED_MULTIPLIER)
COMPONENT(CAnimComponentSetCreatureMode, "NextCreatureMode", ANIM_COMPONENT_SET_CREATURE_MODE)
COMPONENT(CAnimComponentFollowingOnAnimFilter, "NextAnimFilter", ANIM_COMPONENT_FOLLOWING_ON_ANIM_FILTER)
COMPONENT(CAnimComponentStrikeResponseAnim, "StrikeResponseAnim", ANIM_COMPONENT_STRIKE_RESPONSE_ANIM)
#define REGISTER_ANIM_COMPONENT(T) \
    { \
        CPtr component(new T); \
        component->SetDefaultValue(); \
        TokenInsert(T::StatGetTokenName(), component); \
        g_byType.Insert(component->GetType(), component); \
        g_defaults.Insert(T::StatGetType(), component); \
    }
static void StatRegisterClass()
{
    REGISTER_ANIM_COMPONENT(CAnimComponentFlags) REGISTER_ANIM_COMPONENT(CAnimComponentTransitionInTime)
    REGISTER_ANIM_COMPONENT(CAnimComponentDelayTime) REGISTER_ANIM_COMPONENT(CAnimComponentCombatHandedness)
    REGISTER_ANIM_COMPONENT(CAnimComponentCombatComboChain) REGISTER_ANIM_COMPONENT(CAnimComponentCombatRecoil)
    REGISTER_ANIM_COMPONENT(CAnimComponentCombatMisc) REGISTER_ANIM_COMPONENT(CAnimComponentCombatFlourishTargetOffset)
    REGISTER_ANIM_COMPONENT(CAnimComponentFollowingOnAnim) REGISTER_ANIM_COMPONENT(CAnimComponentStrikeSpeedMultiplier)
    REGISTER_ANIM_COMPONENT(CAnimComponentSetCreatureMode) REGISTER_ANIM_COMPONENT(CAnimComponentFollowingOnAnimFilter)
    REGISTER_ANIM_COMPONENT(CAnimComponentStrikeResponseAnim)
    g_byType.Update();
    g_defaults.Update();
}
int main()
{
    StatRegisterClass();
    int bad = 0;
    if (g_tokens != 13 || g_byType.Count != 13 || g_defaults.Count != 13 || g_byType.Dirty || g_defaults.Dirty) ++bad;
    if (g_byType.Sorts != 1 || g_defaults.Sorts != 1 || g_deleted != 0) ++bad;
    for (int i = 0; i < 13; ++i) {
        if (g_byType.Key[i] != (EAnimComponent)i || g_defaults.Key[i] != (EAnimComponent)i) ++bad;
        if (g_byType.Value[i] != g_defaults.Value[i] || g_byType.Value[i]->IVRefCount != 3 || !g_byType.Value[i]->Defaulted) ++bad;
        if (g_byToken[i].Object->GetType() != (EAnimComponent)i) ++bad;
    }
    if (std::strcmp(g_byToken[8].Token, "NextAnim") != 0 || std::strcmp(g_byToken[12].Token, "StrikeResponseAnim") != 0) ++bad;
    if (bad) { std::printf("005dd670_TEST FAIL\n"); return 1; }
    std::printf("005dd670_TEST PASS\n");
    return 0;
}