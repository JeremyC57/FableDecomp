#include <cstdio>
#include <cstring>
#include <new>
// Behaviour test for CTCInventoryExperience::CTCInventoryExperience(CThing*) (0x0057832D): same body as src/, with the
// base, map and list constructors stubbed and counted.

class CThing;
struct CComponent;
struct CViewport;
struct CYesNo;

static CThing* g_baseThing = 0; static int g_calls = 0;
struct CTCInventoryBase {
    CThing* thing;
    CTCInventoryBase(CThing* t) : thing(t) { g_baseThing = t; ++g_calls; }
};
namespace NHeroInformationScreens {
class CBase { public: int built; CBase() : built(1) { ++g_calls; } };
}

struct CTutorialSoundMap { int built; CTutorialSoundMap() : built(1) { ++g_calls; } };
struct CStatUndoMap { int built; CStatUndoMap() : built(1) { ++g_calls; } };
struct CAbilityUndoMap { int built; CAbilityUndoMap() : built(1) { ++g_calls; } };
struct CStatComponentMap { int built; CStatComponentMap() : built(1) { ++g_calls; } };
struct CAbilityComponentMap { int built; CAbilityComponentMap() : built(1) { ++g_calls; } };

template <class T> struct CAllocator { CAllocator() { } };
template <class V, class A> struct CAllocProxy : A {      // STLport _STLP_alloc_proxy
    V _M_data;
    CAllocProxy(const A& a, V p) : A(a), _M_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*, CAllocator<T> > _M_end_of_storage;
    CVectorBase(const CAllocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {        // STLport vector (release: 0xc bytes)
    explicit CVector(const CAllocator<T>& a = CAllocator<T>()) : CVectorBase<T>(a) { }
};
class CSliderAnimator;
struct SAnimatorNode;
struct CAnimatorList {                                      // STLport list<CSliderAnimator> (release: node pointer)
    SAnimatorNode* _M_node;
    explicit CAnimatorList(const CAllocator<CSliderAnimator>& a = CAllocator<CSliderAnimator>()) : _M_node((SAnimatorNode*)&a) { ++g_calls; _M_node = (SAnimatorNode*)1; }
};
struct CAnimatorIterator {
    SAnimatorNode* _M_node;
    CAnimatorIterator() { }
};
class CEnginePrimitiveHandle { public: long Handle; };
struct ExperienceSpendEvent { bool enable; bool initiate; long time; };
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
class CCharString { void* PStringData; };

class CTCInventoryExperience : public CTCInventoryBase, public NHeroInformationScreens::CBase { public:
    CTutorialSoundMap TutorialSoundHandles;                                 // +0x134
    CStatUndoMap UndoableStatBuys;                                          // +0x140
    CAbilityUndoMap UndoableAbilityBuys;                                    // +0x14C
    CVector<CEnginePrimitiveHandle> ExperiencePrimitiveHandles;             // +0x158
    CVector<CEnginePrimitiveHandle> ExperienceStrengthPrimitiveHandles;     // +0x164
    CVector<CEnginePrimitiveHandle> ExperienceSkillPrimitiveHandles;        // +0x170
    CVector<CEnginePrimitiveHandle> ExperienceWeaponMagicPrimitiveHandles;  // +0x17C
    CVector<CEnginePrimitiveHandle> ExperienceAbilityMagicPrimitiveHandles; // +0x188
    CVector<CEnginePrimitiveHandle> ExperiencePureMagicPrimitiveHandles;    // +0x194
    bool OpenedExperienceSpending;                                          // +0x1A0
    CAnimatorList Animators;                                                // +0x1A4
    CAnimatorIterator AnimIt;                                               // +0x1A8
    long OrigGeneral;                                                       // +0x1AC
    long OrigStrength;                                                      // +0x1B0
    long OrigSkill;                                                         // +0x1B4
    long OrigWill;                                                          // +0x1B8
    long statIndicatorPos;                                                  // +0x1BC
    long statIndicatorMax;                                                  // +0x1C0
    CYesNo* pYesNo;                                                         // +0x1C4
    ExperienceSpendEvent ExperienceSpendAdd;                                // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove;                             // +0x1D0
    CViewport* pViewport;                                                   // +0x1D8
    CViewport* pTextViewport;                                               // +0x1DC
    CVector<ExperienceSpendInfo> ExperienceSpendValuesPool;                 // +0x1E0
    CVector<ExperienceSpendInfo> ExperienceSpendValuesStats;                // +0x1EC
    CVector<ExperienceSpendInfo> ExperienceSpendValuesAbility;              // +0x1F8
    long ExperienceSpendMagnitude;                                          // +0x204
    CVector<long> MenuDisplayListExperienceItem;                            // +0x208
    bool CreateBecauseOfUndo;                                               // +0x214
    long NumItems;                                                          // +0x218
    bool DestroyEverything;                                                 // +0x21C
    long PreviousScreenType;                                                // +0x220 EScreenType
    CStatComponentMap UpgradableStats;                                      // +0x224
    CAbilityComponentMap UpgradableAbilities;                               // +0x230
    CVector<CComponent*> ExpSliders;                                        // +0x23C
    CVector<CCharString> TutorialsPlayed;                                   // +0x248
    float AccumFadeTime;                                                    // +0x254
    CComponent* PMousePointer;                                              // +0x258
    float Counter;                                                          // +0x25C
    long PointsGeneral;                                                     // +0x260
    long PointsStrength;                                                    // +0x264
    long PointsSkill;                                                       // +0x268
    long PointsWill;                                                        // +0x26C
    CTCInventoryExperience(CThing* thing);
};

CTCInventoryExperience::CTCInventoryExperience(CThing* thing)
    : CTCInventoryBase(thing), AccumFadeTime(0.0f), PMousePointer(0)
{
    DestroyEverything = false;
    CreateBecauseOfUndo = false;
    OpenedExperienceSpending = false;
}

int main()
{
    int fails = 0;
    static unsigned char raw[sizeof(CTCInventoryExperience)];
    std::memset(raw, 0xCD, sizeof raw);
    CThing* thing = (CThing*)0x1234;
    CTCInventoryExperience* e = new (raw) CTCInventoryExperience(thing);
    if (g_baseThing != thing || e->thing != thing || g_calls != 8) ++fails;
    if (!e->built || !e->TutorialSoundHandles.built || !e->UndoableStatBuys.built || !e->UndoableAbilityBuys.built
        || !e->UpgradableStats.built || !e->UpgradableAbilities.built || e->Animators._M_node != (SAnimatorNode*)1) ++fails;
    CVector<CEnginePrimitiveHandle>* v[6] = { &e->ExperiencePrimitiveHandles, &e->ExperienceStrengthPrimitiveHandles,
        &e->ExperienceSkillPrimitiveHandles, &e->ExperienceWeaponMagicPrimitiveHandles,
        &e->ExperienceAbilityMagicPrimitiveHandles, &e->ExperiencePureMagicPrimitiveHandles };
    for (int i = 0; i < 6; ++i) if (v[i]->_M_start || v[i]->_M_finish || v[i]->_M_end_of_storage._M_data) ++fails;
    if (e->ExperienceSpendValuesPool._M_start || e->ExperienceSpendValuesStats._M_finish
        || e->ExperienceSpendValuesAbility._M_end_of_storage._M_data || e->MenuDisplayListExperienceItem._M_start
        || e->ExpSliders._M_finish || e->TutorialsPlayed._M_end_of_storage._M_data) ++fails;
    if (e->AccumFadeTime != 0.0f || e->PMousePointer != 0) ++fails;
    if (e->DestroyEverything || e->CreateBecauseOfUndo || e->OpenedExperienceSpending) ++fails;
    if (((unsigned char*)&e->NumItems)[0] != 0xCD) ++fails;                 // not initialised by the ctor
    if (fails) { std::printf("0057832d_TEST FAIL %d\n", fails); return 1; }
    std::printf("0057832d_TEST PASS\n");
    return 0;
}