// CTCDParticleEmitter::SetPEmitter(NParticleEngine::CParticleEmitter* emitter) -- adopt a new particle emitter: kill the old one (with its destruction sound), point PEmitter at the new one, copy its name and play the creation sound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E1080, length 143 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x4) / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (x2) / dtor @ 0x00A01C10,
// SendDestructionCriteriaSound(CThing*) @ 0x006E0F10 and SendCreationCriteriaSound(CThing*) @ 0x006E0E80 (file-static __fastcall
// helpers of this TU), NParticleEngine::CParticleEmitter::Kill @ 0x00AEE8C0, CCharString::operator= @ 0x0099EFB0.
// Layout: CTCDParticleEmitter as in the landed SetAlwaysUpdate 0x006E0590: EmitterName +0xC, PEmitter +0x14
// (CIntelligentPointer<NParticleEngine::CParticleEmitter>, vtable 0x0125A26C). The retail emitter's name CCharString is at +0x140.
// Shape: `PEmitter = CIntelligentPointer<CParticleEmitter>(emitter)` -- inline converting ctor (base ctor, vtable store, SetPItem)
// and inline operator= (`SetPItem(other.GetPItem())`) over a frame temporary, then its out-of-line base dtor.
class CThing;
class CCharString { public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad00[0x140];
    CCharString Name;                           // +0x140
    void Kill();                                // 0x00AEE8C0
};
}
void __fastcall SendDestructionCriteriaSound(CThing* thing);   // 0x006E0F10
void __fastcall SendCreationCriteriaSound(CThing* thing);      // 0x006E0E80
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};
struct CIntelligentPointerEmitter : CBaseIntelligentPointer {   // vtable 0x0125A26C
    virtual void v00();
    CIntelligentPointerEmitter() {}
    CIntelligentPointerEmitter(NParticleEngine::CParticleEmitter* item) { SetPItem(item); }
    CIntelligentPointerEmitter& operator=(const CIntelligentPointerEmitter& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
    NParticleEngine::CParticleEmitter* GetPItem() const { return (NParticleEngine::CParticleEmitter*)CBaseIntelligentPointer::GetPItem(); }
};
class CTCDParticleEmitter { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[0x4];
    CCharString EmitterName;                    // +0x0c
    long ParticleTypeID;                        // +0x10
    CIntelligentPointerEmitter PEmitter;        // +0x14
    void SetPEmitter(NParticleEngine::CParticleEmitter* emitter);
};

void CTCDParticleEmitter::SetPEmitter(NParticleEngine::CParticleEmitter* emitter)
{
    if (emitter) {
        if (PEmitter.GetPItem()) {
            SendDestructionCriteriaSound(ParentThing);
            PEmitter.GetPItem()->Kill();
        }
        PEmitter = CIntelligentPointerEmitter(emitter);
        EmitterName = PEmitter.GetPItem()->Name;
        SendCreationCriteriaSound(ParentThing);
    }
}