// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::FrameUpdate() -- the ice troll's icicle assault: keep the
//   assault-loop action going until it is time to quit; after the lead-in, every SecondsBetweenIcicles pick a viable point
//   clear of the live icicles (std::partition), at random or the best one for a live target, and start a warning effect
//   there; age every icicle: when its warning runs out spawn the trap (immune to the troll, triggered, oriented at
//   random), kill the effect; when the trap's time runs out reset it and drop the icicle; switch to the assault-out-of
//   action once the assault time is over and no icicle is left
// Grade: DIFFER(1239v1239) 25 difflines, 3 of them masked relocations (wave 2 sec4b: first reconstruction from the
//   disassembly; was the 5-byte skeleton)
// Tried (graded): g1 draft with the trap lookups stubbed 1167v1239/309; g2 the real GetInterface 1265/247; g3
//   std::partition __forceinline (retail calls the fastcall __partition directly) + the loop action's own vtable 204;
//   g4 CGetBestIciclePointForTarget with a user copy ctor (VC7.1 then builds the by-value for_each argument in place, no
//   rep movsd) 1239v1239/78; g5 the frame count as a named long before the functor 41; g6 the effect pointer as a
//   block-scoped named CIntelligentPointer (retail's `lea` for operator=, temp destroyed at once) 31; g7 no fps local:
//   `g_framesPerSecond * x` everywhere so VC CSEs (float)fps and spills it with `fst` 28; g8 vector::operator[] through
//   begin() (index colouring) 25. No gain: `const float fps` (78), named effect pointer at function scope (35, longer),
//   `*(begin() + i)` (96), CCharString("") temp (32), GetPlayerNumber() accessor (25), if-form GetInterface (25).
// Residue: the trap spawn: retail loads ParentThing->PlayerNumber (movsx) before PDef, ours loads both bases first; the
//   trap-lookup key push and SetThingAsImmune's argument use ecx/edx where ours use eax/ecx; fmul base eax vs edx.
// Next: a different trap-spawn argument shape (the player number as a separate local, or the CThingObject::Create
//   overload with the player as an `unsigned long`).
// Leaf: ?FrameUpdate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DE7A0, 1239 bytes, virtual FrameUpdate, aligned (`and esp,-8`) frame 0x160.
// Layout (Ego_r CModeIceTrollGroundSmashIcicleAssault 0xA4, retail offsets): PTarget +0x68, Filter +0x70, Timer +0x7C,
//   PDef CDefPointer<const CSnowTrollDef> +0x80, Icicles vector<CIcicleData> +0x84, ViablePoints vector<C3DVector> +0x90;
//   CIcicleData (0x24): PEffect +0x00, PTrap +0x08, EffectTimer +0x10, TrapTimer +0x14, Position +0x18; CSnowTrollDef
//   +0x34..+0x58 (Ego_r); CTCTrapBase +0x5C state (name unproven).
// Callees: CModeBase::FrameUpdate 0x0073AC50, CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030 /
//   ~CCombatActionBase 0x0062E900, CThingCreatureBase::SetCurrentAction 0x006644F0, CCreatureAction_IceTrollIcicleAssaultOutOf
//   ctor 0x008E4560, std::__partition<C3DVector*, CIsOutsideIcicles> 0x008E3430 (predicate name unproven), IS_THING_ALIVE
//   <CIntelligentPointer<CThing>> 0x004CC340 / <CThing*> 0x0040DCE0, CModeBase::GetWorld 0x0073A680, CWorld::GetWorldSeed
//   0x0049D810, GFRandom 0x004CDAE0, CGetBestIciclePointForTarget ctor 0x008E2A90 / GetBestPosition 0x008E2B60 (Ego_r
//   anonymous-namespace names), std::for_each 0x008E2F10, CTCDParticleEmitter::Create 0x006E0880, CIcicleData ctor 0x008E2E00,
//   CIntelligentPointer<CThing> ctor 0x004A96F0 / operator= 0x004A9760, vector<CIcicleData>::push_back 0x008E4360 / erase
//   0x008E3890, CThingObject::Create 0x00703210, NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection
//   0x0080FFB0 (Ego_r 0x0080A950), CTCTrapBase::SetThingAsImmune 0x0078839A / ManualTrigger 0x00788388 / ManualReset
//   0x00787C0C / IsOpen 0x00787C1B (Ego_r IsOpen/IsClosed are 8 bytes apart as retail 0x00787C1B/0x00787C23: inferred),
//   CThing::Kill 0x004C9B80.
class CBaseObject { public: virtual void v00(); };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CRightHandedSet;
extern long g_framesPerSecond;                            // 0x01375550
namespace std {
struct bidirectional_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    void push_back(const _Tp& __x);
    iterator erase(iterator __position);
};
template <class _BidirectionalIter, class _Predicate>
_BidirectionalIter __fastcall __partition(_BidirectionalIter __first, _BidirectionalIter __last, _Predicate __pred,
                                         const bidirectional_iterator_tag&);   // <C3DVector*, CIsOutsideIcicles> 0x008E3430
template <class _ForwardIter, class _Predicate>
__forceinline _ForwardIter __fastcall partition(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    return __partition(__first, __last, __pred, random_access_iterator_tag());
}
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x008E2F10
}
class CTCBase;
enum ETCInterfaceType { TC_TRAP = 0x7d };                 // enumerator name unproven
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
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0x1c];
    unsigned long   UsingComponents[9];                  // +0x20
    CInterfaceMap   Components;                          // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    unsigned char   _pad4c[0x20];
    unsigned long   ThingTypeFlags;                      // +0x6C
    unsigned char   _pad70[0x20];
    signed char     PlayerNumber;                        // +0x90
    unsigned char   Flags91;                             // +0x91 bit 0 dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                         // 0x004C9B80
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const class CCreatureActionBase& action);   // 0x006644F0
};
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& pos, long player, const CRightHandedSet* rhs,
                                           unsigned long flags, const CCharString& scriptName);   // 0x00703210
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& pos, bool attached);   // 0x006E0880
};
namespace NExplosiveTrailHelpers {
void __fastcall RandomiseThingOrientationAroundDirection(CThing& thing, const C3DVector& direction);   // 0x0080FFB0
}
class CTCTrapBase { public:
    unsigned char _pad00[0x5c];
    long State;                                           // +0x5C (name unproven)
    void SetThingAsImmune(CThing& thing);                 // 0x0078839A
    void ManualTrigger();                                 // 0x00788388
    void ManualReset();                                   // 0x00787C0C
    bool IsOpen() const;                                  // 0x00787C1B
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
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;
    mutable long IVRefCount;
};
template <class T> class CDefPointer { public:
    T* operator->() const { return (T*)Object; }
    CDefPointeeBase* Object;
};
class CSnowTrollDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c];
    float SecondsForIcicleAssault;                        // +0x34
    float SecondsLeadIntoIcicles;                         // +0x38
    float SecondsBetweenIcicles;                          // +0x3C
    float SecondsBetweenWarningFXAndTrigger;              // +0x40
    float SecondsForTrapToRemain;                         // +0x44
    float DistanceRange;                                  // +0x48
    float DistanceAroundTarget;                           // +0x4C
    float DistanceExclusionAroundIcicles;                 // +0x50
    long IcicleAssaultWarningFXIndex;                     // +0x54
    long IcicleAssaultTrapIndex;                          // +0x58
};
class CWorld { public:
    unsigned long* GetWorldSeed() const;                  // 0x0049D810
};
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
class CSoundPair { public:
    long FirstSound, LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    virtual void PerformAction();
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _pad04[0x110];
    CCombatActionBase(CThingCreatureBase& creature, long animation, const CSoundPair& sounds);   // 0x00858030
    virtual ~CCombatActionBase();                         // 0x0062E900
};
class CCreatureAction_IceTrollIcicleAssaultLoop : public CCombatActionBase { public:
    CCreatureAction_IceTrollIcicleAssaultLoop(CThingCreatureBase& creature)
        : CCombatActionBase(creature, 0x96, CSoundPair()) { }
    virtual void PerformAction();                         // vtable 0x012860A4
};
class CCreatureAction_IceTrollIcicleAssaultOutOf : public CCombatActionBase { public:
    CCreatureAction_IceTrollIcicleAssaultOutOf(CThingCreatureBase& creature);   // 0x008E4560
};
class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual ~CModeBase();
        CThing* ParentThing;                             // +0x04
        unsigned char _pad08[0x60];
        virtual void FrameUpdate();                      // 0x0073AC50
        CWorld& GetWorld() const;                        // 0x0073A680
    };
};
namespace {
class CGetBestIciclePointForTarget { public:
    C3DVector TargetPos;
    float BestPosScore;
    C3DVector BestPos;
    CGetBestIciclePointForTarget(const CThing& target, float frames);   // 0x008E2A90
    CGetBestIciclePointForTarget(const CGetBestIciclePointForTarget& o)
        : TargetPos(o.TargetPos), BestPosScore(o.BestPosScore), BestPos(o.BestPos) { }
    bool GetBestPosition(C3DVector& pos) const;           // 0x008E2B60
};
}
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault : public CTCCreatureModeManager::CModeBase { public:
    class CIcicleData { public:
        CIntelligentPointer<CThing> PEffect;              // +0x00
        CIntelligentPointer<CThing> PTrap;                // +0x08
        long EffectTimer;                                 // +0x10
        long TrapTimer;                                   // +0x14
        C3DVector Position;                               // +0x18
        CIcicleData();                                    // 0x008E2E00
    };
    class CIsOutsideIcicles { public:                     // partition predicate (name unproven)
        const std::vector<CIcicleData>* Icicles;
        float Distance;
        CIsOutsideIcicles(const std::vector<CIcicleData>& icicles, float distance) : Icicles(&icicles), Distance(distance) { }
    };
    CIntelligentPointer<CThing> PTarget;                  // +0x68
    unsigned char Filter[0xc];                            // +0x70 CAnimFilterOfGroup
    long Timer;                                           // +0x7C
    CDefPointer<const CSnowTrollDef> PDef;                // +0x80
    std::vector<CIcicleData> Icicles;                     // +0x84
    std::vector<C3DVector> ViablePoints;                  // +0x90
    virtual void FrameUpdate();
private:
    bool IsTimeToQuit() const
    {
        return Timer > (float)g_framesPerSecond * PDef->SecondsForIcicleAssault && Icicles.empty();
    }
};

void CModeIceTrollGroundSmashIcicleAssault::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (!IsTimeToQuit()) {
        CThing* parent = ParentThing;
        if (parent->ThingTypeFlags & 8)
            ((CThingCreatureBase*)parent)->SetCurrentAction(CCreatureAction_IceTrollIcicleAssaultLoop(*(CThingCreatureBase*)parent));
    }
    long lead = (long)(g_framesPerSecond * PDef->SecondsLeadIntoIcicles);
    if (Timer < lead) {
        ++Timer;
        return;
    }
    if (Timer > g_framesPerSecond * PDef->SecondsForIcicleAssault) {
        if (Icicles.empty()) {
            CThing* parent = ParentThing;
            if (parent->ThingTypeFlags & 8)
                ((CThingCreatureBase*)parent)->SetCurrentAction(CCreatureAction_IceTrollIcicleAssaultOutOf(*(CThingCreatureBase*)parent));
            return;
        }
    } else if (!ViablePoints.empty()) {
        long elapsed = Timer - lead;
        if (elapsed < 0)
            elapsed = 0;
        if (elapsed % (long)(g_framesPerSecond * PDef->SecondsBetweenIcicles) == 0) {
            float exclusion = PDef->DistanceExclusionAroundIcicles;
            float warning = PDef->SecondsBetweenWarningFXAndTrigger;
            C3DVector* last = std::partition(ViablePoints.begin(), ViablePoints.end(), CIsOutsideIcicles(Icicles, exclusion));
            if (last != ViablePoints.begin()) {
                C3DVector pos;
                if (!IS_THING_ALIVE(PTarget)) {
                    pos = ViablePoints[GFRandom(last - ViablePoints.begin(), GetWorld().GetWorldSeed())];
                } else {
                    long frames = (long)(g_framesPerSecond * warning);
                    C3DVector best;
                    if (std::for_each(ViablePoints.begin(), last, CGetBestIciclePointForTarget(*PTarget, (float)frames))
                            .GetBestPosition(best))
                        pos = best;
                }
                CThing* effect = CTCDParticleEmitter::Create(PDef->IcicleAssaultWarningFXIndex, pos, false);
                if (IS_THING_ALIVE(effect)) {
                    CIcicleData icicle;
                    {
                        CIntelligentPointer<CThing> effectPointer(effect);
                        icicle.PEffect = effectPointer;
                    }
                    icicle.EffectTimer = (long)(g_framesPerSecond * warning);
                    icicle.Position = pos;
                    Icicles.push_back(icicle);
                }
            }
        }
    }
    for (CIcicleData* it = Icicles.begin(); it != Icicles.end(); ) {
        long effectTimer = it->EffectTimer--;
        if (effectTimer > 0) {
            ++it;
        } else if (effectTimer == 0) {
            CThing* trap = CThingObject::Create(PDef->IcicleAssaultTrapIndex, it->Position, ParentThing->PlayerNumber, 0, 0, "");
            if (trap && !trap->IsDead()) {
                NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection(*trap, C3DVector(1.0f, 0.0f, 0.0f));
                CTCTrapBase* trapTC;
                if (trap->GetInterface(&trapTC, TC_TRAP)) {
                    trapTC->SetThingAsImmune(*ParentThing);
                    trapTC->ManualTrigger();
                }
                it->PTrap = CIntelligentPointer<CThing>(trap);
                it->TrapTimer = (long)(g_framesPerSecond * PDef->SecondsForTrapToRemain);
            }
            if (IS_THING_ALIVE(it->PEffect))
                it->PEffect->Kill(true);
            ++it;
        } else {
            CTCTrapBase* trapTC;
            it->PTrap->GetInterface(&trapTC, TC_TRAP);
            if (it->TrapTimer > 0) {
                if (trapTC->IsOpen())
                    --it->TrapTimer;
                ++it;
            } else if (it->TrapTimer == 0) {
                trapTC->State = 4;
                trapTC->ManualReset();
                it = Icicles.erase(it);
            }
        }
    }
    ++Timer;
}
}
