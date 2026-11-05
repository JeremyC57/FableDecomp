// NCreatureMode::CModeThunderLightningStorm::ResumeForCutscene() -- when no looping sound is playing, restart the storm's looping sound through the owner's sound component (interface 16)
// Leaf: ResumeForCutscene
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA990, 135 bytes. Catalog: "CTCPhysicalShieldSpell::CancelForCutscene" (propagated over the
//   same shape as 0x007A0400); this body sits between CModeDrained::FrameUpdate 0x008DA890 and
//   CModeThunderLightningStorm::CreateSkyBeam 0x008DAA80 and its offsets fit PDB CModeThunderLightningStorm, so the class is
//   CModeThunderLightningStorm; the name is inferred from the CTCPhysicalShieldSpell::ResumeForCutscene sibling (0x0079E520).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 16 = sound),
//   CDefStringTable::GetString(long) @ 0x009D49B0 on GDefStringTable 0x013CA828, the sound component's PlaySound @ 0x0066BD00,
//   CCharString::~CCharString @ 0x0099EAE0. g_InvalidSoundHandle @ 0x0129D25C.
// Layout: PDB CModeThunderLightningStorm (0xd4) with 12-byte retail vectors: Owner thing +0x04, PLightningStormDef PDB +0xac ->
//   retail +0xa4, LoopingSound PDB +0xd0 -> retail +0xc4. Def LoopingSound (CDefString id) at retail +0x64 (PDB +0x68).
// Shape: as 0x007A0400 (named sound component in esi, string in its own block sharing the key's frame word).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)        // 0x013CA828
extern unsigned long g_InvalidSoundHandle;      // 0x0129D25C
class CTCBase;
class CTCSoundPlayer { public:                               // the thing's interface-16 sound component
public:
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CSpecialAbilitiesThunderLightningStormDef { public:
    unsigned char _pad00[0x64];
    long LoopingSound;                          // +0x64 CDefString id
};
namespace NCreatureMode {
struct CModeThunderLightningStorm {
    void*   __vftable;                          // +0x00 (CModeBase)
    CThing* Owner;                              // +0x04
    unsigned char _pad08[0x9c];
    const CSpecialAbilitiesThunderLightningStormDef* PLightningStormDef;   // +0xa4 CDefPointer
    unsigned char _pada8[0x1c];
    unsigned long LoopingSound;                 // +0xc4
    void ResumeForCutscene();
};
}

void NCreatureMode::CModeThunderLightningStorm::ResumeForCutscene()
{
    if (LoopingSound == g_InvalidSoundHandle) {
        CTCSoundPlayer* sound;
        if (Owner->GetTC(TCI_SOUND_PLAYER, &sound)) {
            CCharString name = GDefStringTable.GetString(PLightningStormDef->LoopingSound);
            LoopingSound = sound->PlayAttachedCriteria(name, false, 0, 0, -1);
        }
    }
}