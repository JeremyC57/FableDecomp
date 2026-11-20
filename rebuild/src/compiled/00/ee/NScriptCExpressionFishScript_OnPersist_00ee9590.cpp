// NScript::CExpression_FishScript::OnPersist(CPersistContext& context) -- persists LastFishingLevel (+0x44) with a zero default
// Leaf: ?OnPersist@CExpression_FishScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE9590, oracle length 35 bytes.
// Retail bytes:  push ecx ; lea eax,[esp] ; push eax ; add ecx,0x44 ; push ecx ; mov ecx,[esp+10h] ; push offset "LastFishingLevel" ; mov dword ptr [esp+0Ch],0 ; call CPersistContext::Transfer<long> ; pop ecx ; ret 4
// NF188 (2026-11-20, q6280): self rename. Ego_r.pdb 0x00F62880 + FableWin.pdb 0x03AD6DE0
//   ?OnPersist@CExpression_FishScript@NScript@@UAEXAAVCPersistContext@@@Z; vtable 0x012F54E4 slot 4 of CExpression_FishScript (stored by the
//   landed ctor 0x00EE94A0); the transferred key "LastFishingLevel" is the Ego_r member at retail +0x44. Now on the shared
//   engine/CExpression_FishScript.h (was the stand-in CPersistable_00ee9590::PersistTransfer_LastFishingLevel_00ee9590, one of 16 bodies of a
//   generated shape). Callee CPersistContext::Transfer<long>(name, value, default) 0x00410BE0 through a row-local declaration.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CExpression_FishScript.h"
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>
};

void NScript::CExpression_FishScript::OnPersist(CPersistContext& context)
{
    long defaultValue = 0;
    context.Transfer("LastFishingLevel", LastFishingLevel, defaultValue);
}
