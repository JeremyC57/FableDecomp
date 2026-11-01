// CTCTrophy::~CTCTrophy() -- trophy component dtor: leave the global TC update list, then the members in reverse order (the WitnessesAchieved and AvailableWitnesses lists, the trophy def pointer) and ~CTCBase
// Leaf: ??1CTCTrophy
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
#pragma pack(push, 1)
class CBaseIntelligentPointer {                                          // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();                                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;                                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
#pragma pack(pop)

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
