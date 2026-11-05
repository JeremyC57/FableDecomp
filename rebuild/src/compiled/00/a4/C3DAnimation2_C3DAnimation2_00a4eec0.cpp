// C3DAnimation2::~C3DAnimation2() -- free the animation's movable-resource block back to its memory pool, then destroy InstanceList, MovementTrackData, MovementTrack and Allocator, the inline ~CMovableResource and ~C3DAnimationStats
// Leaf: ??1C3DAnimation2@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4EEC0, 203 bytes, tail jump to ~C3DAnimationStats 0x00A4EB50; sole caller the scalar
//   deleting dtor 0x00A555C0 (slot 0 of C3DAnimation2's primary vtable 0x0129D548; the CMovableResource-in-C3DAnimation2
//   vtable 0x0129D534 slot 0 is the -0x2C thunk 0x00A55400 that jumps to it).
// Identity (proven): rtti-vtables.tsv / the RTTI COL at 0x0129D548-4 (.?AVC3DAnimation2@@: C3DAnimationStats : CBaseObject :
//   CBaseClass at 0, CMovableResource at mdisp 0x2C); the body stores both C3DAnimation2 vtables first (user dtor) and resets
//   CMovableResource's own vtable 0x0129B860 before the tail jump. Ego_r.pdb carries ??1C3DAnimation2@@UAE@XZ. The tail target
//   0x00A4EB50 stores C3DAnimationStats' vtable 0x0129D514 (slot 0 ??_G 0x00A555A0) = ??1C3DAnimationStats@@UAE@XZ.
//   The catalog label (CEditWorld::~CEditWorld) is propagated and wrong.
// Layout: Ego_r.pdb C3DAnimation2 with retail STLport sizes (engine/C3DAnimation2.h has the same offsets but is a flattened
//   pack(1) record with no bases or methods, so it cannot model the two-vtable class; local model, NOTES.md "Header debt"):
//   C3DAnimationStats 0x2c, CMovableResource vptr +0x2c, Allocator +0x30, MemoryPool +0x38, MemoryHandle +0x3c,
//   MovementTrack +0x40, MovementTrackData +0x48, ... InstanceList +0x78 (STLport list, 4 bytes), 0x7c.
// Layout: engine/CMovableResource.h and engine/CMovableResourceMemoryPool.h are flattened pack(1) records (a void* vptr slot /
//   byte arrays, no virtual functions or methods), so CMovableResource (polymorphic, own vtable 0x0129B860, inline dtor) and
//   the FreeResource member are restated locally (NOTES.md "Header debt").
// Callees: CMovableResourceMemoryPool::FreeResource(long) 0x00A62ED0 (Ego_r ?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z),
//   STLport _List_base<CIntelligentPointer<C3DAnimation2> >::clear 0x00A53300 (out of line, _list.c), free 0x00BFEA14,
//   CCountedPointer releases inline (DeleteFunc fastcall, operator delete 0x00BFE9BC), ~C3DAnimationStats 0x00A4EB50.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_pair.h, stl/_function_base.h, stl/_tree.h, stl/_list.h + _list.c (the
// members this row needs; the malloc-backed allocator of this build: deallocate = free 0x00BFEA14)
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
protected:
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class multimap {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
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
}
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    bool operator<(const CCharString&) const;
    CCharStringData* PStringData;
};
#include "engine/C3DVector.h"
class CMatrix3x4;
class CQuaternion;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
enum EAnimationType { };
class C3DAnimationStats : public CBaseObject {
public:
    virtual ~C3DAnimationStats();                                        // 0x00A4EB50
    virtual void GetDeltaMovement(CMatrix3x4&, float, float) const;
    virtual C3DVector GetMovementAtTime(float) const;
    virtual C3DVector GetMovementOffsetAtTime(float) const;
    virtual CQuaternion GetRotationAtTime(float) const;
    virtual bool HasRotation() const;
    EAnimationType                       AnimType;                       // +0x08
    float                                Duration;                       // +0x0c
    bool                                 LoopFlag;                       // +0x10
    bool                                 MovementFlag;                   // +0x11
    C3DVector                            MovementVector;                 // +0x14
    std::multimap<CCharString, float>    TimingEvents;                   // +0x20
};
class CMovableResource {
public:
    virtual ~CMovableResource() { }                                      // inline in Ego_r (only FableWin lists ??1CMovableResource)
    virtual void RelocateData(long, void*, void*, long) = 0;
    virtual bool CanRelocateData(long) const;
    virtual unsigned long Verify() const;
};
class CMovableResourceMemoryPool {
public:
    void FreeResource(long);                                             // 0x00A62ED0
};
class CSimpleMemoryPool;
class C3DAnimationSequence;
class C3DAnimationSequenceData;
class C3DAnimation2 : public C3DAnimationStats, public CMovableResource {
public:
    C3DAnimation2();
    virtual ~C3DAnimation2();
    virtual void GetDeltaMovement(CMatrix3x4&, float, float) const;
    virtual C3DVector GetMovementAtTime(float) const;
    virtual C3DVector GetMovementOffsetAtTime(float) const;
    virtual CQuaternion GetRotationAtTime(float) const;
    virtual bool HasRotation() const;
    virtual void RelocateData(long, void*, void*, long);
    virtual bool CanRelocateData(long) const;
    CCountedPointer<CSimpleMemoryPool>        Allocator;                 // +0x30
    CMovableResourceMemoryPool*               MemoryPool;                // +0x38
    long                                      MemoryHandle;              // +0x3c
    CCountedPointer<C3DAnimationSequence>     MovementTrack;             // +0x40
    CCountedPointer<C3DAnimationSequenceData> MovementTrackData;         // +0x48
    float                                     SamplesPerSecond;          // +0x50
    unsigned long                             FrameCount;                // +0x54
    unsigned long                             SequenceCount;             // +0x58
    C3DAnimationSequence*                     Sequences;                 // +0x5c
    unsigned long                             SequenceDataBufferSize;    // +0x60
    C3DAnimationSequenceData*                 SequenceDataBuffer;        // +0x64
    bool*                                     BoneMask;                  // +0x68
    bool*                                     RigidBoneMask;             // +0x6c
    C3DVector*                                RigidBonePositions;        // +0x70
    bool                                      LockedFromMemMemoryRelocation; // +0x74
    std::list<CIntelligentPointer<C3DAnimation2> > InstanceList;         // +0x78
};
C3DAnimation2::~C3DAnimation2()
{
    if (MemoryPool && MemoryHandle != -1)
        MemoryPool->FreeResource(MemoryHandle);
}