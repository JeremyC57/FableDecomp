// CTCDParticleEmitter::EnterTimelineEnd() -- let the live emitter play out its timeline end, or kill the parent thing when there is no emitter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E05F0, length 47 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2); NParticleEngine::CParticleEmitter::EnterTimelineEnd()
// @ 0x00AED250 (sibling `jmp`); CThing::Kill(bool) @ 0x004C9B80.
// PDB CTCDParticleEmitter: PEmitter (CIntelligentPointer<NParticleEngine::CParticleEmitter>) +0x14; CTCBase
// ParentThing +0x04.
// Shape: the emitter branch tail-jumps (no arguments), the Kill(true) branch is a plain call (it needs a
// stack argument the caller does not have).
class CThing { public:
    void Kill(bool immediately);                            // 0x004C9B80
};

namespace NParticleEngine {
class CParticleEmitter { public:
    void EnterTimelineEnd();                                // 0x00AED250
};
}

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
    CThing*                 ParentThing;                    // +0x04 (CTCBase)
    bool                    ToBeRemoved;                    // +0x08
    bool                    IsInGlobalUpdateTCs;            // +0x09
    void*                   EmitterName;                    // +0x0c CCharString
    long                    ParticleTypeID;                 // +0x10
    CIntelligentPointer<NParticleEngine::CParticleEmitter> PEmitter;                       // +0x14
    void EnterTimelineEnd();
};

void CTCDParticleEmitter::EnterTimelineEnd()
{
    if (PEmitter.GetPItem() != 0)
        PEmitter.GetPItem()->EnterTimelineEnd();
    else
        ParentThing->Kill(true);
}