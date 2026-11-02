// NCreatureMode::CModeOutOfActionBase::~CModeOutOfActionBase() -- store the class vtable; if the mode's emitter thing still exists and is not dead, kill it; then destroy the PEmitter pointer and run ~CModeStandBase
// Leaf: ??1CModeOutOfActionBase@NCreatureMode@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7580, 72 bytes, thiscall, tail `jmp ~CModeStandBase`.
// Identity (proven): (1) it stores the NCreatureMode::CModeOutOfActionBase vtable 0x0126743C (rtti-vtables.tsv; the landed
//   CModeStunned ctor 0x00740160 builds the same base with that vtable) first; its callers are the class's own ??_G 0x0073DA30
//   and the derived scalar deleting destructors 0x00740220 / 0x00740310 (CModeStunned / CModeDazed); (2) Ego_r.pdb / FableWin.pdb
//   ??1CModeOutOfActionBase@NCreatureMode@@UAE@XZ, and Ego_r's CModeOutOfActionBase = CModeStandBase (0x68) + RecoveryCount +0x68,
//   Recoverable +0x6C, PEmitter (CIntelligentPointer<CThing>) +0x6D: the unaligned `lea esi,[edi+6Dh]` here.
//   The catalog label "CModeOutOfActionBase::~CModeOutOfActionBase" lacks the NCreatureMode namespace.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (Ego_r ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ; three
//   calls: CIntelligentPointer<CThing>::GetPItem is inline over it), CThing::Kill(bool) 0x004C9B80 (Ego_r ?Kill@CThing@@QAEX_N@Z),
//   CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the member's implicit CIntelligentPointer<CThing> dtor stores no
//   vtable), CModeStandBase::~CModeStandBase 0x0073D860 (Ego_r ??1CModeStandBase@@UAE@XZ, global namespace).
// Shape: `if (PEmitter.GetPItem() && !PEmitter.GetPItem()->IsDead()) PEmitter.GetPItem()->Kill(true);` -- IsDead (FableWin
//   ?IsDead@CThing@@QBE_NXZ, inline in release) tests CThing::Dead (+0x91 bit 0, Ego_r +0xA1).
// Layout: CModeStandBase is engine/CModeStandBase.h's 0x68 bytes, but that header is flattened (vptr as a data member), so a
//   local polymorphic base stands in; the out-of-action members are byte-packed (pack(1), PDB offset 109 = +0x6D).
class CBaseObject;
class CBaseObjectPointer;

class CThing {                                              // local model: only the flag byte is read (no engine/CThing.h)
public:
    virtual ~CThing();
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                                 // +0x91 bit 0 (Ego_r +0xA1)
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    bool IsDead() const { return Dead; }
    void Kill(bool immediately);                            // 0x004C9B80
};

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CModeStandBase {                                      // local polymorphic stand-in for engine/CModeStandBase.h (0x68 bytes)
public:
    virtual ~CModeStandBase();                              // 0x0073D860
    unsigned char _body[0x68 - 0x04];
};

namespace NCreatureMode {

#pragma pack(push, 1)
class CModeOutOfActionBase : public CModeStandBase {        // vtable 0x0126743C
public:
    virtual ~CModeOutOfActionBase();
    long                        RecoveryCount;              // +0x68
    bool                        Recoverable;                // +0x6C
    CIntelligentPointer<CThing> PEmitter;                   // +0x6D
};
#pragma pack(pop)

CModeOutOfActionBase::~CModeOutOfActionBase()
{
    if (PEmitter.GetPItem() && !PEmitter.GetPItem()->IsDead())
        PEmitter.GetPItem()->Kill(true);
}

} // namespace NCreatureMode