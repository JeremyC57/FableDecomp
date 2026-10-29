// CAIStateGroup_BirdGroundWander::Update() -- hop to one to seven nearby wander positions, idling after each, then terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914460, length 395 bytes.
// Callees: the creature's +0x60 subsystem virtual slot 67 (+0x10C, called with 0), GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890,
//          CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 7 = CTCCreatureNavigation as a bitset::test byte),
//          CThing::GetPosition const @ 0x006A4D00, CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0,
//          CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (repeat 2, re-issued while it reports 4), CCharString(const char*,
//          long) @ 0x0099EBF0 ("ST_IDLE" @ 0x0127138C), CCreatureAction_PlayAnimation(creature, anim, sound, ...) @ 0x008424F0,
//          AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0, ~CCharString @ 0x0099EAE0,
//          Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo" is propagated).
// Shape: the ChickenWander body (0x009118A0) in a counted loop: `hops = Random(7)` and the loop test `i < hops + 1`
//          (VC7.1 hoists hops + 1 into edi and tests it against the zero register `lea edi,[edx+1] ; cmp edi,ebp ; jbe`;
//          a `Random(7) + 1` count is folded to `inc ; je`); the subsystem call goes through an inline creature accessor;
//          every exit breaks to the one Terminate call.
// Layout: CAIStateGroupBase per engine/CAIStateGroupBase.h (PBrain +4); creature bitset +0x20, interface map +0x44.
//          No engine header exists for CAIStateGroup_BirdGroundWander (the PDB header lists it without members).
class CTCBase;
class CThing;
class CThingCreatureBase;

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

class C3DVector { public:
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { x = v.x; y = v.y; z = v.z; }
};

class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThing : public CThingPhysical { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    struct CThingSubsystem* m_pSubsystem60;     // +0x60 (role not established; virtual slot 67 takes a zero)
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
        return has;
    }
};

struct CThingSubsystem {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41();
    virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53();
    virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65();
    virtual void s66();
    virtual void Slot67(long value);            // slot 67 (+0x10c)
};

class CTCCreatureNavigation { public:                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFROR13(unsigned long);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    CThing* GetCreatureThing() const { return PBrain->PCreature; }
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_BirdGroundWander : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_BirdGroundWander::Update()
{
    GetCreatureThing()->m_pSubsystem60->Slot67(0);
    unsigned long hops = Random(7);
    for (unsigned long i = 0; i < hops + 1; ++i) {
        CThing* creature = PBrain->PCreature;
        CTCCreatureNavigation* navigation;
        creature->GetInterfaceT(&navigation, 7);
        C3DVector target;
        if (!navigation->GetNearbyWanderPosition(creature->PeekPos(), 1.5f, &target, true))
            break;
        bool reached = false;
        long result;
        do {
            result = AIAction_MoveToPos(2, target, &reached, 3, true, true, 0, true, 0.0f);
        } while (result == 4);
        if (result != 2)
            break;
        bool failed;
        {
            CCharString animation("ST_IDLE");
            failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation,
                CSoundPair(), false, true, false, 0, 0), false) != 2;
        }
        if (failed)
            break;
    }
    Terminate();
}