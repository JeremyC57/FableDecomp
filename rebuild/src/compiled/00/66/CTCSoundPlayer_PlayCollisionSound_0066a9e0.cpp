// CTCSoundPlayer::PlayCollisionSound(const C3DVector& pos, const CDefPointer<const CMaterialDef>& pmaterial1, const CDefPointer<const CMaterialDef>& pmaterial2) -- when sounds are enabled play the collision sound event "SE_COLLISION;<material1>;<material2>" at pos, falling back to "SE_COLLISION;<material1>" then "SE_COLLISION;<material2>" (or the one material that is set)
// Grade: RELOCATION_MATCH under the 2026-11-19 CDefString binding (owner: "Swap and re-land"): the FableWin shape (early both-null return,
//   dead ASSERT arms, implicit conversions, no `name` local); VC7.1's plain inline heuristic expands the conversion at the first four
//   sites and calls it out of line at the fifth, ??BCDefString 0x00415DD0, as retail 0x0066AC08.
// Leaf: ?PlayCollisionSound@CTCSoundPlayer@@QAEXABVC3DVector@@ABV?$CDefPointer@$$CBVCMaterialDef@@@@1@Z
// Flags: none
// Retail: 0x0066A9E0, 626 bytes (ret 0Ch).
// Names: Ego_r / FableWin ?PlayCollisionSound@CTCSoundPlayer@@...; Ego_d parameters pos / pmaterial1 / pmaterial2 and the block locals
//        bank (CASoundBank&) and first_part (CCharString). Globals: the same-TU landed spellings g_bSoundEnabled 0x0137E974 and
//        g_InvalidSoundHandle 0x0129D25C (descriptive; no PDB data name found).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CTCSOUNDPLAYER_INTERNALPLAYSOUNDEVENTATPOS
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_string.h"
#include "fable_assert.h"
#include "engine/C3DVector.h"
#include "engine/CDefString.h"
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CMaterialDef.h"
#include "engine/CThing.h"
#include "engine/CTCSoundPlayer.h"
extern bool          g_bSoundEnabled;                  // 0x0137E974
extern const unsigned long g_InvalidSoundHandle;             // 0x0129D25C

void CTCSoundPlayer::PlayCollisionSound(const C3DVector& pos, const CDefPointer<const CMaterialDef>& pmaterial1,
                                        const CDefPointer<const CMaterialDef>& pmaterial2)
{
    if (g_bSoundEnabled)
    {
        CASoundBank& bank = GetParentThing().GetSampleBank();
        CCharString first_part("SE_COLLISION;");
        if (pmaterial1.IsNull() && pmaterial2.IsNull())
            return;
        if (pmaterial1.IsNull())
        {
            FABLE_RETAIL_ASSERT(pmaterial2.Get());
            InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial2->GetInstantiationName(), false, false);
        }
        else if (pmaterial2.IsNull())
        {
            FABLE_RETAIL_ASSERT(pmaterial1.Get());
            InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName(), false, false);
        }
        else if (InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName() + ";" + pmaterial2->GetInstantiationName(), false, false) == g_InvalidSoundHandle)
        {
            if (InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName(), false, false) == g_InvalidSoundHandle)
                InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial2->GetInstantiationName(), false, false);
        }
    }
}