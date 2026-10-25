// Behaviour test for 0x00A3E430 CSoundManagerLH::vfunc_1 (links the source object): a 0x20-byte bank is constructed with the
// manager as owner, then its slot 1 Load gets the very CWideString the caller passed.
#include <cstdio>
struct CSoundManagerLH;
class CWideString { public: void* m_pData; };
struct CSoundBankLH {
    virtual void v00();
    virtual void Load(const CWideString& fileName);
    unsigned char _04[0x1c];
    CSoundBankLH(CSoundManagerLH* owner);
};
struct CSoundManagerLH {
    virtual void v00();
    virtual CSoundBankLH* vfunc_1(const CWideString& fileName);
};
static CSoundManagerLH* g_owner = 0;
static const CWideString* g_loaded = 0;
CSoundBankLH::CSoundBankLH(CSoundManagerLH* owner) { g_owner = owner; }
void CSoundBankLH::v00() { }
void CSoundBankLH::Load(const CWideString& fileName) { g_loaded = &fileName; }
void CSoundManagerLH::v00() { }
int main()
{
    CSoundManagerLH m;
    CWideString name; name.m_pData = 0;
    CSoundBankLH* b = m.vfunc_1(name);
    bool ok = b != 0 && g_owner == &m && g_loaded == &name && sizeof(CSoundBankLH) == 0x20;
    if (!ok) { std::printf("00a3e430_TEST FAIL\n"); return 1; }
    std::printf("00a3e430_TEST PASS\n");
    return 0;
}
