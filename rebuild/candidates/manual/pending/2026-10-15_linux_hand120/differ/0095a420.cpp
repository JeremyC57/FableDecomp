// Grade: DIFFER(ours v retail) -- 97v97: this/target registers swapped (retail this=edi, target/element=esi) and the merged `return false` tail placed after the second alive test (retail: right after the first, jumped back to)
// CAIStateGroup_HoverHornetQueen::IsStillRunnable() -- still runnable while the target is alive, the first circle marker (if any) is alive and the group is not stopping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A420, length 97 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x4), CAIStateGroupBase::IsStopping @ 0x008FD010.
//
// Shape: two inlined IS_THING_ALIVE tests (GetPItem null test, then GetPItem again for the dead bit 0 of the
// thing's byte at +0x91 -- the landed 0x00930DC0 helper), the first on PTargetThing (+0x14, address kept in
// esi), the second on the front of the CircleMarkers vector (retail 12-byte STLport vector at +0x50: the
// PDB's 16-byte vectors put it at +0x54) behind a `size() > 0` test (`sub ; sar 3 ; test ; jbe`, element
// = 8-byte CIntelligentPointer, reached as `*begin()`); the tail is `return !IsStopping();`
// written `IsStopping() ? false : true` (`neg al ; sbb al,al ; inc al`; `!IsStopping()` widens to eax).
// Tried: nested-if alive helper (v1), the macro form (re-reads the vector's first pointer), this `&&`
// helper (best), a named target pointer local (no register change). The engine header CAIStateGroup_HoverHornetQueen.h is flattened
// data-only: local model with the retail offsets.
class CThingCreatureBase;

struct C3DVector { float X, Y, Z; };

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}

// Retail STLport vector<CIntelligentPointer<CThing>>: {start, finish, end_of_storage}.
struct CThingPointerVector {
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    CBaseIntelligentPointer* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    const CBaseIntelligentPointer& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    bool IsStopping();                          // 0x008FD010
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CBaseIntelligentPointer PTargetThing;       // +0x14 CIntelligentPointer<CThingCreatureBase>
};

struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_EnemyCreatureBase {
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    CThingPointerVector Things;                 // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CThingPointerVector CircleMarkers;          // +0x50
    long EnemyKnockDownFrame;                   // +0x5c
    bool TimingCritical;                        // +0x60
    long RouteDirection;                        // +0x64 eWaspDirection
    bool IsStillRunnable();
};

bool CAIStateGroup_HoverHornetQueen::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    if (CircleMarkers.size() > 0) {
        if (!IS_THING_ALIVE(CircleMarkers[0]))
            return false;
    }
    return IsStopping() ? false : true;
}
