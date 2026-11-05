// CTCTrophy::~CTCTrophy() -- trophy component dtor: leave the global TC update list, then the members in reverse order (the WitnessesAchieved and AvailableWitnesses lists, the trophy def pointer) and ~CTCBase
// Leaf: ??1CTCTrophy@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DDB80, 98 bytes, tail jump to the base destructor.
// Identity (proven): the first store is vtable 0x0125FC54, RTTI `.?AVCTCTrophy@@` (rtti_bases: CTCTrophy : CTCBase :
//   CBaseClassNonCopyable : CBaseClass); the sole caller 0x006DFB43 is the scalar deleting dtor 0x006DFB40 = slot 0 of that
//   vtable. Ego_r.pdb has ??1CTCTrophy@@UAE@XZ (user-declared virtual: the store stays). Catalog label
//   "CTCActionUseBuyOrSteal::~CTCActionUseBuyOrSteal" is propagated and wrong.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (Ego_r.pdb ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ); STLport
//   std::_List_base<CIntelligentPointer<CThingAICreature> >::clear 0x00510D40 (kept out of line by VC7.1 from the _list.c body,
//   as retail; the landed OnCreate 0x006DDA20 calls it for AvailableWitnesses); free 0x00BFEA14 (each list's sentinel node);
//   the def's virtual Delete (slot 1) on its last release; CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb CTCTrophy with the retail one-word STLport list (PDB 12 bytes -> 4): PDef +0x0C, AvailableWitnesses
//   +0x34, WitnessesAchieved +0x38, IsGlobalTC +0x40, Mountable +0x48 (as the landed OnCreate 0x006DDA20). engine/CTCBase.h is
//   flattened (vptr word), so the CTCBase chain is the local model of the landed 0x007710A0; CDefPointer and
//   CBaseClassNonCopyable come from the shared headers. No engine/CTCTrophy.h.
// Shape: the body is the one call; members die in reverse order (WitnessesAchieved, AvailableWitnesses: ~_List_base =
//   clear() + sentinel deallocate with its null test; PDef: CDefPointer::Release).
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
// ---- list (stl/_list.h, stl/_list.c)
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
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
};
} // namespace std

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CThingAICreature;
class CTrophyDef;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCTrophy : public CTCBase {                          // vtable 0x0125FC54
public:
    virtual ~CTCTrophy();
    CDefPointer<const CTrophyDef>                   PDef;                           // +0x0C
    long                                            NonMinigameDeedHandle;          // +0x10
    bool                                            MinigameInProgress;             // +0x14
    long                                            BestWitnessesAheadToDate;       // +0x18
    long                                            MinigameInitialFramesLeft;      // +0x1C
    long                                            MinigameCurrentFramesLeft;      // +0x20
    long                                            MinigameStartingWitnessCount;   // +0x24
    long                                            NumberOfWitnessesLeft;          // +0x28
    long                                            PendingGainedWitnessSounds;     // +0x2C
    long                                            FrameForNextGainedWitnessSound; // +0x30
    std::list<CIntelligentPointer<CThingAICreature> > AvailableWitnesses;           // +0x34
    std::list<CIntelligentPointer<CThingAICreature> > WitnessesAchieved;            // +0x38
    bool                                            WaitingForConfirmation;         // +0x3C
    bool                                            WaitingForScore;                // +0x3D
    bool                                            WaitingForTerminate;            // +0x3E
    bool                                            WaitingForNextInstructionsPage; // +0x3F
    bool                                            IsGlobalTC;                     // +0x40
    long                                            CurrentInstructionsPage;        // +0x44
    bool                                            Mountable;                      // +0x48
};

CTCTrophy::~CTCTrophy()
{
    RemoveFromGlobalTCs();
}