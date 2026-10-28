# Retry chunk 537 (agent537, 2026-10-28): 2 of 8 parked rows land (RELOCATION_MATCH), 6 re-parked

| Address | Function | What fixed it |
|---|---|---|
| 0x00492BAB | CSubtitleRenderer::Clear | repeat `reinterpret_cast<CTextInfoQueue&>(TextInfoQueue)` in each use instead of a `queue&` local (the local swapped edi/ebx for the whole body); shared CWorld/CPlayer/CPlayerManager/CGameEvent headers |
| 0x0049EF50 | CWorld::GetPathNameForFileName | STLport inline `operator==(basic_string, const _CharT*)`, two `const CWideStringData*` locals, `wchar_t[262]` buffers; shared CWorld.h |

Re-parked (pending/2026-10-27_linux_hand537/differ): 0x0040CEC0 CUserProfileManager ctor (984v977, regalloc/frame slots);
three CTCInGameMenu rows 0x0047E651 / 0x0047E9AE / 0x0047F30F at equal length (`push ecx` scheduled before the X `fsubp`,
lane gap: scheduling); 0x00489D40 CPlayer::CreateCharacter (815v815, frame slots of three C3DVector temporaries);
0x0049BE90 CParticleEnvironmentInteraction::IsParticleCollidingAt (806v808, regalloc).

NAME_FIXES.tsv: 34 rows, deferred to the naming pass. Key: 0x00403C60 `std::__char_traits_base<wchar_t,wint_t>::compare`
(not MemCmp_Unsigned16); 0x00999850 CAFile::ScanNextFile; 0x0099A2F0 CBaseClassNonCopyable; 0x0040A740
CUserProfileManager::Save (0x0040BC80 is Apply); 0x009F1760 CGameEvent::AddData; 0x0041CC14 / 0x0041CC2B
NUISystem::CUIScreenUnitConverter::GetBackBufferWidth / Height.

Review at landing: 0x00686D40 is an identical-code-folded getter bound under 6 spellings; 0x013BCA24 (empty wide string)
has 5 bound spellings.
