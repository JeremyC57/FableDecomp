// CTCPhysicalShieldSpell::PlaySoundW(const CCharString& name, bool loop) -- play a sound criteria through the parent thing's sound
// player (interface 16), or return the invalid sound handle when it has none.
// Leaf: ?PlaySoundW@CTCPhysicalShieldSpell@@IAEKABVCCharString@@_N@Z
// Retail 0x0079DE00, 91 bytes (`ret 8`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (GetTC expanded),
// CTCSoundPlayer::PlayAttachedCriteria @ 0x0066BD00 (name, false, loop, false, -1). Global 0x0129D25C = the invalid sound handle.
// Name (agentNF32, 2026-11-12; was ?PlaySoundW@CTCBulletTime@@QAEKKK@Z, the propagated label of the byte-identical 0x0079CE80):
//   Ego_r.pdb 0x007A37C0 and FableWin.pdb 0x026AAC50 ?PlaySoundW@CTCPhysicalShieldSpell@@IAEKABVCCharString@@_N@Z (protected); its
//   callers are CTCPhysicalShieldSpell::CancelMove 0x0079E300, ::FrameUpdate 0x0079EB30 and ::UnPause 0x0079EC10 (the last binds this
//   symbol); engine/CTCPhysicalShieldSpell.h declares it protected (FABLE_RETAIL_BRIAR_ROSE). Callee: the shared engine/CTCSoundPlayer.h
//   PlayAttachedCriteria. The name is the Unicode-renamed "PlaySound" (windows.h maps it to PlaySoundW).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/CThing.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CTCSoundPlayer.h"

extern unsigned long g_InvalidSoundHandle;          // 0x0129D25C

unsigned long CTCPhysicalShieldSpell::PlaySoundW(const CCharString& name, bool loop)
{
    CTCSoundPlayer* sounds;
    if (ParentThing->GetTC(TCI_SOUND_PLAYER, &sounds))
        return sounds->PlayAttachedCriteria(name, false, loop, false, -1);
    return g_InvalidSoundHandle;
}
