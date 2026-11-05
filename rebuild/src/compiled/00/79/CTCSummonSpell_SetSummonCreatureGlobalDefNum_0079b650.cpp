// CTCSummonSpell::SetSummonCreatureGlobalDefNum(long defNum, const C3DVector& position, const C3DVector& direction) -- record the summoned creature's global def, play the summon-upgrade sound on the caster, kill the current minion and create the new one
// Leaf: SetSummonCreatureGlobalDefNum
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079B650, length 149 bytes (`ret 0Ch`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC of interface 0x10 = bit 16 of CThing+0x20, the
// sound-player component), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("HERO_ABILITY_SUMMON_CREATURE_UPGRADE" @ 0x0126C3F0),
// 0x0066BD00 (a CTCSoundPlayer play-sound member taking (const CCharString&, bool, long, long, long); manifest label "CSoundInfo::PlaySound"),
// CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80 (true),
// CTCSummonSpell::CreateMinion(const C3DVector&, const C3DVector&) @ 0x0079ACC0.
// Globals: CTCSummonSpell::SummonCreatureGlobalDefNum @ 0x0138306C (static member).
// Layout: Ego_r.pdb CTCSummonSpell: Minion (CIntelligentPointer<CThingAICreature>) at the odd offset +0x11 (`#pragma pack(1)`).
// Shape: the component is a named local (`mov esi,[eax+4]`); the CCharString is a named local in its own block, sharing the dead defNum
// argument slot with the lookup key.
class CTCBase;
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCSoundPlayer { public:
public:
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};
#pragma pack(push, 1)
class CTCSummonSpell { public:
    void* __vfptr;                              // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _base08[4];
    void* PDef;                                 // +0x0c CDefPointer<const CSpecialAbilitiesSummonSpellDef>
    bool  JustReleased;                         // +0x10
    CIntelligentPointer<CThing> Minion;             // +0x11
    static long SummonCreatureGlobalDefNum;     // 0x0138306C
    void CreateMinion(const C3DVector& position, const C3DVector& direction);   // 0x0079ACC0
    void SetSummonCreatureGlobalDefNum(long defNum, const C3DVector& position, const C3DVector& direction);
};
#pragma pack(pop)

void CTCSummonSpell::SetSummonCreatureGlobalDefNum(long defNum, const C3DVector& position, const C3DVector& direction)
{
    SummonCreatureGlobalDefNum = defNum;
    if (ParentThing->HasTC(TCI_SOUND_PLAYER)) {
        CTCSoundPlayer* sound = (CTCSoundPlayer*)((CTCSoundPlayer*)&ParentThing->GetTC(TCI_SOUND_PLAYER));
        {
            CCharString name("HERO_ABILITY_SUMMON_CREATURE_UPGRADE");
            sound->PlayAttachedCriteria(name, true, 0, 0, -1);
        }
    }
    Minion.Get()->Kill(true);
    CreateMinion(position, direction);
}