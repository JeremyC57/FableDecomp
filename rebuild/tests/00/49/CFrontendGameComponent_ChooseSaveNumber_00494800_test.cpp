#include <cstdio>
class CWideString {
public:
    void* PData;
    CWideString& operator=(const CWideString& s);        // 0x0099B7D0
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class CFrontendGameComponent {
public:
    virtual ~CFrontendGameComponent();
    virtual void _slot01(); virtual void _slot02(); virtual void _slot03();
    virtual void _slot04();                              // vtable +0x10
    unsigned char            _pad0004[0x1e1c - 4];
    std::vector<CWideString> SaveNames;                  // +0x1e1c
    long                     SaveNamesOffset;            // +0x1e28
    CWideString              SaveToLoad;                 // +0x1e2c
private:
    void ChooseSaveNumber(long number);
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ChooseSaveNumber(long number)
{
    unsigned long index = SaveNamesOffset + number - 1;
    if (index < SaveNames.size())
    {
        SaveToLoad = SaveNames[index];
        _slot04();
    }
}

static int g_slot4 = 0;
CWideString& CWideString::operator=(const CWideString& s) { PData = s.PData; return *this; }
CFrontendGameComponent::~CFrontendGameComponent() {}
void CFrontendGameComponent::_slot01() {} void CFrontendGameComponent::_slot02() {} void CFrontendGameComponent::_slot03() {}
void CFrontendGameComponent::_slot04() { ++g_slot4; }
struct CFrontendGameComponent_Test {
    static bool Run()
    {
        static CWideString names[3];
        names[0].PData = (void*)1; names[1].PData = (void*)2; names[2].PData = (void*)3;
        CFrontendGameComponent* c = new CFrontendGameComponent;
        c->SaveNames._M_start = names; c->SaveNames._M_finish = names + 3; c->SaveNamesOffset = 1; c->SaveToLoad.PData = 0;
        c->ChooseSaveNumber(2);
        bool ok = c->SaveToLoad.PData == (void*)3 && g_slot4 == 1;
        c->ChooseSaveNumber(3);
        ok = ok && c->SaveToLoad.PData == (void*)3 && g_slot4 == 1;
        c->SaveNamesOffset = 0; c->ChooseSaveNumber(0);
        ok = ok && g_slot4 == 1;
        c->ChooseSaveNumber(1);
        return ok && c->SaveToLoad.PData == (void*)1 && g_slot4 == 2;
    }
};
int main()
{
    if (CFrontendGameComponent_Test::Run()) std::printf("00494800_TEST PASS\n");
    return 0;
}