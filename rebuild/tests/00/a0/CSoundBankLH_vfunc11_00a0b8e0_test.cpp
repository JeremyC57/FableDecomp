#include <cstdio>
struct CSoundBankObject_00a0b8e0 { virtual void v00() { } virtual long Get() { return 42; } };
struct CSoundBankLH {
    virtual void v00() { }
    long m_4, m_8, m_bankIndex;
    CSoundBankObject_00a0b8e0* m_pObject;
    virtual long vfunc_11() { if (m_pObject) return m_pObject->Get(); return 0; }
};
int main()
{
    CSoundBankObject_00a0b8e0 obj;
    CSoundBankLH bank; bank.m_pObject = 0;
    long a = bank.vfunc_11();
    bank.m_pObject = &obj;
    long b = bank.vfunc_11();
    if (a != 0 || b != 42) { std::printf("00a0b8e0_TEST FAIL\n"); return 1; }
    std::printf("00a0b8e0_TEST PASS\n");
    return 0;
}