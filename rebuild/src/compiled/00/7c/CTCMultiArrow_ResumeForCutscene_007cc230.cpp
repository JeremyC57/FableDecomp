// CTCMultiArrow::ResumeForCutscene() -- after a cancelled cutscene: wake every ammo effect's particle emitter (clear its Asleep bit)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC230, length 118 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the ammo effect), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface for CTCDParticleEmitter, type 0x4B = bit 11 of CThing+0x28).
// Layout: PDB CTCMultiArrow with the retail 12-byte vector: AmmoEffects +0x30 (vector<CIntelligentPointer<CThing>>, 8-byte elements),
// BowEffect +0x3C, CutSceneCancelled +0x44 (PDB +0x48); CTCDParticleEmitter PEmitter +0x14, Asleep = bit 2 of +0x45 (landed 0x006E0590).
// Shape: `for (long i = 0; i < (long)size(); ++i)` (signed `test eax,0FFFFFFF8h ; jle`); the emitter pointer is preset to 0 and the
// GetInterface result is not tested (the bit is set on a null component when the interface is missing, as retail does).
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
    void Kill(bool immediately);                // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CRetailVector {        // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CIEngine;
namespace NParticleEngine {
struct CParticleEmitter {
    void ClearAllParticles(CIEngine* engine);   // 0x00AED140
    void ClearAllDecals(CIEngine* engine);      // 0x00AED1E0
};
}
struct CIntelligentPointerEmitter : CBaseIntelligentPointer {
    NParticleEngine::CParticleEmitter* GetPItem() const { return (NParticleEngine::CParticleEmitter*)CBaseIntelligentPointer::GetPItem(); }
};
struct CTCDParticleEmitter {
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
struct CTCMultiArrow {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x30 - 0x08];
    CRetailVector<CBaseIntelligentPointer> AmmoEffects;   // +0x30
    CBaseIntelligentPointer BowEffect;          // +0x3c
    bool CutSceneCancelled;                     // +0x44
    void ResumeForCutscene();
};

void CTCMultiArrow::ResumeForCutscene()
{
    if (CutSceneCancelled) {
        CutSceneCancelled = false;
        for (long i = 0; i < (long)AmmoEffects.size(); ++i) {
            CTCDParticleEmitter* emitter = 0;
            AmmoEffects[i].GetPItem()->GetInterface(&emitter, 0x4b);
            emitter->Asleep = false;
        }
    }
}