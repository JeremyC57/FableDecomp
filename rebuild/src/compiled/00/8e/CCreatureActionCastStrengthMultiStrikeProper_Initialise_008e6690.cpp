// CCreatureAction_CastStrengthMultiStrikeProper::Initialise() -- load SPECIAL_ABILITIES_MULTI_STRIKE_DEF, play its sound for this spell level through the owner's sound component (if any), then CCombatActionBase::Initialise
// Leaf: ?Initialise@CCreatureAction_CastStrengthMultiStrikeProper@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6690, length 226 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("SPECIAL_ABILITIES_MULTI_STRIKE_DEF" @ 0x01235A9C), CGameDefinitionManager::Get()
// 0x0044C6B0 (`__fastcall`, twice), the manager's GetDefIndexFromName(const CCharString&, long) 0x009AD390 (manifest label
// "COptimisedPrimitive::ActualToUniqueIndex" is propagated), CDefinitionManager::GetDef<CSpecialAbilitiesMultiStrikeDef>(long,
// CDefPointer*) 0x007CD760, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x10),
// CDefStringTable::GetString(CDefString) 0x009D49B0 on the global table @ 0x013CA828 (CCharString by hidden pointer), the sound
// component's play-by-name 0x0066BD00, CCombatActionBase::Initialise 0x00857520; the def's virtual slot 1 on the last release.
// Shape: the def-name string is a named local in its own block (as the landed CTCHeroStats::OnInitialActivate 0x0057D1A4); the
// def pointer is a function-scope local released at the end (refcount at +4); Sounds[SpellLevel - 1] reads `[def+level*4+3Ch]`.
// Layout: SpellLevel at retail +0x114 (PDB +0x12c); CSpecialAbilitiesMultiStrikeDef Sounds (CDefString[4]) at +0x40 (Ego_r.pdb).
class CTCBase;

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

class CDefString { public: long Index; };

class CDefStringTable { public:
    CCharString GetString(long index) const;     // 0x009D49B0 Ego_r/FableWin ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z (agentNF16)
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)        // 0x013CA828

class CSpecialAbilitiesMultiStrikeDef { public:
    virtual void v00();
    virtual void Release();                     // slot 1 (+4): destroys the def when the last reference goes
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x40 - 0x08];
    CDefString    Sounds[4];                    // +0x40
    CDefString    Animations[4];                // +0x50
};

struct CDefPointerMultiStrike {                 // CDefPointer<const CSpecialAbilitiesMultiStrikeDef>
    CSpecialAbilitiesMultiStrikeDef* Object;
    CDefPointerMultiStrike() : Object(0) { }
    ~CDefPointerMultiStrike()
    {
        if (Object && --Object->RefCount == 0)
            Object->Release();
    }
    CSpecialAbilitiesMultiStrikeDef* operator->() const { return Object; }
};

class CDefinitionManager { public:
    long GetGlobalIndexFromClassIndex(CCharString const&a0, long a1) const;   // 0x009AD390 (Ego_r.pdb ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z)
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                      // 0x0044C6B0
    bool GetDef(long index, CDefPointerMultiStrike* out);                 // 0x007CD760
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointerMultiStrike* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
class CTCSoundPlayer { public:
    unsigned long PlayAttachedCriteria(CCharString const&a0, bool a1, bool a2, bool a3, long a4);   // 0x0066BD00 (Ego_r.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z)
};
#pragma auto_inline(on)

struct CTCSound {                               // the interface-0x10 component
};

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

class CCreatureAction_CastStrengthMultiStrikeProper : public CCombatActionBase {
public:
    long SpellLevel;                            // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_CastStrengthMultiStrikeProper::Initialise()
{
    CDefPointerMultiStrike def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_MULTI_STRIKE_DEF");
        index = ((CDefinitionManager*)&(CGameDefinitionManager::Get()))->GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    CTCSound* sound;
    if (PCreature.Get()->GetTC(TCI_SOUND_PLAYER, &sound)) {
        CCharString soundName = GDefStringTable.GetString((def->Sounds[SpellLevel - 1]).Index);
        ((CTCSoundPlayer*)sound)->PlayAttachedCriteria(soundName, 1, 0, 0, -1);
    }
    CCombatActionBase::Initialise();
}