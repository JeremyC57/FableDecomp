# Chunk 586 (2026-10-30): 7 of 10 engine-range released pass 9 rows landed (audio / libpk / zlib / vorbis / one NScript)

All RELOCATION_MATCH + PASS (g++ and VC7.1). Proven names: zlib 1.2.1 fill_window (same-TU `extern "C" { static
__declspec(noinline) }` read_buf), libvorbis decode_packed_entry_number (same-TU static bitreverse), NScript::
CPersonalScript_GlobalThingsScript::CBowerstoneTeleportPointMarker::Main (shared NScript headers). Descriptive (libpk /
GameCODA have no PDB): CSoundEvent::SetPriority, CCodaPlayback::GetPlayProgress (/EHsc: the lane's first C++ EH-frame row),
CDSoundAccelerationQuery::FindDeviceInterface (/GS), CAudioSource::Initialise.
Parked (../../pending/2026-10-27_linux_hand586/differ/, each 4 grades, each with a passing test): 0x00C73660
DetectWindowsVersion (260v262: retail keeps the major version live across the NT switch), 0x00C62790
CPKDecompressedStream::Read (269v264: one read-modify-write `add [esi+8],eax` split by VC7.1), 0x00C6B9D0
CStateVariableFilter::FilterWithGain (269v269: one x87 operand order in damping*band; retail_asm row, its differ copy
carries the `__asm` helper with its Retail-asm line -- allowed for a retail_asm grade; next try the ramped twin 0x00C6B7C0 in
the same TU).
NAME_FIXES.tsv: 10 rows queued. The libpk lock family is a naming sub-wave handed to wave 10: 0x0143BAFC g_soundLock (11
bound spellings), 0x00C0FFC0 / 0x00C0FFE0 PKCProtectionInstance ctor/dtor (a per-address CScopedLock_<addr> invention in a
dozen landed rows each), 0x00C35CA0 CODA_IsSourcePlaying (3 spellings), 0x00C596E0 Initialise (landed as
CEnvironmentSpellEffectInterface::ClearScreenFilter).
Header gaps (NOTES.md): no shared header for CScriptDef (Ego_r Dinkumware sizes; retail offsets run 0xBC low near +0xF00);
CSoundEvent 0x012B3674 and CAudioContext 0x012B8C18 are flattened in the landed rows with unknown slot names; no libpk /
GameCODA engine headers at all. GameCODA has its own all-zero GUID 0x012B8FF4 (not uuid.lib's GUID_NULL 0x012C0694).
