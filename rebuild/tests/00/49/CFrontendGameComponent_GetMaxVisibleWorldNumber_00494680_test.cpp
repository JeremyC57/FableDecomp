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
    unsigned char            _pad0000[0x1e30];
    std::vector<CWideString> WorldNames;         // +0x1e30
    long                     WorldNamesOffset;   // +0x1e3c
    long GetMaxVisibleWorldNumber() const;
};
long CFrontendGameComponent::GetMaxVisibleWorldNumber() const
{
    if (WorldNames.size() - WorldNamesOffset <= 8)
        return WorldNames.size() - WorldNamesOffset;
    return 8;
}

int main()
{
    static double mem[0x2000 / 8];
    CFrontendGameComponent* c = (CFrontendGameComponent*)mem;
    static CWideString names[12];
    c->WorldNames._M_start = names; c->WorldNames._M_end_of_storage = names + 12;
    bool ok = true;
    c->WorldNames._M_finish = names + 3; c->WorldNamesOffset = 0; ok = ok && c->GetMaxVisibleWorldNumber() == 3;
    c->WorldNames._M_finish = names + 12; c->WorldNamesOffset = 0; ok = ok && c->GetMaxVisibleWorldNumber() == 8;
    c->WorldNamesOffset = 4; ok = ok && c->GetMaxVisibleWorldNumber() == 8;
    c->WorldNamesOffset = 5; ok = ok && c->GetMaxVisibleWorldNumber() == 7;
    c->WorldNames._M_finish = names + 8; c->WorldNamesOffset = 0; ok = ok && c->GetMaxVisibleWorldNumber() == 8;
    c->WorldNames._M_finish = names + 2; c->WorldNamesOffset = 3; ok = ok && c->GetMaxVisibleWorldNumber() == 8;
    std::printf(ok ? "00494680_TEST PASS\n" : "FAIL\n");
    return 0;
}