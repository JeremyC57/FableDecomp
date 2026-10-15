// Behaviour test for 0x009775F0 (CCreatureAction_JackScream copy constructor): the base is copy-constructed,
// the counted pointer copied and its shared count incremented (a null count block is left alone).
#include <cstdio>

struct CCPPointerInfo {
    long m_count;
};

struct CModeJackOfBladesScream;

struct CCountedPointer_T {
    CModeJackOfBladesScream* Data;
    CCPPointerInfo* Info;
    CCountedPointer_T() : Data(0), Info(0) { }
    CCountedPointer_T(const CCountedPointer_T& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

static int g_baseCopies = 0;

struct CCombatActionBase {
    virtual void v00() { }
    virtual ~CCombatActionBase() { }
    unsigned char _body[272];
    CCombatActionBase() { for (int i = 0; i < 272; ++i) _body[i] = 0; }
    CCombatActionBase(const CCombatActionBase& other) { ++g_baseCopies; for (int i = 0; i < 272; ++i) _body[i] = 0; }
};

struct CCreatureAction_JackScream : CCombatActionBase {
    CCountedPointer_T PModeCountedPointer;
    virtual void v00() { }
    CCreatureAction_JackScream() { }
    CCreatureAction_JackScream(const CCreatureAction_JackScream& other);
};

CCreatureAction_JackScream::CCreatureAction_JackScream(const CCreatureAction_JackScream& other)
    : CCombatActionBase(other), PModeCountedPointer(other.PModeCountedPointer)
{
}

int main()
{
    CCPPointerInfo info;
    info.m_count = 1;
    CCreatureAction_JackScream src;
    src.PModeCountedPointer.Data = (CModeJackOfBladesScream*)&info;
    src.PModeCountedPointer.Info = &info;
    CCreatureAction_JackScream copy(src);
    if (g_baseCopies != 1 || copy.PModeCountedPointer.Data != src.PModeCountedPointer.Data || copy.PModeCountedPointer.Info != &info || info.m_count != 2) {
        std::printf("009775f0_TEST FAIL\n");
        return 1;
    }
    CCreatureAction_JackScream empty;
    CCreatureAction_JackScream copy2(empty);
    if (g_baseCopies != 2 || copy2.PModeCountedPointer.Data != 0 || copy2.PModeCountedPointer.Info != 0 || info.m_count != 2) {
        std::printf("009775f0_TEST FAIL\n");
        return 1;
    }
    std::printf("009775f0_TEST PASS\n");
    return 0;
}