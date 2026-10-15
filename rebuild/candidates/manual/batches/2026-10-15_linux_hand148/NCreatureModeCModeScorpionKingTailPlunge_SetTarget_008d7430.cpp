// NCreatureMode::CModeScorpionKingTailPlunge::SetTarget(CThingCreatureBase*) -- PTarget pointed at the target creature, TargetPos = its position
// Leaf: SetTarget
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7430, 109 bytes (`ret 4`).
// Label: the catalog's "CModeJackOfBladesScream::SetSwordObject" is a byte-twin propagation; the temporary's vtable
// 0x0123E7D8 (CIntelligentPointer<CThingCreatureBase>) and the byte-packed offsets +0xa1 / +0x89 are
// CModeScorpionKingTailPlunge's PTarget and TargetPos (Ego_r.pdb), so the name is descriptive.
// Callees: CBaseIntelligentPointer: ctor 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50, ~ 0x00A01C10;
// CThing::GetPosition @ 0x004C73D0.
// Shape: as 0x008D7370 (temporary + inline operator= with the item in a local, position copied word by word), members under `#pragma pack(1)`.
struct C3DVector { float x, y, z; };

struct CThing {
    const C3DVector& GetPosition() const;          // 0x004C73D0 (CThing::GetPosition, GOTCHAS chunk 141)
};
struct CThingCreatureBase : CThing { };

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                          // +4
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    void SetPItem(CBaseObject* item);              // 0x00A01B90
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
    T* operator->() const { return (T*)GetPItem(); }
};

struct CModeBase {
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
    unsigned char _pad08[0x14];
};

struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                    // +0x1c .. +0x68
};

namespace NCreatureMode {

#pragma pack(push, 1)
struct CModeScorpionKingTailPlunge : CModeStandBase {
    long                                    RecoveryCount;      // +0x68
    long                                    DustEffectCounter;  // +0x6c
    long                                    SpikeTime;          // +0x70
    long                                    SpikeDecisionTime;  // +0x74
    unsigned char                           _filter[0xc];       // +0x78 CAnimFilterOfGroup Filter
    CThingCreatureBase*                     PCreature;          // +0x84
    bool                                    bRecovering;        // +0x88
    C3DVector                               TargetPos;          // +0x89
    C3DVector                               TailPos;            // +0x95
    CIntelligentPointer<CThingCreatureBase> PTarget;            // +0xa1 (vtable 0x0123E7D8)
    void SetTarget(CThingCreatureBase* target);
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::SetTarget(CThingCreatureBase* target)
{
    PTarget = CIntelligentPointer<CThingCreatureBase>(target);
    TargetPos = PTarget->GetPosition();
}

}
