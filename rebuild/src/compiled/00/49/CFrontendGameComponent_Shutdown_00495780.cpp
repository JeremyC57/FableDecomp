// CFrontendGameComponent::Shutdown() -- release the frontend font and temporary graphics bank and empty the save and world name lists
// Leaf: ?Shutdown@CFrontendGameComponent@@QAEXXZ (Ego_r.pdb AAEXXZ, private; modelled public)
// Retail: Fable.exe @ 0x00495780, 162 bytes, plain `ret`. Catalog label C3DMeshStripifier::~C3DMeshStripifier is wrong: the
// three callers 0x00496D40 / 0x00496DB7 / 0x00496E78 are the exit paths of CFrontendGameComponent::Run (vtable 0x01238768
// slot 2, 0x00496CF0) before it hands over to the next component, and the offsets are CFrontendGameComponent members.
// Name from Ego_r.pdb ?Shutdown@CFrontendGameComponent@@AAEXXZ (the only private void() teardown in the class's symbol list).
// Body: MainFont.Clear() then PTempGraphics.Clear() (inline CCountedPointer release: --RefCount, at zero DeleteFunc(Data)
// and operator delete 0x00BFE9BC; Info = 0, Object = 0), then SaveNames.clear() and WorldNames.clear() (each the
// out-of-line vector<CWideString>::erase(begin, end) 0x00412450).
// Layout: Ego_r.pdb CFrontendGameComponent PTempGraphics +0x1dfc, MainFont +0x1e04, ProfileNames +0x1e0c; retail STLport
// vectors are 12 bytes (Ego_r Dinkumware 16), so SaveNames is +0x1e1c and WorldNames +0x1e30 (both pushed as ecx).
// Opt: /O2 (16-aligned entry).
class CWideString { public: void* PStringData; };
class CGraphicDataBank;
class CFontBank;
class CCPPointerInfo { public:
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