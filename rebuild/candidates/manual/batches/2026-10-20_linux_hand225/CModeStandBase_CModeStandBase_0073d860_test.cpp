// Behaviour test for 0x0073D860 CModeStandBase::~CModeStandBase: each animation counted pointer drops its reference;
// a block reaching zero destroys its item through the block's member pointer and is deleted; the order is last first.
#include <cstdio>
#include <new>

static int g_order[16]; static int g_destroyed; static int g_deleted;
struct CCountedItem {
    int id;
    void Destroy() { g_order[g_destroyed++] = id; }
};
struct SCountBlock {
    long m_references;
    void (CCountedItem::*m_pfnDestroy)();
    CCountedItem* m_pItem;
};
static void DeleteBlock(SCountBlock* b) { ++g_deleted; delete b; }
struct CCountedPointer {
    CCountedItem* m_pObject;
    SCountBlock* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
    ~CCountedPointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_references;
            if (m_pBlock->m_references == 0) {
                (m_pBlock->m_pItem->*m_pBlock->m_pfnDestroy)();
                DeleteBlock(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};
static int g_strings;
struct CCharString { int id; ~CCharString() { ++g_strings; } };
struct CModeBase { CCharString WeaponGroup; };
struct CModeStandBase : CModeBase {
    CCharString PoseAnimName;
    CCountedPointer PStandAnimation, PSlowWalkAnimation, PWalkAnimation, PJogAnimation, PRunAnimation, PSprintAnimation;
};
static void Attach(CCountedPointer& p, CCountedItem* item, long refs)
{
    SCountBlock* b = new SCountBlock;
    b->m_references = refs; b->m_pfnDestroy = &CCountedItem::Destroy; b->m_pItem = item;
    p.m_pObject = item; p.m_pBlock = b;
}
int main()
{
    CCountedItem items[6] = { {1}, {2}, {3}, {4}, {5}, {6} };
    SCountBlock* shared = 0;
    {
        CModeStandBase mode;
        Attach(mode.PStandAnimation, &items[0], 1);
        Attach(mode.PSlowWalkAnimation, &items[1], 1);
        Attach(mode.PWalkAnimation, &items[2], 2);          // still referenced elsewhere
        shared = mode.PWalkAnimation.m_pBlock;
        Attach(mode.PJogAnimation, &items[3], 1);
        Attach(mode.PSprintAnimation, &items[5], 1);        // PRunAnimation stays empty
    }
    if (g_destroyed != 4 || g_deleted != 4 || g_strings != 2) { std::printf("0073d860_TEST FAIL\n"); return 1; }
    if (g_order[0] != 6 || g_order[1] != 4 || g_order[2] != 2 || g_order[3] != 1) { std::printf("0073d860_TEST FAIL\n"); return 1; }
    if (shared->m_references != 1) { std::printf("0073d860_TEST FAIL\n"); return 1; }
    delete shared;
    std::printf("0073d860_TEST PASS\n");
    return 0;
}
