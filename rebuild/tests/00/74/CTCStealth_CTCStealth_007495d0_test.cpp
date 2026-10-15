#include <cstdio>
#include <stdlib.h>

static int g_ptrDtors = 0, g_baseDtor = 0, g_defDestroyed = 0;
struct CThing;
struct CTCBase {
    virtual ~CTCBase() { ++g_baseDtor; }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
};
struct CBaseIntelligentPointer {
    void* m_vtable; void* m_pItem;
    ~CBaseIntelligentPointer() { ++g_ptrDtors; }
};
struct SDangerThing { CBaseIntelligentPointer first; long secondStatus; long secondTimer; };
struct CRetailVector_SDangerThing {
    SDangerThing* m_pFirst; SDangerThing* m_pLast; SDangerThing* m_pEnd;
    static void Destroy(SDangerThing* first, SDangerThing* last) { for (; first != last; ++first) first->~SDangerThing(); }
    ~CRetailVector_SDangerThing() { Destroy(m_pFirst, m_pLast); if (m_pFirst) free(m_pFirst); }
};
struct CStealthDef {
    virtual ~CStealthDef() { }
    virtual void Destroy() { ++g_defDestroyed; }
    long m_refCount;
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CDefPointer_CStealthDef {
    const CStealthDef* m_pDef;
    ~CDefPointer_CStealthDef() { if (m_pDef) { const_cast<CStealthDef*>(m_pDef)->Release(); m_pDef = 0; } }
};
struct CTCStealth : CTCBase {
    bool Viewable, Smellable, Hearable, Sneaking, SneakingToggleSet;
    float DistanceCanBeHeardFrom, MaxSneakingSpeed;
    CDefPointer_CStealthDef PDef;
    long FrameStartedWalking, FrameStartedSneakingAndMoving, SneakingAndMovingDeedHandle, ActionPermittedToSneakID;
    long RunningAwayFromCombatTimer, InDangerCoolOffTimer;
    CRetailVector_SDangerThing DangerThings;
    long MaxNumberOfDangerThings, RegionBasedMaxNumberOfDangerThings;
    ~CTCStealth() { }
};

int main()
{
    CStealthDef def; def.m_refCount = 2;
    {
        CTCStealth s;
        s.PDef.m_pDef = &def;
        s.DangerThings.m_pFirst = (SDangerThing*)malloc(4 * sizeof(SDangerThing));
        s.DangerThings.m_pLast = s.DangerThings.m_pFirst + 3;
        s.DangerThings.m_pEnd = s.DangerThings.m_pFirst + 4;
    }
    if (g_ptrDtors != 3 || g_baseDtor != 1 || def.m_refCount != 1 || g_defDestroyed != 0) { std::printf("007495d0_TEST FAIL\n"); return 1; }
    {
        CTCStealth s;
        s.PDef.m_pDef = &def;
        s.DangerThings.m_pFirst = s.DangerThings.m_pLast = s.DangerThings.m_pEnd = 0;
    }
    if (g_ptrDtors != 3 || g_baseDtor != 2 || def.m_refCount != 0 || g_defDestroyed != 1) { std::printf("007495d0_TEST FAIL\n"); return 1; }
    std::printf("007495d0_TEST PASS\n");
    return 0;
}