// NUISystem::CKeyRedefiner::GetActionText(CWideString& text, const CCharString& symbol) const -- the redefiner's action caption: for the movement action the direction's game text (forward / back / left / right by sub-type, the tag itself when the bank has no text), otherwise the symbol's game text (or the symbol) followed by the sub-type number
// Leaf: ?GetActionText@CKeyRedefiner@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00558170, 945 bytes of code + the four-entry case table (976 bytes in all),
//   thiscall `ret 8`. One caller (0x00558736, NUISystem::CKeyRedefiner::SetAction).
// Identity: Ego_r.pdb ?GetActionText@CKeyRedefiner@NUISystem@@ABEXAAVCWideString@@ABVCCharString@@@Z (private const);
//   the catalog's "GetMovementActionText" is descriptive.
// Callees: NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CCharString(const char*, long) 0x0099EBF0, ~CCharString
//   0x0099EAE0, CCharString::ToWideString 0x0099E480, CWideString::operator=(const CWideString&) 0x0099B7D0 /
//   operator=(const wchar_t*) 0x0099B800 / operator+= 0x0099B8D0, ~CWideString 0x0099B510, operator+(const wchar_t*,
//   const CWideString&) 0x0099BFF0 (__fastcall), GFIntToWideString 0x00405F00 (__fastcall), wcscmp 0x00BFE94C,
//   char_traits<wchar_t>::compare 0x00403C60 (cdecl). Literals: L"" 0x0122D70C, L" " 0x0122D820, the four
//   "TEXT_GUI_ACTION_MOVE_*" tags (narrow and wide), L"Invalid Movement!!" 0x0124AC14.
// Globals: g_pMainGameComponent 0x013B86A0 (game-text bank +0x14), else g_pNewFrontendGameComponent 0x013B871C (+0x60),
//   as the landed NUISystem::CText::SetValue 0x0054F410.
// Layout: retail CKeyRedefiner: Actions (STLport list<EGameAction>, 4 bytes: the header node) +0x19c, SubType +0x1a0.
//   Action 0x3c is the movement action, sub-types 10..13 its directions.
// Shape: speed-optimised TU (esp frame). CWideString's `== L""` is written as the two tests its inline body makes (null
//   data -> wcscmp(L"", L""); else the force-inlined STLport `operator==(const wstring&, const wchar_t*)`, the literal's
//   length folded to 0: a size test and the out-of-line char_traits compare) that `goto` the empty arm -- an inline
//   operator materialises the bool (`neg/sbb/inc`) -- as the landed NInventory::CCategory::GetHighlightedItemText
//   0x005BDA1D. The movement tags reach GetTextBySymbol through the implicit const char* -> CCharString conversion: that
//   temporary takes the addressed (`lea`) slot and each case keeps its own slots (an explicit CCharString temporary
//   swaps the tag and result slots; named locals share one slot set across the cases). Every movement case returns
//   straight away.
typedef unsigned short wchar_t;                                                       // VC7.1 without /Zc:wchar_t
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);                    // 0x00BFE94C

namespace std {
template <class _CharT> struct char_traits;
template <> struct char_traits<wchar_t> {
    static int __cdecl compare(const wchar_t* s1, const wchar_t* s2, unsigned int n);  // 0x00403C60
    static unsigned int length(const wchar_t* s)
    {
        unsigned int i;
        for (i = 0; s[i] != 0; ++i)
            ;
        return i;
    }
};
class wstring {                                                                       // STLport basic_string<wchar_t>
public:
    wchar_t* _M_start;                                                                 // +0x00
    wchar_t* _M_finish;                                                                // +0x04
    wchar_t* _M_end_of_storage;                                                        // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
    const wchar_t* data() const { return _M_start; }
};
__forceinline bool operator==(const wstring& s, const wchar_t* text)
{
    unsigned int n = char_traits<wchar_t>::length(text);
    return s.size() == n && char_traits<wchar_t>::compare(s.data(), text, n) == 0;
}
}

class CWideString {
public:
    std::wstring* PStringData;
    ~CWideString();                                                                    // 0x0099B510
    CWideString& operator=(const CWideString& other);                                  // 0x0099B7D0
    CWideString& operator=(const wchar_t* text);                                       // 0x0099B800
    CWideString& operator+=(const CWideString& other);                                 // 0x0099B8D0
    __forceinline bool operator==(const wchar_t* text) const
    {
        if (PStringData == 0)
            return wcscmp(L"", text) == 0;
        return *PStringData == text;
    }
};
CWideString __fastcall operator+(const wchar_t* left, const CWideString& right);      // 0x0099BFF0
CWideString __fastcall GFIntToWideString(long value);                                 // 0x00405F00

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);                                   // 0x0099EBF0
    ~CCharString();                                                                    // 0x0099EAE0
    CWideString ToWideString() const;                                                  // 0x0099E480
};

namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;                // 0x009C95E0
};
}
struct CMainGameComponent {
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;                                           // +0x14
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
struct CNewFrontendGameComponent {
    unsigned char         _pad00[0x60];
    NGameText::CDataBank* PGameTextDataBank;                                           // +0x60
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
extern CMainGameComponent*        g_pMainGameComponent;                               // 0x013B86A0
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;                        // 0x013B871C

namespace std {
template <class _Tp> struct _List_node {
    _List_node* _M_next;                                                               // +0x00
    _List_node* _M_prev;                                                               // +0x04
    _Tp         _M_data;                                                               // +0x08
};
template <class _Tp> class list {
public:
    _List_node<_Tp>* _M_node;                                                          // the header node
    const _Tp& front() const { return _M_node->_M_next->_M_data; }
};
}

enum EGameAction { GAME_ACTION_MOVE = 0x3c };
enum EGameActionSubType { GAME_ACTION_SUB_TYPE_FORWARD = 10, GAME_ACTION_SUB_TYPE_BACK, GAME_ACTION_SUB_TYPE_LEFT,
                          GAME_ACTION_SUB_TYPE_RIGHT };

namespace NUISystem {
class CKeyRedefiner {
public:
    unsigned char           _pad_0x00[0x19c];
    std::list<EGameAction>  Actions;                                                   // +0x19c
    EGameActionSubType      SubType;                                                   // +0x1a0
    void GetActionText(CWideString& text, const CCharString& symbol) const;
};

void CKeyRedefiner::GetActionText(CWideString& text, const CCharString& symbol) const
{
    NGameText::CDataBank* bank = g_pMainGameComponent ? g_pMainGameComponent->GetGameTextDataBank()
                                                      : g_pNewFrontendGameComponent->GetGameTextDataBank();
    if (Actions.front() == GAME_ACTION_MOVE) {
        switch (SubType) {
        case GAME_ACTION_SUB_TYPE_FORWARD:
            text = bank->GetTextBySymbol("TEXT_GUI_ACTION_MOVE_FORWARD");
            if (text.PStringData == 0) {
                if (wcscmp(L"", L"") == 0)
                    goto forwardEmpty;
            }
            else if (*text.PStringData == L"")
                goto forwardEmpty;
            return;
        forwardEmpty:
            text = L"TEXT_GUI_ACTION_MOVE_FORWARD";
            return;
        case GAME_ACTION_SUB_TYPE_BACK:
            text = bank->GetTextBySymbol("TEXT_GUI_ACTION_MOVE_BACK");
            if (text.PStringData == 0) {
                if (wcscmp(L"", L"") == 0)
                    goto backEmpty;
            }
            else if (*text.PStringData == L"")
                goto backEmpty;
            return;
        backEmpty:
            text = L"TEXT_GUI_ACTION_MOVE_BACK";
            return;
        case GAME_ACTION_SUB_TYPE_LEFT:
            text = bank->GetTextBySymbol("TEXT_GUI_ACTION_MOVE_LEFT");
            if (text.PStringData == 0) {
                if (wcscmp(L"", L"") == 0)
                    goto leftEmpty;
            }
            else if (*text.PStringData == L"")
                goto leftEmpty;
            return;
        leftEmpty:
            text = L"TEXT_GUI_ACTION_MOVE_LEFT";
            return;
        case GAME_ACTION_SUB_TYPE_RIGHT:
            text = bank->GetTextBySymbol("TEXT_GUI_ACTION_MOVE_RIGHT");
            if (text.PStringData == 0) {
                if (wcscmp(L"", L"") == 0)
                    goto rightEmpty;
            }
            else if (*text.PStringData == L"")
                goto rightEmpty;
            return;
        rightEmpty:
            text = L"TEXT_GUI_ACTION_MOVE_RIGHT";
            return;
        default:
            text = L"Invalid Movement!!";
            return;
        }
    }
    text = bank->GetTextBySymbol(symbol);
    if (text.PStringData == 0) {
        if (wcscmp(L"", L"") == 0)
            goto symbolEmpty;
    }
    else if (*text.PStringData == L"")
        goto symbolEmpty;
    goto addSubType;
symbolEmpty:
    text = symbol.ToWideString();
addSubType:
    if (SubType >= 1)
        text += L" " + GFIntToWideString(SubType);
}
}