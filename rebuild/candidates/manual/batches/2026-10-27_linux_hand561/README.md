# Chunk 561 (agent561, 2026-10-29): 10 of 12 never-tried rows land (RELOCATION_MATCH), 2 parked

Landed: CPersistContext::Transfer<bool> 0x004045C0, <float> 0x00410620, <unsigned long> 0x004106F0,
<CWorld::ESaveType> 0x004AB640 (the text-arm wrapper vtable's RTTI names T; the catalog's "Transfer<signed char>" was
propagated across every instantiation), CUserProfileManager::ApplyNewProfileValues 0x0040CCA0, NUISystem::CSprite::~CSprite
0x0041AB40, NUISystem::CManager::GetCorrectComponentInterface 0x0041CD93, CDisplayEngine::SetAboutToRun 0x00434CD0,
CWorld::UpdateSaveGameLoad 0x004A2F10, CThing::~CThing 0x004C8310.

0x004045C0 replaces the bootstrap asm_bake oracle `CPersistContext_TransferSignedChar_004045c0.cpp`, which fixmove moved
to rebuild/integration/bootstrap_asm/ (build_bootstrap.ps1 repointed).

Parked (pending/2026-10-27_linux_hand561/differ), both byte-exact over their true extent with relocations masked; the
catalog rows are over-spanned: 0x00405A60 Transfer<unsigned char> (true end 0x00405B30) and 0x0044BB00
Transfer<unsigned __int64> (runs into the 2-argument overload at 0x0044BBE0). Next: re-split the rows and land as is.

NAME_FIXES.tsv: 55 rows, deferred (naming pass).
