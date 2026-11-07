// CWorldMap::GetLevelFilesFromWorldFile(const CWideString& worldFile, std::vector<CWideString>& levelFiles) -- read the world file into a zeroed buffer and parse it: "NewMap <n>" starts map n, "LevelName = "<file>"" names its level file (under the levels directory), "EndMap" stores that name as levelFiles[n]
// Leaf: ?GetLevelFilesFromWorldFile@CWorldMap@@IAEXABVCWideString@@AAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
extern "C" int __cdecl strcmp(const char* a, const char* b);
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
#pragma intrinsic(memset, strcmp)

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    static _Tp* allocate(size_type n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    static _Tp* fill(_Tp* first, size_type n) { memset(first, 0, n * sizeof(_Tp)); return first + n; }
    explicit vector(size_type n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = fill(_M_start, n);
    }
    ~vector() { if (_M_start) free(_M_start); }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type n) { return _M_start[n]; }
    void resize(size_type n);                           // 0x00412B70 (vector<CWideString>)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;
    ~list();                                            // 0x004046B0 (list<CStringParserCommentBoundaries>)
};
} // namespace std

class CCharString;
class CWideString { public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ToCharString() const;                   // 0x0099B2C0
};
class CCharString;
template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570
public:
    const T* PeekData() const { return PData; }
    bool operator==(const T* text) const;               // 0x004115A0
};
// Row-local template bodies (the landed 0x00837E40 precedent): with them in the TU VC7.1 knows the out-of-line compare
// leaves the token alone and keeps its PStringData in esi across the compare chain.
template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a, const T* b)
{
    while (*a || *b) {
        if (*a < *b) return -1;
        if (*a > *b) return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData, text) == 0; }
class CCharStringData { public:
    CBasicString<char> Data;
    long NoRefs;
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CWideString ToWideString() const;                   // 0x0099E480
    const char* GetAsCharArray() const { return PStringData == 0 ? &NullChar : PStringData->Data.PeekData(); }
    static const char NullChar;                         // 0x0129AAF4
    bool operator==(const char* text) const { if (!PStringData) { if (strcmp(text, "") == 0) return true; return false; } return PStringData->Data == text; }
    bool EqualsCaseInsensitive(const CCharString& other) const { return _stricmp(GetAsCharArray(), other.GetAsCharArray()) == 0; }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

namespace NResourceDirectoryNames { CWideString __fastcall GetLevelDir(); }   // 0x0041A060
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString& text, float fraction, bool a, bool b); }   // 0x009E9F40

class CExceptionBase { public: CExceptionBase(const char*) { } };
class CGenericException : public CExceptionBase { public: CGenericException(const char* text) : CExceptionBase(text) { } };

class CBaseClass {
public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable { public:      // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);
    virtual void Close();                               // 0x0099A920 (slot 2)
    virtual void Read(void* buffer, long length, CASuspendableProcess* process);   // 0x0099A960
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};

enum EParsedItemType { PARSED_ITEM_NULL = 0, PARSED_ITEM_SYMBOL = 1, PARSED_ITEM_STRING = 2, PARSED_ITEM_INTEGER = 3, PARSED_ITEM_FLOAT = 4 };
class CParsedItem { public:
    EParsedItemType Type;
    long            Integer;
    CCharString     String;
};
class CStringParserCommentBoundaries;
struct CCommentListNode { CCommentListNode* Next; CCommentListNode* Prev; long Start; long End; };
class CStringParser : public CBaseClass { public:     // retail 0x28 bytes, vtable 0x0122D874
    const char*   String;                               // +0x04
    unsigned long BaseLineOffset;                       // +0x08
    CCharString   Filename;                             // +0x0c
    CCharString   TempCharString;                       // +0x10
    std::list<CStringParserCommentBoundaries> Comments; // +0x14
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    long          CursorPos[3];                         // +0x1c
    CStringParser(const char* text, void (__fastcall* errorCallback)(const CCharString&) = 0, const CCharString& filename = CCharString(""), long baseLineOffset = 0)
    {
        CCommentListNode* node;
        Comments._M_node = 0;
        node = (CCommentListNode*)malloc(sizeof(CCommentListNode));
        node->Next = node;
        node->Prev = node;
        Comments._M_node = node;
        Init(text, errorCallback, filename, baseLineOffset);
    }
    void Init(const char* text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset);   // 0x009B9530
    bool NextItemExists();                              // 0x009BA4F0
    EParsedItemType PeekNextItemType();                 // 0x009BA900
    bool ReadNextItem(CParsedItem& item);               // 0x009BA080
    long ReadAsInteger();                               // 0x009BA540
    CCharString ReadAsString();                         // 0x009BA6F0
    CCharString ReadAsSymbol();                         // 0x009BA790
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
};

class CWorldMap {
protected:
    void GetLevelFilesFromWorldFile(const CWideString& worldFile, std::vector<CWideString>& levelFiles);
};

void CWorldMap::GetLevelFilesFromWorldFile(const CWideString& worldFile, std::vector<CWideString>& levelFiles)
{
    CDiskFileWin32 file(worldFile, FILE_ACCESS_READ, 2);
    long length = file.Length;
    std::vector<char> buffer(length + 1);
    file.Read(&buffer[0], length, 0);
    buffer[file.Length] = 0;
    CParsedItem item;
    CStringParser parser(&buffer[0]);
    CCharString levelName;
    long map = 0;
    while (parser.NextItemExists()) {
        if (parser.PeekNextItemType() != PARSED_ITEM_STRING) {
            parser.ReadNextItem(item);
        } else {
            CCharString token = parser.ReadAsString();
            if (token == "NewMap") {
                NProgressDisplay::DisplayProgress("Getting level files from world map", -1.0f, false, false);
                map = parser.ReadAsInteger();
            } else if (token == "EndMap") {
                if (map != 0) {
                    if (levelFiles.size() < (unsigned int)(map + 1))
                        levelFiles.resize(map + 1);
                    levelFiles[map] = levelName.ToWideString();
                }
            } else if (token == "LoadedOnPlayerProximity") {
                CCharString value = parser.ReadAsString();
                bool loadedOnPlayerProximity = value.EqualsCaseInsensitive("TRUE");
            } else if (token == "LevelName") {
                if (!(parser.ReadAsSymbol() == "\""))
                    throw CGenericException("GetLevelFilesFromWorldFile");
                levelName = NResourceDirectoryNames::GetLevelDir().ToCharString() + parser.ReadAsStringUntilString("\"");
            }
        }
    }
}
