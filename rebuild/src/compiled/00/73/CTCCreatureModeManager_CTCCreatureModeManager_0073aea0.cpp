// CTCCreatureModeManager::~CTCCreatureModeManager() -- creature mode-manager thing component destructor: RemoveAllModes inline (deactivate the front mode, clear the list, re-add the default mode), then destroy AvailableIdleModes and PModes, then ~CTCBase
// Leaf: ??1CTCCreatureModeManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073AEA0, int3 padding before and after, thiscall, tail `jmp ~CTCBase`.
// Identity (proven, two sources): (1) it stores vtable 0x01268854 (RTTI .?AVCTCCreatureModeManager@@ : CTCBase) and its sole caller is
//   the scalar deleting destructor 0x007442B0, slot 0 of that vtable; (2) Ego_r.pdb 0x007519D0 / FableWin.pdb 0x024C14D0
//   ??1CTCCreatureModeManager@@UAE@XZ (chunk 710 noted the same). The catalog label "CVertexShaderData::~CVertexShaderData" is wrong.
// Callees: CModeBase::OnDeactivate (virtual slot 6) of the front mode, ?clear@?$_List_base@V?$CCountedPointer@VCModeBase@
//   CTCCreatureModeManager@@@@V?$allocator@...@std@@@std@@QAEXXZ 0x0073E6A0, ?AddMode@CTCCreatureModeManager@@QAE_NK@Z 0x0073AE10,
//   free 0x00BFEA14 (vector storage, list sentinel), ??1CTCBase@@UAE@XZ (tail).
// Shape: FableWin's dtor calls RemoveAllModes(), which VC7.1 inlines here (`if (!PModes.empty()) PModes.front()->OnDeactivate();
//   PModes.clear(); AddMode(0);`); then vector<NCreatureMode::EMode> (trivial elements: only the free) and the STLport list
//   (~_List_base: clear() out of line, then the sentinel deallocate) are destroyed. RemoveAllModes' own out-of-line copy is not graded here.
// Layout: Ego_r.pdb CTCCreatureModeManager with the STLport list (4 bytes) and vector (12 bytes): PModes +0x0C .. AvailableIdleModes
//   +0x18, retail sizeof 0x24. CModeBase per chunk 710 (WeaponBasedFilter is opaque storage: CAnimFilterOfGroup +0x08, 12 bytes).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CThing;
class CAnimateThing;
class C3DVector;
enum EMovementType;
class CAnimFilterBase;
namespace NCreatureMode { enum EMode; }

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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();                                                 // _list.c, out of line (0x0073E6A0)
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef _List_node<_Tp> _Node;
    typedef _Tp& reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    ~list() { }
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    reference front() { return *begin(); }
    void clear() { _Base::clear(); }
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CTCCreatureModeManager : public CTCBase {
public:
    class CModeBase {
    public:
        CModeBase(CThing& thing);
        virtual ~CModeBase();                               // slot 0
        virtual unsigned long GetType() const = 0;          // slot 1
        virtual void AnimUpdate(CAnimateThing& animateThing);   // slot 2
        virtual void FrameUpdate();                         // slot 3
        virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4
        virtual void OnActivate();                          // slot 5
        virtual void OnDeactivate();                        // slot 6
        virtual float GetMovementSpeed(const C3DVector& from, const C3DVector& to, EMovementType type) const;   // slot 7
        virtual bool IsIdleMode() const;                    // slot 8
        virtual bool IsRemoveOnDeactivate() const;          // slot 9
        virtual bool IsHoldWeaponWithBothHands() const;     // slot 10
        virtual bool IsAllowingPlayerSpecialMovementMoves() const;   // slot 11
        CThing*       ParentThing;                          // +0x04
        unsigned char WeaponBasedFilter_[0xc];              // CAnimFilterOfGroup WeaponBasedFilter +0x08
        long          FramesBeforeRemovingSelf;             // +0x14
        bool          Active;                               // +0x18
    };
    CTCCreatureModeManager(CThing& thing);
    virtual ~CTCCreatureModeManager();
    void RemoveAllModes();
    bool AddMode(unsigned long modeType);                   // 0x0073AE10
    std::list<CCountedPointer<CModeBase> > PModes;          // +0x0C
    long                                   TimeLeftForCurrentMode;   // +0x10
    long                                   TimeForNextIdleMode;      // +0x14
    std::vector<NCreatureMode::EMode>      AvailableIdleModes;       // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureModeManager) == 0x24);

void CTCCreatureModeManager::RemoveAllModes()
{
    if (!PModes.empty())
        PModes.front()->OnDeactivate();
    PModes.clear();
    AddMode(0);
}

CTCCreatureModeManager::~CTCCreatureModeManager()
{
    RemoveAllModes();
}