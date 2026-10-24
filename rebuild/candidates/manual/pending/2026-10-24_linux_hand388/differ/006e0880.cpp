// CTCDParticleEmitter::Create(long particleTypeID, const C3DVector& position, bool forceParticleToLoad) -- create a level-persistent particle-emitter thing on the map under the position, construct its driver component and activate it; null when the position is on no map, or when a forced load finds no emitter (the thing is then killed)
// Grade: DIFFER(218v228) -- register swap: retail keeps the NULL result in edi and the map number in esi (thing in esi, separate `mov eax,edi` epilogue for the no-map return); ours puts the NULL/thing web in esi and the map in edi, merging the two returns
// Leaf: ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E0880, 228 bytes (240 with int3 padding), static `__fastcall` (ecx = particleTypeID, edx = position), `ret 4`.
// Identity: Ego_r.pdb name set `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z`; 154 callers; the by-name
//   overload 0x006E0970 forwards here. The catalog's bare "Create" is right.
// Callees: CWorld::GetWorldMap (world virtual slot 13, +0x34), CWorldMap::IsLoadingAnyMap() const @ 0x004FB4A0 (Ego_r name
//   set; a forwarder to the loader's list), CWorldMap::GetMapNumberAt(const C3DVector&) const (virtual slot 16, +0x40),
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CGameDefinitionManager::Get() @ 0x0044C6B0,
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CThing::Create(long defIndex, long player,
//   long map, bool levelPersistent) @ 0x004CA1D0 (landed), CTCDParticleEmitter::Construct(long, const C3DVector&, bool, bool)
//   @ 0x006DFE70, CThing::DoActivationSequence @ 0x004C9CA0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool)
//   @ 0x004C9B80.
// Globals: g_pMainGameComponent @ 0x013B86A0 (world +0x24), g_pThingManager @ 0x013B8A1C (world map +0x28: the PDB
//   WorldMap +0x34 sits 0xC lower in retail, like DefinitionManager +0x20), the def name "PARTICLE_EMITTER_NORMAL" @ 0x0125FF8C.
// Layout: retail CThing ThingTypeFlags +0x28 (bit 11 = has a driver), PTCDriver +0x5C (the PDB +0x6C/+0x7C sit 0x10 lower,
//   as the landed CTCDParticleEmitter_Create_006e11f0); CTCDParticleEmitter PEmitter +0x14 (engine Ego_r layout).
// Shape: a by-reference CCharString conversion temporary (re-addressed with `lea`); the driver pointer is a zero-initialised
//   local assigned under the flag test (`xor edi,edi ; test ah,8 ; je`; the ternary gives `je ; mov ; jmp ; xor`); the forced-load failure kills the thing and returns 0.
class C3DVector { public: float X, Y, Z; };

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& position) const;   // slot 16 (+0x40)
    bool IsLoadingAnyMap() const;                           // 0x004FB4A0
};

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();                       // slot 13 (+0x34)
};

class CMainGameComponent {
public:
    unsigned char _pad00[0x24];
    CWorld* World;                                          // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

class CThingManager {
public:
    unsigned char _pad00[0x28];
    CWorldMap* WorldMap;                                    // +0x28
};
extern CThingManager* g_pThingManager;                      // 0x013B8A1C

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PItem;                                            // +0x04
    void* GetPItem() const;                                 // 0x00A01B50
};

class CTCDParticleEmitter;

class CThing {
public:
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                            // 0x004C9CA0
    void Kill(bool immediately);                            // 0x004C9B80
    unsigned char _pad00[0x28];
    unsigned long ThingTypeFlags;                           // +0x28 (bit 11: has a driver)
    unsigned char _pad2c[0x5c - 0x2c];
    CTCDParticleEmitter* PTCDriver;                         // +0x5C
};

class CTCDParticleEmitter {
public:
    virtual void v00();
    unsigned char _base04[0x0c - 0x04];
    CCharString EmitterName;                                // +0x0C
    long ParticleTypeID;                                    // +0x10
    CBaseIntelligentPointer PEmitter;                       // +0x14
    static CThing* __fastcall Create(long particleTypeID, const C3DVector& position, bool forceParticleToLoad);
private:
    void Construct(long particleTypeID, const C3DVector& position, bool independantObject, bool forceParticleToLoad);   // 0x006DFE70
};

CThing* __fastcall CTCDParticleEmitter::Create(long particleTypeID, const C3DVector& position, bool forceParticleToLoad)
{
    if (!forceParticleToLoad && g_pMainGameComponent->World->GetWorldMap()->IsLoadingAnyMap())
        forceParticleToLoad = true;
    long map;
    CThing* thing = 0;
    map = g_pThingManager->WorldMap->GetMapNumberAt(position);
    if (map > 0) {
        long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("PARTICLE_EMITTER_NORMAL");
        thing = CThing::Create(defIndex, 4, map, true);
        CTCDParticleEmitter* emitter = 0;
        if (thing->ThingTypeFlags & 0x800)
            emitter = thing->PTCDriver;
        emitter->Construct(particleTypeID, position, false, forceParticleToLoad);
        thing->DoActivationSequence();
        if (!emitter->PEmitter.GetPItem() && forceParticleToLoad) {
            thing->Kill(false);
            return 0;
        }
    }
    return thing;
}
