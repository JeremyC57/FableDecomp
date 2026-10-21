#include <cstdio>
class CWideString { public: void* PData; };
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};
}
class CFrontendGameComponent {
public:
    unsigned char            _pad0000[0x1e1c];
    std::vector<CWideString> SaveNames;          // +0x1e1c
    long                     SaveNamesOffset;    // +0x1e28
    long GetMaxVisibleSaveNumber() const;
};
long CFrontendGameComponent::GetMaxVisibleSaveNumber() const
{
    if (SaveNames.size() - SaveNamesOffset <= 8)
        return SaveNames.size() - SaveNamesOffset;
    return 8;
}

int main()
{
    static double mem[0x2000 / 8];
    CFrontendGameComponent* c = (CFrontendGameComponent*)mem;
    static CWideString names[12];
    c->SaveNames._M_start = names; c->SaveNames._M_end_of_storage = names + 12;
    bool ok = true;
    c->SaveNames._M_finish = names + 3; c->SaveNamesOffset = 0; ok = ok && c->GetMaxVisibleSaveNumber() == 3;
    c->SaveNames._M_finish = names + 12; c->SaveNamesOffset = 0; ok = ok && c->GetMaxVisibleSaveNumber() == 8;
    c->SaveNamesOffset = 4; ok = ok && c->GetMaxVisibleSaveNumber() == 8;
    c->SaveNamesOffset = 5; ok = ok && c->GetMaxVisibleSaveNumber() == 7;
    c->SaveNames._M_finish = names + 8; c->SaveNamesOffset = 0; ok = ok && c->GetMaxVisibleSaveNumber() == 8;
    c->SaveNames._M_finish = names + 2; c->SaveNamesOffset = 3; ok = ok && c->GetMaxVisibleSaveNumber() == 8;
    std::printf(ok ? "00494770_TEST PASS\n" : "FAIL\n");
    return 0;
}