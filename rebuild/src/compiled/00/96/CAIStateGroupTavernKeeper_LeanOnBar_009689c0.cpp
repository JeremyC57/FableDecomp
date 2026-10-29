// CAIStateGroup_TavernKeeper::LeanOnBar() -- pin the keeper, pick a random bar slot, walk to it (failure: free the point, unpin, terminate), then lean on the bar (into/loop/out-of animation, random 1..10 s loop) until a payment request or an empty jug slot needs attention; returns true for those, else whether the lean action completed
// Leaf: LeanOnBar
// Retail: Fable.exe (TLC, VC7.1) @ 0x009689C0, 512 bytes.
// Callees: SetPushableByHero @ 0x0097E330, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBar +0x1c), CVectorMap::LowerBound
//   @ 0x0040F020 (inlined GetInterface 0xb9 tavern bar = +0x34 bit 25; the miss path reads the never-written key slot),
//   CTCTavernBar::GetRandomBarSlot @ 0x007BA3C0, WalkToPoint @ 0x00968230, FreeBarPoint @ 0x00968950, Terminate @ 0x008FCF40,
//   GetPaymentRequest @ 0x009683A0, CTCTavernBar::HasEmptyJugSlot @ 0x007BA8C0, GetPWorldSeed @ 0x008FCFD0, GFFloatRandom
//   @ 0x004978A0 (__fastcall: seed in ecx, range on the stack), CCharString @ 0x0099EBF0 / 0x0099EAE0 ("BARMAN_LEAN_OUTOF"
//   0x0129663C, "BARMAN_LEAN_LOOP" 0x01296628, "BARMAN_LEAN_INTO" 0x01296614), CCreatureAction_PlayIntoLoopOutOfAnimation ctor
//   @ 0x008427E0, ~CCreatureActionBase @ 0x00693EF0, AIAction_PerformAction @ 0x0097D8B0.
// Layout: engine/CAIStateGroup_TavernKeeper.h (PBar +0x1c, JugActionPointIndex +0x34, JugPos +0x38, JugRHSet +0x44,
//   JugSlot +0x5c) over the landed local CAIStateGroupBase model; the action's own two CCharString members at +0xbc/+0xc0
//   (retail destroys them inline before the base destructor; sizeof 0xc4 from the 0xd0 frame).
// Shape: the three animation names as implicit const char* -> const CCharString& conversions (all three temporaries built
//   before the first push); the creature through inline getters (loaded right after the string constructors); both
//   "needs attention" exits written out in full (`FreeBarPoint(); SetPushableByHero(true); return true;`) -- a shared goto
//   label gives `push 1 ; mov ecx,esi` in the merged tail where retail's cross-jumped copy has `mov ecx,esi ; push 1`.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: float m[6]; };
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
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
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
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
class CThingCreatureBase;
class CTCTavernBar { public:                           // interface 0xb9
    bool GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot);  // 0x007BA3C0
    bool HasEmptyJugSlot(CThing* creature);     // 0x007BA8C0
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x14];
    CCharString LoopAnimName;                   // +0xbc
    CCharString OutOfAnimName;                  // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
        const CCharString& loop, const CCharString& outOf, float loopTime, bool b, long c, long d);   // 0x008427E0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void SetPushableByHero(bool pushable);      // 0x0097E330
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CIntelligentPointer<CThing> PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CIntelligentPointer<CThing> PPayee;             // +0x2c CIntelligentPointer<CThingAICreature>
    long JugActionPointIndex;                   // +0x34
    C3DVector JugPos;                           // +0x38
    CRightHandedSet JugRHSet;                   // +0x44
    long JugSlot;                               // +0x5c
    bool WalkToPoint(C3DVector* pos, CRightHandedSet* rh);   // 0x00968230
    bool GetPaymentRequest();                   // 0x009683A0
    void FreeBarPoint();                        // 0x00968950
    bool LeanOnBar();
};

bool CAIStateGroup_TavernKeeper::LeanOnBar()
{
    SetPushableByHero(false);
    CTCTavernBar* bar;
    PBar.GetPItem()->GetInterface(&bar, 0xb9);
    bar->GetRandomBarSlot(*PBrain->PCreature, JugPos, JugRHSet, JugActionPointIndex, JugSlot);
    if (!WalkToPoint(&JugPos, &JugRHSet)) {
        FreeBarPoint();
        SetPushableByHero(true);
        Terminate();
        return false;
    }
    if (GetPaymentRequest() || bar->HasEmptyJugSlot(GetCreature())) {
        FreeBarPoint();
        SetPushableByHero(true);
        return true;
    }
    float loopTime = GFFloatRandom(9.0f, GetPWorldSeed()) + 1.0f;
    long result;
    do {
        if (GetPaymentRequest() || bar->HasEmptyJugSlot(GetCreature())) {
            FreeBarPoint();
            SetPushableByHero(true);
            return true;
        }
        result = AIAction_PerformAction(&CCreatureAction_PlayIntoLoopOutOfAnimation(
            GetCreatureBase(), "BARMAN_LEAN_INTO", "BARMAN_LEAN_LOOP",
            "BARMAN_LEAN_OUTOF", loopTime, false, 0, 0));
    } while (result == 4);
    FreeBarPoint();
    SetPushableByHero(true);
    return result == 2;
}