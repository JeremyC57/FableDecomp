// CTCVillage::RemoveCrimeIfApologisable(long deedHandle) -- unless the hero apologised less than 450 frames ago, drops the crime when its deed is over and its deed type can be apologised for
// Leaf: RemoveCrimeIfApologisable
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065C1FC, 210 bytes (`ret 4`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb `?RemoveCrimeIfApologisable@CTCVillage@@QAEXJ@Z`.
// Callees: CTCBase::GetWorld() 0x00686A70 (its result only feeds the static frame getter); CWorld::GetWorldFrame()
//          0x0049D870 (static); CWorld::PeekMyPlayer() const 0x0049D9D0 (CMainGameComponent::Get() 0x013B86A0 +0x24);
//          CPlayer::GetPControlledCreature() 0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x51); CVectorMap<
//          ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020; CTCHeroOpinionDeedLog::GetDeedFromHandle(long) 0x006C58F0;
//          the effects-def getter 0x006C5630 (landed spelling GetDeedEffectsDef); CTCVillage::RemoveCrime(long) 0x0065C146;
//          the def's slot 1 on the last release.
// Layout: retail CTCVillage LastFrameApologised +0xd8 (PDB +0x164); COpinionDeed IsStillHappening +0x21; retail
//         COpinionDeedEffectsDef flag byte +0x7c (PDB +0x90: one 12-byte list and two vectors shrink), CanApologise bit 3.
// Shape: the apology timer is reached through its member address kept in esi (`lea esi,[ebx+0D8h]`); the deed log comes
//        from the unchecked out-parameter GetInterface (the absent arm reads the handle's argument slot); the effects def
//        is a named counted-pointer local in the lookup key's frame word, released by its `__forceinline` null-tested
//        destructor (the clear of its pointer is elided before the return).
// GetWorld (agentNF14 2026-11-11): the class is now the retail engine/CTCVillage.h (FABLE_RETAIL_CTCVILLAGE: CTCVillage : CTCBase), so
//   the frame read binds CTCBase::GetWorld() const 0x00686A70; the deed-log effects getter is Ego_r.pdb
//   ?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z (engine/
//   CTCHeroOpinionDeedLog.h _DEEDS) and its result the shared CDefPointer<const COpinionDeedEffectsDef> (count-reference release).
// Layout (supersedes any local-model note above): engine/CTCVillage.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
// Name fix (agentNF5 2026-11-10): CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0 is const and returns a const pointer (?GetDeedFromHandle@CTCHeroOpinionDeedLog@@QBEPBVCOpinionDeed@@J@Z, Ego_r.pdb / FableWin.pdb).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CRIMES
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"
#include "stlport/fable_map.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeed.h"
#include "engine/CVectorMap.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CWorld { public:
    const CPlayer* PeekMyPlayer() const;        // 0x0049D9D0 (Ego_r.pdb / FableWin.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
    static long __fastcall GetFrame();                // 0x0049D870
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};

class COpinionDeedEffectsDef : public CDefPointeeBase { public:   // the def's flag byte (retail +0x7c; Ego_r.pdb +0x90)
    unsigned char _pad_0x1c[0x7c - 0x1c];
    bool CanBeHeard : 1;                        // +0x7c bit 0
    bool ReactionRestrictToTarget : 1;          // bit 1
    bool AllowSimultaneousReactionDialogue : 1; // bit 2
    bool CanApologise : 1;                      // bit 3
};

void CTCVillage::RemoveCrimeIfApologisable(long deedHandle)
{
    long* lastApology = &LastFrameApologised;
    if (*lastApology != 0 && GetWorld().GetFrame() - *lastApology < 450)
        return;
    CThing* hero = CMainGameComponent::Get()->World->PeekMyPlayer()->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        const COpinionDeed* deed = log->GetDeedFromHandle(deedHandle);
        if (deed) {
            CDefPointer<const COpinionDeedEffectsDef> effects = log->PeekPDeedEffectsDef(deed->DeedType);
            if (!deed->IsStillHappening && effects->CanApologise)
                RemoveCrime(deedHandle);
        }
    }
}
