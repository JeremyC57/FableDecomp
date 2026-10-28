// CAIStateGroup_ScorpionKing::CAIStateGroup_ScorpionKing() -- the CAIStateGroupBase constructor, the vtable, then the arena-centre pointer, the empty trap-thing vector and the enemy pointer
// Leaf: ??0CAIStateGroup_ScorpionKing@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903177, 63 bytes (packed state-group factory TU), thiscall, `ret`.
// Identity (proven): it stores vtable 0x0128D5B0 (rtti-vtables.tsv: CAIStateGroup_ScorpionKing) after the CAIStateGroupBase
//   constructor 0x008FD255, and its one reference is the factory Alloc at 0x00903424 (`push 40h ; call new ; jmp 0x00903177`);
//   Ego_r.pdb ??0CAIStateGroup_ScorpionKing@@QAE@XZ; members PArenaCentre (CIntelligentPointer<CThing>, +0x14), TrapThings
//   (vector<CIntelligentPointer<CThing> >, +0x1C), bIsRangeAttack +0x28, ChargeCount +0x2C, PEnemy
//   (CIntelligentPointer<CThingCreatureBase>, +0x30), EnemyKnockDownFrame +0x38, HealthAtLastTailStrike +0x3C: retail size
//   0x40 (Ego_r 0x44 minus 4 for the STLport vector) = the Alloc's `push 40h`.
// Label correction: the catalog's "CAIStateGroup_Guildmaster::CAIStateGroup_Guildmaster" is wrong (Guildmaster has no vector
//   at +0x1C and stores another vtable).
// Callees: CAIStateGroupBase::CAIStateGroupBase() 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10
//   (twice); vtables CIntelligentPointer<CThing> 0x01238C6C and CIntelligentPointer<CThingCreatureBase> 0x0123E7D8.
// Shape: the landed CAIStateGroup_MinionReactToCriminal ctor 0x008FEC9C model; the STLport vector's default ctor gives the
//   three zero stores from one xor'ed register between the two pointer members.
// Header debt: engine/CAIStateGroupBase.h is a flat layout without methods; modelled locally.
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
};
}
class CThing;
class CThingCreatureBase;
class CAIStateGroupBase {                                  // retail 0x14 bytes
public:
    CAIStateGroupBase();                                   // 0x008FD255
    virtual ~CAIStateGroupBase();
    unsigned char _pad04[0x10];                            // PBrain, ScriptAIPriority, Name, Terminated, Priority, Enabled
};
class CAIStateGroup_ScorpionKing : public CAIStateGroupBase {
public:
    CAIStateGroup_ScorpionKing();
    virtual ~CAIStateGroup_ScorpionKing();
    CIntelligentPointer<CThing>                   PArenaCentre;           // +0x14
    std::vector<CIntelligentPointer<CThing> >     TrapThings;             // +0x1C
    bool                                          bIsRangeAttack;         // +0x28
    long                                          ChargeCount;            // +0x2C
    CIntelligentPointer<CThingCreatureBase>       PEnemy;                 // +0x30
    long                                          EnemyKnockDownFrame;    // +0x38
    float                                         HealthAtLastTailStrike; // +0x3C
};
CAIStateGroup_ScorpionKing::CAIStateGroup_ScorpionKing()
{
}