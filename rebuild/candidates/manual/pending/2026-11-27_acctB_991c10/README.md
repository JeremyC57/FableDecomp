# 0x00991C10 CASoundManager::RegisterAtmosSoundBank

Descriptive name, not a PDB name. No symbol in Ego_r or FableWin. The retail
progress string at 0x0129A67C is "Register Atmos Sound Bank". The landed caller
CMainGameComponent::InitSound 0x00417A58 already binds
`?RegisterAtmosSoundBank@CASoundManager@@QAEXKABVCWideString@@0@Z`.

Retail 338 bytes, thiscall `ret 0Ch`. RELOCATION_MATCH. NeedsSort is volatile:
retail stores the flag and re-reads it (`mov byte [esi+0Dh],1 ; cmp byte [esi+0Dh],bl ; je`).
Add and Sort are FABLE_FORCEINLINE so Add stays in line.

functions.tsv still says CMainGameComponent_RegisterSoundBank. Queued in NAME_FIXES.tsv.
