// CTCScriptedControl::~CTCScriptedControl() -- the compiler-generated destructor: destroy the attitude-tag list, the action list and the script-handle pointer in reverse order, then ~CTCBase
// Leaf: ??1CTCScriptedControl@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00719C70, 54 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00719C50 of vtable 0x012658E4 (RTTI .?AVCTCScriptedControl@@ : CTCBase :
//   CBaseClassNonCopyable : CBaseClass; slots 13 / 14 / 21 / 22 = OnDie / OnKill / GetInterfaceType / GetClassTypeName, the
//   FableWin.pdb overrides); Ego_r.pdb / FableWin.pdb ??1CTCScriptedControl@@UAE@XZ. No 0x012658E4 store: the destructor is the
//   implicit one. The catalog label CExclusiveAssert::~CExclusiveAssert is propagated.
// Layout (Ego_r.pdb members at STLport sizes, GOTCHAS chunk 169: the 4-byte list puts Locked at +0x18): PScriptHandleImp
//   (CIntelligentPointer<CScriptGameResourceObjectScriptedThing>, 8) +0x0c, PActionList (list<CCountedPointer<CActionBase> >) +0x14,
//   Locked +0x18, ScriptAIPriority +0x1c, AttitudeTags (list<pair<long, EConversationAttitude> >) +0x20; sizeof 0x24.
// Callees: std::_List_base<pair<long, EConversationAttitude> >::~_List_base 0x00718EC0 (clear inlined, out of line; its ~list thunk
//   0x007191A0 jumps to it), std::_List_base<CCountedPointer<CTCScriptedControl::CActionBase> >::clear 0x00719280 (landed spelling),
//   free 0x00BFEA14 (the sentinel), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CTCBase::~CTCBase 0x00686830.
// Shape: the real STLport 4.6 list text (`~list() {}`, `~_List_base() { clear(); deallocate(sentinel); }`, clear() out of class)
//   lets VC7.1 choose retail's split itself: the action list's ~_List_base expands with an out-of-line clear, the attitude
//   list's ~_List_base is called with its clear inlined (chunk 698). DestroyOne (a destruction by name, ungraded) makes VC7.1
//   emit this out-of-line implicit `??1` (chunk 656/657).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                // CTCBase : CBaseClassNonCopyable; ~CTCBase 0x00686830
#include "engine/CCountedPointer.h"                        // retail model (8 bytes: Data, Info)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
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
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
}   // namespace std

class CBaseObject;
class CBaseObjectPointer;
class CCharString;
class CThing;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

enum EScriptAIPriority;                                    // MS forward enum (Ego_r CTCScriptedControl::ScriptAIPriority)
enum EConversationAttitude;                                // MS forward enum (the pair's second, Ego_r PDB U?$pair@JW4EConversationAttitude@@@std@@)
class CScriptGameResourceObjectScriptedThing;

class CTCScriptedControl : public CTCBase {                // vtable 0x012658E4, 0x24 bytes
public:
    class CActionBase;                                     // nested (vtable'd action, Ego_r.pdb CTCScriptedControl::CActionBase)
    CTCScriptedControl(CThing& thing);                     // ??0CTCScriptedControl@@QAE@AAVCThing@@@Z
    virtual void OnDie();                                  // 13 0x00713740
    virtual void OnKill();                                 // 14 0x00713700
    virtual ETCInterfaceType GetInterfaceType() const;     // 21 0x00719A40
    virtual CCharString GetClassTypeName() const;          // 22 0x00719A50
    CIntelligentPointer<CScriptGameResourceObjectScriptedThing>   PScriptHandleImp;   // +0x0c
    std::list<CCountedPointer<CActionBase> >                      PActionList;        // +0x14
    bool                                                          Locked;             // +0x18
    EScriptAIPriority                                             ScriptAIPriority;   // +0x1c
    std::list<std::pair<long, EConversationAttitude> >            AttitudeTags;       // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CTCScriptedControl) == 0x24);

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CTCScriptedControl* control)
{
    control->CTCScriptedControl::~CTCScriptedControl();
}