#include <cstdio>
class CWideString { public: void* PStringData; };
class CGraphicDataBank;
class CFontBank;
struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Object;
    CCPPointerInfo* Info;
    void Clear()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Object = 0;
    }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                         // 0x00412450 for CWideString
    void clear() { erase(begin(), end()); }
};
}
class CFrontendGameComponent {
public:
    unsigned char                     _pad0000[0x1dfc];
    CCountedPointer<CGraphicDataBank> PTempGraphics;     // +0x1dfc
    CCountedPointer<const CFontBank>  MainFont;          // +0x1e04
    std::vector<CWideString>          ProfileNames;      // +0x1e0c
    long                              ProfileNamesOffset; // +0x1e18
    std::vector<CWideString>          SaveNames;         // +0x1e1c
    long                              SaveNamesOffset;   // +0x1e28
    CWideString                       SaveToLoad;        // +0x1e2c
    std::vector<CWideString>          WorldNames;        // +0x1e30
    void Shutdown();
};
void CFrontendGameComponent::Shutdown()
{
    MainFont.Clear();
    PTempGraphics.Clear();
    SaveNames.clear();
    WorldNames.clear();
}

static int g_deleted[2], g_erase; static void* g_eraseThis[4]; static CWideString* g_eraseArgs[8];
static void __fastcall DelA(void* d) { ++g_deleted[0]; }
static void __fastcall DelB(void* d) { ++g_deleted[1]; }
template <class T> T* std::vector<T>::erase(T* first, T* last) { g_eraseThis[g_erase] = this; g_eraseArgs[2 * g_erase] = first; g_eraseArgs[2 * g_erase + 1] = last; ++g_erase; return first; }
int main()
{
    static unsigned char raw[sizeof(CFrontendGameComponent)]; CFrontendGameComponent& c = *(CFrontendGameComponent*)raw;
    CCPPointerInfo* a = new CCPPointerInfo; a->RefCount = 1; a->DeleteFunc = DelA; a->Data = (void*)1;
    CCPPointerInfo b = { 2, DelB, (void*)2 };
    c.MainFont.Object = (const CFontBank*)8; c.MainFont.Info = a;
    c.PTempGraphics.Object = (CGraphicDataBank*)9; c.PTempGraphics.Info = &b;
    static CWideString s[4];
    c.SaveNames._M_start = s; c.SaveNames._M_finish = s + 2; c.WorldNames._M_start = s + 2; c.WorldNames._M_finish = s + 3;
    c.Shutdown();
    bool ok = g_deleted[0] == 1 && g_deleted[1] == 0 && b.RefCount == 1 && c.MainFont.Object == 0 && c.MainFont.Info == 0
           && c.PTempGraphics.Object == 0 && c.PTempGraphics.Info == 0 && g_erase == 2
           && g_eraseThis[0] == &c.SaveNames && g_eraseArgs[0] == s && g_eraseArgs[1] == s + 2
           && g_eraseThis[1] == &c.WorldNames && g_eraseArgs[2] == s + 2 && g_eraseArgs[3] == s + 3
           && (char*)&c.WorldNames - (char*)&c == 0x1e30;
    c.Shutdown();   // both pointers already empty
    ok = ok && g_deleted[0] == 1 && g_erase == 4;
    std::printf(ok ? "00495780_TEST PASS\n" : "00495780_TEST FAIL\n");
    return ok ? 0 : 1;
}