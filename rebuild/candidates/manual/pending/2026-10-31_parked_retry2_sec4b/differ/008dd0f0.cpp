// NCreatureMode::CModeScorpionKingVulnerable::OnActivate() -- the Scorpion King's vulnerable mode start: recovery count
//   (5 s of frames), base activation, suspend his AI, halve the physics radius (keeping the old one), enable the belly
//   hit location and disable the body one, drop a 60 s physical obstruction at each pincer helper point, start the
//   TAIL_CURL combination anim, spawn the EYE_VULNERABLE / EYE_VULNERABLE_FLASH / STING_IDLE effects on their emitters,
//   remember STING_IDLE as the tail effect, reset the frame count
// Grade: DIFFER(1461v1513) 353 difflines (wave 2 sec4b: first reconstruction; was triage-only DIFFER(-v1513) with no
//   extent; extent fixed to 1513 bytes, see BOUNDS_FIX.tsv / oracle_extra.tsv)
// Tried (graded): g1 full draft 1465v1513/353; g2 `origin -= GetFacingVector()` member operator-= 353 (same count,
//   shorter); g3 a named facing local then `-=` 353; g4 the creature local typed CThingAICreature* 383.
// Residue: frame 0x70 vs 0x60 (locals packed differently, so most lines are esp offsets); ours caches 0 in ebx for the
//   whole function (`push ebx` for every zero argument) and spills the creature pointer, where retail keeps PCreature in
//   ebx until GetAnimFromName and zeroes ebx only afterwards; the facing subtraction keeps facing X/Y on the x87 stack.
// Next: scope the hit-location, pincer, anim and effect sections in blocks to get retail's slot sharing (string temps
//   at +0x10, effectIndex +0x10, effects +0x14, origin +0x20, helper +0x2C, pos +0x38, CAnimInit +0x44); then the
//   zero register.
// Leaf: ?OnActivate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DD0F0, 1513 bytes (true extent: ret at 0x008DD6D8, int3 pad to 0x008DD6E0),
//   aligned frame 0x60.
// Layout (Ego_r CModeScorpionKingVulnerable, retail = Ego_r - 4 past the Obstructions vector): RecoveryCount +0x68,
//   Filter +0x6C, PCreature +0x78, bRecovering +0x7C, PTarget +0x7D, Obstructions +0x88, PEyeEmitter +0x94, PStingEmitter
//   +0x9C, FrameCount +0xA4, EffectsDone +0xA8, TailEffectIndex +0xAC, CollisionRadius +0xB0, TailCurlFinished +0xB4;
//   CAnimInit (0x2C, Ego_r) with its CAnimFrameInfo zeroed inline.
// Callees: CModeBase::OnActivate 0x0073AAC0, CThingAICreature::SuspendAI 0x00833A10, CTCHitLocations::
//   GetPHitLocationWithDefName 0x00815820, NHitLocations::CHitLocation::SetEnabled 0x00816390, CTCGraphicAppearance::
//   GetHelperPoint 0x004C0560, CTCDPhysicalObstruction::Create 0x007FD4B0 / SetLifeTimeSeconds 0x007FD3D0,
//   vector<CIntelligentPointer<CThing> >::push_back 0x004AD520, CThingCreatureBase::GetAnimFromName 0x00662A00,
//   CAnimationEntry::GetAnimateThingFlags 0x005DC390, CAnimInit::Init 0x0070C050, CTCAnimationComplex::GetAnimation
//   0x0070B460, CAnimateThing::AddCombinationAnim 0x0070D780, GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CTCDParticleEmitter::Create 0x006E0880 / AttachToThing 0x006E0BE0,
//   CBaseIntelligentPointer 0x00A01B10/B50/B90/C10. CTCPhysicsBase +0x34 (Radius) and the AddCombinationAnim / Init
//   parameter names are descriptive.
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
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
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
    SEntry* LowerBound(const ETCInterfaceType& type);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase { public:
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6)
    virtual C3DVector GetFacingVector() const;           // slot 70 (+0x118)
    unsigned char _pad04[0x30];
    float Radius;                                        // +0x34 (name descriptive)
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& pos, float scale) const;   // 0x004C0560
};
}
class CThingBaseDef : public CDefClassBase { };
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0x1c];
    unsigned long   UsingComponents[9];                  // +0x20
    CInterfaceMap   Components;                          // +0x44
    unsigned char   _pad4c[0x10];
    void*           PTCDriver;                           // +0x5C
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char   _pad68[0x8];
    CDefPointer<const CThingBaseDef> PBaseDef;           // +0x70
    unsigned char   _pad74[0x1d];
    unsigned char   Flags91;                             // +0x91
    const C3DVector& GetPos() const;                     // 0x004C73D0
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> T* GetInterface(long type)
    {
        T* result = 0;
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound((ETCInterfaceType)type);
            result = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
        }
        return result;
    }
};
class CAnimationEntry { public:
    long Anim;                                           // +0x00
    unsigned long GetAnimateThingFlags() const;          // 0x005DC390
};
class CThingCreatureBase : public CThing { public:
    const CAnimationEntry& GetAnimFromName(const CCharString& name, bool warn) const;   // 0x00662A00
};
class CThingAICreature : public CThingCreatureBase { public:
    void SuspendAI(bool suspend, void* requester);       // 0x00833A10
};
enum EAnimUpdateMode { };
class CAnimFrameInfo { public:
    long Frame;
    long Offset;
    CAnimFrameInfo() : Frame(0), Offset(0) { }
};
class CAnimInit { public:
    EAnimUpdateMode UpdateMode;                          // +0x00
    unsigned char Type;                                  // +0x04
    bool Looping;                                        // +0x05
    long Anim;                                           // +0x08
    float Speed;                                         // +0x0C
    float Alpha;                                         // +0x10
    float LastAlpha;                                     // +0x14
    CAnimFrameInfo Frame;                                // +0x18
    unsigned long Flags;                                 // +0x20
    unsigned long TypeFlags;                             // +0x24
    long Priority;                                       // +0x28
    CAnimInit() { }
    void Init(unsigned char type, unsigned long flags, long anim, EAnimUpdateMode mode, bool looping,
              unsigned long typeFlags, long priority);   // 0x0070C050
};
class CAnimateThing { public:
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                       // 0x0070B460
};
namespace NHitLocations {
class CHitLocation { public:
    void SetEnabled(bool enabled);                       // 0x00816390
};
}
class CTCHitLocations { public:
    NHitLocations::CHitLocation* GetPHitLocationWithDefName(const CCharString& name);   // 0x00815820
};
class CTCDPhysicalObstruction { public:
    static CThing* __fastcall Create(const C3DVector& pos, const C3DVector& origin, float radius);   // 0x007FD4B0
    void SetLifeTimeSeconds(float seconds);              // 0x007FD3D0
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
class CSpecialEffectsDef : public CDefClassBase { public:
    bool GetEffectFromString(const CCharString& name, long& index) const;   // 0x00669D30
};
class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual void v00();
        CThing* ParentThing;                             // +0x04
        void OnActivate();                               // 0x0073AAC0
    };
};
namespace NCreatureMode {
class CModeScorpionKingVulnerable : public CTCCreatureModeManager::CModeBase { public:
    unsigned char _pad08[0x60];
    long RecoveryCount;                                  // +0x68
    unsigned char Filter[0xc];                           // +0x6C
    CThingCreatureBase* PCreature;                       // +0x78
    bool bRecovering;                                    // +0x7C
    unsigned char PTarget[0x8];                          // +0x7D CIntelligentPointer<CThingCreatureBase> (packed)
    unsigned char _pad85[0x3];
    std::vector<CIntelligentPointer<CThing> > Obstructions;   // +0x88
    CIntelligentPointer<CThing> PEyeEmitter;             // +0x94
    CIntelligentPointer<CThing> PStingEmitter;           // +0x9C
    long FrameCount;                                     // +0xA4
    bool EffectsDone;                                    // +0xA8
    long TailEffectIndex;                                // +0xAC
    float CollisionRadius;                               // +0xB0
    bool TailCurlFinished;                               // +0xB4
    virtual void OnActivate();
};

template <class T> inline T* GetObstructionDriver(CThing* thing)
{
    T* driver = 0;
    if (thing->UsingComponents[7] & 0x2000)
        driver = (T*)thing->PTCDriver;
    return driver;
}

void CModeScorpionKingVulnerable::OnActivate()
{
    RecoveryCount = (long)(g_framesPerSecond * 5.0f);
    CModeBase::OnActivate();
    CThingCreatureBase* creature = PCreature;
    ((CThingAICreature*)creature)->SuspendAI(true, 0);
    CollisionRadius = creature->PhysicsTC->Radius;
    creature->PhysicsTC->Radius = CollisionRadius * 0.5f;

    CTCHitLocations* hitLocations = PCreature->GetInterface<CTCHitLocations>(0x7f);
    NHitLocations::CHitLocation* belly = hitLocations->GetPHitLocationWithDefName(CCharString("HIT_LOCATION_SCORPION_KING_BELLY"));
    NHitLocations::CHitLocation* body = hitLocations->GetPHitLocationWithDefName(CCharString("HIT_LOCATION_SCORPION_KING"));
    belly->SetEnabled(true);
    body->SetEnabled(false);

    C3DVector origin = PCreature->GetPos();
    origin -= PCreature->PhysicsTC->GetFacingVector();
    C3DVector helper;
    PCreature->PTCGraphicAppearance->GetHelperPoint(CCharString("weapon_pos_pincer_right"), 0, 0, helper, 1.0f);
    helper.Z = origin.Z;
    CThing* obstruction = CTCDPhysicalObstruction::Create(helper, origin, 0.5f);
    if (obstruction) {
        GetObstructionDriver<CTCDPhysicalObstruction>(obstruction)->SetLifeTimeSeconds(60.0f);
        Obstructions.push_back(CIntelligentPointer<CThing>(obstruction));
    }
    PCreature->PTCGraphicAppearance->GetHelperPoint(CCharString("weapon_pos_pincer_left"), 0, 0, helper, 1.0f);
    helper.Z = origin.Z;
    obstruction = CTCDPhysicalObstruction::Create(helper, origin, 0.5f);
    if (obstruction) {
        GetObstructionDriver<CTCDPhysicalObstruction>(obstruction)->SetLifeTimeSeconds(60.0f);
        Obstructions.push_back(CIntelligentPointer<CThing>(obstruction));
    }

    CTCAnimationComplex* animation = ParentThing->GetInterface<CTCAnimationComplex>(0x5a);
    const CAnimationEntry& tailCurl = creature->GetAnimFromName(CCharString("TAIL_CURL"), false);
    CAnimInit init;
    init.Init(0x21, 0, tailCurl.Anim, (EAnimUpdateMode)0, false, tailCurl.GetAnimateThingFlags(), 100);
    animation->GetAnimation().AddCombinationAnim(init, 0, 0, true, false);
    TailCurlFinished = false;

    C3DVector pos = ParentThing->GetPos();
    CDefPointer<const CSpecialEffectsDef> effects(NULL_DEF);
    long effectIndex = 0;
    ParentThing->PBaseDef->GetSubDef(&effects);
    if (!effects.IsNull()) {
        if (effects->GetEffectFromString(CCharString("EYE_VULNERABLE"), effectIndex)) {
            CThing* eye = CTCDParticleEmitter::Create(effectIndex, pos, false);
            if (IS_THING_ALIVE(eye)) {
                PEyeEmitter = CIntelligentPointer<CThing>(eye);
                if (eye->UsingComponents[2] & 0x800)
                    ((CTCDParticleEmitter*)eye->PTCDriver)->AttachToThing(*ParentThing, 10, CCharString("EMITTER_EYE"), 0, 0.0f);
            }
        }
        effectIndex = 0;
        if (effects->GetEffectFromString(CCharString("EYE_VULNERABLE_FLASH"), effectIndex)) {
            CThing* flash = CTCDParticleEmitter::Create(effectIndex, pos, false);
            if (IS_THING_ALIVE(flash)) {
                if (flash->UsingComponents[2] & 0x800)
                    ((CTCDParticleEmitter*)flash->PTCDriver)->AttachToThing(*ParentThing, 10, CCharString("EMITTER_EYE"), 0, 0.0f);
            }
        }
        effectIndex = 0;
        if (effects->GetEffectFromString(CCharString("STING_IDLE"), effectIndex)) {
            CThing* sting = CTCDParticleEmitter::Create(effectIndex, pos, false);
            if (IS_THING_ALIVE(sting)) {
                PStingEmitter = CIntelligentPointer<CThing>(sting);
                if (sting->UsingComponents[2] & 0x800)
                    ((CTCDParticleEmitter*)sting->PTCDriver)->AttachToThing(*ParentThing, 10, CCharString("EMITTER_STING"), 0, 0.0f);
            }
        }
        effects->GetEffectFromString(CCharString("STING_IDLE"), TailEffectIndex);
    }
    FrameCount = 0;
    EffectsDone = false;
}
}
