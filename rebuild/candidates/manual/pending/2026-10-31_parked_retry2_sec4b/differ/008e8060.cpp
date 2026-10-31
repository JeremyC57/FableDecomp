// NCreatureMode::CModeBriarRoseConcentrate::OnActivate() -- Briar Rose's concentrate mode: base stand activation, the
//   drain-charge timer from CBriarRoseDef::SecondsToChargeDrainLife (15 s default), suspend her AI; unless she is an
//   imposter, teleport her to a random TeleportMarker and spawn a CREATURE_RIVAL_HERO_BRIAR_ROSE_IMPOSTER at every other
//   marker (extra CTCAbsorbHits hits, the concentrate-into action, kept in Imposters), face everyone toward the markers'
//   centre; then attach the DRAIN_CHARGE_BODY effect to her and the DRAIN_CHARGE_SWORD effect to her primary-slot weapon
// Grade: DIFFER(1758v1784) 298 difflines (wave 2 sec4b: first reconstruction from the disassembly; was the 5-byte
//   DIFFER(5v1784) skeleton)
// Tried (graded): g1 full draft 1746v1784/302; g2 `CTCPhysicsBase* physics` local for the teleport (retail hoists the
//   vptr before GetPos and spills it) 311; g3 the timer as one ternary expression 308 (the base-def temp then lives to
//   the end of the statement: retail releases it before the timer); g4 a separate `bool hasDef` 305; g5 the timer and
//   the effects section in their own blocks 298.
// Residue: the frame is 0x1A0 vs 0x190 and VC7.1 packs the locals differently (retail shares 0x14-0x30 between the
//   CCharString temps, briarIndex, i, imposterDefIndex, the uninitialised emitter out-pointer and the 0x46 key; markers at
//   +0x40, centre +0x34, CRightHandedSet +0x4C, init +0x64, the action temp +0x80), so most lines are esp offsets; ours
//   caches 0 in ebx for the whole function where retail keeps the GetSubDef bool in bl and zeroes eax/ebx/ebp locally.
// Next: scope each section so the temps share retail's slots (imposterDefIndex and the emitter out-pointer in disjoint
//   blocks); then the register choice for the zero constant.
// Leaf: ?OnActivate@CModeBriarRoseConcentrate@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E8060, 1784 bytes, aligned frame 0x190.
// Layout (Ego_r CModeBriarRoseConcentrate 0xA8, retail = Ego_r - 4 past the Imposters vector): Filter +0x68, Timer +0x74,
//   PTarget +0x78, Imposters +0x80, Imposter +0x8C, PDef CDefPointer<const CBriarRoseDef> +0x90, PBodyEffect +0x94,
//   PSwordEffect +0x9C; CBriarRoseDef SecondsToChargeDrainLife +0x30, NumberOfHitsToKillImposters +0x38;
//   CThingAICreatureInit (0x1C, Ego_r); CTCAbsorbHits::HitsRemaining +0x0C; CCreatureAction_BriarRoseConcentrateInto
//   Imposter +0x114, PEnemy +0x115 (Ego_r packs the pointer at an odd offset).
// Callees: CModeStandBase::OnActivate 0x008621A0, GetSubDef<CBriarRoseDef> 0x008E7790, CThingAICreature::SuspendAI
//   0x00833A10, CModeBase::GetWorld 0x0073A680, CThingSearchTools::GetAllThings<CThingMarker, vector<CThing*>,
//   CThingFilter_HasScriptName> 0x008E8AC0, vector<CIntelligentPointer<CThing> >::reserve 0x0077E580 (catalog label
//   vector_reserve_CConversation is a shared 8-byte-element body), CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CCharString::operator=(const char*) 0x0099EFE0, CThingAICreature::Create 0x00833800, CThing::AddTC 0x004C9D60,
//   CCreatureAction_BriarRoseConcentrateInto ctor 0x008E8BB0, CThingCreatureBase::SetCurrentAction 0x006644F0,
//   ~CCombatActionBase 0x0062E900, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/B50/B90/C10,
//   vector<CIntelligentPointer<CThing> >::push_back 0x004AD520, C3DVector::AccurateNormalise 0x00A14440, CTCPhysicsBase
//   slots 31/32 (teleport / set right-handed set; names descriptive), GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CTCDParticleEmitter::Create 0x006E0880 / AttachToThing
//   0x006E0BE0, CTCCarrying::GetThingInPrimarySlot 0x006A1440 (const LowerBound 0x004365B0), free.
class CBaseObject { public: virtual void v00(); };
class CThing;
class CThingPhysical;
class CThingBuilding;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& AccurateNormalise();                       // 0x00A14440
};
class CRightHandedSet { public:
    C3DVector Up;                                         // +0x00
    C3DVector Forward;                                    // +0x0C
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
extern long g_framesPerSecond;                            // 0x01375550
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~vector() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    bool empty() const { return _M_start == _M_finish; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void reserve(size_type __n);
    void push_back(const _Tp& __x);
};
}
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase { public:
    FILL10(0) FILL10(1) FILL10(2)
    virtual void v30();
    virtual void SetPositionImmediately(const C3DVector& pos);   // slot 31 (+0x7C) (name descriptive)
    virtual void SetRHSet(const CRightHandedSet& rhs);           // slot 32 (+0x80) (name descriptive)
};
class CThingBaseDef : public CDefClassBase { };
class CTCDParticleEmitter;
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0xc];
    unsigned char   ThingType;                           // +0x10 (name descriptive)
    unsigned char   _pad11[0xf];
    unsigned long   UsingComponents[9];                  // +0x20
    CInterfaceMap   Components;                          // +0x44
    unsigned char   _pad4c[0x10];
    void*           PTCDriver;                           // +0x5C
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    unsigned char   _pad64[0xc];
    CDefPointer<const CThingBaseDef> PBaseDef;           // +0x70
    unsigned char   _pad74[0x1d];
    unsigned char   Flags91;                             // +0x91
    const C3DVector& GetPos() const;                     // 0x004C73D0
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> bool GetTCDriver(T** out)
    {
        if (UsingComponents[2] & 0x800) {
            *out = (T*)PTCDriver;
            return true;
        }
        return false;
    }
    template <class T> bool GetInterface(T** out, long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound((ETCInterfaceType)type);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    CTCBase* AddTC(const CCharString& name, bool initialise, const class CTCParamsBase* params);   // 0x004C9D60
};
class CCreatureActionBase { public: virtual ~CCreatureActionBase(); };
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingAICreatureInit { public:
    CThingPhysical* OriginalThing;                       // +0x00
    CThingPhysical* PreviousThing;                       // +0x04
    CCharString     ScriptName;                          // +0x08
    CCharString     OverridingBrainName;                 // +0x0C
    bool            PlayerCharacter;                     // +0x10
    bool            Drawable;                            // +0x11
    bool            PlaceAtEndOfList;                    // +0x12
    bool            EnableCreatureAutoPlacing;           // +0x13
    CThingBuilding* HomeBuilding;                        // +0x14
    CThingBuilding* WorkBuilding;                        // +0x18
    CThingAICreatureInit(const CCharString& scriptName = CCharString(""), const CCharString& brainName = CCharString(""))
        : OriginalThing(0), PreviousThing(0), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(false), Drawable(true), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) { }
};
class CThingAICreature : public CThingCreatureBase { public:
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& pos, long player,
                                               const CThingAICreatureInit& init);   // 0x00833800
    void SuspendAI(bool suspend, void* requester);       // 0x00833A10
};
class CTCAbsorbHits { public:
    unsigned char _pad00[0xc];
    long HitsRemaining;                                  // +0x0C
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot() const;               // 0x006A1440
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& pos, bool attached);   // 0x006E0880
    void AttachToThing(CThing& thing, long index, const CCharString& dummyName, long flags, float delay);   // 0x006E0BE0
};
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* operator->() const { return (T*)GetPItem(); }
    operator T*() const { return (T*)GetPItem(); }
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
class CThingMarker;
class CThingFilter_HasScriptName { public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) { }
};
class CThingSearchTools { public:
    template <class TThing, class TContainer, class TFilter> void GetAllThings(const TFilter& filter, TContainer& result) const;
};
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;                // +0x20
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; }
    unsigned long* GetWorldSeed() const;                 // 0x0049D810
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
};
class CBriarRoseDef : public CDefClassBase { public:
    unsigned char _pad[0x30 - sizeof(CDefClassBase)];
    float SecondsToChargeDrainLife;                      // +0x30
    long  GuranteedImpostersHitBeforeAllowedToHitBriar;  // +0x34
    long  NumberOfHitsToKillImposters;                   // +0x38
};
class CSpecialEffectsDef : public CDefClassBase { public:
    bool GetEffectFromString(const CCharString& name, long& index) const;   // 0x00669D30
};

#pragma pack(push, 4)
class CCombatActionBase : public CCreatureActionBase { public:
    virtual ~CCombatActionBase();                        // 0x0062E900
    unsigned char _pad04[0x110];
};
#pragma pack(pop)
#pragma pack(push, 1)
class CCreatureAction_BriarRoseConcentrateInto : public CCombatActionBase { public:
    bool Imposter;                                       // +0x114
    CIntelligentPointer<CThingCreatureBase> PEnemy;      // +0x115
    CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* creature, CThingCreatureBase* enemy, bool imposter);   // 0x008E8BB0
};
#pragma pack(pop)

class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual void v00();
        CThing* ParentThing;                             // +0x04
        CWorld* GetWorld() const;                        // 0x0073A680
    };
};
namespace NCreatureMode {
class CModeStandBase : public CTCCreatureModeManager::CModeBase { public:
    unsigned char _pad08[0x60];
    virtual void OnActivate();                           // 0x008621A0
};
class CModeBriarRoseConcentrate : public CModeStandBase { public:
    unsigned char Filter[0xc];                           // +0x68
    long Timer;                                          // +0x74
    CIntelligentPointer<CThingCreatureBase> PTarget;     // +0x78
    std::vector<CIntelligentPointer<CThing> > Imposters; // +0x80
    bool Imposter;                                       // +0x8C
    CDefPointer<const CBriarRoseDef> PDef;               // +0x90
    CIntelligentPointer<CThing> PBodyEffect;             // +0x94
    CIntelligentPointer<CThing> PSwordEffect;            // +0x9C
    virtual void OnActivate();
};

void CModeBriarRoseConcentrate::OnActivate()
{
    CModeStandBase::OnActivate();
    {
        float seconds = 15.0f;
        if (ParentThing->GetPBaseDef()->GetSubDef(&PDef))
            seconds = PDef->SecondsToChargeDrainLife;
        Timer = (long)(g_framesPerSecond * seconds);
    }
    if (ParentThing->ThingType == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(true, this);

    if (!Imposter) {
        std::vector<CThing*> markers;
        GetWorld()->GetThingSearchTools().GetAllThings<CThingMarker>(CThingFilter_HasScriptName(CCharString("TeleportMarker")), markers);
        if (!markers.empty()) {
            long numMarkers = markers.size();
            Imposters.reserve(numMarkers);
            unsigned long random = GFRandom(GetWorld()->GetWorldSeed());
            long briarIndex = numMarkers ? random % numMarkers : 0;
            CTCPhysicsBase* physics = ParentThing->PhysicsTC;
            physics->SetPositionImmediately(markers[briarIndex]->GetPos());
            long imposterDefIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(CCharString("CREATURE_RIVAL_HERO_BRIAR_ROSE_IMPOSTER"));
            CThingAICreatureInit init;
            init.ScriptName = "BRIAR_ROSE_IMPOSTER";
            C3DVector centre(0.0f, 0.0f, 0.0f);
            for (long i = 0; i != numMarkers; ++i) {
                const C3DVector& pos = markers[i]->GetPos();
                centre.X += pos.X;
                centre.Y += pos.Y;
                centre.Z += pos.Z;
                if (i != briarIndex) {
                    CThingAICreature* imposter = CThingAICreature::Create(imposterDefIndex, pos, 4, init);
                    if (IS_THING_ALIVE(imposter)) {
                        if (!PDef.IsNull() && PDef->NumberOfHitsToKillImposters > 1) {
                            long extraHits = PDef->NumberOfHitsToKillImposters - 1;
                            CTCAbsorbHits* absorb = (CTCAbsorbHits*)imposter->AddTC(CCharString("CTCAbsorbHits"), true, 0);
                            if (absorb)
                                absorb->HitsRemaining += extraHits;
                        }
                        imposter->SetCurrentAction(CCreatureAction_BriarRoseConcentrateInto(imposter, PTarget, true));
                        Imposters.push_back(CIntelligentPointer<CThing>(imposter));
                    }
                }
            }
            float scale = 1.0f / numMarkers;
            centre.X *= scale;
            centre.Y *= scale;
            {
                const C3DVector& pos = ParentThing->GetPos();
                C3DVector direction(centre.X - pos.X, centre.Y - pos.Y, 0.0f);
                direction.AccurateNormalise();
                ParentThing->PhysicsTC->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction));
            }
            for (CIntelligentPointer<CThing>* it = Imposters.begin(); it != Imposters.end(); ++it) {
                CThing* imposter = *it;
                const C3DVector& pos = imposter->GetPos();
                C3DVector direction(centre.X - pos.X, centre.Y - pos.Y, 0.0f);
                direction.AccurateNormalise();
                imposter->PhysicsTC->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction));
            }
        }
    }

    {
    CDefPointer<const CSpecialEffectsDef> effects(NULL_DEF);
    if (ParentThing->GetPBaseDef()->GetSubDef(&effects)) {
        long effectIndex = 0;
        if (effects->GetEffectFromString(CCharString("DRAIN_CHARGE_BODY"), effectIndex)) {
            CThing* body = CTCDParticleEmitter::Create(effectIndex, ParentThing->GetPos(), false);
            if (IS_THING_ALIVE(body)) {
                CTCDParticleEmitter* emitter;
                body->GetTCDriver(&emitter);
                emitter->AttachToThing(*ParentThing, 8, CCharString(""), 0, 0.0f);
                PBodyEffect = CIntelligentPointer<CThing>(body);
            }
        }
        if (effects->GetEffectFromString(CCharString("DRAIN_CHARGE_SWORD"), effectIndex)) {
            CThing* sword = CTCDParticleEmitter::Create(effectIndex, ParentThing->GetPos(), false);
            if (IS_THING_ALIVE(sword)) {
                CTCDParticleEmitter* emitter;
                sword->GetTCDriver(&emitter);
                CTCCarrying* carrying;
                if (ParentThing->GetInterface(&carrying, 0x46)) {
                    CThing* weapon = carrying->GetThingInPrimarySlot();
                    if (IS_THING_ALIVE(weapon))
                        emitter->AttachToThing(*weapon, 10, CCharString("WEAPON_FOCUS_01"), 0, 0.0f);
                }
                PSwordEffect = CIntelligentPointer<CThing>(sword);
            }
        }
    }
    }
}
}
