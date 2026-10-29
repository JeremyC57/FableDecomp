// CTCTeleporter::FrameUpdate() -- finish a teleport held back for a tutorial; otherwise, while the hero is in the teleporter's region, activate the teleporter for named guild markers, start teleporting (or a tutorial) when the hero steps inside the radius, keep the active particle effect alive, and kill it when teleporting is disabled
// Leaf: ?FrameUpdate@CTCTeleporter
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E00D0, 858 bytes, plain `ret`.
// Callees: CTCBase getters 0x00686A80 (world) / 0x00686D30 (world map) / 0x00686D20 / 0x00686D40 / 0x00686D50 (labels
//   "CViewBase::GetDisplayEngine" are propagated); CPlayerManager getters 0x00449960 / 0x00449970; CPlayer::GetPControlledCreature
//   0x00487DC0; CTCTeleporter::StartTeleporting 0x007DFF90; CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / the current
//   region 0x004FB150; CCharString::operator!= 0x0099E960; the player GUI refresh 0x004378C0 on g_pPlayerGui 0x013B8790;
//   IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0 / IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 (`__fastcall`);
//   CThing::GetPos 0x004C73D0 / Kill 0x004C9B80; LowerBound 0x0040F020 (interfaces 0x29 hero, 0x4B emitter);
//   CTCHero::GiveTutorialForCategory 0x006C9FA0; particle Create 0x006E0880 (`__fastcall`); CIntelligentPointer<CThing>
//   (CThing*) 0x004A96F0 / operator= 0x004A9760 / ~ 0x00A01C10 / GetPItem 0x00A01B50; CTCDParticleEmitter::AttachToThing
//   0x006E0BE0 / SetAlwaysUpdate 0x006E0590; CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Layout (retail CTCTeleporter = Ego_r.pdb, packed): PDef +0x0c, Active +0x10, Activated +0x11, HeroInsideRadius +0x12,
//   PParticle +0x13, WaitingForTutorialToFinish +0x1b; CTeleporterDef Radius +0x28, ParticleEffect +0x2c, flag +0x30.
// Layout (supersedes any local-model note above): engine/CWorld.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
class C3DVector { public: float X, Y, Z; };
class CTCBase;
class CBaseObject;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x74 - 0x4c];
    CCharString ScriptName;                             // +0x74 (descriptive)
    unsigned char _pad78[0x8e - 0x78];
    short RegionMapIndex;                               // +0x8e (descriptive)
    unsigned char _pad90;
    unsigned char m_flags;                              // +0x91 bit 0: dead
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> __forceinline T* GetInterface(long wanted)
    {
        if (HasTC(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
        }
        return 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
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
    const C3DVector& GetPos() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
};
class CThingPlayerCreature : public CThing { public: };
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0 (T = CThingPlayerCreature*: fed by GetPControlledCreature 0x00487DC0 at 25 of 32 retail call sites)
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pData;
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
};
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<CThing> : public CBaseIntelligentPointer { public:
    CIntelligentPointer(CThing* item);                  // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
#pragma pack(pop)
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>& p);   // 0x004CC340 IS_THING_ALIVE<CIntelligentPointer<CThing> >
CThing* __fastcall CreateParticle(long effect, const C3DVector* pos, bool attach);   // 0x006E0880

class CTCDParticleEmitter { public:                            // interface 0x4B
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetAlwaysUpdate(bool always);                  // 0x006E0590
};
class CTCHero { public:                                        // interface 0x29
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;         // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPlayer_00449960();                               // 0x00449960
    CPlayer& GetMainPlayer();                           // 0x00449970
};
class CWorld : public CWorldDonor {
 public:
};
class CWorldMap { public:
    long GetRegionNumberMapIsIn(long mapIndex);         // 0x004FC190
    long GetLoadedRegion() const;                            // 0x004FB150
};
struct CRegionManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long mapIndex);            // slot 10 (+0x28)
};
class CPlayerGui { public:
    unsigned char _pad00[0xfd];
    bool TutorialShowing;                               // +0xfd (descriptive)
    void UpdateMiniMapMarkers(bool a);                  // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

class CTeleporterDef { public:
    unsigned char _pad00[0x28];
    float Radius;                                       // +0x28
    long ParticleEffect;                                // +0x2c
    bool GuildMarkerOnly;                               // +0x30 (descriptive)
};
struct CTeleporterDefPointer {
    CTeleporterDef* Object;
    CTeleporterDef* operator->() const { return Object; }
    bool IsGuildMarkerOnly() const { return Object->GuildMarkerOnly; }
};

#pragma pack(push, 1)
class CTCTeleporter { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    CTeleporterDefPointer PDef;                         // +0x0c
    bool Active;                                        // +0x10
    bool Activated;                                     // +0x11
    bool HeroInsideRadius;                              // +0x12
    CIntelligentPointer<CThing> PParticle;                      // +0x13
    bool WaitingForTutorialToFinish;                    // +0x1b
    CWorld* GetWorld();                                 // 0x00686A80
    CRegionManager* GetRegionManager();                 // 0x00686D20
    CWorldMap* GetWorldMap();                           // 0x00686D30
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    CPlayerManager* GetPlayerManager2();                // 0x00686D50
    void StartTeleporting(CThingPlayerCreature* hero);  // 0x007DFF90
    void FrameUpdate();
};
#pragma pack(pop)

void CTCTeleporter::FrameUpdate()
{
    if (WaitingForTutorialToFinish) {
        if (g_pPlayerGui->TutorialShowing || GetWorld()->Paused)
            return;
        WaitingForTutorialToFinish = false;
        CThingPlayerCreature* hero = GetPlayerManager2()->GetPlayer_00449960()->GetPControlledCreature();
        if (hero && !(hero->m_flags & 1))
            StartTeleporting(hero);
        return;
    }
    long mapIndex = ParentThing->RegionMapIndex;
    long region = GetWorldMap()->GetRegionNumberMapIsIn(mapIndex);
    if (GetWorldMap()->GetLoadedRegion() == region) {
    if (!Activated) {
        if (!PDef.IsGuildMarkerOnly()) {
            CThing* parent = ParentThing;
            if (parent->ScriptName != "HERO_GUILD_TELEPORT_MARKER" && Active != true) {
                Active = true;
                Activated = true;
                g_pPlayerGui->UpdateMiniMapMarkers(false);
            }
        }
    }
    if (!PDef.Object)
        return;
    if (!GetRegionManager()->IsMapLoaded(ParentThing->RegionMapIndex))
        return;
    if (GetWorld()->TeleportingEnabled) {
    CThingPlayerCreature* hero = GetPlayerManager()->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        const C3DVector& myPos = ParentThing->GetPos();
        const C3DVector& heroPos = hero->GetPos();
        float dx = heroPos.X - myPos.X;
        float dy = heroPos.Y - myPos.Y;
        float radius = PDef->Radius;
        if (dx * dx + dy * dy < radius * radius) {
            if (!Activated) {
                if (PDef->GuildMarkerOnly) {
                    CThing* parent = ParentThing;
                    if (parent->ScriptName != "HERO_GUILD_TELEPORT_MARKER" && Active != true) {
                        Active = true;
                        Activated = true;
                        g_pPlayerGui->UpdateMiniMapMarkers(false);
                    }
                }
            }
            if (!Active)
                return;
            if (!HeroInsideRadius) {
                HeroInsideRadius = true;
                CTCHero* heroTC = hero->GetInterface<CTCHero>(0x29);
                if (heroTC && heroTC->GiveTutorialForCategory(0x1f, false)) {
                    WaitingForTutorialToFinish = true;
                    return;
                }
                StartTeleporting(hero);
            }
        } else
            HeroInsideRadius = false;
    }
    if (!Active)
        return;
    if (((CThing*)PParticle.GetPItem()))
        return;
    {
        CIntelligentPointer<CThing> particle(CreateParticle(PDef->ParticleEffect, &ParentThing->GetPos(), false));
        PParticle = particle;
    }
    if (!((CThing*)PParticle.GetPItem()))
        return;
    CTCDParticleEmitter* emitter = 0;
    ((CThing*)PParticle.GetPItem())->GetInterface(&emitter, 0x4b);
    emitter->AttachToThing(*ParentThing, 1, "", 0, 0.0f);
    emitter->SetAlwaysUpdate(true);
    } else {
        if (IS_THING_ALIVE(PParticle)) {
            ((CThing*)PParticle.GetPItem())->Kill(true);
            CIntelligentPointer<CThing> none(0);
            PParticle = none;
        }
    }
    } else
        HeroInsideRadius = true;
}