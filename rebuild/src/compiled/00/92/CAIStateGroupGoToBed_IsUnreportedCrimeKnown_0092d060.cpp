// CAIStateGroup_GoToBed::IsUnreportedCrimeKnown() -- does this creature know of a hero crime it should run to report, or an ongoing crime against its home?
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092D060, length 388 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (the creature is reached through a const
//          getter), CTCVillageMember::GetPVillage() const @ 0x007EE540 (interface 0x23; manifest "IS_THING_ALIVE<...>" is
//          propagated, landed callers 0x00833710 / 0x007B6A80 name it), CTCCreatureOpinionOfHero::FindMostRecentDeed<
//          COpinionDeedFilter_IsCrimeToRunAndFindAGuard> @ 0x006610C7 and FindMostRecentDeed<CFilter_Bind2<
//          COpinionDeedFilter_IsStillHappening, COpinionDeedFilter_TargetIs> > @ 0x0092E630, CBaseIntelligentPointer::GetPItem
//          @ 0x00A01B50.
// Shape: HasTC returns the masked bitset word (`test byte ptr [eax+24h],8` for 0x23, `test ch,ch ; jns` for 0x4F);
//          the components come from the ternary GetTC pick (0x23 lands in ecx as `this`, the opinion in esi); the
//          tests are nested so every miss shares the one `xor al,al` epilogue;
//          the brain's creature is re-read for every use; the home is fetched again for each TargetIs filter
//          (filter objects: the empty filters in frame byte +3, TargetIs at +4, the bind pair at +8). Deed types
//          10, 11 and 2 are tried in that order.
// Layout: creature Home intelligent pointer at retail +0x168 (as the landed 0x009580E0); bitset +0x20, interface map
//          +0x44, dead flag +0x91. No engine header exists for CAIStateGroup_GoToBed.
#include "fable_compiler.h"
class CTCBase;
class CThing;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingPhysical;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x168 - 0x92];
    CBaseIntelligentPointer Home;               // +0x168 (CThingAICreature)
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
};

class CTCVillage;
class CTCVillageMember { public:                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};

class COpinionDeed;
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public: COpinionDeedFilter_IsCrimeToRunAndFindAGuard() {} };
class COpinionDeedFilter_IsStillHappening { public: COpinionDeedFilter_IsStillHappening() {} };
class COpinionDeedFilter_TargetIs { public: CThing* Target; COpinionDeedFilter_TargetIs(CThing* p) : Target(p) {} };
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {}
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

class CTCCreatureOpinionOfHero { public:               // interface 0x4f
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* index, bool includeReported);   // 0x006610C7
    COpinionDeed* FindMostRecentDeed(long deedType, const CFilter_Bind2<COpinionDeedFilter_IsStillHappening, COpinionDeedFilter_TargetIs>& filter,
                                     long* index, bool includeReported);   // 0x0092E630
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
    const CThing* GetPCreature() const { return PCreature; }
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
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CBaseIntelligentPointer PBed;               // +0x14
    bool IsBedSleepPointMarker;                 // +0x1c
    bool IsUnreportedCrimeKnown();
};

bool CAIStateGroup_GoToBed::IsUnreportedCrimeKnown()
{
    if (PBrain->GetPCreature()->HasTC(0x23)
        && PBrain->GetPCreature()->GetTC<CTCVillageMember>(0x23)->GetPVillage()
        && PBrain->GetPCreature()->HasTC(0x4f)) {
        CTCCreatureOpinionOfHero* opinion = PBrain->GetPCreature()->GetTC<CTCCreatureOpinionOfHero>(0x4f);
        if (opinion->FindMostRecentDeed(COpinionDeedFilter_IsCrimeToRunAndFindAGuard(), 0, true))
            return true;
        CThing* home = ((CThing*)PBrain->GetPCreature()->Home.GetPItem());
        if (home && !(home->m_flags91 & 1)) {
            if (opinion->FindMostRecentDeed(10, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(((CThing*)PBrain->GetPCreature()->Home.GetPItem())), 0, true)
                || opinion->FindMostRecentDeed(11, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(((CThing*)PBrain->GetPCreature()->Home.GetPItem())), 0, true)
                || opinion->FindMostRecentDeed(2, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(((CThing*)PBrain->GetPCreature()->Home.GetPItem())), 0, true))
                return true;
        }
    }
    return false;
}