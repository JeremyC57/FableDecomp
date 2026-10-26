#include <cstdio>
namespace NUISystem {
struct CComponentRoot_Retail { virtual void v00() { } };
struct CComponentBase2_Retail { virtual void v00() { } unsigned char _body[0x10]; };
struct CComponentBase3_Retail { virtual void v00() { } };
class CChangingStateComponent : public CComponentRoot_Retail, public CComponentBase2_Retail, public CComponentBase3_Retail { public:
    unsigned char _body[0x15c - 0x1c];
    long m_state;
    CChangingStateComponent(long state) : m_state(state) { }
};
struct COk : CChangingStateComponent {
    long m_result;
    virtual void v00() { }
    COk(long state);
};
COk::COk(long state)
    : CChangingStateComponent(state), m_result(0)
{
}
}
int main()
{
    NUISystem::COk dialog(42);
    dialog.m_result = 0;
    if (dialog.m_state != 42 || dialog.m_result != 0) { std::printf("0041cb70_TEST FAIL\n"); return 1; }
    unsigned char* base = reinterpret_cast<unsigned char*>(static_cast<NUISystem::CChangingStateComponent*>(&dialog));
    if (reinterpret_cast<unsigned char*>(&dialog.m_result) - base != 0x15c + sizeof(long)) { std::printf("0041cb70_TEST FAIL\n"); return 1; }
    std::printf("0041cb70_TEST PASS\n");
    return 0;
}