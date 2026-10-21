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
    unsigned char            _pad0000[0x1e0c];
    std::vector<CWideString> ProfileNames;       // +0x1e0c
    long                     ProfileNamesOffset; // +0x1e18
    long GetMaxVisibleProfileNumber() const;
};
long CFrontendGameComponent::GetMaxVisibleProfileNumber() const
{
    if (ProfileNames.size() - ProfileNamesOffset <= 8)
        return ProfileNames.size() - ProfileNamesOffset;
    return 8;
}

int main()
{
    static double mem[0x2000 / 8];
    CFrontendGameComponent* c = (CFrontendGameComponent*)mem;
    static CWideString names[12];
    c->ProfileNames._M_start = names; c->ProfileNames._M_end_of_storage = names + 12;
    bool ok = true;
    c->ProfileNames._M_finish = names + 3; c->ProfileNamesOffset = 0; ok = ok && c->GetMaxVisibleProfileNumber() == 3;
    c->ProfileNames._M_finish = names + 12; c->ProfileNamesOffset = 0; ok = ok && c->GetMaxVisibleProfileNumber() == 8;
    c->ProfileNamesOffset = 4; ok = ok && c->GetMaxVisibleProfileNumber() == 8;
    c->ProfileNamesOffset = 5; ok = ok && c->GetMaxVisibleProfileNumber() == 7;
    c->ProfileNames._M_finish = names + 8; c->ProfileNamesOffset = 0; ok = ok && c->GetMaxVisibleProfileNumber() == 8;
    c->ProfileNames._M_finish = names + 2; c->ProfileNamesOffset = 3; ok = ok && c->GetMaxVisibleProfileNumber() == 8;
    std::printf(ok ? "00494520_TEST PASS\n" : "FAIL\n");
    return 0;
}