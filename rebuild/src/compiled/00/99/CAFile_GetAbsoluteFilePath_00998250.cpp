// CAFile::GetAbsoluteFilePath(const CCharString& basePath, const CCharString& relativePath) -- resolve a backslash path relative to the folder of basePath: drop basePath's last component, then walk relativePath's folders ("..": drop one more, otherwise append) and finally append its file name
// Leaf: ?GetAbsoluteFilePath@CAFile@@SI?AVCCharString@@
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998250, 800 bytes (incl. int3 padding). FableWin.pdb
//   `?GetAbsoluteFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z`: a public static `__fastcall` returning CCharString (hidden
//   result in ecx, basePath in edx, relativePath on the stack), `ret 4`. Callers: the CWideString overload 0x00998C30.
// Callees: CBasicString<char>::operator==(const char*) const 0x004115A0, CCharString copy ctor 0x0099EC30,
//   CCharString::Truncate(unsigned long) 0x0099F2A0, CCharString(const char*, long = -1) 0x0099EBF0, CStringParser ctor
//   0x00404720, ~CCharString 0x0099EAE0, CCharString() 0x0099E4B0, CStringParser::ReadAsStringUntilString(const
//   CCharString&, CParsedItem&) 0x009BA110, CParsedItem::GetString 0x009B9880, operator+(const char*, const
//   CCharString&) 0x0099F690, CCharString::operator+= 0x0099F0A0, CStringParser::SkipPastString 0x009B9BA0,
//   GetSubString 0x0099EC70, CCharString::operator= 0x0099EFB0, free 0x00BFEA14, CBase::~CBase 0x0099A2E0.
//   Data: "" 0x0122D70E, "\\" 0x0124C530, ".." 0x0129A7B8, the empty text 0x0129AAF4.
// Shape: the working copy of basePath is homed in relativePath's dead argument slot; both "drop the last component"
//   scans are the same backwards loop; the parser's STLport comment-list destructor is inlined here (node loop + sentinel
//   free), unlike IsFilePathSyntaxValid 0x00998A30 where VC7.1's inline budget leaves it a call to 0x004046B0. The "\\"
//   arguments are implicit conversions: VC7.1 builds such a temporary before pushing any argument (retail order), while
//   an explicit CCharString("\\") is built in argument order, after &item is pushed.
#include <string.h>
#pragma intrinsic(strcmp)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
extern const char g_emptyLiteral_0129aaf4[];            // 0x0129AAF4: ""

template <class T> class CBasicString { public:                               // CBasicString<char>
    char*         PData;                                // +0x00
    long          StringLength;                         // +0x04
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;                              // +0x00
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    void Truncate(unsigned long length);                // 0x0099F2A0
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
    const char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->Data.PData; }
    char operator[](long index) const { return GetString()[index]; }
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

class CBase
{
public:
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};

class CStringParserCommentBoundaries
{
public:
    long Start;                                         // +0x00
    long End;                                           // +0x04
};
namespace std {
class _List_node_base
{
public:
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> class _List_node : public _List_node_base
{
public:
    _Tp _M_data;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class list
{
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;                           // +0x00 the sentinel
    void clear()
    {
        _Node* __cur = (_Node*)_M_node->_M_next;
        while (__cur != _M_node) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
} // namespace std

class CParsedItem                                       // 12 bytes
{
public:
    long        Type;                                   // +0x00 EParsedItemType
    long        Value;                                  // +0x04 (union)
    CCharString String;                                 // +0x08
    const CCharString& GetString() const;               // 0x009B9880
};

class CStringParser : public CBase                      // vtable 0x0122D874, 0x28 bytes
{
public:
    const char*   String;                               // +0x04
    unsigned long BaseLineOffset;                       // +0x08
    CCharString   Filename;                             // +0x0C
    CCharString   TempCharString;                       // +0x10
    std::list<CStringParserCommentBoundaries> Comments; // +0x14
    void (*ErrorCallback)(CCharString*);                // +0x18
    unsigned long CursorLine;                           // +0x1C CursorPos (12 bytes)
    unsigned long StringPos;                            // +0x20
    unsigned long CursorColumn;                         // +0x24
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename, long baseLineOffset);   // 0x00404720
    bool ReadAsStringUntilString(const CCharString& until, CParsedItem& item);   // 0x009BA110
    bool SkipPastString(const CCharString& text);       // 0x009B9BA0
};

class CAFile
{
public:
    static CCharString __fastcall GetAbsoluteFilePath(const CCharString& basePath, const CCharString& relativePath);
};

CCharString __fastcall CAFile::GetAbsoluteFilePath(const CCharString& basePath, const CCharString& relativePath)
{
    if (relativePath == "")
        return basePath;
    CCharString path(basePath);
    for (long i = path.GetLength() - 1; i >= 0; --i) {
        if (path[i] == '\\') {
            path.Truncate(i);
            break;
        }
    }
    CStringParser parser(relativePath, 0, CCharString(""), 0);
    CParsedItem item;
    while (parser.ReadAsStringUntilString("\\", item)) {
        if (item.GetString() == "..") {
            for (long i = path.GetLength() - 1; i >= 0; --i) {
                if (path[i] == '\\') {
                    path.Truncate(i);
                    break;
                }
            }
        } else
            path += "\\" + item.GetString();
        parser.SkipPastString("\\");
    }
    CCharString fileName(relativePath);
    for (long i = relativePath.GetLength() - 1; i >= 0; --i) {
        if (relativePath[i] == '\\') {
            fileName = relativePath.GetSubString(i + 1, relativePath.GetLength());
            break;
        }
    }
    path += "\\" + fileName;
    return path;
}