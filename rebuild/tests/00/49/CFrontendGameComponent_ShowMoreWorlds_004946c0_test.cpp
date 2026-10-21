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
    unsigned char            _pad0000[0x1e30];
    std::vector<CWideString> WorldNames;                  // +0x1e30
    long                     WorldNamesOffset;            // +0x1e3c
private:
    void ShowMoreWorlds();
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ShowMoreWorlds()
{
    if (WorldNames.size() > 8)
    {
        WorldNamesOffset += 8;
        if ((unsigned long)WorldNamesOffset > WorldNames.size())
            WorldNamesOffset = 0;
    }
}


struct CFrontendGameComponent_Test {
    static void Run(CFrontendGameComponent* c) { c->ShowMoreWorlds(); }
};
int main()
{
    static CFrontendGameComponent c;
    static CWideString names[20];
    c.WorldNames._M_start = names; c.WorldNames._M_finish = names + 20; c.WorldNamesOffset = 0;
    CFrontendGameComponent_Test::Run(&c); bool ok = c.WorldNamesOffset == 8;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.WorldNamesOffset == 16;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.WorldNamesOffset == 0;
    c.WorldNames._M_finish = names + 8; c.WorldNamesOffset = 3;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.WorldNamesOffset == 3;
    if (ok) std::printf("004946c0_TEST PASS\n");
    return 0;
}