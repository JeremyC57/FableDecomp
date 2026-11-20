# NF200 notes (2026-11-20, round 10)

## Descriptive / library names (made-up-name rule)
- `CAudioSource::StartExponentialFade` (0x00C720C0, libpk OpenAL; no PDB class): kept as the self row's existing Leaf spelling; 0x00C72E00 respelled in place from `StartFade` so the address ends on one name. Descriptive (body is an exponential gain ramp); light renaming only.
- 0x008EC1A0 `std::vector<NCreatureMode::CModeHoverBase::CHoverState>::push_back`: STLport header spelling (library-attested; PDBs hold no STL instance names). Element class from the Ego_r.pdb class key `CModeHoverBase::HoverStates`.
- 0x00CB1890 `CBaseList::~CBaseList`, 0x00CB1340 `CBaseList::RemoveAll`, 0x00CB1210 `CBaseList::CNodeCache::~CNodeCache`: DirectShow strmbase (wxlist.cpp) vendor names, already proven by the landed vendor-proof lines; not Fable PDB names.

## `// Inferred:` lines added
- 0x007C9180: the block-scoped named noise-string local (retail pushes it from a register, frame 0x120).
- 0x0047DB73: `IS_THING_ALIVE(static_cast<CThing*>(c))` with FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (the cast is a stand-in reach to the template; reported).

## Local models (narrow rule: classes with no shared header)
- 0x006412B0: `CTCParticleAttacher` (replaces the old `CTCAttachedParticles` local stand-in; no shared header exists).
- 0x008EC1A0: local `NCreatureMode::CModeHoverBase` on `CTCCreatureModeManager::CModeBase` (no shared header).
- 0x00CB1890: local strmbase `CBaseList` / `CNode` / `CNodeCache` (vendor class, no shared header).
- 0x00C162A0 (rewritten libpk self row + test): local `SPKVoiceSettings` (the landed 0x00C2D1A0 row's class, no shared header) with member `CMember_00c2d3b0` (the token 0x00C2D3B0 already uses for 0x00C167C0's class). A self-row and test rewrite on a libpk entry: flag for the coordinator under the light-renaming rule.

## Stand-in casts (reported per the brief)
- 0x0047DB73: `static_cast<CThing*>` on the row's creature pointer to reach `IS_THING_ALIVE<CThing*>`; other stand-ins in that row are findings only: `D::Do` = bitset::test, `GlobalGui::Notify` = `CPlayerGui::ToggleLiveGui` on the singleton PClass at 0x013B8790.

## New lock claims
- 007c93b0, 00da26b0, 00cb1890, 012173e0, 00c287e0, 012134ab, 00c2d3b0 (was `released`), 01213b0a (plus the rows.txt locks).
- Read-only HEAD grades (no edits, no locks) of unclaimed rows for census: 0x00C28AE0, 0x00C2D2F0, 0x00C161A0, 0x00CB1890 HEAD, 0x00C287E0 HEAD, 0x00C2D3B0 HEAD.

## Decisions for the coordinator
1. TU-emitter `??_G` rows 0x00868350 / 0x008C9690 / 0x008CED90 / 0x008CEDD0 still spell `??1CBase_0062e900@@QAE@XZ` (q4411 / q6405 blocked on them).
2. 0x00787E12 LookAtPos: a same-TU stand-in body with 14 local classes; needs a one-definition decision before porting (q4492 / q4522 blocked).
3. 0x007C93B0 defines `?FrameUpdate@CTCFireballSpell@@QAEXXZ` but both PDBs say UAE (virtual): NAME_FIXES.tsv.
4. 0x006463FF: the CIntelligentPointer template hijacks the `IS_THING_ALIVE(&PThing)` stand-in (C2039 IsDead); blocked.
5. Self renames that narrow onto an EXISTING spelling but leave other REFS rows unmoved (0x00CB1890 -> ??1CBaseList, 0x00C162A0 -> ??1SPKVoiceSettings): the brief's letter says the rename waits; q6930 / q6932 marked blocked on that decision (see STATUS census).
6. libpk scope: the brief title says libpk / GameCODA are excluded, yet q6931 (0x00C29CA0), q6932 (0x00C162A0), q12157 (0x00C10940) and q12164 (0x00C1EB10) are libpk-range; NF200 applies in-place unification only to these.

## Funclet includes
- 0x012173E0 includes the 0x00CB1890 parent by path (rebuild/src/compiled/00/cb/unattributed_CEngineLandscapePatch00cb1890_00cb1890.cpp). If the coordinator renames that file (its catalog name changes to CBaseList), the funclet's `#include` must follow.
- 0x012134AB includes the 0x00C287E0 parent by path (rebuild/src/compiled/00/c2/unattributed_CStackOwner00c287e0dtor_00c287e0.cpp); NF200 does not rename that file.

## q6931 (libpk, light unify)
- 0x00C29CA0 kept its self Leaf `??1CBuffer_00c29ca0@@QAE@XZ` (house form of the 11 libpk vector-_Tidy bodies); the parent 0x00C287E0's member struct token respelled in place. Coordinator decision: the real name is either `CArray<CHandleInfo>::~CArray` (both PDBs' member type) or `std::vector<CHandleInfo>::~vector` (libpk header), not decidable from bytes; the proper fix is a family port of 0x00C287E0 + 0x00C22520 onto libpk/CHandleBasedMemoryPool_012b4098.h (would move the parent's self leaf).

## q6932 (libpk identity)
- 0x00C162A0 = destructor of the 0x1c record constructed by 0x00C2D1A0; named after that landed ctor (SPKVoiceSettings, descriptive). Self row and test rewritten (bytes and PassPattern unchanged).
- Coordinator decisions: (1) the same 0x1c type is SPKVoiceSettings (0x00C2D1A0 / 0x00C2D2F0 / now 0x00C162A0 / 0x00C2D3B0), SThemeRequest (0x00C2D5E0 self leaf ?Assign@CInstanceManager@@QAEXMPBUSThemeRequest@@@Z) and CVolumeRecord_00c161a0 (0x00C161A0): pick one; 0x00C161A0 + funclet 0x01211CA9 wait on it. (2) 0x00C167C0 has 8 spellings; PKCAutoDeleteArray<unsigned short>::~PKCAutoDeleteArray is the likely name (0x00C167B0 ctor landed as ??0?$PKCAutoDeleteArray@G@@QAE@XZ; 0x00C2D1A0 constructs the record's +4 member with it).
- Funclet include: 0x01213B0A includes the 0x00C2D3B0 parent by path; file not renamed.

## recheck_final.log (22 rows, one vland --recheck-landed run, all PASS: 21 RELOCATION_MATCH + 0x00E83680 MATCH)
- Rows: every changed row, the three funclets (0x012173E0 / 0x012134AB / 0x01213B0A on their parents' new sources) and the overlay includers 0x008BABE0 / 0x007A6760.
- Residual warnings, all pre-existing at HEAD (not introduced here): SHARED_HEADER_AVAILABLE for 0x006412B0 (local CThing / CTCLight models), 0x007C93B0 (local models; only the callee was respelled), 0x008BABE0, 0x00C287E0 / 0x012134AB (local libpk CString); VENDOR_PROOF_MISMATCH on 0x00CB1890 (the strmbase tree is not under FABLE_THIRDPARTY on this lane). The 0x012173E0 VENDOR_PROOF_MISMATCH ('claim names 0x00CB1890') is an artifact of the recheck payload pasting the parent source into the funclet row, not of the landed funclet file (it only #includes the parent).
- 0x007C7A00: the local `ptc_emitter` was renamed `particleEmitter` (selfcheck's `_emit` token scan matched the identifier); bytes unchanged.
- Funclet payloads used funclet_offset 0: verify_and_land.py's funclet_extractor prefers that offset but searches the whole $L block (relocation-masked) for the oracle slice, so the funclet grades are real; the parents' defined-symbol diffs prove the bound names.
