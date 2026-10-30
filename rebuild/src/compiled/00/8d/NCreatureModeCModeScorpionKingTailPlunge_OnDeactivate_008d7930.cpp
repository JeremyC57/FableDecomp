// NCreatureMode::CModeScorpionKingTailPlunge::OnDeactivate() -- kill the live tail emitter (+0xa9) and dust-trail emitter (+0xb1), clear Active
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7930, 105 bytes (`ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge is byte-packed after bRecovering (+0x88): TargetPos +0x89, TailPos +0x95,
// PTarget +0xa1, PTailEmitter +0xa9, PDustTrailEmitter +0xb1 (the unaligned `lea esi,[edi+0A9h]` confirms it), so the
// derived members sit under `#pragma pack(1)`.
#pragma optimize("s", on)   // size-optimised TU (retry wave 2 sweep: /O2 gives the esi/edi swap, /Os retail's this-in-edi)
// Global classes (Ego_r.pdb spellings): CThing::Kill ?Kill@CThing@@QAEX_N@Z, CBaseIntelligentPointer::GetPItem
// ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ, CTCCreatureModeManager::CModeBase, CModeStandBase.
class CBaseObject;
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
    void Kill(bool flag);                          // 0x004C9B80
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;                 // 0x00A01B50
    void* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }   // inline in retail (the /Os TU calls 0x00A01B50 directly)
};
class CTCCreatureModeManager {
public:
    class CModeBase {                              // vtable 0x012673CC
    public:
        virtual ~CModeBase();
        CThing*       m_pThing;                    // +0x04
        unsigned char _pad08[0x10];
        bool          Active;                      // +0x18
    };
};
class CModeStandBase : public CTCCreatureModeManager::CModeBase {
public:
    unsigned char _pad1c[0x4c];                    // +0x1c .. +0x68
};
class C3DVector { public: float X, Y, Z; };

namespace NCreatureMode {
#pragma pack(push, 1)
class CModeScorpionKingTailPlunge : public CModeStandBase {
public:
    long                    RecoveryCount;         // +0x68
    long                    DustEffectCounter;     // +0x6c
    long                    SpikeTime;             // +0x70
    long                    SpikeDecisionTime;     // +0x74
    unsigned char           _filter[0xc];          // +0x78 CAnimFilterOfGroup Filter
    CThing*                 PCreature;             // +0x84
    bool                    bRecovering;           // +0x88
    C3DVector               TargetPos;             // +0x89
    C3DVector               TailPos;               // +0x95
    CIntelligentPointer<CThing> PTarget;             // +0xa1
    CIntelligentPointer<CThing> PTailEmitter;          // +0xa9
    CIntelligentPointer<CThing> PDustTrailEmitter;     // +0xb1
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