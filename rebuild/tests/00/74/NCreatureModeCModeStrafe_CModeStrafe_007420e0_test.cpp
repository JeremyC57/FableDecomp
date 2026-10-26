// behaviour copy of NCreatureMode::CModeStrafe::~CModeStrafe(): the four counted pointers drop a reference each (destroying the
// item and deleting the block at zero, zeroing both words), then the strings are destroyed (here: logged)
#include <cstdio>
static int g_destroyed = 0, g_deleted = 0, g_strings = 0;
static bool g_armed = false;
static void DeleteSpy(void* p) { if (g_armed && p) ++g_deleted; }   // records only, never frees
struct CCountedItem { void Destroy() { ++g_destroyed; } };
struct SCountBlock { long m_references; void (CCountedItem::*m_pfnDestroy)(); CCountedItem* m_pItem; };
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pBlock;
    ~CCountedPointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_references;
            if (m_pBlock->m_references == 0) {
                (m_pBlock->m_pItem->*m_pBlock->m_pfnDestroy)();
                DeleteSpy(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};
class CCharString { public: void* p; ~CCharString() { ++g_strings; } };
struct CModeStrafeModel {
    CCharString StrafeAnimName; CCharString FilterGroup;
    CCountedPointer PAnimProjectileAim, PAnimProjectileBuildUp, PAnimProjectilePullBack, PAnimStrafeUpperBody;
};
int main()
{
    CCountedItem item;
    SCountBlock shared = { 2, &CCountedItem::Destroy, &item };
    SCountBlock single = { 1, &CCountedItem::Destroy, &item };
    g_armed = true;
    {
        CModeStrafeModel m;
        m.StrafeAnimName.p = 0; m.FilterGroup.p = 0;
        m.PAnimProjectileAim.m_pObject = &item; m.PAnimProjectileAim.m_pBlock = &shared;
        m.PAnimProjectileBuildUp.m_pObject = &item; m.PAnimProjectileBuildUp.m_pBlock = &single;
        m.PAnimProjectilePullBack.m_pObject = 0; m.PAnimProjectilePullBack.m_pBlock = 0;
        m.PAnimStrafeUpperBody.m_pObject = 0; m.PAnimStrafeUpperBody.m_pBlock = 0;
    }
    g_armed = false;
    if (shared.m_references != 1 || single.m_references != 0 || g_destroyed != 1 || g_deleted != 1 || g_strings != 2)
    { std::printf("007420e0_TEST FAIL\n"); return 1; }
    std::printf("007420e0_TEST PASS\n");
    return 0;
}