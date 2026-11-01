// CCombatManager::~CCombatManager() -- combat manager destructor: destroy the combat dialogue manager, the combatant map and the registered-combat-sequence map, then the non-copyable base
// Leaf: ??1CCombatManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED480, 143 bytes, `jmp` to the base destructor.
// Identity (proven): the sole caller is the scalar deleting destructor 0x006F5120, slot 0 of vtable 0x01261EB4 (RTTI
//   CCombatManager : CBaseClassNonCopyable); the body stores that vtable and ends in ~CBaseClassNonCopyable 0x0099A300 (the
//   direct base, RTTI). Ego_r.pdb ??1CCombatManager@@UAE@XZ (0x00717340) / ??_G (0x0071B640), FableWin.pdb the same pair
//   (0x02417A40 / 0x024178A0). The catalog label CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: CombatDialogueManager::~CombatDialogueManager 0x006F2F60 (landed), std::_Rb_tree<unsigned __int64,
//   pair<const unsigned __int64, CCombatantInfo>, ...>::_M_erase 0x006F4C20 (landed), std::_Rb_tree<CCharString,
//   pair<const CCharString, CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>, ...>::_M_erase
//   0x006F0DD0 (landed under the CUnproven placeholders; this row proves the instantiation: NAME_FIXES), free 0x00BFEA14,
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Layout (Ego_r.pdb members at retail STLport sizes; no engine header): RegisteredCombatSequences +0x04 (map),
//   Combatants +0x10 (map), CombatStrafeTurningEnabled +0x1c, World +0x20, DrawDebug +0x24, TraceDebug +0x25,
//   ProximityForNavigation +0x28, EnableAINonAttackMode +0x2c, AttackPatternIdleStandardInterval +0x30,
//   AttacksFromNearestCombatantOnly +0x34, DialogueManager +0x38 (CombatDialogueManager, 0x24 retail bytes).
// Shape: a user-declared empty destructor (vtable store first); members in reverse order: the dialogue manager's
//   out-of-line destructor, then the two STLport maps (clear() with the shared zero register, header free).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                // +0x00
    _Base_ptr   _M_parent;                               // +0x04
    _Base_ptr   _M_left;                                 // +0x08
    _Base_ptr   _M_right;                                // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                             // +0x04
    _Compare  _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                       // out of line, one instantiation per tree type
public:
    ~_Rb_tree() { clear(); }
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
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
} // namespace std
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp> inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray over the STLport vector (12 bytes)

#include "engine/CBaseClassNonCopyable.h"               // ~CBaseClassNonCopyable 0x0099A300

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;                       // +0x00
};

class CThing;
class CWorld;
class CThingCreatureBase;
class CCombatSequenceBase;
#include "engine/CCountedPointer.h"                    // retail model (8 bytes: Data, Info)
class C2DVector { public: float X; float Y; };

class CAttackingCombatantInfo {                         // Ego_r.pdb members (retail 0x30: 12-byte STLport vector)
public:
    CThingCreatureBase* Attacked;                       // +0x00
    long      AttackerNumber;                           // +0x04
    long      CombatWheelRingIndex;                     // +0x08
    bool      PreferredAttackDollySet;                  // +0x0c
    long      PreferredAttackDollyTimer;                // +0x10
    C2DVector PreferredAttackDollyVector;               // +0x14
    bool      UsingCombatWheel;                         // +0x1c
    long      CurrentSequence;                          // +0x20
    std::vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;   // +0x24
};
class CDefendingCombatantInfo {                         // retail 0x3c bytes (landed model of 0x006F4C20)
public:
    unsigned char _body[0x3c];
    ~CDefendingCombatantInfo();                         // 0x006F48A0
};
struct CCombatantInfo {                                 // Ego_r.pdb (retail 0x70)
    CThingCreatureBase*     Parent;                     // +0x00
    CAttackingCombatantInfo AttackerInfo;               // +0x04
    CDefendingCombatantInfo DefenderInfo;               // +0x34
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                         // retail model (4 bytes: CDefPointeeBase* Object)
class CCombatDialogueDef;

class CombatDialogueManager {                           // Ego_r.pdb members, retail 0x24 bytes
public:
    class TSpeechGroupInfo { public: long NextAllowedFrame; };
    ~CombatDialogueManager();                           // 0x006F2F60 (landed)
    std::map<unsigned long, TSpeechGroupInfo> SpeechGroupInfoMap;   // +0x00
    CDefPointer<const CCombatDialogueDef>     PDialogueDef;         // +0x0c
    CIntelligentPointer<CThingCreatureBase>   PCurrentTalker;       // +0x10
    unsigned long                             CurrentTalkerTextGroupIndex;   // +0x18
    long                                      LastSpeechFrame;      // +0x1c
    long                                      LastBattleCryFrame;   // +0x20
};

class CCombatManager : public CBaseClassNonCopyable {
public:
    CCombatManager(CWorld& world);
    virtual ~CCombatManager();
    std::map<CCharString, CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>
                          RegisteredCombatSequences;    // +0x04
    std::map<unsigned __int64, CCombatantInfo> Combatants;   // +0x10
    bool                  CombatStrafeTurningEnabled;   // +0x1c
    CWorld*               World;                        // +0x20
    bool                  DrawDebug;                    // +0x24
    bool                  TraceDebug;                   // +0x25
    float                 ProximityForNavigation;       // +0x28
    bool                  EnableAINonAttackMode;        // +0x2c
    float                 AttackPatternIdleStandardInterval;   // +0x30
    bool                  AttacksFromNearestCombatantOnly;     // +0x34
    CombatDialogueManager DialogueManager;              // +0x38
};

CCombatManager::~CCombatManager()
{
}