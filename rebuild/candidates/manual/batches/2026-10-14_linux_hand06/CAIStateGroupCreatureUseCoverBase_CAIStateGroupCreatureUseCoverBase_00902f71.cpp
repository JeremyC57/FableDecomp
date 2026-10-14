// CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902F71, length 33 bytes.
// Callees: the cover-list vector destructor @ 0x004406D0 on CoverList (+0x28), CBaseIntelligent
// Pointer::~CBaseIntelligentPointer @ 0x00A01C10 on PCoverThing (+0x1f), then the
// CAIStateGroup_EnemyCreatureBase destructor @ 0x008FD769 (PTargetThing at +0x14, then
// CAIStateGroupBase) tail-called. Own vtable 0x0128D31C stored first (rtti-vtables.tsv).
// Member names/offsets match the engine header (CAIStateGroup_CreatureUseCoverBase.h:
// HasDiveAnim +0x1c, HasJumpOutAnims +0x1d, TimingCritical +0x1e, PCoverThing +0x1f, CoverList
// +0x28); the header's opaque byte arrays cannot be destroyed, so the members are typed here
// (packed to keep the odd +0x1f). The base destructor is modelled non-virtual so the plain
// `??1` body is the TU's largest symbol.
struct CBaseIntelligentPointer {         // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                          // +4
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
};

struct CThingPointerVector {                // 12-byte retail vector
    void* m_first; void* m_last; void* m_end;
    ~CThingPointerVector();                 // 0x004406D0
};

struct CAIStateGroup_EnemyCreatureBase {
    virtual void v00();
    ~CAIStateGroup_EnemyCreatureBase();     // 0x008FD769
    unsigned char _pad[0x18];               // +0x4..+0x1b (brain, priority, name, flags, PTargetThing)
};

#pragma pack(push, 1)
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroup_EnemyCreatureBase {   // vtable 0x0128D31C
    bool HasDiveAnim;                       // +0x1c
    bool HasJumpOutAnims;                   // +0x1d
    bool TimingCritical;                    // +0x1e
    CIntelligentPointer PCoverThing;        // +0x1f
    unsigned char _pad_0x27;                // +0x27
    CThingPointerVector CoverList;          // +0x28
    ~CAIStateGroup_CreatureUseCoverBase();
};
#pragma pack(pop)

CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase()
{
}
