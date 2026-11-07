// CConsole::Initialise(char, EInputKey, const CFontBank*) -- store the toggle key and font, empty the registered-input map, register the console's own commands and the ConsoleAlpha variable, clear the history and greet the user
// Leaf: ?Initialise@CConsole@@QAEXDW4EInputKey@@PBVCFontBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ED190, 2266 bytes, `ret 0Ch`.
// Identity: Ego_r.pdb ?Initialise@CConsole@@QAEXDW4EInputKey@@PBVCFontBank@@@Z; the registered names (0x0129C74C "BindString" ..
//   0x0129C630 "ConsoleAlpha") and the static __fastcall Console* / SetConsole*Colour handlers (Ego_r.pdb SIXAAV1@ABV?$vector@VCGenericVar..)
//   stored at +0x18 of each CConsoleCommand<CConsole> (vtable 0x0129C4E0, rtti-vtables).
// Callees: CConsole::AddInput 0x009EC5E0, the map's _M_erase 0x009EF6E0, operator new 0x00BFEA1A, CCharString(const char*, long)
//   0x0099EBF0, CCharString copy 0x0099EC30 / dtor 0x0099EAE0, CConsoleCommandParameters(type, type) 0x009EBB80 /
//   (type, type, type) 0x009EBC00 / copy 0x00419F50, CConsoleCommandBase(const CCharString&, const EGenericVarType&) 0x00414F30,
//   vector<EGenericVarType>::_M_insert_overflow 0x009EF900 (the one-type parameter list, inlined here), free 0x00BFEA14,
//   _List_base<CConsoleCommandLine>::clear 0x009EE7B0, CConsole::SendMessageToUser 0x009EB260.
// Model: local, as the landed CConsole siblings (CConsole::CConsole 0x009ECD80, NGlobalConsole::Initialise 0x00419D90): no CConsole
//   header exists and engine/CConsoleCommandBase.h is data-only (no vptr, no ctors). Layout from the landed CConsole::CConsole.
//   EGenericVarType enumerator names are descriptive (1 long, 2 unsigned long, 5 string: the CGenericVarRef union order).
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14

enum EGenericVarType { GENERIC_VAR_TYPE_NONE, GENERIC_VAR_TYPE_SLONG, GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_FLOAT,
    GENERIC_VAR_TYPE_BOOL, GENERIC_VAR_TYPE_STRING };
enum EInputKey { };
class CGenericVar;
class CFontBank;
class CConsoleInputBase;
class CConsoleCommandLine;

namespace std {
struct __false_type { __false_type() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc
{
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _Tp, class _Alloc> class _Vector_base
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) {}
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc>
{
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) {}
protected:
    void _M_insert_overflow(_Tp* __pos, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);  // 0x009EF900
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
// Same-TU stand-in for the shared reallocating insert 0x009EF900 (as the landed 0x009EBB30): retail homes the empty tag on the live
// name-string slot, which VC7.1 only does when it can see that the callee never reads the tag. Its body is never graded.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __pos, const _Tp& __x, const __false_type&,
                                                        unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = (unsigned int)(this->_M_finish - this->_M_start);
    unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = this->_M_start; __p != __pos; ++__p, ++__new_finish) _Construct(__new_finish, *__p);
    for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish) _Construct(__new_finish, __x);
    if (!__atend)
        for (_Tp* __q = __pos; __q != this->_M_finish; ++__q, ++__new_finish) _Construct(__new_finish, *__q);
    if (this->_M_start) free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree
{
public:
    _Rb_tree_node<_Value>* _M_header;
    unsigned int           _M_node_count;
    _Compare               _M_key_compare;
    void _M_erase(_Rb_tree_node<_Value>* __x);           // 0x009EF6E0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Rb_tree_node<_Value>*)_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map
{
public:
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;
    void clear() { _M_t.clear(); }
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { };
template <class _Tp> struct _List_iterator { _List_node_base* _M_node; _List_iterator(_List_node_base* __x) : _M_node(__x) {} _List_iterator() {} };
template <class _Tp, class _Alloc> class _List_base
{
public:
    _List_node_base* _M_node;
    void clear();                                        // 0x009EE7B0
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc>
{
public:
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node->_M_next); }
    void clear() { _List_base<_Tp, _Alloc>::clear(); }
};
} // namespace std

class CConsoleCommandParameters                          // retail 12 bytes: the STLport vector<EGenericVarType>
{
public:
    std::vector<EGenericVarType> ParamTypes;
    CConsoleCommandParameters(EGenericVarType type1) { AddParameterType(type1); }
    CConsoleCommandParameters(EGenericVarType type1, EGenericVarType type2);                          // 0x009EBB80
    CConsoleCommandParameters(EGenericVarType type1, EGenericVarType type2, EGenericVarType type3);   // 0x009EBC00
    CConsoleCommandParameters(const CConsoleCommandParameters& other);                                // 0x00419F50
    CConsoleCommandParameters() { }
    void AddParameterType(const EGenericVarType& type) { ParamTypes.push_back(type); }
};

class CConsoleInputBase                                  // vtable 0x0122E5B0
{
public:
    CConsoleInputBase(const CCharString& name) : Name(name) { }
    virtual ~CConsoleInputBase();
    CCharString Name;                                    // +0x04
};
class CConsoleCommandBase : public CConsoleInputBase     // vtable 0x0122E638
{
public:
    CConsoleCommandBase(const CCharString& name) : CConsoleInputBase(name) { }
    CConsoleCommandBase(const CCharString& name, const CConsoleCommandParameters& params) : CConsoleInputBase(name), ParamTypes(params) { }
    CConsoleCommandBase(const CCharString& name, const EGenericVarType& type);   // 0x00414F30
    virtual ~CConsoleCommandBase();
    CConsoleCommandParameters ParamTypes;                // +0x08
};
template <class T> class CConsoleCommand : public CConsoleCommandBase   // CConsoleCommand<CConsole> vtable 0x0129C4E0
{
public:
    typedef void (__fastcall *TFunction)(T& object, const std::vector<CGenericVar>& args);
    CConsoleCommand(const CCharString& name, T& object, TFunction function)
        : CConsoleCommandBase(name), Object(&object), Function(function) { }
    CConsoleCommand(const CCharString& name, T& object, TFunction function, const EGenericVarType& type)
        : CConsoleCommandBase(name, type), Object(&object), Function(function) { }
    CConsoleCommand(const CCharString& name, T& object, TFunction function, const CConsoleCommandParameters& params)
        : CConsoleCommandBase(name, params), Object(&object), Function(function) { }
    virtual ~CConsoleCommand();
    T*        Object;                                    // +0x14
    TFunction Function;                                  // +0x18
};
class CGenericVarRef                                     // Ego_r.pdb, 8 bytes
{
public:
    CGenericVarRef(long& value) : Type(GENERIC_VAR_TYPE_SLONG), PValSLONG(&value) { }   // ??0CGenericVarRef@@QAE@AAJ@Z
    EGenericVarType Type;                                // +0x00
    union {                                              // +0x04
        long*          PValSLONG;
        unsigned long* PValULONG;
        float*         PValFloat;
        bool*          PValBool;
        CCharString*   PValString;
    };
};
class CConsoleVariable : public CConsoleInputBase        // vtable 0x0122E5C8
{
public:
    CConsoleVariable(const CCharString& name, long& value) : CConsoleInputBase(name), Var(value) { }   // ??0CConsoleVariable@@QAE@ABVCCharString@@AAJ@Z
    virtual ~CConsoleVariable();
    CGenericVarRef Var;                                  // +0x08
};

template <class T> class CTBaseSingleton
{
public:
    virtual ~CTBaseSingleton() { }
};

class CConsole : public CTBaseSingleton<CConsole>        // vtable 0x0129C600
{
public:
    unsigned char KeyBindingMap[0xc];                    // +0x04 map<char, CCharString>
    unsigned char StringBindingMap[0xc];                 // +0x10 map<CCharString, CCharString>
    unsigned char FrontCommandLine[0x10];                // +0x1c CConsoleCommandLine
    unsigned char BackCommandLine[0x10];                 // +0x2c CConsoleCommandLine
    CCharString   HelpBarString;                         // +0x3c
    std::map<CCharString, CCountedPointer<CConsoleInputBase> > RegisteredConsoleInputs;   // +0x40
    std::list<CConsoleCommandLine> CommandLineHistory;   // +0x4c
    std::list<CConsoleCommandLine>::iterator VisibleHistoryStart;    // +0x50
    std::list<CConsoleCommandLine>::iterator VisibleHistoryFinish;   // +0x54
    long          NumberOfHistoryLinesOnscreen;          // +0x58
    bool          HistoryMode;                           // +0x5c
    std::list<CConsoleCommandLine>::iterator HistoryPos; // +0x60
    bool          MovingIntoHistory;                     // +0x64
    bool          GuessMode;                             // +0x65
    void*         GuessPos;                              // +0x68
    char          ToggleChar;                            // +0x6c
    EInputKey     ToggleKey;                             // +0x70
    const CFontBank* PFont;                              // +0x74
    bool          Active;                                // +0x78
    long          ConsoleAlpha;                          // +0x7c

    void Initialise(char toggle_char, EInputKey toggle_key, const CFontBank* font);
    void AddInput(CConsoleInputBase* input);             // 0x009EC5E0
    void SendMessageToUser(const CCharString& text);     // 0x009EB260
    static void __fastcall ConsoleConsoleListContainingString(CConsole& console, const std::vector<CGenericVar>& args);   // 0x009EB590
    static void __fastcall ConsoleCommandList(CConsole& console, const std::vector<CGenericVar>& args);                   // 0x009EB690
    static void __fastcall ConsoleCommandListContainingString(CConsole& console, const std::vector<CGenericVar>& args);   // 0x009EB760
    static void __fastcall ConsoleVariableList(CConsole& console, const std::vector<CGenericVar>& args);                  // 0x009EB8E0
    static void __fastcall ConsoleVariableListContainingString(CConsole& console, const std::vector<CGenericVar>& args);  // 0x009EB9B0
    static void __fastcall ConsoleRunScript(CConsole& console, const std::vector<CGenericVar>& args);                     // 0x009ECB70
    static void __fastcall ConsoleAddKeyBinding(CConsole& console, const std::vector<CGenericVar>& args);                 // 0x009ECC70
    static void __fastcall ConsoleAddStringBinding(CConsole& console, const std::vector<CGenericVar>& args);              // 0x009ECCE0
    static void __fastcall ConsoleRunStringBinding(CConsole& console, const std::vector<CGenericVar>& args);              // 0x009ECD40
    static void __fastcall SetConsoleTextColour(CConsole& console, const std::vector<CGenericVar>& args);                 // 0x009EAA50
    static void __fastcall SetConsoleHistoryWindowColour(CConsole& console, const std::vector<CGenericVar>& args);        // 0x009EAA80
    static void __fastcall SetConsoleClientWindowColour(CConsole& console, const std::vector<CGenericVar>& args);         // 0x009EAAB0
    static void __fastcall SetConsoleHelpWindowColour(CConsole& console, const std::vector<CGenericVar>& args);           // 0x009EAAE0
    static void __fastcall SetConsoleScrollBarColour(CConsole& console, const std::vector<CGenericVar>& args);            // 0x009EAB10
};

void CConsole::Initialise(char toggle_char, EInputKey toggle_key, const CFontBank* font)
{
    ToggleChar = toggle_char;
    ToggleKey = toggle_key;
    PFont = font;
    RegisteredConsoleInputs.clear();
    AddInput(new CConsoleCommand<CConsole>("BindString", *this, ConsoleAddStringBinding,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_STRING, GENERIC_VAR_TYPE_STRING)));
    AddInput(new CConsoleCommand<CConsole>("RunBoundString", *this, ConsoleRunStringBinding,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_STRING)));
    AddInput(new CConsoleCommand<CConsole>("BindKey", *this, ConsoleAddKeyBinding,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_STRING, GENERIC_VAR_TYPE_STRING)));
    AddInput(new CConsoleCommand<CConsole>("RunScript", *this, ConsoleRunScript, GENERIC_VAR_TYPE_STRING));
    AddInput(new CConsoleCommand<CConsole>("ConsoleListContaining", *this, ConsoleConsoleListContainingString, GENERIC_VAR_TYPE_STRING));
    AddInput(new CConsoleCommand<CConsole>("CommandList", *this, ConsoleCommandList));
    AddInput(new CConsoleCommand<CConsole>("CommandListContaining", *this, ConsoleCommandListContainingString, GENERIC_VAR_TYPE_STRING));
    AddInput(new CConsoleCommand<CConsole>("VarList", *this, ConsoleVariableList));
    AddInput(new CConsoleCommand<CConsole>("VarListContaining", *this, ConsoleVariableListContainingString, GENERIC_VAR_TYPE_STRING));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleTextColour", *this, SetConsoleTextColour,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleHistoryWindowColour", *this, SetConsoleHistoryWindowColour,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleClientWindowColour", *this, SetConsoleClientWindowColour,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleHelpWindowColour", *this, SetConsoleHelpWindowColour,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleScrollBarColour", *this, SetConsoleScrollBarColour,
        CConsoleCommandParameters(GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_ULONG)));
    AddInput(new CConsoleVariable("ConsoleAlpha", ConsoleAlpha));
    CommandLineHistory.clear();
    HistoryPos = CommandLineHistory.begin();
    SendMessageToUser("Try and shut up and get on with your work.");
}