// NScript::CGlobal_DebugCycleThroughSpeechScript::Main() -- debug: collect every text entry of the targeted thing, then have the hero speak each one in turn under a screen title of its number, waiting for each line to finish; finally deactivate this quest
// Grade: DIFFER(575v613) -- one residue: retail inlines the entries set's destructor at the in-loop terminate exits (~set and ~_Rb_tree expanded to calls of clear 0x004CFF70 and ~_Rb_tree_base 0x004CFFB0), calls it out of line at the pre-loop terminate exit (0x004D0740, own ~hero call + epilogue) and at the normal scope end (0x004D0E20); the lane calls ~set out of line at every site and so tail-merges the pre-loop exit into the loop-exit tail. Everything else (frame 0x30, hero inline copy / IsAlive / end-of-function inline dtor, loop, Speak, title, waits) matches. Probes (probe/ee91e0_e*.cpp): explicit / __forceinline ~set, real STLport __malloc_alloc chain, visible _M_erase body, QFE 4035 toolset and /GL+/LTCG do not reproduce the per-site inlining; out-of-line clear / ~_Rb_tree_base (e7) fixes the loop exits but then inlines at the other two sites.
// Leaf: ?Main@CGlobal_DebugCycleThroughSpeechScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE91E0, 613 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the script vtable 0x012F54B4 (slot 1 = the landed RegisterMain 0x00EE9120, whose
//   "Main" thread calls slot 2 through the vcall thunk); Ego_r.pdb and FableWin.pdb ?Main@CGlobal_DebugCycleThroughSpeechScript@NScript@@UAEXXZ.
//   Catalog label Script_MakeThingsSpeakNumbered (invented).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; Game (+0x40) slots 70 GetHero, 596 DebugGetAllTextEntriesForTargetedThing,
//   371 FixMovieSequenceCamera, 8 StartScriptingEntity, 113 AddScreenTitleMessage, 7 NewScriptFrame, 655 GetActiveQuestName,
//   281 DeactivateQuestLater; CScriptThing slot 75 IsAlive (inline wrapper), ~CScriptThing 0x004AA840, ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC; std::_Rb_tree<unsigned long,...>::_Rb_tree(const less&, const allocator&) 0x00A394D0,
//   _Rb_global<bool>::_M_increment 0x004292C0; GFIntToCharString 0x0099F830 (__fastcall), ~CCharString 0x0099EAE0; CBaseObject()
//   0x0099A380; CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&, unsigned long, ETextGroupSelectionMethod,
//   bool, bool, bool) 0x007E7380, ::IsPerformingScriptTask 0x007E7450, ~ 0x007E74D0; vtables CScriptThing 0x01238C8C,
//   CScriptGameResourceObjectScriptedThingBase 0x0127094C.
// Layout: retail CScriptBase (0x40) then the interface Game +0x40 (Ego_r.pdb 0x58 = 0x54 + 4). The set's tree is {header node,
//   node count, compare} (STLport 4.6 text of the landed 0x00A394D0 model).
// Header debt: CBaseObject / CScriptGameResourceObjectBase / CScriptGameResourceObjectScriptedThingBase are local polymorphic
//   models (the shared engine headers for them are flat, non-polymorphic); the STLport set / tree is a local std model.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC (MSVCR71)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);   // 0x004292C0
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef typename _Base::allocator_type allocator_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
public:
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
private:
    void _M_erase(_Link_type __x);                                       // 0x004CF810
public:
    _Rb_tree(const _Compare& __comp, const allocator_type& __a);         // 0x00A394D0
    ~_Rb_tree() { clear(); }
    iterator begin() { return iterator(_M_leftmost()); }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    bool empty() const { return _M_node_count == 0; }
    size_type size() const { return _M_node_count; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Compare, class _Alloc>
class set {
public:
    typedef _Key key_type;
    typedef _Key value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Identity<value_type>, key_compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    typedef typename _Rep_type::size_type size_type;
    typedef typename _Rep_type::allocator_type allocator_type;
private:
    _Rep_type _M_t;
public:
    set() : _M_t(_Compare(), allocator_type()) { }
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
    bool empty() const { return _M_t.empty(); }
    size_type size() const { return _M_t.size(); }
};
}   // namespace std
typedef std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > CTextEntrySet;
class CCPPointerInfo {
public:
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
__forceinline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
#include "engine/CScriptBase.h"
#include "engine/CGameScriptInterface.h"

class CBaseObjectPointer;
class CBaseObject : public CBaseClass {                 // vtable 0x0129A7D8 (RTTI .?AVCBaseObject@@)
protected:
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;             // +0x04
};
class CScriptGameResourceObjectBase : public CBaseObject {   // vtable 0x0126008C
public:
    CScriptGameResourceObjectBase() { }
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    virtual ~CScriptGameResourceObjectScriptedThingBase() { }          // out of line 0x007E74D0
    virtual void Speak(const CScriptThing& speaker, unsigned long textEntry, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7380
    virtual bool IsPerformingScriptTask() const;                       // 0x007E7450
};

namespace NScript {
class CGlobal_DebugCycleThroughSpeechScript : public CScriptBase {
public:
    virtual void Main();
    CGameScriptInterface* Game;                         // +0x40
};

void CGlobal_DebugCycleThroughSpeechScript::Main()
{
    CScriptThing hero = Game->GetHero();
    if (hero.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        CTextEntrySet entries;
        Game->DebugGetAllTextEntriesForTargetedThing(entries);
        if (entries.size() != 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->FixMovieSequenceCamera(true);
            CScriptGameResourceObjectScriptedThingBase speaker;
            Game->StartScriptingEntity(hero, speaker, (EScriptAIPriority)4);
            for (CTextEntrySet::iterator it = entries.begin(); it != entries.end(); ++it) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddScreenTitleMessage(GFIntToCharString(*it), 0.0f, true);
                speaker.Speak(Game->GetHero(), *it, (ETextGroupSelectionMethod)0, false, true, false);
                while (speaker.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->FixMovieSequenceCamera(false);
        }
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript
