// Behaviour test for 0x00521AE0 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingManager::LoadFromFile(CCountedPointer<CAFile> file, long version, set<CCharString> names, vector<CThing*>& things,
// CASuspendableProcess* process, int mode) -- reads the whole file into a string, parses its optional "Version" header
// and loads the entity script; mode 3 follows the manager's current load kind (1 -> 1, 2 -> 0)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00521AE0, `ret 0x24` (CCountedPointer is 8 bytes, the set 12).
// Callees: NProgressDisplay::DisplayProgress 0x009E9F40 (__fastcall, the bool in dl), CCharString(const char*, long)
//   0x0099EBF0 / CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0 / CCharString::LoadFromFile 0x0099F2E0, CStringParser
//   ctor 0x00404720 + inline dtor (~list 0x004046B0, CBaseClass::~CBaseClass 0x0099A2E0), SkipPastNextItemIfIs
//   0x009B9E00, ReadAsFloat 0x009BA650, __ftol2, CThingManager::LoadEntitiesFromScript 0x00520D00, set _M_erase
//   0x004ABA70, operator delete 0x00BFE9BC, free 0x00BFEA14.
// Layout: CThingManager +0x80 load kind, +0x9a / +0x9b loading flags (descriptive); CCountedPointer {object, data}
//   with data {count, __fastcall deleter, object}.
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
struct CAFile;
struct CASuspendableProcess;
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
    void LoadFromFile(CAFile* file, CASuspendableProcess* process, long size);   // 0x0099F2E0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
struct CCountedData {
    long Count;                                            // +0x00
    void (__fastcall* Deleter)(void* object);              // +0x04
    void* Object;                                          // +0x08
};
template<class T> struct CCountedPointer {
    T*            Object;
    CCountedData* Data;
    ~CCountedPointer()
    {
        if (Data && --Data->Count == 0) {
            Data->Deleter(Data->Object);
            delete Data;
        }
    }
};
struct SSetNode {
    long        _M_color;
    SSetNode*   _M_parent;
    SSetNode*   _M_left;
    SSetNode*   _M_right;
    CCharString _M_value;
};
struct CCharStringSet {                                    // STLport set<CCharString>
    SSetNode* _M_header;
    long      _M_node_count;
    long      _M_key_compare;
    void _M_erase(SSetNode* node);                         // 0x004ABA70
    ~CCharStringSet()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
        if (_M_header)
            free(_M_header);
    }
};
#pragma auto_inline(off)
__declspec(noinline) void CCharStringSet::_M_erase(SSetNode* node)
{
    while (node != 0) {
        _M_erase(node->_M_right);
        SSetNode* left = node->_M_left;
        node->_M_value.~CCharString();
        free(node);
        node = left;
    }
}
#pragma auto_inline(on)
struct CThingVector { CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage; };
struct CBaseClass {
    virtual void v00() { }
    ~CBaseClass();                                         // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                           // 0x004046B0
};
struct CStringParser : CBaseClass {                        // retail 0x28 bytes
    char*                    String;                       // +0x04
    unsigned long            BaseLineOffset;               // +0x08
    CCharString              Filename;                     // +0x0c
    CCharString              TempCharString;               // +0x10
    CStringParserCommentList Comments;                     // +0x14
    void (*ErrorCallback)(CCharString*);                   // +0x18
    long                     CursorPos[3];                 // +0x1c
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);                    // 0x00404720
    bool SkipPastNextItemIfIs(const CCharString& item);    // 0x009B9E00
    float ReadAsFloat();                                   // 0x009BA650
};
extern const char g_emptyString_0122d70e[];               // 0x0122D70E ""
struct CThingManager {
    unsigned char _pad00[0x80];
    long          LoadKind;                                // +0x80
    unsigned char _pad84[0x9a - 0x84];
    bool          LoadingFromFile;                         // +0x9a
    bool          Loading;                                 // +0x9b
    void LoadEntitiesFromScript(const CCharString& text, long fileVersion, long version, const CCharStringSet& names,
                                CASuspendableProcess* process, long mode, CThingVector& things);   // 0x00520D00
    void LoadFromFile(CCountedPointer<CAFile> file, long version, CCharStringSet names, CThingVector& things,
                      CASuspendableProcess* process, int mode);
};
void CThingManager::LoadFromFile(CCountedPointer<CAFile> file, long version, CCharStringSet names, CThingVector& things,
                                 CASuspendableProcess* process, int mode)
{
    Loading = true;
    LoadingFromFile = false;
    long loadMode = mode;
    if (mode == 3) {
        switch (LoadKind) {
        case 1:
            loadMode = 1;
            break;
        case 2:
            loadMode = 0;
            break;
        }
    }
    NProgressDisplay::DisplayProgress("Thing Manager: Load From File", -1.0f, false, false);
    CCharString text;
    text.LoadFromFile(file.Object, process, -1);
    CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    NProgressDisplay::DisplayProgress("Thing Manager: Load From File End", -1.0f, false, false);
    long fileVersion = 0;
    if (parser.SkipPastNextItemIfIs("Version")) {
        fileVersion = (long)parser.ReadAsFloat();
        parser.SkipPastNextItemIfIs(";");
    }
    LoadEntitiesFromScript(text, fileVersion, version, names, process, loadMode, things);
    Loading = false;
}

static int g_strings = 0;
CCharString::CCharString() { PStringData = (void*)""; ++g_strings; }
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; (void)length; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
static CASuspendableProcess* g_loadProcess = 0; static long g_loadSize = 0;
void CCharString::LoadFromFile(CAFile* file, CASuspendableProcess* process, long size)
{
    PStringData = (void*)file; g_loadProcess = process; g_loadSize = size;
}
static int g_progress = 0; static float g_progressValue = 0.0f;
void __fastcall NProgressDisplay::DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    if (std::strncmp((const char*)text.PStringData, "Thing Manager: Load From File", 29) == 0) ++g_progress;
    g_progressValue = progress; if (a || b) g_progress += 100;
}
CBaseClass::~CBaseClass() { }
static int g_listDtor = 0;
CStringParserCommentList::~CStringParserCommentList() { ++g_listDtor; }
static const char* g_cursor = 0;
CStringParser::CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                             long baseLineOffset)
    : Filename(""), TempCharString("")
{
    String = (char*)text.PStringData; g_cursor = String; ErrorCallback = errorCallback; BaseLineOffset = baseLineOffset;
    (void)filename;
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item)
{
    const char* s = (const char*)item.PStringData; unsigned int n = (unsigned int)std::strlen(s);
    if (std::strncmp(g_cursor, s, n) == 0) { g_cursor += n; return true; }
    return false;
}
float CStringParser::ReadAsFloat() { float f = (float)std::atof(g_cursor); while (*g_cursor && *g_cursor != ';') ++g_cursor; return f; }
const char g_emptyString_0122d70e[] = "";
static long g_fileVersion = -1, g_version = -1, g_mode = -1; static const CCharStringSet* g_names = 0;
static CThingVector* g_things = 0; static const char* g_text = 0; static int g_loadingDuring = -1;
void CThingManager::LoadEntitiesFromScript(const CCharString& text, long fileVersion, long version, const CCharStringSet& names,
                                           CASuspendableProcess* process, long mode, CThingVector& things)
{
    g_text = (const char*)text.PStringData; g_fileVersion = fileVersion; g_version = version; g_names = &names;
    g_mode = mode; g_things = &things; g_loadingDuring = Loading ? 1 : 0; (void)process;
}
static int g_deleted = 0;
static void __fastcall DeleteFile(void* object) { (void)object; ++g_deleted; }
static void FillSet(CCharStringSet& s, int n)
{
    s._M_header = (SSetNode*)std::malloc(sizeof(SSetNode));
    s._M_header->_M_color = 0; s._M_header->_M_parent = 0; s._M_header->_M_left = s._M_header; s._M_header->_M_right = s._M_header;
    s._M_node_count = 0; s._M_key_compare = 0;
    if (n) {
        SSetNode* node = (SSetNode*)std::malloc(sizeof(SSetNode));
        new (&node->_M_value) CCharString("a");
        node->_M_color = 1; node->_M_parent = s._M_header; node->_M_left = 0; node->_M_right = 0;
        s._M_header->_M_parent = node; s._M_header->_M_left = node; s._M_header->_M_right = node; s._M_node_count = 1;
    }
}
static void Load(CThingManager& mgr, CCountedPointer<CAFile>& file, long version, int n, CThingVector& things,
                 CASuspendableProcess* process, int mode)
{
    CCharStringSet names; FillSet(names, n);
    mgr.LoadFromFile(file, version, names, things, process, mode);
    names._M_header = 0; names._M_node_count = 0;
}

int main()
{
    static CThingManager mgr; std::memset(&mgr, 0, sizeof(mgr));
    CThingVector things; things._M_start = things._M_finish = things._M_end_of_storage = 0;
    CASuspendableProcess* process = (CASuspendableProcess*)&things;
    {
        CCountedData* data = new CCountedData; data->Count = 1; data->Deleter = DeleteFile; data->Object = 0;
        CCountedPointer<CAFile> file; file.Object = (CAFile*)"Version 7;entities"; file.Data = data;
        mgr.LoadKind = 1;
        Load(mgr, file, 12, 1, things, process, 3);
        file.Data = 0;
    }
    if (g_deleted != 1 || g_fileVersion != 7 || g_version != 12 || g_mode != 1 || g_things != &things) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
    if (g_loadProcess != process || g_loadSize != -1 || g_progress != 2 || g_progressValue != -1.0f) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
    if (mgr.Loading || mgr.LoadingFromFile || g_loadingDuring != 1 || g_listDtor != 1) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_text, "Version 7;entities") != 0 || g_cursor[0] != 'e') { std::printf("00521ae0_TEST FAIL\n"); return 1; }
    {
        CCountedData* data = new CCountedData; data->Count = 2; data->Deleter = DeleteFile; data->Object = 0;
        CCountedPointer<CAFile> file; file.Object = (CAFile*)"things"; file.Data = data;
        mgr.LoadKind = 2;
        Load(mgr, file, 3, 0, things, process, 3);
        if (data->Count != 1 || g_deleted != 1) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
        ++data->Count;
        Load(mgr, file, 3, 0, things, process, 1);
        if (g_mode != 1) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
        mgr.LoadKind = 5;
        ++data->Count;
        Load(mgr, file, 3, 1, things, process, 3);
        if (g_mode != 3) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
        file.Data = 0;
        delete data;
    }
    if (g_fileVersion != 0 || g_progress != 8 || g_strings != 0) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
    mgr.LoadKind = 2;
    {
        CCountedPointer<CAFile> file; file.Object = (CAFile*)"x"; file.Data = 0;
        Load(mgr, file, 3, 0, things, process, 3);
    }
    if (g_mode != 0) { std::printf("00521ae0_TEST FAIL\n"); return 1; }
    std::printf("00521ae0_TEST PASS\n");
    return 0;
}