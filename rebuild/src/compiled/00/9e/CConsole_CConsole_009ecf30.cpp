// CConsole::~CConsole() -- tear down the console: clear both key/string binding maps, release every registered console input's counted pointer, clear the input map and the command-line history, then the member destructors free the history, the maps, the help bar string and both command lines, and the CTBaseSingleton vtable is restored
// Leaf: ??1CConsole@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ECF30, 606 bytes, thiscall. Caller: the scalar deleting destructor 0x009F0040
//   (slot 0 of CConsole's vtable 0x0129C600, rtti-vtables.tsv). The chunk label "ParticleMorphs_Destructor" is wrong.
// Layout (Ego_r.pdb CConsole, offsets re-derived for STLport's 12-byte map and 4-byte list): vptr +0, KeyBindingMap +4,
//   StringBindingMap +0x10, FrontCommandLine +0x1C, BackCommandLine +0x2C (CConsoleCommandLine, 0x10 bytes), HelpBarString
//   +0x3C, RegisteredConsoleInputs +0x40 (map<CCharString, CCountedPointer<CConsoleInputBase> >), CommandLineHistory +0x4C.
// Callees: _Rb_tree::_M_erase for KeyBindingMap @ 0x009EECC0, StringBindingMap @ 0x009AA830, RegisteredConsoleInputs
//   @ 0x009EF6E0; CCharString::~CCharString @ 0x0099EAE0; CBaseClass::~CBaseClass @ 0x0099A2E0; the counted pointer's delete
//   function through Info+4 (__fastcall, ecx = Info->Data); 0x00BFE9BC operator delete; 0x00BFEA14 free.
// Shape: the history list is cleared twice (body, then ~_List_base), each node through CConsoleCommandLine's virtual
//   destructor (`push 0 ; call [vtbl]`); CConsoleCommandLine's destructor is implicit, so its inlined copies store no vtable;
//   the input-map walk is STLport's inline _M_increment, and the counted pointer clears Info before Data.
extern "C" void __cdecl free(void* block);                          // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    ~_List_base()
    {
        clear();
        if (_M_node._M_data != 0)
            _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    void clear() { _List_base<_Tp, _Alloc>::clear(); }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator<_Value>& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator<_Value>& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc>
class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base()
    {
        if (_M_header._M_data != 0)
            _M_header.deallocate(_M_header._M_data, 1);
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    ~_Rb_tree() { clear(); }
    protected:
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    private:
    void _M_erase(_Link_type __x);
    public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    void clear() { _M_t.clear(); }
    _Rep_type _M_t;
};

}

class CCharStringData;
class CCharString {
public:
    CCharStringData* m_pStringData;
    ~CCharString();                                 // 0x0099EAE0
    bool operator<(const CCharString& other) const;
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo { public:
    unsigned long RefCount;                         // +0x00
    void (__fastcall* DeleteFunc)(void* data);      // +0x04
    void* Data;                                     // +0x08
};

template <class T> class CCountedPointer {
public:
    T* Data;                                        // +0x00
    CCPPointerInfo* Info;                           // +0x04
    void Clear()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};

template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}                   // vtable 0x0129C44C
};

class CConsoleInputBase;
class CFontBank;
enum EInputKey { INPUT_KEY_NONE = 0 };

class CRGBColour { public:
    unsigned char B, G, R, A;
};

class CConsoleCommandLine : public CBaseClass {     // 0x10 bytes; implicit destructor
public:
    CCharString CurrentEntry;                       // +0x04
    bool UserEntry;                                 // +0x08
    long CursorPosition;                            // +0x0C
};

class CConsole : public CTBaseSingleton<CConsole> {
public:
    virtual ~CConsole();

    std::map<char, CCharString> KeyBindingMap;                                  // +0x04
    std::map<CCharString, CCharString> StringBindingMap;                        // +0x10
    CConsoleCommandLine FrontCommandLine;                                       // +0x1C
    CConsoleCommandLine BackCommandLine;                                        // +0x2C
    CCharString HelpBarString;                                                  // +0x3C
    std::map<CCharString, CCountedPointer<CConsoleInputBase> > RegisteredConsoleInputs;   // +0x40
    std::list<CConsoleCommandLine> CommandLineHistory;                          // +0x4C
    std::_List_node_base* VisibleHistoryStart;                                  // +0x50
    std::_List_node_base* VisibleHistoryFinish;                                 // +0x54
    long NumberOfHistoryLinesOnscreen;                                          // +0x58
    bool HistoryMode;                                                           // +0x5C
    std::_List_node_base* HistoryPos;                                           // +0x60
    bool MovingIntoHistory;                                                     // +0x64
    bool GuessMode;                                                             // +0x65
    std::_Rb_tree_node_base* GuessPos;                                          // +0x68
    char ToggleChar;                                                            // +0x6C
    EInputKey ToggleKey;                                                        // +0x70
    CFontBank* PFont;                                                           // +0x74
    bool Active;                                                                // +0x78
    long ConsoleAlpha;                                                          // +0x7C
    CRGBColour HistoryWindowColour;                                             // +0x80
    CRGBColour ClientWindowColour;                                              // +0x84
    CRGBColour HelpWindowColour;                                                // +0x88
    CRGBColour ScrollBarColour;                                                 // +0x8C
    CRGBColour TextColour;                                                      // +0x90
    long CursorBlink;                                                           // +0x94
    long CursorBlinkRate;                                                       // +0x98
};

CConsole::~CConsole()
{
    KeyBindingMap.clear();
    StringBindingMap.clear();
    for (std::map<CCharString, CCountedPointer<CConsoleInputBase> >::iterator it = RegisteredConsoleInputs.begin();
         it != RegisteredConsoleInputs.end(); ++it)
        it->second.Clear();
    RegisteredConsoleInputs.clear();
    CommandLineHistory.clear();
}