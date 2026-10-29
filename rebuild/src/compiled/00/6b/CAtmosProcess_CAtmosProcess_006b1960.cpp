// CAtmosProcess::CAtmosProcess() -- construct the atmosphere process (day/night state, the atmos block map, rain flags, the rain player pointer, building/reverb state) and, when the sound system exists, hand it the atmos map with the MAIN_SOUND_SETUP listener damping, then load the atmos themes
// Leaf: ??0CAtmosProcess
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B1960, length 185 bytes, `ret`, returns `this`.
// Label correction: the catalog/manifest name "CThingCreatureBase::GetCombatAnimationSet" is propagated; the body is a
//   constructor (`mov eax,esi`) storing vtable 0x0125D47C = CAtmosProcess::CMyAtmosMap at +4 (rtti-vtables.tsv; its
//   landed slots 0x006B1580/0x006B15C0) and CIntelligentPointer vtable 0x0125A860 at +0xa; the single caller is
//   CWorld's init code at 0x004A65B3.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CCharString(const char*, long) @ 0x0099EBF0 /
//   ~CCharString @ 0x0099EAE0, CDefinitionManager::Get @ 0x0044C6B0 (static `__fastcall`), CDefinitionManager::
//   GetDef<const CSoundDef> @ 0x004196B2, the sound system's virtual slot 36 (+0x90: atmos map, damping min, damping
//   range, flags 0x8010; name unproven), two CAtmosProcess theme loaders @ 0x006B16B0 / 0x006B1600 (names unproven),
//   the def's virtual Destroy (slot 1).
// Globals: g_pSoundSystem @ 0x013B8394; literal "MAIN_SOUND_SETUP" @ 0x0122EF98.
// Layout: Ego_r.pdb CAtmosProcess is 28 bytes without the map; retail inserts the 4-byte CMyAtmosMap at +4, so
//   DayNightState +0, AtmosMap +4, RainCurrentlyPlaying +8, AtmosSoundsMuted +9, PRainPlayer +0xa (an 8-byte
//   CIntelligentPointer at an odd offset: pack(1) on the pointer classes only), InsideBuilding +0x14 (-1), ReverbType
//   +0x18, ReverbLevel +0x1c. Retail CSoundDef AtmosListenerDampingMin +0x78, AtmosListenerDampingRange +0x7c (PDB
//   +0x90/+0x94: five 12-byte vectors over a 0x3c base).
// Shape: all members in the initialiser list (one zero register); the def lookup is the landed CMusicManager ctor's
//   block-scoped "MAIN_SOUND_SETUP" string with the chained `Get().GetDef(name, def)`; the def is released by hand
//   without a null test after the two loaders.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CSoundDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x78 - 0x08];
    float AtmosListenerDampingMin;              // +0x78
    float AtmosListenerDampingRange;            // +0x7c
};

class CDefinitionManager { public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<T>& out);   // 0x004196B2 for T = const CSoundDef
};
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0

class C2DCoordI;
struct CMyAtmosMap {                            // CAtmosProcess::CMyAtmosMap, vtable 0x0125D47C
    virtual long GetBlockValue(const C2DCoordI& block);     // 0x006B1580
    virtual float GetBlockHeight(const C2DCoordI& block);   // 0x006B15C0
};

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2)
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetAtmosMap_slot36(CMyAtmosMap* map, float dampingMin, float dampingRange, unsigned long flags);   // slot 36 (+0x90)
};
extern CSoundSystem* g_pSoundSystem;            // 0x013B8394

class CBaseObject;
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0125A860
    virtual void v00();
    CIntelligentPointer() { }
};
#pragma pack(pop)

class CThingMarker;
class CAtmosProcess {
public:
    long DayNightState;                         // +0x00 EDayNightState
    CMyAtmosMap AtmosMap;                       // +0x04
    bool RainCurrentlyPlaying;                  // +0x08
    bool AtmosSoundsMuted;                      // +0x09
    CIntelligentPointer<CThingMarker> PRainPlayer;   // +0x0a
    long InsideBuilding;                        // +0x14
    long ReverbType;                            // +0x18 EReverbEnvironmentType
    float ReverbLevel;                          // +0x1c
    CAtmosProcess();
    void LoadThemes_006b16b0();                 // 0x006B16B0
    void LoadMainTheme_006b1600();              // 0x006B1600
};

CAtmosProcess::CAtmosProcess()
    : DayNightState(0), RainCurrentlyPlaying(false), AtmosSoundsMuted(false), InsideBuilding(-1), ReverbType(0), ReverbLevel(0)
{
    if (g_pSoundSystem) {
        CDefPointer<const CSoundDef> soundDef;
        {
            CCharString name("MAIN_SOUND_SETUP", -1);
            CGameDefinitionManager::Get().GetDef(name, soundDef);
        }
        CSoundDef* def = const_cast<CSoundDef*>(soundDef.Object);
        g_pSoundSystem->SetAtmosMap_slot36(&AtmosMap, def->AtmosListenerDampingMin, def->AtmosListenerDampingRange, 0x8010);
        LoadThemes_006b16b0();
        LoadMainTheme_006b1600();
        if (--def->RefCount == 0)
            def->Destroy();
    }
}