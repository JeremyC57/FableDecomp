#include <cstdio>

class CSoundBankObject_00a0b6c0 {
public:
    virtual void Release_00a0b6c0();
};
class CSoundBankLH {
public:
    virtual void v00();
    virtual void v01();
    virtual void Unload_00a0b6c0();
    unsigned char _pad_0x04[0xc];
    CSoundBankObject_00a0b6c0* BankObject;
};
void CSoundBankLH::v00() { }
void CSoundBankLH::v01() { }
void CSoundBankObject_00a0b6c0::Release_00a0b6c0() { }

static int g_called = 0;
static CSoundBankObject_00a0b6c0* g_called_on = 0;
class CTestObject : public CSoundBankObject_00a0b6c0 {
public:
    virtual void Release_00a0b6c0() { g_called++; g_called_on = this; }
};

int main() {
    CTestObject c;
    CSoundBankLH r;
    r.BankObject = &c;

    r.Unload_00a0b6c0();
    bool ok1 = (g_called == 1) && (g_called_on == &c) && (r.BankObject == 0);

    // null path: no call, stays null
    g_called = 0;
    r.BankObject = 0;
    r.Unload_00a0b6c0();
    bool ok2 = (g_called == 0) && (r.BankObject == 0);

    if (ok1 && ok2) printf("00a0b6c0_TEST PASS\n");
    else printf("FAIL c=%d r=%p\n", g_called, (void*)r.BankObject);
    return 0;
}