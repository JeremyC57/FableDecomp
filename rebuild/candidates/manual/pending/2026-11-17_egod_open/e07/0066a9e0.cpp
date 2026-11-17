// CTCSoundPlayer::PlayCollisionSound(const C3DVector& pos, const CDefPointer<const CMaterialDef>& pmaterial1, const CDefPointer<const CMaterialDef>& pmaterial2) -- when sounds are enabled play the collision sound event "SE_COLLISION;<material1>;<material2>" at pos, falling back to "SE_COLLISION;<material1>" then "SE_COLLISION;<material2>" (or the one material that is set)
// Grade: DIFFER(626v626) -- instruction-identical (q.py RELOCATION_MATCH) but WRONG CALL TARGET: the last fallback (0x0066AC08) calls ??BCDefString (0x00415D70) where retail calls ?GetString@CDefString (0x00415DD0) -- vland RELOC_CONFLICT
// Leaf: ?PlayCollisionSound@CTCSoundPlayer@@QAEXABVC3DVector@@ABV?$CDefPointer@$$CBVCMaterialDef@@@@1@Z
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
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefString.h"
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CMaterialDef.h"
#include "engine/CThing.h"
#include "engine/CTCSoundPlayer.h"
extern bool          g_bSoundEnabled;                  // 0x0137E974
extern unsigned long g_InvalidSoundHandle;             // 0x0129D25C

void CTCSoundPlayer::PlayCollisionSound(const C3DVector& pos, const CDefPointer<const CMaterialDef>& pmaterial1,
                                        const CDefPointer<const CMaterialDef>& pmaterial2)
{
    if (g_bSoundEnabled)
    {
        CASoundBank& bank = GetParentThing().GetSampleBank();
        CCharString first_part("SE_COLLISION;");
        if (pmaterial1.IsNull())
        {
            if (!pmaterial2.IsNull())
                InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial2->GetInstantiationName(), false, false);
        }
        else if (pmaterial2.IsNull())
        {
            InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName(), false, false);
        }
        else if (InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName() + ";" + pmaterial2->GetInstantiationName(), false, false) == g_InvalidSoundHandle)
        {
            if (InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName(), false, false) == g_InvalidSoundHandle)
                InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial2->GetInstantiationName(), false, false);
        }
    }
}
