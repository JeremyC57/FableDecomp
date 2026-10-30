// CAnimateThing::~CAnimateThing() -- animation player dtor: the member dtors in reverse order (the anim-from-file path, the four blend states, the delayed animation and its queue) and the current complete animation
// Leaf: ??1CAnimateThing@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070BA90, 204 bytes, tail jump to the first member's destructor.
// Identity: Ego_r.pdb has `??1CAnimateThing@@QAE@XZ` (CAnimateThing has no vtable: its Ego_r layout starts with CurrentAnim at +0),
//   bound so; its callers 0x0070B5C3 / 0x0070BB63 / 0x0070BB93 sit in the same TU. The catalog label is right.
// Callees: ~CWideString 0x0099B510 (AnimFromFileInfo.PathName, the inline implicit ~CAnimFromFileInfo), ~CBaseClass 0x0099A2E0
//   (each C3DAnimationBlendState's base, out of line), STLport std::_List_base<CAnimSet>::clear 0x0070B8A0 (frees each node's
//   InternalAlphas / Anims blocks at node +0x18 / +0x0C = CAnimSet +0x10 / +0x04, then the node) inside the inline
//   ~CDelayedAnimation, std::_List_base<CAnimateThing::CDelayedAnimation>::clear 0x0070BA00 (its nodes run that clear on
//   node +8), free 0x00BFEA14 (list header nodes), CAnimateThing::CCompleteAnimation::~CCompleteAnimation 0x0070B930 (tail
//   jump: it destroys its two list<CAnimSet> at +0x58/+0x54 and the MainAnimSet vectors at +0x10/+0x04). Virtual call: each
//   blend state's shared data CIVCountedPointeeBase::Delete (slot 1) on the last reference.
// Layout: Ego_r.pdb CAnimateThing (0xE4) with retail STLport sizes (list 4, vector 0xC): CurrentAnim +0x00 (CCompleteAnimation
//   0x60: MainAnimSet 0x54, TransitionAnimSets +0x54, CombinationAnims +0x58, MainAnimCanTransitionOut +0x5C), DelayedAnimQueue
//   +0x60, DelayedAnim +0x64 (CDelayedAnimation = one list), StartAnimState +0x68, EndAnimState +0x70, LastStartAnimState
//   +0x78, LastEndAnimState +0x80, Scale +0x88 ... ParentThing +0xA0, AnimFromFileInfo +0xA4 (PathName +0xA8). No engine header.
// Shape: no user code; C3DAnimationBlendState's dtor is implicit (no vptr store), its CIVCountedPointer member releases with
//   the pointer read once into a local (`dec [ecx+4] ; jne`, Object = 0), then ~CBaseClass. Retail destroys each blend state
//   through `lea edi,[esi+x]` with ebx = 0 for the null stores.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x0: the header node
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
} // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class C3DAnimationBlendState : public CBaseClass { public:   // vtable + shared data pointer, 8 bytes
    class CSharedData : public CIVCountedPointeeBase { public:
    };
    CIVCountedPointer<CSharedData> Data;                    // +0x04
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
class CAnimFromFileInfo { public:                           // 0x18 bytes
    bool        Active;                                     // +0x00
    CWideString PathName;                                   // +0x04
    float       SegmentStartTime;                           // +0x08
    float       SegmentEndTime;                             // +0x0C
    float       Duration;                                   // +0x10
    long        KeyFrameCount;                              // +0x14
};
class CTransitionBlendAlpha;
class CAnimateThing;
enum EAnimUpdateMode { };                                   // values not needed here
class CAnimSet { public:                                    // retail 0x54 bytes
    EAnimUpdateMode                         UpdateMode;                   // +0x00
    std::vector<short>                      Anims;                        // +0x04
    std::vector<CTransitionBlendAlpha>      InternalAlphas;               // +0x10
    float                                   CurrentPercentageComplete;    // +0x1C
    float                                   LastPercentageComplete;       // +0x20
    float                                   Speed;                        // +0x24
    unsigned long                           FrameInternalAlphasUpdated;   // +0x28
    float                                   CurrentAlpha;                 // +0x2C
    float                                   LastAlpha;                    // +0x30
    float                                   LastUpdateChange;             // +0x34
    float                                   ChangeAlphaBy;                // +0x38
    CAnimateThing*                          PAnimator;                    // +0x3C
    unsigned long                           Flags;                        // +0x40
    unsigned long                           TypeFlags;                    // +0x44
    long                                    AnimDelay;                    // +0x48
    long                                    Priority;                     // +0x4C
    unsigned char                           Type;                         // +0x50
    bool                                    Looping : 1;                  // +0x51
    bool                                    Finished : 1;
    bool                                    Forwards : 1;
    bool                                    HadUpdate : 1;
    bool                                    OwnedByMainAnim : 1;
};
class CThing;

class CAnimateThing { public:
    class CCompleteAnimation { public:                      // retail 0x60 bytes
        CAnimSet                    MainAnimSet;            // +0x00
        std::list<CAnimSet>         TransitionAnimSets;     // +0x54
        std::list<CAnimSet>         CombinationAnims;       // +0x58
        bool                        MainAnimCanTransitionOut;   // +0x5C
        ~CCompleteAnimation();                              // 0x0070B930
    };
    class CDelayedAnimation { public:                       // retail 4 bytes
        std::list<CAnimSet>         CombinationAnims;       // +0x00
    };
    CCompleteAnimation                  CurrentAnim;                // +0x00
    std::list<CDelayedAnimation>        DelayedAnimQueue;           // +0x60
    CDelayedAnimation                   DelayedAnim;                // +0x64
    C3DAnimationBlendState              StartAnimState;             // +0x68
    C3DAnimationBlendState              EndAnimState;               // +0x70
    C3DAnimationBlendState              LastStartAnimState;         // +0x78
    C3DAnimationBlendState              LastEndAnimState;           // +0x80
    float                               Scale;                      // +0x88
    float                               ChangeAlphaBy;              // +0x8C
    long                                AnimDelay;                  // +0x90
    bool                                Interruptable : 1;          // +0x94
    unsigned long                       CurrentAnimRefreshStamp;    // +0x98
    unsigned long                       LastAnimRefreshStamp;       // +0x9C
    CThing*                             ParentThing;                // +0xA0
    CAnimFromFileInfo                   AnimFromFileInfo;           // +0xA4
    ~CAnimateThing();
};

CAnimateThing::~CAnimateThing()
{
}