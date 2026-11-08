// CCreatureAction_TentacleStrikeOutOf::Initialise() -- remove creature mode 0x53 through the owner's mode manager, clear the tentacle's obstruction and striking state, animation TENTACLE_SMASH_OUT_OF, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_TentacleStrikeOutOf@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF540, length 201 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, then 0xE3 = CTCKrakenTentacle), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0,
// CTCKrakenTentacle::RemoveObstruction @ 0x007F0810, CTCKrakenTentacle::SetIsStriking(bool) @ 0x007F0770,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the checked mode-manager lookup (component left in eax), the UNCHECKED tentacle lookup in an inline accessor (absent
// arm reads the shared key word, which the block-scoped name string reuses afterwards), the tentacle kept in esi for two calls.
// Layout: owner CIntelligentPointer +8 (CCombatActionBase is 0x114 in retail).
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CTCKrakenTentacle { public:
    void RemoveObstruction();                   // 0x007F0810
    void SetIsStriking(bool striking);          // 0x007F0770
};

class CCombatActionBase { public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    FABLE_FORCEINLINE CTCKrakenTentacle* GetTentacle() const
    {
        CTCKrakenTentacle* tentacle;
        PCreature.Get()->GetTC(TCI_KRAKEN_TENTACLE, &tentacle);
        return tentacle;
    }
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_TentacleStrikeOutOf : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_TentacleStrikeOutOf::Initialise()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        modeManager->RemoveMode(0x53);
    CTCKrakenTentacle* tentacle = GetTentacle();
    tentacle->RemoveObstruction();
    tentacle->SetIsStriking(false);
    {
        CCharString name("TENTACLE_SMASH_OUT_OF", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}