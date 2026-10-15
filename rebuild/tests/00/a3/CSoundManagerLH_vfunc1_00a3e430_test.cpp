#include <cstdio>
struct CSoundManagerLH;
struct CSoundBankLH {
    virtual void v00() { }
    virtual void Load(long bank) { m_loaded = bank; }
    CSoundManagerLH* m_owner; long m_loaded;
    CSoundBankLH(CSoundManagerLH* owner) : m_owner(owner), m_loaded(-1) { }
};
struct CSoundManagerLH {
    virtual void v00() { }
    virtual CSoundBankLH* vfunc_1(long bank)
    {
        CSoundBankLH* result = new CSoundBankLH(this);
        result->Load(bank);
        return result;
    }
};
int main()
{
    CSoundManagerLH m;
    CSoundBankLH* b = m.vfunc_1(7);
    bool ok = b && b->m_owner == &m && b->m_loaded == 7;
    delete b;
    if (!ok) { std::printf("00a3e430_TEST FAIL\n"); return 1; }
    std::printf("00a3e430_TEST PASS\n");
    return 0;
}