// CThingCreatureBase::~CThingCreatureBase() -- probe
// Leaf: ??1CThingCreatureBase@@UAE@XZ
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
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};
class CEnginePrimitiveHandle { public:                      // 8 bytes, virtual dtor (slot 0)
    virtual ~CEnginePrimitiveHandle();
    unsigned long Handle;
};
class CThingBodyReorienter { public:
    ~CThingBodyReorienter();                                // 0x00863790
    unsigned char _body[0x10];
};
class C3DVector { public: float X, Y, Z; };
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
