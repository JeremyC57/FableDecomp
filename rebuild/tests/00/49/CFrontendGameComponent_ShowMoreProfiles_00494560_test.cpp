#include <cstdio>
class CWideString {
public:
    void* PData;
};
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
    std::vector<CWideString> ProfileNames;                  // +0x1e0c
    long                     ProfileNamesOffset;            // +0x1e18
private:
    void ShowMoreProfiles();
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ShowMoreProfiles()
{
    if (ProfileNames.size() > 8)
    {
        ProfileNamesOffset += 8;
        if ((unsigned long)ProfileNamesOffset > ProfileNames.size())
            ProfileNamesOffset = 0;
    }
}


struct CFrontendGameComponent_Test {
    static void Run(CFrontendGameComponent* c) { c->ShowMoreProfiles(); }
};
int main()
{
    static CFrontendGameComponent c;
    static CWideString names[20];
    c.ProfileNames._M_start = names; c.ProfileNames._M_finish = names + 20; c.ProfileNamesOffset = 0;
    CFrontendGameComponent_Test::Run(&c); bool ok = c.ProfileNamesOffset == 8;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.ProfileNamesOffset == 16;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.ProfileNamesOffset == 0;
    c.ProfileNames._M_finish = names + 8; c.ProfileNamesOffset = 3;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.ProfileNamesOffset == 3;
    if (ok) std::printf("00494560_TEST PASS\n");
    return 0;
}