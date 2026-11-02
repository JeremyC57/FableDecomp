// CTCCreatureModeManager::ResetModes() -- drop every mode (the front one is told it is leaving), select mode 0, then set up the default and the initial mode again
// Leaf: ?ResetModes@CTCCreatureModeManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073B0D0, 56 bytes, thiscall; tail `jmp SetupInitialMode` 0x0073AFC0.
// Identity (proven, two sources): Ego_r.pdb defines the CTCCreatureModeManager members in the order AddMode(K) 0x00751900,
//   RemoveAllModes 0x00751960, ~ 0x007519D0, ctor 0x00751A30, SetupDefaultMode 0x00751B00, SetupInitialMode 0x00751B70, ResetModes
//   0x00751CB0, OnCreate 0x00751CD0, InitialActivate 0x00751CE0; retail has the same run: AddMode(K) 0x0073AE10, RemoveAllModes
//   0x0073AE70, ~ 0x0073AEA0 (stores vtable 0x01268854), ctor 0x0073AF00, SetupDefaultMode 0x0073AF60, SetupInitialMode 0x0073AFC0,
//   this row 0x0073B0D0, OnCreate 0x0073B110 (`jmp SetupDefaultMode`), InitialActivate 0x0073B120 (`jmp SetupInitialMode`).
//   FableWin.pdb ?ResetModes@CTCCreatureModeManager@@QAEXXZ (0x024C2240, right after SetupInitialMode 0x024C1EC0). The body is
//   RemoveAllModes inlined (byte-identical to 0x0073AE70 apart from the register choice: grading this TU's emitted
//   ?RemoveAllModes@CTCCreatureModeManager@@QAEXXZ COMDAT against 0x0073AE70 gives RELOCATION_MATCH) + the two private Setup
//   calls. Only caller 0x0089DE3B. The catalog label "CMapsAndRegionsDialog::SetTooltipAsOpen" is propagated and wrong.
// Callees: the front mode's virtual slot 6 CModeBase::OnDeactivate (vtable 0x012673CC slot 6 = 0x00739E60 `mov byte [ecx+18h],0`:
//   Active = false), STLport std::_List_base<CCountedPointer<CTCCreatureModeManager::CModeBase>, allocator<..> >::clear 0x0073E6A0
//   (out of line, _list.c; the catalog / binding stand-in is CRetailList_CountedPointer::clear), ?AddMode@CTCCreatureModeManager@@QAE_NK@Z
//   0x0073AE10, the private ?SetupDefaultMode@CTCCreatureModeManager@@AAEXXZ 0x0073AF60 and ?SetupInitialMode@..@@AAEXXZ 0x0073AFC0
//   (both PDBs; tail).
// Shape: RemoveAllModes is a plain member defined above ResetModes in the same file; VC7.1 inlines it here and keeps its out-of-line
//   COMDAT (retail does both: 0x0073AE70 and this row, and the destructor 0x0073AEA0 inlines it too). `empty()` is STLport's
//   `_M_node._M_data->_M_next == _M_node._M_data` (`cmp [eax],eax`); front() dereferences the first node (data at node+8) and
//   CCountedPointer::operator-> returns Data. AddMode(0) re-selects mode 0.
// Layout: retail sizeof 0x24 (Alloc 0x004D3547 `push 24h`): CTCBase 0xC, PModes (STLport list, 4 bytes) +0x0C, TimeLeftForCurrentMode
//   +0x10, TimeForNextIdleMode +0x14 (the ctor's ConstantFPS product), AvailableIdleModes (STLport vector) +0x18 (Ego_r names; its
//   Dinkumware containers put them 8 higher). engine/CTCBase.h retail opt-in and engine/CCountedPointer.h; CTCCreatureModeManager has
//   no shared header (local class). CModeBase's WeaponBasedFilter (CAnimFilterOfGroup) is opaque storage here (not touched).
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CThing;
class CAnimateThing;
class CAnimationEntry;
class C3DVector;
enum EMovementType;
class CAnimFilterBase;
namespace NCreatureMode { enum EMode; }

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
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
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x0: the sentinel node
    void clear();                                                 // _list.c, out of line
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef _List_node<_Tp> _Node;
    typedef _Tp& reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    reference front() { return *begin(); }
    void clear() { _Base::clear(); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
}

class CTCCreatureModeManager : public CTCBase {             // vtable 0x01268854; new(0x24) in Alloc 0x004D3547
public:
    class CModeBase {                                       // vtable 0x012673CC (12 slots; RTTI no base)
    public:
        CModeBase(CThing& thing);
        virtual ~CModeBase();                               // slot 0
        virtual unsigned long GetType() const = 0;          // slot 1 (_purecall)
        virtual void AnimUpdate(CAnimateThing& animateThing);   // slot 2 0x00739E40
        virtual void FrameUpdate();                         // slot 3 0x0073AC50
        virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4 0x00739E50
        virtual void OnActivate();                          // slot 5 0x0073AAC0
        virtual void OnDeactivate();                        // slot 6 0x00739E60 (Active = false)
        virtual float GetMovementSpeed(const C3DVector& from, const C3DVector& to, EMovementType type) const;   // slot 7
        virtual bool IsIdleMode() const;                    // slot 8
        virtual bool IsRemoveOnDeactivate() const;          // slot 9
        virtual bool IsHoldWeaponWithBothHands() const;     // slot 10 0x0073A9F0
        virtual bool IsAllowingPlayerSpecialMovementMoves() const;   // slot 11
        CThing*       ParentThing;                          // +0x04
        unsigned char WeaponBasedFilter_[0xc];              // CAnimFilterOfGroup WeaponBasedFilter +0x08
        long          FramesBeforeRemovingSelf;             // +0x14
        bool          Active;                               // +0x18
    };
    void ResetModes();
    void RemoveAllModes();
    bool AddMode(unsigned long modeType);                   // 0x0073AE10 ?AddMode@CTCCreatureModeManager@@QAE_NK@Z
private:
    void SetupDefaultMode();                                // 0x0073AF60 ?SetupDefaultMode@CTCCreatureModeManager@@AAEXXZ
    void SetupInitialMode();                                // 0x0073AFC0 ?SetupInitialMode@CTCCreatureModeManager@@AAEXXZ
public:
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

void CTCCreatureModeManager::ResetModes()
{
    RemoveAllModes();
    SetupDefaultMode();
    SetupInitialMode();
}