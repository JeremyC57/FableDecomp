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
    unsigned char            _pad0000[0x1e1c];
    std::vector<CWideString> SaveNames;                  // +0x1e1c
    long                     SaveNamesOffset;            // +0x1e28
private:
    void ShowMoreSaves();
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ShowMoreSaves()
{
    if (SaveNames.size() > 8)
    {
        SaveNamesOffset += 8;
        if ((unsigned long)SaveNamesOffset > SaveNames.size())
            SaveNamesOffset = 0;
    }
}


struct CFrontendGameComponent_Test {
    static void Run(CFrontendGameComponent* c) { c->ShowMoreSaves(); }
};
int main()
{
    static CFrontendGameComponent c;
    static CWideString names[20];
    c.SaveNames._M_start = names; c.SaveNames._M_finish = names + 20; c.SaveNamesOffset = 0;
    CFrontendGameComponent_Test::Run(&c); bool ok = c.SaveNamesOffset == 8;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.SaveNamesOffset == 16;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.SaveNamesOffset == 0;
    c.SaveNames._M_finish = names + 8; c.SaveNamesOffset = 3;
    CFrontendGameComponent_Test::Run(&c); ok = ok && c.SaveNamesOffset == 3;
    if (ok) std::printf("004947b0_TEST PASS\n");
    return 0;
}