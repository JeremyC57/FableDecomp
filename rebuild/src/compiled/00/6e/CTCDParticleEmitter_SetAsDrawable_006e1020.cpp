// CTCDParticleEmitter::SetAsDrawable(bool drawable) -- while attached to a live thing, sync the particle-creation switch with the component's state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E1020, length 84 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2); CTCDParticleEmitter::IsParticleCreationDisabled() const
// @ 0x006E0DF0; ::SetParticleCreationDisabled(bool) @ 0x006E0690 (x2).
// Signature from ghidra_out/egor_signatures.tsv (`?SetAsDrawable@CTCDParticleEmitter@@QAEX_N@Z`); the argument is
// unused (`ret 4`).
// PDB CTCDParticleEmitter: PAttachedToThing (CIntelligentPointer<CThing>) +0x1c, bitfield byte +0x44 (bit 3
// ParticleCreationDisabled). Retail CThing: the Dead bit is bit 0 of +0x91.
// Shape: the inline "thing alive" test (non-null item, dead bit clear) guards the same switch sync as
// OnSetInLimbo 0x006E0FC0 (flag byte loaded once after the predicate's `test al,al`, tested in both arms).
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                                // +0x91 bit 0: Dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCDParticleEmitter { public:
    virtual void v00();
    unsigned char           _base[0x8];                     // CTCBase: ParentThing +4, flags +8/+9
    void*                   EmitterName;                    // +0x0c CCharString
    long                    ParticleTypeID;                 // +0x10
    CIntelligentPointer<CThing> PEmitter;                       // +0x14
    CIntelligentPointer<CThing> PAttachedToThing;               // +0x1c
    unsigned char           _body[0x44 - 0x24];             // AttachmentFlags .. PCreationParameters
    bool WaitingForParticleToLoad : 1;                      // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;                      // bit 3
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool IsParticleCreationDisabled() const;                // 0x006E0DF0
    void SetParticleCreationDisabled(bool disabled);        // 0x006E0690
    void SetAsDrawable(bool drawable);
};

void CTCDParticleEmitter::SetAsDrawable(bool drawable)
{
    if (PAttachedToThing.GetPItem() != 0 && !PAttachedToThing.GetPItem()->IsDead()) {
        if (IsParticleCreationDisabled()) {
            if (!ParticleCreationDisabled)
                SetParticleCreationDisabled(true);
        } else if (ParticleCreationDisabled) {
            SetParticleCreationDisabled(false);
        }
    }
}