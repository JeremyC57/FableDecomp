// NCreatureMode::CModeThunderLightningStorm::FrameUpdate() -- advance the storm timer; once the main beams have started, spin the storm at the lerped rotation speed and lerp its radius; create the sky beam / hand glow and the lightning beams when their start times pass, update damage and the beams, and remove the mode (0x36) when the storm's lifetime is over
// Grade: DIFFER(369v375) -- VC7.1 sinks the duration subtraction below the `elapsed >= 0` test (`fcom [0.0]`); retail computes it first (`fld st(1) ; fcomp [0.0]`) and pops three values on the skip path
// Leaf: ?FrameUpdate@CModeThunderLightningStorm
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E28F0, 375 bytes, `ret`; vtable slot 3 of NCreatureMode::CModeThunderLightningStorm.
// Name: FableWin.pdb `?FrameUpdate@CModeThunderLightningStorm@NCreatureMode@@UAEXXZ`.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CreateSkyBeam @ 0x008DAA80,
//   CreateHandGlow @ 0x008DACB0, CreateLightningBeams @ 0x008DDAD0, UpdateDamage @ 0x008DD7E0, UpdateLightningBeams @ 0x008E18D0,
//   CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x31 creature-mode manager),
//   CTCCreatureModeManager::RemoveMode @ 0x0073ABA0.
// Globals: game frames per second (long) @ 0x01375550, 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CModeThunderLightningStorm, the two STLport vectors 4 bytes shorter each in retail: PSkyBeam +0x8c,
//   PCreatorEffect +0x94, PHandGlow +0x9c, PLightningStormDef +0xa4, HitThings +0xa8, MainBeamsCreated +0xb4, Timer +0xb8,
//   StormRotation +0xbc, StormRadius +0xc0. CSpecialAbilitiesThunderLightningStormDef (retail = PDB - 4): StartRadius +0x40,
//   EndRadius +0x44, SkyBeamStartTime +0x48, MainBeamsStartTime +0x4c, BeamRotationStartSpeed +0x50, BeamRotationEndSpeed +0x54,
//   TotalLifetime +0x58. CThing::Dead is bit 0 of +0x91.
// Shape: the frames-per-second float stays on the x87 stack for both divisions; the elapsed time and the main-beam duration are
//   both computed before the `>= 0` test; the mode-manager lookup result is not tested (the absent path calls through the
//   never-written out variable, as in the landed CModeIntangible::FrameUpdate 0x008E7600).
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char Dead : 1;                     // +0x91 bit 0
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
};
struct CBaseIntelligentPointer {
    void* VTable;
    void* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
__forceinline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() && !p.GetPItem()->Dead;
}
struct CTCCreatureModeManager {                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CSpecialAbilitiesThunderLightningStormDef {
    unsigned char _pad00[0x3c];
    long  NumBeams;                             // +0x3c
    float StartRadius;                          // +0x40
    float EndRadius;                            // +0x44
    float SkyBeamStartTime;                     // +0x48
    float MainBeamsStartTime;                   // +0x4c
    float BeamRotationStartSpeed;               // +0x50
    float BeamRotationEndSpeed;                 // +0x54
    float TotalLifetime;                        // +0x58
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
template <class T> struct CVector { T* First; T* Last; T* End; };
extern long g_GameFramesPerSecond;              // 0x01375550

struct CModeBase {                              // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x14];
    void FrameUpdate();                         // 0x0073AC50
};

namespace NCreatureMode {
struct CModeThunderLightningStorm : CModeBase {
    unsigned char _pad1c[0x4c];
    unsigned char Filter[0x0c];                 // +0x68 CAnimFilterOfGroup
    CVector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters;     // +0x74
    CVector<CIntelligentPointer<CThing> > LightningBeamEndParticleEmitters;  // +0x80
    CIntelligentPointer<CThing> PSkyBeam;       // +0x8c
    CIntelligentPointer<CThing> PCreatorEffect; // +0x94
    CIntelligentPointer<CThing> PHandGlow;      // +0x9c
    CDefPointer<CSpecialAbilitiesThunderLightningStormDef> PLightningStormDef;   // +0xa4
    CVector<CIntelligentPointer<CThing> > HitThings;                         // +0xa8
    bool          MainBeamsCreated;             // +0xb4
    long          Timer;                        // +0xb8
    float         StormRotation;                // +0xbc
    float         StormRadius;                  // +0xc0
    void FrameUpdate();
    void UpdateDamage();                        // 0x008DD7E0
    void UpdateLightningBeams();                // 0x008E18D0
    void CreateLightningBeams();                // 0x008DDAD0
    void CreateSkyBeam();                       // 0x008DAA80
    void CreateHandGlow();                      // 0x008DACB0
};

void CModeThunderLightningStorm::FrameUpdate()
{
    CModeBase::FrameUpdate();
    ++Timer;
    float time = (float)Timer / (float)g_GameFramesPerSecond;
    float elapsed = time - PLightningStormDef->MainBeamsStartTime;
    float duration = PLightningStormDef->TotalLifetime - PLightningStormDef->MainBeamsStartTime;
    if (elapsed >= 0.0f) {
        float fraction = elapsed / duration;
        StormRotation += (PLightningStormDef->BeamRotationStartSpeed
            + (PLightningStormDef->BeamRotationEndSpeed - PLightningStormDef->BeamRotationStartSpeed) * fraction)
            / (float)g_GameFramesPerSecond;
        StormRadius = PLightningStormDef->StartRadius
            + (PLightningStormDef->EndRadius - PLightningStormDef->StartRadius) * fraction;
    }
    if (time >= PLightningStormDef->SkyBeamStartTime) {
        if (!IsThingAlive(PSkyBeam))
            CreateSkyBeam();
        if (!IsThingAlive(PHandGlow))
            CreateHandGlow();
    }
    if (time >= PLightningStormDef->MainBeamsStartTime) {
        if (!MainBeamsCreated)
            CreateLightningBeams();
        UpdateDamage();
    }
    UpdateLightningBeams();
    if (time > PLightningStormDef->TotalLifetime) {
        CTCCreatureModeManager* modes;
        ParentThing->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x36);
    }
}
}
