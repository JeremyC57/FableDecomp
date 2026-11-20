# NF158 notes (round 10, partial hand-back: context)

Counts after the resume session (from APPLIED.tsv / STATUS.tsv; supersede the first session's line): 31 entries: 6 done (q4754 q4919
q4954 q4977 q5158 q11719), 5 done-already, 13 blocked, 1 rejected (q5179), 6 open (q11775 + the five cutscene lines, scoped). Rows: 21 ported
(rows/), 20 not-a-binder, 22 blocked. recheck_final.log is one vland run over all 21 rows. (First session's original line: 2 done, 5
done-already, 3 blocked, 21 open; 5 ported, 11 not-a-binder, 5 blocked.)
(q4754 partial + 20 not started). Rows: 5 ported, 11 not-a-binder, 5 blocked; all five ported rows RELOCATION_MATCH + PASS in recheck_final.log.

## Census
- work/probe.tsv: every landed REFS row of the 26 non-cutscene entries compiled once (112 lines); `tools/summ.py` prints the per-address
  spellings. The cutscene family (q6368 q6493 q6504 q6509 q6510, ~109 rows) was not probed.
- WORKLIST undercounts: q4754 has 11 wrong binders (WORKLIST lists 6), q5208 seven, q11775 five.

## Entries
- q4020: self right. 0x00B0F990 ported onto engine/CPSCUpdateNormal.h. Its object now emits ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ
  as a pick-any COMDAT, which clashed (LNK2005) with the strong definition in 0x00B0CA90, so 0x00B0CA90 was ported the same way (its
  dtor is now the header inline body, emitted pick-any through the nf86 / nf88 scaffolding). Link proof in link/: ctl_psc.log (HEAD
  0x00B0CA90 + new 0x00B0F990: LNK2005 + LNK1169, exit 145), new_psc.log (both new: exit 0, LNK4088 only). Blocked: the two unreferenced
  25-byte twins 0x00B0F790 / 0x00B0F810 (STLport ~list and ~_List_base of list<CIntelligentPointer<CParticleEmitter> >, both calling
  clear 0x00B0F630): which is which is not provable (no callers, identical bytes), so they keep their list_T_* stand-ins.
- q4219 / q4220: the wrong binder 0x00AEEF40 is not CGuiVarTransferStruct::Add: it is NParticleEngine::CParticleEmitter::AddParticleSystem
  (Ego_r 0x00AB3FF0 + FableWin 0x02F07600; sole caller DatabaseCopy 0x00AEEFE0; +8 = ParticleSystems; stores the CParticleSystem
  DeleteData). NAME_FIXES.tsv has it and 0x00AEEFE0's PDB name. Its port (work/00aeef40.cpp) grades RELOCATION_MATCH first try, but the
  self rename needs 0x00AEEFE0 on the new spelling, and 0x00AEEFE0's port is DIFFER(526v526) after 3 forms (one register,
  work/00aeefe0_blocked.cpp; the opt-ins are kept in work/hdr_draft/, not in hdr/). After that: 0x00AF0650 (references 0x00AEEFE0) and
  the self rename of 0x00AF0710 (`?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z` -> the claim).
- q4254, q4481, q4482, q4788, q5106: done-already (probe: every referencer binds the claim; self leaves right). 0x00445800 is a false
  exe-scan hit for 0x005BE284. q4788's data symbol 0x013D2E08 waits for the PDB data-symbol dump (owner 2026-11-01).
- q4754: (first session) open; finished in the resume session (below). 0x0088F4A0 ported (also a leaf-only self rename: CGameScriptInterface_Methods::EnableDecals(char) ->
  CGameScriptInterface::EnableDecals(bool) const, both PDBs, header slot 688, no referencer). Ten rows left (APPLIED.tsv lists them).
- q4919: done (self rename of 0x00AF1860 onto engine/CCountedPointer.h; FableWin + the body; its only referencer already bound it).
- q5158: done (0x005C2E68 ported onto engine/CTCInventoryWeapons.h).
- Not started (APPLIED `open`): q4894 q4895 q4896 q4954 q4977 q5053 q5179 q5181..q5186 q5208 q6368 q6493 q6504 q6509 q6510 q11719 q11775.

## Findings for the next pass
- q4977 / q11719 / q4754 share rows 0x00543D30, 0x00543DF0, 0x00645CE9 (local CParticleEmitterManager models): port each onto
  engine/CParticleEmitterManager.h once (FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D + the NF158 _GETMANAGER opt-in).
- 0x00AF5010 / 0x00AF5380 (CParticleSystem::AllocateParticle / AllocateDecal) expand FableWin's inline
  CParticleEmitterManager::ClearLowestPriorityNonVisibleEmitterParticles() / ..Decals() (`ActiveEmitters.ClearLowest..(PEngine)`).
- q5179: three signatures in play (self `?Initialize@Object@Row00c21c20@@...`, binder 0x00C12940 `.._N555@Z`, claim `.._N33_N@Z`,
  marked PROPOSAL); settle each argument from the body and the caller before any grade.

## Descriptive names
- None added. The scaffolding helpers (`ReferenceCPSCUpdateNormalVtable`, `ReferenceCPSCUpdateNormalDestructor`) are anonymous-namespace,
  unreferenced, marked `// Fidelity: scaffolding, not retail` (nf86 / nf88 pattern).

## Funclets
- None of the changed rows has EH funclets (no /EHsc, no Unwind rows include them; checked by grep of rebuild/src/compiled/01/21).

## Flags lines
- None added.

## Additions after the advisor review
- RENAMES section 2 now has every changed pair from pdiff (HEAD v new) for all five rows: 0x00B0CA90 has none; 0x00AF1860 also moves
  0x00AE91D0 from ?Destroy_00ae91d0@@YIXPAX@Z to the DeleteData spelling 0x00AE8C30 already binds (coll.py: OK, no NEW-SPELLING).
- NAME_FIXES: 0x00AE91D0 (catalog `_global::DeleteViaVtable0_00ae91d0`, self leaf a free stand-in) is CCountedPointer<CParticleEmitter>::DeleteData.
- Twins 0x00B0F790 / 0x00B0F810 (lead, one source only): the 0x00B0F990 / 0x00B0CA90 objects emit the ~_List_base COMDAT before ~list,
  and retail has 0x00B0F790 before 0x00B0F810, so 0x00B0F790 = ~_List_base would follow if a caller-proven pair elsewhere shows the same
  order. Not applied.
- vland prints WARN STANDIN_LEAF for 0x00B0F990 (catalog "`scalar_deleting_destructor'" vs the ??_G leaf, same symbol as HEAD); not checked
  whether HEAD shows it too (non-blocking).
- Pending overlays NF149..NF157 hold none of CPSCBase.h, CPSCUpdateNormal.h, CParticleEmitterManager.h, CTCInventoryWeapons.h.
- Tests: all five ported rows keep their landed tests unchanged (self-contained host models; they do not link the row).

## Decisions needed from the coordinator (first session; answered in RESUME.md: 1 accepted as additive opt-ins, 2 stays blocked,
## 3 stays blocked on identity)
- CPSCBase.h: the two `#ifndef` opt-ins omit the existing destructor declarations (default path token-identical). Additive opt-in, or a
  `header` claim (Account B holds 0x00B0DC20 on CPSCUpdateNormal.h)?
- 0x00AEEFE0: may the resume go past three forms (one register in 526 bytes; include-order / bracket levers untried)?
- Twins 0x00B0F790 / 0x00B0F810: accept the emission-order lead as a second source, or keep them blocked?

# Resume session (NF158, 2026-11-20)

## q4754 / q4954 / q4977 / q11719 (done)
- The ten rows left were finished as in-place respells of each row's EXISTING local CParticleEmitterManager model (same class token;
  moved into namespace NParticleEngine as `class`; the stand-in static `Get()` replaced by a local declaration of the free
  `CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager()`), per the in-place rule (advisor 2026-11-20). Exceptions:
  - 0x00B09360: ported onto engine/CParticleEmitterManager.h with existing opt-ins only (BULLETTIME_PASS23, DEFAULTBLEND,
    DEFAULTALIGNMENT), defaults through the inline GetDefault* getters as the landed sibling ctors 0x00B012E0 / 0x00B1DFD0.
  - 0x0080D0B0: the local class dropped; only a forward declaration + the free accessor (the row models C3DVector locally, so the
    shared header cannot be included).
  - 0x006E11F0 (X_006e11f0 stand-in namespace): moving the stand-in-namespace class to ::NParticleEngine would be a stand-in renamed
    to the real class (not in-place), so the local class is dropped for engine/CParticleEmitterManager.h in a new declaration-only
    partial mode (INTERPOLATED_ONLY, hdr/README.md); the full header cannot be included (it would pull engine/CCountedPointer.h out of
    the row's namespace). The PDB spelling of 0x00AEB8C0 needs the global C3DVector / CRightHandedSet and the NParticleEngine emitter,
    while the row keeps its X_006e11f0 stand-ins; the call reinterprets the two argument references and the returned pointer
    (`(const ::C3DVector&)position`, `(const ::CRightHandedSet&)axes`, `(::CParticleEmitter*)`; no code, as the row's existing
    adapter casts). Coordinator: say if these reference casts are not acceptable; the alternative is a full port of the row off
    its X_006e11f0 models.
- Collateral family (no new split): moving the class respells every member called through it. 0x00AEB8C0's self leaf was the
  non-namespaced spelling 0x006E14E0 bound, so the family 0x00AEB850 + 0x00AEB8C0 (leaf-only self renames, both PDBs) + 0x006E14E0
  + 0x006E11F0 hands back whole; 0x00AEB850 also closes q4954 (binds 0x00AEB600's PDB spelling). 0x00AEB8C0's lock was claimed this
  session. 0x00AEB6D0: all five referencers now bind the PDB spelling 0x00643D8C already bound (its self row's own leaf differs: NAME_FIXES).
- CParticleEmitterList stays global in 0x00AF5010 / 0x00AF5380: the self rows 0x00AEAF20 / 0x00AEAF80 define the non-namespaced
  spelling (NAME_FIXES, two-row families left for a later pass).
- The first session's GETMANAGER opt-in is withdrawn (duplicate of the existing BULLETTIME_PASS23 block); 0x0088F4A0 now defines PASS23.
- All 13 rows (12 + 0x0088F4A0) RELOCATION_MATCH + PASS in work/b2.log; every row's defined-symbol set equals HEAD except 0x00B09360
  (adds the header pick-any GetDefault* COMDATs) and the two self renames (own leaf only). Tests unchanged (self-contained host models).

## libpk sound / CInstance entries (q4894 q4895 q4896 q5053 q5179 q5181..q5184 q5186): blocked or rejected, no grades
- CSoundEvent HAS shared headers, and two of them disagree: libpk/LHAudioEventPosition.h (unconditional `class CSoundEvent`, 12
  slots mostly UnresolvedSlotN) and libpk/CSoundEventGroup.h (under FABLE_RETAIL_CSOUNDEVENTGROUP, nine named slots). The landed
  CSoundEvent rows all model `struct CSoundEvent` locally. Every wrong binder of q4894 (0x00C219F0 0x00C21A20 0x00C21C20) and q5053
  (0x00C213B0) is a row whose own class is a stand-in for CSoundEvent, so the fix is a port onto one header plus a self rename to a
  descriptive CSoundEvent method name. Decision needed: which CSoundEvent header is canonical (and slot 10's name, a header rename).
- q4895 / q4896: claim holds (CStreamBuffer: `new(0x2C)` + ctor 0x00C262A0 in Play; the +0xA4 object is `this` of the landed
  CStreamBuffer::AddUsers / RemoveUsers 0x00C26410 / 0x00C26480). The local type CSoundChannel spans nine rows (APPLIED q4895) and
  the self rows of 0x00C26410 / 0x00C26480 still carry the CPanel stand-in for CSoundEventManager in their own leaves, so they must be
  ported first. Locks claimed for the scan were released (owner `released`): 0x00C20B50 0x00C209C0 0x00C21560 0x00C21700 0x00C20CE0
  0x00C26410 0x00C26480.
- q5179 rejected: the claimed leaf is not an emittable MSVC name (`_N33_N`: a repeated bool is a back-reference digit).
- q5181..q5184 / q5186: no header defines CInstance; the self rows and the binder 0x00C38B20 are stand-in classes. Renaming their
  tokens to CInstance is a new local model of a header-less class with no base (the 2026-11-20 rule asks for a shared-header base).
  q5186 (0x00C388A0) is the smallest: one leaf-only self rename whose only referencer 0x00C38D60 already binds the claim, and whose
  shape matches 0x00C38D60's local CInstance. Decision needed.

## q5208 (blocked, 4 binders respelled)
- 0x005C0E41 / 0x005C20D8 / 0x005C2A5C / 0x005C1390: the local counted-pointer stand-ins replaced by engine/CCountedPointer.h
  CCountedPointer<NInventory::CPerItemData>; the header's inline destructor is called out of line in these size TUs (as in the landed
  siblings 0x005C19A3 / 0x005C2AD8). 0x005C1390's object also emits the compiler-generated ??_G of the header class (pick-any, bound by
  nothing). Their own stand-in names (CSoundPairTable_005c2a5c, CLandscapeBackgroundPatch free vector_deleting_destructor) are left.
- Left: 0x005BF00B / 0x005C1946 bind ?ReleaseData@... because the shared header's inline Reset spells ReleaseData (the claim's own
  evidence says ReleaseData is never out of line in retail). Binding the destructor there needs a change to what the existing header
  member Reset calls (needs header claim). 0x005C04EB has no Leaf line (July row): left.
- No funclet includes any of the changed files (grep of rebuild/src/compiled/01/21).

## Corrections to the first session's notes
- "the NF158 _GETMANAGER opt-in" (q4754 and Findings): withdrawn in the resume session; 0x0088F4A0 defines the existing
  FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23 instead (same declaration text).
- "q4977 / q11719 / q4754 share rows ... port each onto engine/CParticleEmitterManager.h": done as in-place respells instead (the rows
  model C2DVector / CRightHandedSet locally and cannot include the header).

## Descriptive names (resume session)
- None added. No new class token in any row; the only header addition is the INTERPOLATED_ONLY partial mode (hdr/README.md).

## Funclets / Flags lines (resume session)
- None of the 16 rows changed in this session is included by an EH funclet (grep of rebuild/src/compiled/01/21 for each file name);
  the two self renames (0x00AEB850, 0x00AEB8C0) move their files, no funclet follows them. No Flags lines added.

## Decisions needed from the coordinator (resume session)
1. 0x006E11F0: the reference / pointer reinterpretations between the row's X_006e11f0 stand-ins and the global C3DVector /
   CRightHandedSet / NParticleEngine::CParticleEmitter (no code). Acceptable, or must the row be ported off its stand-ins first
   (then the 0x00AEB8C0 family waits with it)?
2. Which libpk CSoundEvent header is canonical (LHAudioEventPosition.h vs CSoundEventGroup.h), before q4894 / q5053 can port.
3. CInstance (q5181..q5184, q5186): may stand-in tokens be renamed to the header-less CInstance (no base), as 0x00C38D60 already models it?
4. q5208: 0x005BF00B / 0x005C1946 reach 0x005C04BF through engine/CCountedPointer.h's inline Reset, which spells ReleaseData; a header
   claim on what Reset calls is needed before they can bind the destructor spelling.

## Locks (resume session)
- Claimed and used: 0x00AEB8C0 (self rename). Claimed for q5208 then released (rows unchanged): 0x005BF00B 0x005C04EB; kept: 0x005C20D8
  (changed). Claimed for the q4895 scan then released: 0x00C20B50 0x00C209C0 0x00C21560 0x00C21700 0x00C20CE0 0x00C26410 0x00C26480.
- Path includes of the two self-renamed files (0x00AEB850, 0x00AEB8C0): none in rebuild/src or rebuild/tests.
