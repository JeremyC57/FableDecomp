# Chunk 717 (NA060) -- 10 of 10 byte-exact (1 MATCH + 9 RELOCATION_MATCH), one opt-in branch (2026-11-02)

Nine gap-scan rows at the tail of .text are whole compiler-generated dynamic initializers (`_$E`, `.text$yc`, one `.CRT$XCU` entry each),
named by lining retail's `.CRT$XCU` order up against the `dynamic initializer for 'X'` runs of Ego_r.pdb and FableWin.pdb, Ego_r's body
sizes and each global's retail readers: VALUE_DISPLAY_DURATION 0x0121F220, CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking 0x0121F260,
JACK_TOTAL_DURATION 0x0121F780 and JACK_GLOW_START_FRAME 0x0121F7A0 (`_$E3`), CTCCreatureModeManager::CModeBase::
NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS 0x0121F7E0, MIN_TIME_TO_SIT 0x01221B60, NUISystem::CList::GPreviousSelected 0x0121A6AC (STLport list;
size-optimised TU, `#pragma optimize("s", on)`), CTCTrophy::InstructionsText 0x0121E3A0 and NFire::OPINION_SOURCE_DEF_NAME 0x0121EF70
(a `const CCharString&` bound to a temporary: atexit first, then construct, then store the address). The tenth is
NScript::CIsThingAlive::operator() 0x00CDF120 (MATCH; slot 0 of vtable 0x012C32F0). `verify.log`: WINS 10, all PASS.

Headers (hdr/README.md): opt-in FABLE_RETAIL_CCHARSTRING in fable_string.h declares retail's only `const char*` ctor
`CCharString(const char*, long length = -1)` (`??0CCharString@@QAE@PBDJ@Z`, both PDBs), which retail uses for implicit literal conversion.
Default text unchanged under `cpp`; only this chunk's rows 0x0121E3A0 / 0x0121EF70 define the macro, so no includer recheck was needed.

Anonymous-namespace objects: VALUE_DISPLAY_DURATION, JACK_*, MIN_SPEECH_FRAMES and RANGE_TIME_TO_SIT are TU-local in retail, so their mangled
names carry a per-file `?A0x<hash>` that depends on the source path. 0x013BAFD8 (JACK_TOTAL_DURATION) is therefore bound under two hashes by
0x0121F780 and 0x0121F7A0 (one retail object, two TU-local spellings; unavoidable while the two rows are separate TUs). Readers in other
TUs keep their extern stand-ins until the owner decides (queued).

Queued PRIORITY (NAME_FIXES_QUEUE.tsv, c717): the class statics DrunkennessDeltaPerFrameOfDrinking and the three CModeBase NO_FRAMES_* bound
as g_* stand-ins by their landed readers / initializer rows; the anonymous-namespace reader question; the three fake atexit-thunk rows
0x01228E44 / 0x012294D0 / 0x01229B40 (replacements graded RELOCATION_MATCH from this chunk's TUs are in thunks/); the descriptive landed
initializer rows 0x0121F7C0 / 0x0121F7D0 / 0x0121F800 / 0x0121E3D0 / 0x01221B80 and 0x0121E260's non-retail test literals. Header debt:
engine/CList.h flattens NUISystem::CList into a global struct; fable_string.h's `storage_` is PDB `PStringData`. The agent's 15 NAME_FIXES
rows are queued as c717; labels in ghidra_out/labels_chunk717_2026-10-27.tsv.
