// CAIStateGroup_HoverHornetQueen::~CAIStateGroup_HoverHornetQueen() -- the compiler-generated destructor of CAIStateGroup_HoverHornetQueen: CircleMarkers (+0x50) then Things (+0x40) (reverse declaration order), then the inline CAIStateGroup_HoverMovementBase::~CAIStateGroup_HoverMovementBase() (its vptr store) and a tail jump to CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase()
// Leaf: ??1CAIStateGroup_HoverHornetQueen@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903383, 33 bytes, thiscall, tail `jmp`.
// Identity: CAIStateGroup_HoverHornetQueen -- proven; evidence: its only caller is the scalar deleting destructor 0x00903367 in slot 0 of vtable 0x0128D484
//   (RTTI .?AVCAIStateGroup_HoverHornetQueen@@ : CAIStateGroup_HoverMovementBase, CAIStateGroup_EnemyCreatureBase, CAIStateGroupBase, CBaseClass); FableWin.pdb names ??1CAIStateGroup_HoverHornetQueen@@UAE@XZ; members per Ego_r.pdb CAIStateGroup_HoverHornetQueen (Ego_r.pdb Things +0x40, CircleMarkers +0x54; retail +0x50 with 12-byte STLport vectors).
// Label: the manifest's "~CAIStateGroup_HoverHornetQueen" is propagated across byte-identical bodies (no CAIStateGroup_HoverHornetQueen is involved
//   except at 0x00903383).
// Callees: vector<CIntelligentPointer<CThing> >::~vector @ 0x004406D0 (twice), CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase @ 0x008FD769 (tail jmp).
// Shape: no vptr store of CAIStateGroup_HoverHornetQueen's own (the destructor is implicit: VC7.1 stores the vptr only in user-written destructors);
//   the members' destructors run in reverse order, then CAIStateGroup_HoverMovementBase's inline user-written destructor stores vtable 0x0128A1DC
//   (RTTI .?AVCAIStateGroup_HoverMovementBase@@) and the call to CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase() becomes the tail `jmp`. A size-optimised TU (odd addresses, no
//   16-byte alignment): `#pragma optimize("s", on)`. The class is constructed once (MakeOne) so its vtable, scalar
//   deleting destructor and this destructor are emitted.
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport: 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector();
    ~vector();
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                              // STLport: the 12-byte _Rb_tree (header proxy, node count, compare)
public:
    void*        _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    map();
    ~map();
};
}
class CBaseClass {                                       // RTTI root, vtable 0x01231710
public:
    virtual ~CBaseClass();                               // 0x0099A2E0 (FableWin.pdb ??1CBaseClass@@UAE@XZ)
};
#pragma pack(push, 1)
class CBaseIntelligentPointer {                          // 8 bytes, 1-aligned in retail
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CAIBrain;
class CThingCreatureBase;
class CAIStateGroupBase : public CBaseClass {            // 20 bytes
public:
    CAIBrain* PBrain;                                    // +0x04
    long      ScriptAIPriority;                          // +0x08
    void*     Name;                                      // +0x0c (CCharString)
    bool      Terminated;                                // +0x10
    unsigned char Priority;                              // +0x11
    bool      Enabled;                                   // +0x12
};
class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {   // 28 bytes
public:
    CAIStateGroup_EnemyCreatureBase();
    virtual ~CAIStateGroup_EnemyCreatureBase();          // 0x008FD769
    CIntelligentPointer<CThingCreatureBase> PTargetThing;   // +0x14
};
class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {   // vtable 0x0128A1DC
protected:
    virtual ~CAIStateGroup_HoverMovementBase() { }      // FableWin.pdb ??1CAIStateGroup_HoverMovementBase@@MAE@XZ (protected)
};
class C3DVector { public: float X, Y, Z; };
enum eWaspDirection { WASP_DIRECTION_NONE = 0 };
class CThing;
class CAIStateGroup_HoverHornetQueen : public CAIStateGroup_HoverMovementBase {
public:
    C3DVector StartPos;                                  // +0x1c
    C3DVector ArenaCentrePos;                            // +0x28
    bool      bOneTime;                                  // +0x34
    CThing*   PEmitter;                                  // +0x38
    CThing*   PEmitter2;                                 // +0x3c
    std::vector<CIntelligentPointer<CThing> > Things;   // +0x40 (~vector 0x004406D0)
    long      CurrentCirclePoint;                        // +0x4c
    std::vector<CIntelligentPointer<CThing> > CircleMarkers;   // +0x50
    long      EnemyKnockDownFrame;                       // +0x5c
    bool      TimingCritical;                            // +0x60
    eWaspDirection RouteDirection;                       // +0x64
};
void* MakeOne() { return new CAIStateGroup_HoverHornetQueen; }