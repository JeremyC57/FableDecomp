// CThingCreatureBase::~CThingCreatureBase() -- clear the mode-manager and debug-text pointers, then the members die in reverse order (the unsheathe-item pointer, the combat debug primitive handles, the body reorienter, the creature-interaction and current-action counted pointers, the queued-action list, the last-attacker pointer, the creature def, the collision debug handles) and ~CThingGameObject runs
// Leaf: ??1CThingCreatureBase@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006661B0, 348 bytes, thiscall, no frame; ends `jmp ~CThingGameObject` (0x00838A00).
// Identity (proven): the first store is vtable 0x0125AC0C (RTTI .?AVCThingCreatureBase@@ : CThingGameObject : CThingPhysical :
//   CThing ...); callers are that vtable's slot-0 ??_G 0x0066A6F0 (call at 0x0066A6F3) and the tail jumps of the two derived
//   destructors ~CThingPlayerCreature (0x006AA2CC) and ~CThingAICreature (0x00832259, the landed 0x008320C0 names this
//   address ??1CThingCreatureBase@@UAE@XZ); Ego_r.pdb has ??1CThingCreatureBase@@UAE@XZ. The catalog label
//   CThingGameObject_Finalize is a propagated guess.
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (twice), free 0x00BFEA14 (vector storage, list sentinel), each
//   CEnginePrimitiveHandle's slot-0 destructor (`push 0 ; call [vt]`), ~CThingBodyReorienter 0x00863790 + operator delete
//   0x00BFE9BC (boost::checked_delete through scoped_ptr), the counted pointers' DeleteFunc + operator delete,
//   std::_List_base<CCountedPointer<CCreatureActionBase> >::clear 0x00669B50 (out of line), the def's slot-1 Delete,
//   ~CThingGameObject 0x00838A00 (tail jump).
// Shape: the user body is only `PTCModeManager = 0; DebugText = 0;`; everything else is the implicit member destruction from
//   the real class texts -- STLport ~vector (_Destroy loop + _Vector_base free), ~_List_base (clear() out of line, as _list.c
//   has it, then the sentinel free), engine/CCountedPointer.h (release, then Data = 0 / Info = 0), engine/CDefPointer.h,
//   boost::scoped_ptr / checked_delete (Ego_r: __fastcall). First probe.
// Layout: Ego_r.pdb CThingCreatureBase at STLport sizes (Dinkumware vector 16 -> 12, list 12 -> 4): LastMessageEventICreatedID
//   +0xD0 (Ego_r 0xE8) ... DebugText +0x158 (Ego_r 0x180). engine/CThingCreatureBase.h / CThingGameObject.h flatten the vptr,
//   so CThingGameObject is a polymorphic 0xD0-byte local base; CEnginePrimitiveHandle (8 bytes, virtual dtor: Ego_r has
//   ??_GCEnginePrimitiveHandle@@UAEPAXI@Z) and CThingBodyReorienter (non-virtual ??1, size immaterial here) are reduced models.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
// ---- vector (stl/_vector.h)
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
// ---- list (stl/_list.h, stl/_list.c)
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();                                           // 0x00669B50 (this instantiation, out of line)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
} // namespace std

namespace boost {
template <class T> inline void __fastcall checked_delete(T* x)   // Ego_r ??$checked_delete@..@boost@@YIXPAV..@Z (__fastcall)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void)sizeof(type_must_be_complete);
    delete x;
}
template <class T> class scoped_ptr {
public:
    ~scoped_ptr() { boost::checked_delete(ptr); }
    T* ptr;
};
}

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CEnginePrimitiveHandle { public:                      // 8 bytes, virtual dtor (slot 0)
    virtual ~CEnginePrimitiveHandle();
    unsigned long Handle;
};
class CThingBodyReorienter { public:
    ~CThingBodyReorienter();                                // 0x00863790
    unsigned char _body[0x10];
};
#include "engine/C3DVector.h"
class CThing;
class CThingCreatureDef;
class CCreatureActionBase;
class CCreatureInteraction;
class CTCCreatureModeManager;
enum ECombatAnimationHandedness { };

class CThingGameObject { public:                            // retail 0xD0 bytes
    virtual ~CThingGameObject();                            // 0x00838A00
    unsigned char _pad_0x04[0xD0 - 0x04];
};
class CThingCreatureBase : public CThingGameObject { public:   // vtable 0x0125AC0C
    virtual ~CThingCreatureBase();
    unsigned long LastMessageEventICreatedID;                                // +0xD0
    std::vector<CEnginePrimitiveHandle> CombatCollisionDebugGraphics;        // +0xD4
    CDefPointer<const CThingCreatureDef> PDef;                               // +0xE0
    long ShotAccuracyPercentage;                                             // +0xE4
    C3DVector InitialPos;                                                    // +0xE8
    CIntelligentPointer<CThingCreatureBase> PLastAttackedByCreature;         // +0xF4
    unsigned long WFLastAttackedByCreature;                                  // +0xFC
    CCountedPointer<CCreatureActionBase> PCurrentAction;                     // +0x100
    std::list<CCountedPointer<CCreatureActionBase> > PQueuedActions;         // +0x108
    C3DVector MovementVector;                                                // +0x10C
    C3DVector HeadPosOffset;                                                 // +0x118
    long IdleCounter;                                                        // +0x124
    float TurnSpeed;                                                         // +0x128
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;              // +0x12C
    CTCCreatureModeManager* PTCModeManager;                                  // +0x134
    ECombatAnimationHandedness PreviousActionHandedness;                     // +0x138
    long PreviousActionHandednessWF;                                         // +0x13C
    boost::scoped_ptr<CThingBodyReorienter> BodyReorienter;                  // +0x140
    std::vector<CEnginePrimitiveHandle> CombatDebugGraphics;                 // +0x144
    CIntelligentPointer<CThing> PItemToUnsheatheAfterCutscene;               // +0x150
    char* DebugText;                                                         // +0x158
    bool CurrentlyFrameUpdating : 1;                                         // +0x15C
};

CThingCreatureBase::~CThingCreatureBase()
{
    PTCModeManager = 0;
    DebugText = 0;
}