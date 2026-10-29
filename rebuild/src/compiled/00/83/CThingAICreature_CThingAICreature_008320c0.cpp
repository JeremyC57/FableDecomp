// CThingAICreature::~CThingAICreature() -- AI creature dtor: drop the brain and leave any creature interaction, then the member dtors in reverse order (the suspender set, the brain and brain-output counted pointers, the debug primitive-handle list, the brain-name string, five creature/building thing pointers, the relationships-init counted pointer) and ~CThingCreatureBase
// Leaf: ??1CThingAICreature@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008320C0, 414 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0127293C, RTTI `.?AVCThingAICreature@@` (rtti_bases: CThingAICreature :
//   CThingCreatureBase : CThingGameObject : CThingPhysical : CThing ...); its slot 0 is the scalar deleting dtor 0x008359C0.
//   Ego_r.pdb has `??1CThingAICreature@@UAE@XZ`. The catalog label `CTCTavernGame::~CTCTavernGame` is wrong (CTCTavernGame's
//   implicit dtor is 0x005EDA50, chunk 562). The tail jump goes to 0x006661B0 (`??1CThingCreatureBase@@UAE@XZ`).
// Callees: operator delete 0x00BFE9BC (count blocks), CThingCreatureBase::RemoveFromCreatureInteraction 0x006629B0 (Ego_r
//   `?RemoveFromCreatureInteraction@CThingCreatureBase@@QAEXXZ`; its body reads PCreatureInteraction +0x12C / +0x130),
//   std::_Rb_tree<void*,void*,_Identity<void*>,less<void*>,allocator<void*> >::_M_erase 0x00834FD0 (out of line, recursive,
//   frees each node), free 0x00BFEA14 (tree header, list sentinel), std::_List_base<CEnginePrimitiveHandle>::clear
//   0x00834F90 (out of line: each element's slot-0 destructor, then the node free), ~CCharString 0x0099EAE0,
//   ~CBaseIntelligentPointer 0x00A01C10 (five times), CThingCreatureBase::~CThingCreatureBase 0x006661B0 (tail jump);
//   indirect calls: each count block's DeleteFunc on the last release.
// Layout: engine/CThingAICreature.h (retail STLport sizes, sizeof 0x1D8) gives every offset used here: RelationshipsInit
//   +0x160, HomeBuilding +0x168, WorkBuilding +0x170, Father/Mother/SpouseCreature +0x178/+0x180/+0x188,
//   OverridingBrainName +0x190, DebugRenderHandles +0x194, PCurrentBrainOutput +0x1A0, PBrain +0x1A8, Suspenders +0x1CC;
//   engine/CThingCreatureBase.h gives PCreatureInteraction +0x12C. Both headers flatten the vptr into a data member, so they
//   cannot drive a virtual destructor chain: local models with the same offsets (NOTES.md "Header debt").
// Shape: user-written dtor `PBrain.Reset(0); if (!PCreatureInteraction.IsNull()) RemoveFromCreatureInteraction();`.
//   CCountedPointer::Reset(0) inlines to the release then `Info = 0; Data = 0;` (Info first), the member dtor to the
//   release then `Data = 0; Info = 0;`. STLport ~_Rb_tree is the inline clear() then the header free.
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A (not reached: Reset(0))
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();                                           // 0x00834F90 (this instantiation)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
template <class _Tp> struct less { };
template <class _Tp> struct _Identity { };
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x0
    _Rb_tree_node_base* _M_parent;                          // +0x4
    _Rb_tree_node_base* _M_left;                            // +0x8
    _Rb_tree_node_base* _M_right;                           // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x0
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    ~_Rb_tree() { clear(); }
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
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
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
private:
    void _M_erase(_Link_type __x);                          // 0x00834FD0 (this instantiation)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set { public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
};
} // namespace std

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    bool IsNull() const { return Data == 0; }
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    static void __fastcall DeleteData(void* data);
    void Reset(T* data)
    {
        ReleaseData();
        Info = 0;
        Data = data;
        if (data)
            Info = new CCPPointerInfo(Data, DeleteData);    // (folded away here: Reset(0))
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

class CCharString { public:
    void* PStringData;                                      // +0x00
    ~CCharString();                                         // 0x0099EAE0
};
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};

class CCreatureInteraction;
class CThingCreatureBase { public:                          // retail 0x160 bytes (engine/CThingCreatureBase.h offsets)
    virtual ~CThingCreatureBase();                          // 0x006661B0
    void RemoveFromCreatureInteraction();                   // 0x006629B0
    unsigned char                           _pad_0x04[0x12C - 0x04];
    CCountedPointer<CCreatureInteraction>   PCreatureInteraction;      // +0x12C
    unsigned char                           _pad_0x134[0x160 - 0x134];
};

class CThingBuilding;
class CEnginePrimitiveHandle;
class CBrainOutput;
class CAIBrain;
class CVillageTask;
class CThingAICreature : public CThingCreatureBase { public:   // vtable 0x0127293C
    class CRelationshipsInit;
    CCountedPointer<CRelationshipsInit>         RelationshipsInit;          // +0x160
    CIntelligentPointer<CThingBuilding>         HomeBuilding;               // +0x168
    CIntelligentPointer<CThingBuilding>         WorkBuilding;               // +0x170
    CIntelligentPointer<CThingAICreature>       FatherCreature;             // +0x178
    CIntelligentPointer<CThingAICreature>       MotherCreature;             // +0x180
    CIntelligentPointer<CThingAICreature>       SpouseCreature;             // +0x188
    CCharString                                 OverridingBrainName;        // +0x190
    std::list<CEnginePrimitiveHandle>           DebugRenderHandles;         // +0x194
    unsigned long                               MoodSprite;                 // +0x198
    unsigned long                               MoodSpriteCounter;          // +0x19C
    CCountedPointer<CBrainOutput>               PCurrentBrainOutput;        // +0x1A0
    CCountedPointer<CAIBrain>                   PBrain;                     // +0x1A8
    float                                       WFTimeLastDamaged;          // +0x1B0
    CVillageTask*                               VillageTaskPerforming;      // +0x1B4
    float                                       MaxWalkingSpeed;            // +0x1B8
    float                                       MaxJoggingSpeed;            // +0x1BC
    float                                       MaxRunningSpeed;            // +0x1C0
    float                                       MaxFlyingSpeed;             // +0x1C4
    bool                                        PerformingVillageTaskFlag : 1;          // +0x1C8
    bool                                        AISuspended : 1;
    bool                                        HasInformation : 1;
    bool                                        WanderWithInformation : 1;
    bool                                        WaveWithInformation : 1;
    bool                                        ContinueAIWithInformation : 1;
    bool                                        ImmuneToAttack : 1;
    bool                                        EnableCreatureAutoPlacing : 1;
    bool                                        AllowedToFollowHero : 1;                // +0x1C9
    bool                                        RegionFollowingOverriddenFromScript : 1;
    bool                                        RespondingToFollowAndWait : 1;
    bool                                        CanBeCourted : 1;
    bool                                        CanBeMarried : 1;
    std::set<void*>                             Suspenders;                 // +0x1CC
    virtual ~CThingAICreature();
};

CThingAICreature::~CThingAICreature()
{
    PBrain.Reset(0);
    if (!PCreatureInteraction.IsNull())
        RemoveFromCreatureInteraction();
}