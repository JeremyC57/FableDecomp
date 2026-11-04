// CTCSoundPlayer::SetVoiceName(const CCharString& name) -- set the voice bank name the sound player uses for dialogue
// Leaf: ?SetVoiceName@CTCSoundPlayer@@QAEXABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A9D0, 8 bytes: `add ecx,14h ; jmp 0x0099EFB0` (VoiceName = name, tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?SetVoiceName@CTCSoundPlayer@@QAEXABVCCharString@@@Z, and VoiceName is the CCharString
//   at +0x14 (engine/CTCSoundPlayer.h / Ego_r struct_layouts: PAttachedEmitter +0x0C, VoiceName +0x14, ExtraTag +0x18), the offset this
//   body assigns; the sole caller CThingCreatureBase::PlayCreatureContinuousSounds 0x00666034 passes a creature's sound player. The
//   catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ??4CCharString@@QAEABV0@ABV0@@Z 0x0099EFB0.
// Header: engine/CTCSoundPlayer.h (FABLE_RETAIL_CTCSOUNDPLAYER_VOICENAME).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_VOICENAME
#include "fable_string.h"
#include "engine/CTCSoundPlayer.h"
void CTCSoundPlayer::SetVoiceName(const CCharString& name)
{
    VoiceName = name;
}