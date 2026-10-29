// CAIStateGroup_WhisperRetreat::Update() -- unsheathe when unarmed; while the target lives: flag its stealth component as in danger from the creature, jump to safety when the target is within 3 units, then advance from boss phase 0 when ready or, in phase 2, use will and terminate; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096B600, length 355 bytes (plain `ret`, virtual).
// Callees: IsThingCarryingWeapon(const CThing*) @ 0x006A1540 (__fastcall), CThingCreatureBase::UnsheatheMeleeWeapon
//          @ 0x006649C0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 173 = bit 13 of +0x34 = CTCBoss, 54 = bit 22
//          of +0x24 = CTCStealth), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCStealth::SetAsInDanger(CThing*, long)
//          @ 0x00747B80, CThing::GetPosition @ 0x004C73D0, CAIStateGroup_WhisperRetreat::JumpToSafety @ 0x0096AC20 and
//          UseWill @ 0x0096AE50, CTCBoss::GetBossPhase @ 0x007AED10, IsReadyForPhaseChange @ 0x007AEE90, SetNextBossPhase
//          @ 0x007AEDA0, CAIStateGroupBase::Terminate @ 0x008FCF40; float pool 3.0f @ 0x0122DED4.
// Shape: the boss lookup is the out-parameter GetInterface (miss path reads the never-written slot); IS_THING_ALIVE is the
//          two-GetPItem inline test and the stealth lookup is a third GetPItem whose hit path alone calls SetAsInDanger; the
//          planar distance (`dx*dx + dy*dy`, dy squared first) is compared `< 3.0f` (`fcomp ; test ah,5 ; jp`); the phase
//          switch is `sub eax,0 ; je` / `sub eax,2 ; jne` with the phase-2 arm ending in its own Terminate + epilogue.
// Layout: engine/CAIStateGroup_WhisperRetreat.h over the landed CAIStateGroupBase model; the retail target pointer sits at
//          +0x14 (the PDB header's `_pad_0x14/_0x18`; PArenaCentre +0x1c is unused here).
#include <math.h>
class CTCBase;
class CThing;

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

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0
    void UnsheatheMeleeWeapon();                // 0x006649C0 (CThingCreatureBase)
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
bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540

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
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCBoss { public:                                // interface 173
    long GetBossPhase();                        // 0x007AED10
    bool IsReadyForPhaseChange();               // 0x007AEE90
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
};
class CTCStealth { public:                             // interface 54
    void SetAsInDanger(CThing* source, long status);     // 0x00747B80
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
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_WhisperRetreat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;            // +0x14 CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PArenaCentre;       // +0x1c
    bool bIsRangeAttack;                        // +0x24
    long NumHits;                               // +0x28
    long FrameOfLastHit;                        // +0x2c
    void JumpToSafety();                        // 0x0096AC20
    void UseWill();                             // 0x0096AE50
    virtual void Update();
};

void CAIStateGroup_WhisperRetreat::Update()
{
    if (!IsThingCarryingWeapon(PBrain->PCreature))
        PBrain->PCreature->UnsheatheMeleeWeapon();
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 173);
    if (IS_THING_ALIVE(PTarget)) {
        CTCStealth* stealth;
        if (PTarget.GetPItem()->GetInterface(&stealth, 54))
            stealth->SetAsInDanger(PBrain->PCreature, 3);
        CThing* creature = PBrain->PCreature;
        CThing* target = PTarget.GetPItem();
        const C3DVector* pos = &creature->GetPos();
        const C3DVector* targetPos = &target->GetPos();
        float dx = targetPos->X - pos->X;
        float dy = targetPos->Y - pos->Y;
        if ((float)sqrt(dx * dx + dy * dy) < 3.0f)
            JumpToSafety();
        switch (boss->GetBossPhase()) {
        case 0:
            if (boss->IsReadyForPhaseChange())
                boss->SetNextBossPhase(1, false);
            break;
        case 2:
            UseWill();
            Terminate();
            return;
        default:
            break;
        }
    }
    Terminate();
}