// CTCAssassinRush::CancelForCutscene() -- when the assassin trail is alive, clear its particle emitter's live particles and decals; the displacement trail's emitter is only looked up
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AA5C0, length 225 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
// (inlined GetInterface for CTCDParticleEmitter, type 0x4B = bit 11 of CThing+0x28), NParticleEngine::CParticleEmitter::
// ClearAllParticles(CIEngine*) @ 0x00AED140 and ClearAllDecals(CIEngine*) @ 0x00AED1E0 (both with 0).
// Layout: Ego_r.pdb CTCAssassinRush AssassinTrail +0x18, AssassinDisplacementTrail +0x20; CTCDParticleEmitter PEmitter +0x14
// (landed CTCMultiArrow::CancelForCutscene 0x007CC190 model).
// Shape: per trail the inline IsThingAlive (GetPItem, null, Dead bit), an emitter pointer preset to 0 (`xor edi,edi`) filled by
// the unchecked out-parameter GetInterface, then the emitter's PEmitter; for the displacement trail retail keeps only the
// GetPItem call (its result unused).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
class CIEngine;
namespace NParticleEngine {
class CParticleEmitter { public:
    void ClearAllParticles(CIEngine* engine);   // 0x00AED140
    void ClearAllDecals(CIEngine* engine);      // 0x00AED1E0
};
}
struct CIntelligentPointerEmitter : CBaseIntelligentPointer {
    NParticleEngine::CParticleEmitter* GetPItem() const { return (NParticleEngine::CParticleEmitter*)CBaseIntelligentPointer::GetPItem(); }
};
class CTCDParticleEmitter { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    void* EmitterName;                          // +0x0c CCharString
    long ParticleTypeID;                        // +0x10
    CIntelligentPointerEmitter PEmitter;        // +0x14
    unsigned char _pad1c[0x44 - 0x1c];
    unsigned char Flags44;                      // +0x44 WaitingForParticleToLoad .. Moving
    bool AttachedToThing : 1;                   // +0x45 bit 0
    bool AlwaysUpdate : 1;                      // +0x45 bit 1
    bool Asleep : 1;                            // +0x45 bit 2
};
class CTCAssassinRush {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x18 - 0x08];
    CBaseIntelligentPointer AssassinTrail;      // +0x18 (vptr + item)
    CBaseIntelligentPointer AssassinDisplacementTrail;   // +0x20
    void CancelForCutscene();
};

void CTCAssassinRush::CancelForCutscene()
{
    if (IsThingAlive(AssassinTrail)) {
        CTCDParticleEmitter* trail = 0;
        AssassinTrail.GetPItem()->GetInterface(&trail, 0x4b);
        NParticleEngine::CParticleEmitter* emitter = trail->PEmitter.GetPItem();
        if (emitter) {
            emitter->ClearAllParticles(0);
            emitter->ClearAllDecals(0);
        }
    }
    if (IsThingAlive(AssassinDisplacementTrail)) {
        CTCDParticleEmitter* trail = 0;
        AssassinDisplacementTrail.GetPItem()->GetInterface(&trail, 0x4b);
        trail->PEmitter.GetPItem();
    }
}