// CGameScriptInterface::GetOpinionAttitudeAsString(EOpinionAttitudeType attitude, CCharString& out) const -- copy the attitude's name string into out
// Leaf: ?GetOpinionAttitudeAsString@CGameScriptInterface@@UBEXW4EOpinionAttitudeType@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890970, 22 bytes: `lea ecx,[eax*4+0x013BABD0] ; push ecx ; mov ecx,[esp+0Ch] ;
//   call 0x0099EFB0 ; ret 8`.
// NF52 (round 7): moved onto the shared headers. The row was a free __stdcall stand-in over a made-up int table g_attTable and a
// stand-in StrDest. Identity: Ego_r.pdb 0x008CD7F0 / FableWin.pdb 0x02A8FA90
//   ?GetOpinionAttitudeAsString@CGameScriptInterface@@UBEXW4EOpinionAttitudeType@@AAVCCharString@@@Z, vtable slot 607 in
//   engine/CGameScriptInterface.h (0x00890970; reached through the vtable only, no E8 caller), between
//   GetNextInOpinionAttitudeGraph 0x00890960 and EntityGetOpinionAttitudeToPlayer 0x00890990.
// Data: 0x013BABD0 is the private static COpinionReactionManager::AttitudeStrings (?AttitudeStrings@COpinionReactionManager@@0QBVCCharString@@B,
//   both PDBs' dynamic initializer / atexit destructor; 4-byte CCharString, hence the `*4` index), read through FableWin.pdb's public
//   inline ?GetAttitudeString@COpinionReactionManager@@QBEABVCCharString@@W4EOpinionAttitudeType@@@Z on the world's manager (FableWin
//   ?GetOpinionReactionManager@CWorld@@QAEAAVCOpinionReactionManager@@XZ, inline; the World / manager loads fold away because the
//   accessor never reads its object). CCharString::operator=(const CCharString&) 0x0099EFB0.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CCHARSTRING
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/CWorld.h"
class CTCOpinionSource;
#include "engine/COpinionReactionManager.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::GetOpinionAttitudeAsString(EOpinionAttitudeType attitude, CCharString& out) const
{
    out = World->GetOpinionReactionManager().GetAttitudeString(attitude);
}
