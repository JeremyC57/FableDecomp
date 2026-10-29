// CTCMultiArrow::CancelForCutscene() -- once per cutscene: put every ammo effect's particle emitter to sleep (Asleep bit) and clear its live particles and decals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC190, length 154 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the ammo effect, then the emitter's PEmitter), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCDParticleEmitter, type 0x4B = bit 11 of CThing+0x28),
// NParticleEngine::CParticleEmitter::ClearAllParticles(CIEngine*) @ 0x00AED140 and ClearAllDecals(CIEngine*) @ 0x00AED1E0 (both 0).
// Layout: PDB CTCMultiArrow with the retail 12-byte vector: AmmoEffects +0x30 (vector<CIntelligentPointer<CThing>>, 8-byte elements),
// BowEffect +0x3C, CutSceneCancelled +0x44 (PDB +0x48); CTCDParticleEmitter PEmitter +0x14, Asleep = bit 2 of +0x45 (landed 0x006E0590).
// Shape: `for (long i = 0; i < (long)size(); ++i)` (signed `test eax,0FFFFFFF8h ; jle`); the emitter pointer is preset to 0 and the
// GetInterface result is not tested (the bit is set on a null component when the interface is missing, as retail does).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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
class CTCMultiArrow { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x30 - 0x08];
    CRetailVector<CBaseIntelligentPointer> AmmoEffects;   // +0x30
    CBaseIntelligentPointer BowEffect;          // +0x3c
    bool CutSceneCancelled;                     // +0x44
    void CancelForCutscene();
};

void CTCMultiArrow::CancelForCutscene()
{
    if (!CutSceneCancelled) {
        CutSceneCancelled = true;
        for (long i = 0; i < (long)AmmoEffects.size(); ++i) {
            CTCDParticleEmitter* emitter = 0;
            AmmoEffects[i].GetPItem()->GetInterface(&emitter, 0x4b);
            emitter->Asleep = true;
            NParticleEngine::CParticleEmitter* particles = emitter->PEmitter.GetPItem();
            if (particles) {
                particles->ClearAllParticles(0);
                particles->ClearAllDecals(0);
            }
        }
    }
}