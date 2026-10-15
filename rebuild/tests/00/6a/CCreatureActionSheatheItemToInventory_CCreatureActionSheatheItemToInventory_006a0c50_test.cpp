// Behaviour test for 0x006A0C50 (CCreatureAction_SheatheItemToInventory copy constructor): the base is copy-constructed,
// the counted pointer copied and its shared count incremented (a null count block is left alone).
#include <cstdio>

struct CCPPointerInfo {
    long m_count;
};

struct CCreatureActionBase;

struct CCountedPointer_T {
    CCreatureActionBase* Data;
    CCPPointerInfo* Info;
    CCountedPointer_T() : Data(0), Info(0) { }
    CCountedPointer_T(const CCountedPointer_T& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

static int g_baseCopies = 0;

struct CCreatureActionBase {
    virtual void v00() { }
    virtual ~CCreatureActionBase() { }
    unsigned char _body[164];
    CCreatureActionBase() { for (int i = 0; i < 164; ++i) _body[i] = 0; }
    CCreatureActionBase(const CCreatureActionBase& other) { ++g_baseCopies; for (int i = 0; i < 164; ++i) _body[i] = 0; }
};

struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {
    CCountedPointer_T PFollowUpAction;
    virtual void v00() { }
    CCreatureAction_SheatheItemToInventory() { }
    CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& other);
};

CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& other)
    : CCreatureActionBase(other), PFollowUpAction(other.PFollowUpAction)
{
}

int main()
{
    CCPPointerInfo info;
    info.m_count = 1;
    CCreatureAction_SheatheItemToInventory src;
    src.PFollowUpAction.Data = (CCreatureActionBase*)&info;
    src.PFollowUpAction.Info = &info;
    CCreatureAction_SheatheItemToInventory copy(src);
    if (g_baseCopies != 1 || copy.PFollowUpAction.Data != src.PFollowUpAction.Data || copy.PFollowUpAction.Info != &info || info.m_count != 2) {
        std::printf("006a0c50_TEST FAIL\n");
        return 1;
    }
    CCreatureAction_SheatheItemToInventory empty;
    CCreatureAction_SheatheItemToInventory copy2(empty);
    if (g_baseCopies != 2 || copy2.PFollowUpAction.Data != 0 || copy2.PFollowUpAction.Info != 0 || info.m_count != 2) {
        std::printf("006a0c50_TEST FAIL\n");
        return 1;
    }
    std::printf("006a0c50_TEST PASS\n");
    return 0;
}