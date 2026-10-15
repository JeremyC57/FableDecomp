// NCreatureMode::CModeScorpionKingTailPlunge::OnDeactivate() -- kill the live tail emitter (+0xa9) and dust-trail emitter (+0xb1), clear Active
// Grade: DIFFER(109v109) -- esi/edi swapped: retail keeps `this` in edi and the member address in esi (as 008D7730 does), the lane gives `this` esi when it is used after the blocks (Active = false)
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7930, 105 bytes (`ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge is byte-packed after bRecovering (+0x88): TargetPos +0x89, TailPos +0x95,
// PTarget +0xa1, PTailEmitter +0xa9, PDustTrailEmitter +0xb1 (the unaligned `lea esi,[edi+0A9h]` confirms it), so the
// derived members sit under `#pragma pack(1)`.
namespace NCreatureMode {
struct CModeBase;
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10 (1 = the player creature)
    unsigned char _pad11[0x5b];
    unsigned char m_animSuspended : 1;             // +0x6c bit 0
    unsigned char m_6c_1 : 7;
    unsigned char _pad6d[0x24];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
    void Kill(bool flag);                          // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing*       m_pThing;                        // +0x04
    unsigned char _pad08[0x10];
    bool          Active;                          // +0x18
};

struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                    // +0x1c .. +0x68
    void OnDeactivate();                           // 0x00861D00
};

struct C3DVector { float x, y, z; };

#pragma pack(push, 1)
struct CModeScorpionKingTailPlunge : CModeStandBase {
    long                    RecoveryCount;         // +0x68
    long                    DustEffectCounter;     // +0x6c
    long                    SpikeTime;             // +0x70
    long                    SpikeDecisionTime;     // +0x74
    unsigned char           _filter[0xc];          // +0x78 CAnimFilterOfGroup Filter
    CThing*                 PCreature;             // +0x84
    bool                    bRecovering;           // +0x88
    C3DVector               TargetPos;             // +0x89
    C3DVector               TailPos;               // +0x95
    CBaseIntelligentPointer PTarget;               // +0xa1
    CBaseIntelligentPointer PTailEmitter;          // +0xa9
    CBaseIntelligentPointer PDustTrailEmitter;     // +0xb1
    virtual void OnDeactivate();
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::OnDeactivate()
{
    if (PTailEmitter.GetPItem() && !PTailEmitter.GetPItem()->m_dead)
        PTailEmitter.GetPItem()->Kill(true);
    if (PDustTrailEmitter.GetPItem() && !PDustTrailEmitter.GetPItem()->m_dead)
        PDustTrailEmitter.GetPItem()->Kill(true);
    Active = false;
}
}
