// CDefinitionManager::CompileString(const char* text, const CCharString& defType) -- parse a definitions text: compile every #definition_template block first, then rewind and compile every #definition block, keeping only blocks whose class matches defType (case-insensitively) unless defType is empty
// Leaf: ?CompileString@CDefinitionManager@@IAEXPBDABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AF1E0, 672 bytes (incl. int3 padding), thiscall `ret 8`. FableWin.pdb
//   `?CompileString@CDefinitionManager@@IAEXPBDABVCCharString@@@Z`; the only caller is the file loader 0x009AF480. The
//   catalog's "CDefinitionManager_LoadDefinitions" is descriptive.
// Callees: CBase() 0x0099A2D0 / ~CBase 0x0099A2E0, CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0, CBasicString<char>::operator== 0x004115A0, malloc 0x00BFEA0E (the comment list's sentinel),
//   CStringParser::Init(const char*, callback, const CCharString&, long) 0x009B9530, CStringParser::SkipPastWholeString
//   0x009B9C00 (catalog "CThingCreatureBase::SerialiseGameState" is a propagated label), CStringParser::ReadAsString
//   0x009BA6F0, CStringParser::SetStringPos 0x009B9940, _stricmp 0x00BFEBA8, CDefinitionManager::CompileDefinition
//   0x009AE7C0, the comment list's destructor 0x004046B0.
// Layout: engine/CDefinitionManager.h (ErrorCallback +0xB0, CurrentFilename +0xB4); retail CStringParser (0x28 bytes,
//   vtable 0x0122D874): vptr, String +4, BaseLineOffset +8, Filename +0xC, TempCharString +0x10, the 4-byte STLport
//   comment list +0x14, ErrorCallback +0x18, CursorPos +0x1C (12 bytes).
// Shape: the CStringParser(const char*, ...) constructor and its destructor are inline (CBase, the two strings, the
//   list's malloc'd self-linked sentinel, then Init); the marker temporaries are homed in the dead `text` argument slot;
//   the two loops are written out (retail repeats the block, it is not a shared helper). Two spellings matter: the
//   marker goes through the implicit const char* -> CCharString conversion (an explicit CCharString("...") temporary
//   takes the dead `text` slot from the loop's class-name string), and CBasicString<char>::operator== has a same-TU
//   noinline body so VC7.1 keeps defType's data pointer in ebx across the call (declaration-only reloads it).
// Fix (sweep 3, FINDINGS chunk 482): `this` is CDefinitionManager itself -- the engine header's struct is included under a
//   layout name and CDefinitionManager derives from it -- so the function and its CompileDefinition callee bind the PDB
//   symbols ?CompileString@CDefinitionManager@@IAE... / ?CompileDefinition@CDefinitionManager@@IAEXAAVCStringParser@@...
//   (protected, FableWin/Ego_r name sets) instead of the CDefinitionManager_Methods wrapper's. Same bytes.
#include <string.h>
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#pragma intrinsic(strcmp)

extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E
extern "C" int __cdecl _stricmp(const char* left, const char* right);   // 0x00BFEBA8
extern const char g_emptyLiteral_0129aaf4[];                         // 0x0129AAF4: ""

struct CBasicStringChar {
    char*         PData;                                // +0x00
    long          StringLength;                         // +0x04
    bool operator==(const char* text) const;            // 0x004115A0
};
// A same-TU stand-in for the out-of-line comparison (retail's body is at 0x004115A0): with its body visible VC7.1 knows the
// call only reads memory, so the filter string's data pointer stays in ebx across it, as retail's does.
#pragma auto_inline(off)
__declspec(noinline) bool CBasicStringChar::operator==(const char* text) const
{
    return strcmp(PData, text) == 0;
}
#pragma auto_inline(on)
struct CCharStringData {
    CBasicStringChar Data;                              // +0x00
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->Data.PData; }
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
typedef void (__fastcall* TParserErrorCallback)(const CCharString& message);

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
    list() : _M_node(0)
    {
        _Node* __sentinel = (_Node*)malloc(sizeof(_Node));
        __sentinel->_M_next = __sentinel;
        __sentinel->_M_prev = __sentinel;
        _M_node = __sentinel;
    }
    ~list();                                            // 0x004046B0 (CStringParserCommentBoundaries)
};
} // namespace std

class CStringParser : public CBase                      // vtable 0x0122D874, 0x28 bytes
{
public:
    const char*   String;                               // +0x04
    unsigned long BaseLineOffset;                       // +0x08
    CCharString   Filename;                             // +0x0C
    CCharString   TempCharString;                       // +0x10
    std::list<CStringParserCommentBoundaries> Comments; // +0x14
    TParserErrorCallback ErrorCallback;                 // +0x18
    unsigned long CursorLine;                           // +0x1C CursorPos (12 bytes)
    unsigned long StringPos;                            // +0x20
    unsigned long CursorColumn;                         // +0x24
    CStringParser(const char* text, TParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
    {
        Init(text, errorCallback, filename, baseLineOffset);
    }
    void Init(const char* text, TParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530
    bool SkipPastWholeString(const CCharString& text);  // 0x009B9C00
    CCharString ReadAsString();                         // 0x009BA6F0
    void SetStringPos(unsigned long position);          // 0x009B9940
};

class CDefinitionManager : public CDefinitionManagerLayout   // the engine header's layout, the PDB's methods
{
public:
    const CCharString& CurrentFilename() const { return *(const CCharString*)&CurrentFilename_PStringData; }
protected:
    void CompileDefinition(CStringParser& parser, const CCharString& defType, const CCharString& defName);   // 0x009AE7C0
    void CompileString(const char* text, const CCharString& defType);
};

void CDefinitionManager::CompileString(const char* text, const CCharString& defType)
{
    CStringParser parser(text, (TParserErrorCallback)ErrorCallback, CurrentFilename(), 0);
    while (parser.SkipPastWholeString("#definition_template")) {
        CCharString type = parser.ReadAsString();
        CCharString name = parser.ReadAsString();
        if (defType == "" || _stricmp(defType.GetString(), type.GetString()) == 0)
            CompileDefinition(parser, type, name);
    }
    parser.SetStringPos(0);
    while (parser.SkipPastWholeString("#definition")) {
        CCharString type = parser.ReadAsString();
        CCharString name = parser.ReadAsString();
        if (defType == "" || _stricmp(defType.GetString(), type.GetString()) == 0)
            CompileDefinition(parser, type, name);
    }
}