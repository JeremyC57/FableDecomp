// CWorldMap::CompileLoadableGlobalThingsToSingleFile(const CWideString& filename) -- for every used, proximity-loaded map: load its global things, append them as a "NEWMAP <n>" ... "ENDMAP" script block, then delete them; finally write the whole script to one file
// Leaf: ?CompileLoadableGlobalThingsToSingleFile@CWorldMap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FE030, 609 bytes, `ret 4` (FableWin.pdb
//   ?CompileLoadableGlobalThingsToSingleFile@CWorldMap@@IAEXABVCWideString@@@Z).
// Callees: CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, DisplayProgress
//   0x009E9F40 (__fastcall), malloc 0x00BFEA0E (list header node), the list dtor 0x004AB490 (catalog
//   CGuiControlTreePane::SortTreeRecursively, propagated), CWorldMap::LoadLoadableGlobalThingsForLevel 0x004FBF60,
//   GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&,
//   const char*) 0x0099F600, CCharString::operator+=(const CCharString&) 0x0099F0A0 / (const char*) 0x0099F100,
//   CWorld slot 3 (+0xc, the thing manager), CThingManager::SaveToScript 0x00522600 / DeleteAllThings 0x0051FBA0,
//   free 0x00BFEA14, CDiskFileWin32(path, 4, 2) 0x0099AD80 (inline dtor: Close 0x0099A920, ~CWideString 0x0099B510,
//   ~CBase 0x0099A300), CCharString::SaveToFile 0x0099E5F0.
//   Literals "Compiling global things" 0x01244620, "NEWMAP " 0x01244618, "\r\n" 0x012397F8, "\r\nENDMAP\r\n" 0x0124460C.
// Layout: retail CWorldMap with 12-byte STLport containers: World +0x08, Maps (CArray<CMapInfo>, 0x48-byte entries;
//   Used +0x24, LoadedOnPlayerProximity +0x28 per Ego_r.pdb) at +0x20; the thing manager's flag word at +0x80.
#include <stdlib.h>
struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    CCharString& operator+=(const char* text);         // 0x0099F100
    void SaveToFile(struct CAFile* file);              // 0x0099E5F0
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);  // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);  // 0x0099F600
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

struct CCharStringVector {                             // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    static __forceinline void _Destroy(CCharString* first, CCharString* last)
    {
        for (; first != last; ++first)
            first->~CCharString();
    }
    ~CCharStringVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
struct SListNode { SListNode* _M_next; SListNode* _M_prev; void* _M_data; };
struct CThingList {                                    // STLport list<> (12-byte nodes)
    SListNode* _M_node;
    CThingList()
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CThingList();                                     // 0x004AB490
};

struct CAFile;
struct CBase {
    virtual void v00();
    ~CBase();                                          // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CDiskFileWin32 : CBase {                        // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                              // 0x0099A920 (slot 2)
    long        AccessType;
    long        Position;
    long        Length;
    void*       WinFileHandle;
    bool        Opened;
    CWideString Pathname;                              // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};

struct CThingManager {
    unsigned char _pad[0x80];
    long SavingFlag;                                   // +0x80
    void SaveToScript(CCharString& script, long map, CCharStringVector& names);   // 0x00522600
    void DeleteAllThings();                            // 0x0051FBA0
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();          // slot 3 (+0xc)
};
struct CMapInfo {
    unsigned char _pad00[0x24];
    bool Used;                                         // +0x24
    bool Placed;                                       // +0x25
    bool Activated;                                    // +0x26
    bool Loading;                                      // +0x27
    bool LoadedOnPlayerProximity;                      // +0x28
    unsigned char _pad29[0x48 - 0x29];
};
struct CMapInfoArray {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CMapInfo& operator[](unsigned long i) { return _M_start[i]; }
};
struct CWorldMap {
    void* __vftable;
    long f4;
    CWorld* World;                                     // +0x08
    unsigned char _pad0c[0x20 - 0x0c];
    CMapInfoArray Maps;                                // +0x20
    void LoadLoadableGlobalThingsForLevel(long map);   // 0x004FBF60
    void CompileLoadableGlobalThingsToSingleFile(const CWideString& filename);
};

// R&D I-3 sweep: a same-TU auto_inline(off) body for the list dtor 0x004AB490 (frees its own nodes) makes the per-map
// list non-escaping, so the int->float temporary shares the list header's frame word (frame 0x48) as in retail.
#pragma auto_inline(off)
CThingList::~CThingList()
{
    SListNode* node = _M_node->_M_next;
    while (node != _M_node) {
        SListNode* next = node->_M_next;
        free(node);
        node = next;
    }
    free(_M_node);
}
#pragma auto_inline(on)
void CWorldMap::CompileLoadableGlobalThingsToSingleFile(const CWideString& filename)
{
    CCharString script;
    long oldFlag = World->GetThingManager()->SavingFlag;
    World->GetThingManager()->SavingFlag = 1;
    for (long i = 1; i < Maps.size(); ++i) {
        DisplayProgress("Compiling global things", false, (float)i / (float)(Maps.size() + 1), false);
        CMapInfo& map = Maps[i];
        if (map.Used && map.LoadedOnPlayerProximity) {
            CThingList things;
            LoadLoadableGlobalThingsForLevel(i);
            script += "NEWMAP " + GFIntToCharString(i) + "\r\n";
            CCharStringVector names;
            World->GetThingManager()->SaveToScript(script, i, names);
            script += "\r\nENDMAP\r\n";
            World->GetThingManager()->DeleteAllThings();
        }
    }
    World->GetThingManager()->SavingFlag = oldFlag;
    CDiskFileWin32 file(filename, FILE_ACCESS_WRITE, 2);
    script.SaveToFile((CAFile*)&file);
}