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
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
class CAFile;
class CASuspendableProcess;
class CThing;
class CCharString { public:
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
template<class T> class CCountedPointer { public:
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
class CBaseClass { public:
    virtual void v00();
    virtual ~CBaseClass();                                         // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                           // 0x004046B0
};
class CStringParser : public CBaseClass { public:                        // retail 0x28 bytes
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
class CThingManager { public:
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