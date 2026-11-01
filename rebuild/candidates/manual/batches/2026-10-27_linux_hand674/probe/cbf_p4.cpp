// PlayCutsceneSound(std::map<int, CCharString>& parameters, int key) -- run a cutscene sound command: if the macro parameter `key` holds text, parse it line by line and execute each Play2DSound / PlayMusic / StopMusic item through the game-script interface
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?PlayCutsceneSound@@YIXAAV?$map@HVCCharString@@U?$less@H@std@@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@3@@std@@H@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF7FE, 480 bytes, `__fastcall` (map in ecx, key in edx), plain `ret`,
//   frame pointer. The start is genuine: the previous function ends with `leave ; ret` at 0x00CBF7FD and the TU packs its
//   functions with no padding (size-optimised, like its landed neighbour CSFindThing 0x00CBF9DE); the 69 callers in the
//   cutscene macro code 0x00CC070E .. 0x00CC56AB all `call 0x00CBF7FE`.
// Identity (proven): Ego_r.pdb ?PlayCutsceneSound@@YIXAAV?$map@HVCCharString@@U?$less@H@std@@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@3@@std@@H@Z
//   (00CA091F), in the same cutscene-script TU as InitializeMusicTypes (00C9F954; retail 0x00CBEB1A landed) whose symbol map
//   0x0143E900 this body reads; the three literals are "Play2DSound" 0x012C1910, "PlayMusic" 0x012C1904, "StopMusic"
//   0x012C18F8. The catalog label is right (module _global, __fastcall).
// Callees: map<int, CCharString> _M_find 0x00CD26EA (node pointer in eax), CCharString() 0x0099E4B0, CCharString(const
//   CCharString&) 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, CCharString(const char*, long)
//   0x0099EBF0, CStringParser ctor 0x00404720 / NextItemExists 0x009BA4F0 / ReadAsString 0x009BA6F0 (by value) /
//   SkipPastNewline 0x009B9D70 / ~CStringParser 0x004047A0, Compare 0x00411570 (the inlined CCharString::operator==),
//   CSymbolMap::GetSymbolSLONG 0x009E5120, CGameScriptInterface slots 692 Play2DSound (+0xAD0), 696 OverrideMusic (+0xAE0),
//   697 StopOverrideMusic (+0xAE4) through g_pGameScriptInterface 0x0143E8F8.
// Shape: the CCharString::operator==(const char*) of the landed CSFindThing (null block: the strcmp intrinsic's `repe
//   cmpsb` against the pooled "" kept in ebx; else the inlined block compare whose result byte is homed at [ebp-1]); three
//   sequential ifs (not else-if) over one `command` string per parsed item; IsEmpty is the inline GetLength() == 0.
// Data: the cutscene-music flag byte 0x0143E8FD (set after OverrideMusic; the cutscene end 0x00CD1E0F tests it and calls
//   StopOverrideMusic) -- descriptive name.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    __forceinline bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                 // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length);             // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other); // 0x0099EFB0
    __forceinline long GetLength() const { return PStringData == 0 ? 0 : (long)PStringData->Data.StringLength; }   // Ego_r ?GetLength@CCharString@@QBEJXZ
    __forceinline bool IsEmpty() const { return GetLength() == 0; }                                                 // FableWin ?IsEmpty@CCharString@@QBE_NXZ
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    CCharStringData* PStringData;
};

namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _Node*       _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
    _Node* _M_find(const _Key& __k) const;              // 0x00CD26EA for map<int, CCharString>
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::_Node _Node;
    struct iterator {
        _Node* _M_node;
        iterator(_Node* __x) : _M_node(__x) { }
        bool operator!=(const iterator& __y) const { return _M_node != __y._M_node; }
        value_type* operator->() const { return &_M_node->_M_value_field; }
    };
    iterator find(const _Key& __k) { return iterator(_M_t._M_find(__k)); }
    iterator end() { return iterator(_M_t._M_header); }
    _Rep_type _M_t;
};
}

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0)
class CStringParserCommentBoundaries;
class CStringParser : public CBaseClass {               // retail 0x28 bytes (layout from the landed CScriptBase::LoadGameState 0x00CB8690)
public:
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename,
                  long baseLineOffset);                 // 0x00404720 (Ego_r ??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z)
    virtual ~CStringParser();                           // 0x004047A0 (Ego_r ??1CStringParser@@UAE@XZ)
    bool NextItemExists();                              // 0x009BA4F0
    CCharString ReadAsString();                         // 0x009BA6F0
    long SkipPastNewline();                             // 0x009B9D70
    char*         String;                               // +0x04
    unsigned long BaseLineOffset;                       // +0x08
    CCharString   Filename;                             // +0x0c
    CCharString   TempCharString;                       // +0x10
    void*         Comments;                             // +0x14 std::list<CStringParserCommentBoundaries>
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    long          CursorPos[3];                         // +0x1c CCursorPos
};

class CSymbolMap {
public:
    long GetSymbolSLONG(const CCharString& symbol) const;   // 0x009E5120
};
extern CSymbolMap* g_pMusicTypeSymbolMap;               // 0x0143E900

#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
extern bool g_bCutsceneMusicOverridden_0143e8fd;        // 0x0143E8FD (descriptive)

void __fastcall PlayCutsceneSound(std::map<int, CCharString>& parameters, int key)
{
    std::map<int, CCharString>::iterator it = parameters.find((int)(long)key);
    CCharString soundCommand;
    if (it != parameters.end() && !it->second.IsEmpty())
        soundCommand = CCharString(it->second);
    if (!soundCommand.IsEmpty()) {
        CStringParser parser(soundCommand, 0, CCharString("", -1), 0);
        while (parser.NextItemExists()) {
            CCharString command = parser.ReadAsString();
            if (command == "Play2DSound") {
                CCharString soundName = parser.ReadAsString();
                g_pGameScriptInterface->Play2DSound(soundName);
            }
            if (command == "PlayMusic") {
                CCharString musicName = parser.ReadAsString();
                long musicType = g_pMusicTypeSymbolMap->GetSymbolSLONG(musicName);
                if (musicType != -1) {
                    g_pGameScriptInterface->OverrideMusic((EMusicSetType)musicType, true, false);
                    g_bCutsceneMusicOverridden_0143e8fd = true;
                }
            }
            if (command == "StopMusic")
                g_pGameScriptInterface->StopOverrideMusic(true);
            parser.SkipPastNewline();
        }
    }
}
