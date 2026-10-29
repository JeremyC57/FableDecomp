// CAIStateGroup_ChickenWander::Update() -- walk to a nearby wander position, then idle one to three times
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009118A0, length 347 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface 7 = CTCCreatureNavigation, bit 7 of +0x20 as a
//          bitset::test byte), CThing::GetPosition const @ 0x006A4D00, CTCCreatureNavigation::GetNearbyWanderPosition
//          @ 0x006CB6E0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890,
//          CCharString(const char*, long) @ 0x0099EBF0 ("ST_IDLE" @ 0x0127138C), CCreatureAction_PlayAnimation ctor @ 0x00843A80,
//          AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0, ~CCharString @ 0x0099EAE0.
// Shape: the navigation out pointer shares its home with the lookup key (miss path reads it); the wander target is a
//          C3DVector local passed by value through its user copy ctor; the idle count is the inlined world-seed random
//          (`% 3` unsigned) plus one, the loop counter spilled; each idle builds a named CCharString in an inner block and
//          the action temporary in the call, the `!= 2` test lands in bl (a bool declared outside the block) before both
//          destructors.
// Layout: CAIStateGroupBase per engine/CAIStateGroupBase.h (PBrain +4); the creature's bitset +0x20, interface map +0x44.
//          No engine header exists for CAIStateGroup_ChickenWander.
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
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation, float blend,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x00843A80
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFROR13(unsigned long);   // 0x00497890

enum EAIActionReturn { };
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
        CThing* avoid, bool c, float speed);    protected: // 0x0097EC00
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool stopWhenReplaced); public:   // 0x0097DB70
};

struct CAIStateGroup_ChickenWander : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_ChickenWander::Update()
{
    CThing* creature = PBrain->PCreature;
    CTCCreatureNavigation* navigation;
    creature->GetInterfaceT(&navigation, 7);
    C3DVector target;
    if (!navigation->GetNearbyWanderPosition(creature->PeekPos(), 2.0f, &target, true))
        return;
    bool reached = false;
    if (AIAction_MoveToPos(1, target, &reached, 3, true, true, 0, true, 0.0f) != 2)
        return;
    unsigned long idles = Random(3) + 1;
    for (unsigned long i = 0; i < idles; ++i) {
        bool failed;
        {
            CCharString animation("ST_IDLE");
            failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), animation, 0.0f,
                CSoundPair(), false, true, false, 0, 0), false) != 2;
        }
        if (failed)
            return;
    }
}