# NF143 notes (round 10: PRIORITY slice 11 tail and CAFile / CWideString leftovers)

Counts (APPLIED.tsv / STATUS.tsv / recheck_final.log): 4 entries: 2 done (q4942, q4943), 1 deferred (q4283: one binder ported, the
0x009F2DD0 family held by NF142), 1 open (q4285: one binder ported, 0x009C7A60 not reached). 22 rows listed: 14 ported (2 MATCH +
12 RELOCATION_MATCH, every test PASS, one recheck_final.log run), 3 not-a-binder (0x0041A060, 0x0099B6B0, 0x00999110), 5 left (the three
July candidates-header rows left as found, 0x009F2DD0 deferred to NF142, 0x009C7A60 not reached; none had three failed forms). Header
opt-ins: 1 (CUserProfileManager.h). selfcheck: 0 problems.

## q4942 / q4943 (CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90): done, one family of 10 rows
- Claim: Ego_r.pdb 0x00B6BE40 / 0x00B6BE60 and FableWin.pdb 0x03176F70 / 0x031772B0, private AAE (donor order ListInsert, ListRemove = retail
  0x009FBF90, 0x009FBFB0); engine/CResource.h already declares both under FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE.
- Re-probe: HEAD objects of every row I changed plus 0x009C8700 / 0x009FDE80 (the REFS rows changed on 11-20, by file time) were compiled
  once (work/head/, t/base.py). 0x009C8700 and 0x009FDE80 bind the claim.
- Self rows: ported onto engine/CResource.h (FABLE_RETAIL_DEF_CLASS_BASE + _STATICBLOCKS_TEXTURE_HELPERS + _CRESOURCE_TOUCHRESOURCE). No header
  change. Objects define only the leaf. MATCH + PASS. Catalog names were already right (leaf-only change; RENAMES s1 repeats them).
- Binders: 0x009C8FB0 (CResourceMethods stand-in) onto the header's inline TouchResource (the 0x009C8700 byte-twin recipe). Its local
  NGameText::CTextEntry has a shared header (engine/NGameText_CDataBank.h, whose CTextEntry base is CIVCountedPointeeBase, not CResource), so
  it is NOT re-based: the header-less CResourceMethods token stays, now empty, on the shared CResource base (narrow rule). New defined
  symbols: only the header inlines ?TouchResource@CResource@@QAEXXZ / ?GetCurrentFrame@CResourceList@@QBEKXZ (pick-any; 0x009C8700,
  0x009FDE80 and the owner 0x0041B9C0 emit the same).
- Six public-QAE rows (0x00A25530, 0x00A25E70, 0x00A269C0, 0x00A26A50, 0x00A26C60, 0x00A42FC0): in-place respell of each row's existing
  local CResource in the landed 0x00A262B0 form: ListRemove / ListInsert private, a public inline `Touch()` member defined after the local
  CResourceList (FABLE_FORCEINLINE kept where the row had it: 0x00A25530 / 0x00A25E70). The derived classes' own identical Touch() bodies
  are gone; 0x00A26A50's open-coded tail calls Touch(). Token check: no new class token. objdump -t diff against HEAD: only the touch
  COMDAT moves from ?Touch@<derived>@@QAEXXZ onto ?Touch@CResource@@QAEXXZ (the symbol 0x00A262B0 already emits; pick-any; nothing binds
  it). `Touch` is the landed descriptive spelling of FableWin.pdb's inline CResource::TouchResource; I did not use the PDB name in these
  local models because a row-local body would emit 0x0041B9C0's leaf.
  These rows keep their pre-existing SHARED_HEADER_AVAILABLE warnings (unchanged from HEAD).
- Spelling sets (exe-scan REFS rows; before = HEAD objects, after = this hand-back): 0x009FBFB0 {AAE, public QAE, CResourceMethods,
  self CResource_Methods} -> {AAE, public QAE (July rows only)}; 0x009FBF90 {AAE, QAE PAV1@0, CResourceMethods, self CResource_Methods,
  QAE PAV1@PAX (July)} -> {AAE, QAE PAV1@PAX (July rows only)}. No spelling created.

### One definition (link proofs in link/, LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED)
- 0x009FC170 CResourceList::UnloadNextResource (claimed, outside my entries) defined BOTH new leaves: a strong same-TU ListRemove copy under
  `auto_inline(off)` and a pick-any in-class ListInsert. link/t1.log: new self rows + HEAD 0x009FC170 -> LNK2005 x2 + LNK1169 (exit 145).
- Fix (2 forms): form 1 declares ListRemove only (RELOCATION_MATCH: the strong copy is not needed by the bytes, so nf96a does not apply);
  form 2 also writes ListInsert's four statements out at its one use (retail expands it there; the 0x009FC4A0 SetAsMRU precedent). Its
  object now defines only its own leaf. link/new.log: new 0x009FBFB0 / 0x009FBF90 / 0x009FC170 + HEAD 0x009FC020 / 0x009FC450 / 0x009FC4A0 /
  0x009FC250 / 0x009C8700 / 0x0041B9C0 -> exit 0 (LNK4088 only).
- HEAD control (link/ctl_*.log): the HEAD self rows already clashed with the pick-any stand-in copies in 0x009FC020, 0x009FC4A0, 0x009FC450
  (?ListRemove@CResource_Methods@@QAEXXZ, 3x) and 0x009FC450 (?ListInsert@CResource_Methods@@..., 1x): 4 LNK2005 at HEAD, 0 now.
- Remaining definers (not changed, no clash): 0x009FC250 defines ?ListRemove@CResource@@QAEXXZ (pick-any local copy; it expands the body
  and references no entry address); 0x009FC020 / 0x009FC450 / 0x009FC4A0 the CResource_Methods stand-in copies (nothing defines those
  strongly any more).
- Funclets: no Unwind funclet or other source includes any changed row's file (grep of rebuild/src and rebuild/tests).

## q4283 (CAFile::ConvertPathToFullPath 0x00999110): deferred (one binder ported, the other family held by NF142)
- Claim: Ego_r.pdb 0x00B00D80 + FableWin.pdb 0x02FEF590 ?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z. Self row already defines it.
- 0x00A0B710 ported: onto engine/CAFile.h (FABLE_RETAIL_CAFILE) + fable_string.h; local CCharString / CWideString models dropped; binds the
  claim plus ?GetAsCharArray@CWideString@@QBEPBGXZ / ??1CWideString@@QAE@XZ (the spellings it already bound). Own parameter
  const CCharString& -> const CWideString& (two sources: the body passes the parameter unchanged in edx to a callee whose parameter both
  PDBs type const CWideString&; the byte-sibling CSoundBankLH::Load 0x00A0B630 takes const CWideString&). Nothing references 0x00A0B710
  (no call, no vtable slot, no 4-byte match in the exe), so the own-leaf change binds no row. Catalog name kept: CShaderRenderManager::
  UpdateLightGlobals is propagated (the address sits among the CSoundBankLH members 0x00A0B630..0x00A0BAC0 and has Load's shape without
  the loaded test), but no PDB names CSoundBankLH, so no second source for a new name.
- 0x009F2DD0 (CSurface_Methods stand-in, own parameter const CCharString&): the fix moves its own leaf to the PDB
  ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z (Ego_r 0x00B46AA0 + FableWin 0x030C9340), so its 7 exe-scan
  referencers (0x0041E6D3, 0x00494900, 0x0049A0B0, 0x0055FC90, 0x00642820, 0x0066CA80, 0x00885F50) must move in the same hand-back.
  0x0066CA80 is locked by NF142: deferred, not drafted. Its HEAD object also binds 0x0099B510 as ??1CWideString@N_0099af00@@QAE@XZ (a
  namespaced model; the same port fixes it).

## q4285 (CWideString::CWideString(const unsigned short*) 0x0099B6B0): open (one of two wrong binders ported)
- 0x0041A060 already binds the claim (re-probe; WORKLIST's ?N@S@@QAEXPBD@Z witness is stale). Rows changed on 11-20 (0x005B5CF8, 0x005B62FE,
  0x0061E6D0) bind it.
- 0x00406610 GetManualSaveFileName ported off CWSraw::fromLit and its extern "C" seams. The retail order (number string first, then L"Save",
  then the fastcall operator+) is `GetManualSaveBaseFileName() + GFIntToWideString(slot)` with both inlines expanded: GFIntToWideString
  from the existing engine/GFIntToWideString.h, GetManualSaveBaseFileName (0x004065F0, its donor-order neighbour, unreferenced in the exe)
  from a new CUserProfileManager.h opt-in. Form 1 (`CWideString(L"Save") + ...`) graded DIFFER(118v120): the explicit temporary pushes
  the constructor's eax and swaps the operand slots, the fable_string.h note about implicit conversions.
  Callee spellings (RENAMES s2): every new spelling is one landed rows already bind (??0CWideString@@QAE@XZ, @@QAE@ABV0@@Z, @@QAE@PBG@Z,
  ??1, ?Sprintf@CWideString@@QAAXPBGZZ, ??H@YI?AVCWideString@@ABV0@0@Z); the stand-ins (CWSraw, @WSctor/@WSdtor, _FmtWide, AddWide,
  kFmtSave_0122de44, kSavePrefix_0122de90) leave the repo with this row, so no address's spelling count grows (the two data addresses
  become the wide string literals, as in the other GFIntToWideString users).
- One definition: the row emits pick-any COMDATs of both header inlines. Their owners were strong:
  - 0x004065F0 GetManualSaveBaseFileName: link/ctl_q4285.log (HEAD owner + new 0x00406610) LNK2005. Re-authored on the header with the
    nf86 / nf92 scaffolding (anonymous-namespace function under inline_depth(0), `// Fidelity: scaffolding, not retail`).
  - 0x00405F00 GFIntToWideString: the strong owner ALREADY clashed at HEAD with the six landed includers of engine/GFIntToWideString.h
    (link/ctl_gfint.log: HEAD 0x00405F00 + HEAD 0x00646762, LNK2005). Same scaffolding; this clears the existing clash too.
  - link/new_q4285.log: new 0x004065F0 + 0x00406610 + 0x00405F00 + HEAD 0x00646762 -> exit 0 (LNK4088 only).
- 0x009C7A60 not attempted (context): see STATUS. Classes that must move onto fable_string.h first: its local NGameText::CWideString /
  CWideStringData (+ the four same-TU noinline member bodies, each a strong copy of a real CWideString leaf under nf96a), and the
  FormatNumber_AddThousandsSeparators / FormatNumber / Prefix stand-ins.

## Locks claimed beyond rows.txt
0x00A25530, 0x00A25E70, 0x00A269C0, 0x00A26A50, 0x00A26C60, 0x00A42FC0 (free), 0x009FC170 (free), 0x004065F0 (free), 0x00405F00 (free).
None is in UNMATCHED_ROWS.tsv. 0x0066CA80 is NF142's (not touched).

## Tests
Rewritten: 0x00406610 (on the shared headers; recording fakes for the out-of-line CWideString members; same PassPattern). All other tests
of changed rows are standalone and pass unchanged (0x009FC170's embeds its old source text and still passes).

## Flags lines
None added.

## Descriptive names
- ?Touch@CResource@@QAEXXZ in the six local-model rows: the landed 0x00A262B0 spelling of FableWin.pdb's inline CResource::TouchResource
  (kept descriptive so a row-local body never emits 0x0041B9C0's leaf).
- Scaffolding helpers (anonymous namespace, internal): ReferenceGetManualSaveBaseFileName (0x004065F0), ReferenceGFIntToWideString
  (0x00405F00).
- The empty `struct CResourceMethods : CResource {}` in 0x009C8FB0 keeps the existing stand-in token only as the base of the row's local
  NGameText::CTextEntry (its shared header has the wrong base, CIVCountedPointeeBase, so a real port is a header claim).

## Checks before hand-back
- Funclets / includes: no Unwind funclet or other source includes the files of 0x00A0B710, 0x00406610, 0x004065F0 or 0x00405F00 (the only
  hit, 0x00627400, names 0x00405F00's file in a comment).
- COORDINATION_CLAIMS.tsv: no Account B line for any changed row or for CUserProfileManager.h; the newest line for q4283 / q4285 / q4942 /
  q4943 is this agent's claim.
- Other pick-any copies of the two owner leaves (they clashed with the strong owners at HEAD and link cleanly now): 0x00407E10 defines a
  local in-class GetManualSaveBaseFileName; 0x00E64FB0 a FABLE_FORCEINLINE GFIntToWideString; the six GFIntToWideString.h includers.

## Decision items for the coordinator
1. q4942 / q4943: the July candidates-header rows 0x00BEC000 / 0x00BEC470 / 0x00BEC950 keep binding the public QAE ListRemove and
   ListInsert(CResource*, void*) through rebuild/include/candidates/preload_bump_texture_compiled.h. The hand-back shrinks both addresses'
   spelling sets (4 -> 2, 5 -> 2) and creates none, but the brief's "every REFS row on the new spelling" is not met for those three. Fix
   would be a candidates-header edit (private + CResource* parameter) on multi-block rows whose relocations vland cannot check.
2. Header claim (not applied): engine/NGameText_CDataBank.h CTextEntry derives from CIVCountedPointeeBase; Ego_d / the rows say CResource
   (0x009C8FB0's local model, CGroupEntry's opt-in). Rebasing it unblocks a real port of 0x009C8FB0.
3. q4283's 0x009F2DD0 family waits for NF142's 0x0066CA80.
4. 0x00A0B710 own-parameter change (CCharString -> CWideString): the two sources are the callee's PDB signature (both PDBs) plus the
   pass-through body (and the sibling Load's shape), not a PDB name for the row itself. If refused, the row stands alone (nothing references it).
5. ?Touch@CResource@@QAEXXZ in the six local-model rows: the in-place rule's clause (2) read strictly forbids a symbol naming the respelled
   class beyond the own leaf. Precedent 0x00A262B0 (landed) emits the same symbol; no row owns it and no header defines it (pick-any).
6. 0x004065F0 and 0x00405F00 are owner rows outside my entries, changed for one definition only. As NF92 noted for q11224, retail's
   GetManualSaveBaseFileName was a .cpp body auto-inlined in its own TU (FableWin / Ego_r place it beside GetManualSaveFileName), so the header
   inline opt-in is the one-TU-per-row spelling and the owner carries `// Fidelity: scaffolding, not retail`.

## Rule slips
- One Bash call exited non-zero (a `cat` of a lock owner file that did not exist, as the last command).
- HEAD baseline compiles (work/head, t/base.py) are not counted in grades_used; draft grades are (work/grades.log).
