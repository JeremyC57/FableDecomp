# NF188 notes (2026-11-20, round 10: engine / render / resource / data-stream names)

Handed back early for context, at an entry boundary (after q6280). Counts from APPLIED.tsv / STATUS.tsv:
- 64 entries: 6 done, 3 done-already, 10 blocked (5 of them partial with rows moved), 45 open (not started; each APPLIED line carries the
  reloc_bindings census at hand-back, table only and unprobed). No REJECTED claims.
- 47 STATUS lines: 27 RELOCATION_MATCH (the 27 changed rows in rows/), 18 not-a-binder, 2 blocked (0x00C1ABB0 + its funclet 0x01212379,
  pulled back to work/pulled/). recheck_final.log: one vland --recheck-landed run over the 27.
- done-already probes: q6246 / q6248 binders 0x00A91640, 0x00AC4150, 0x00ACDB80 and q5202 binder 0x006C9490 compiled at HEAD
  (RELOCATION_MATCH): each binds exactly the claim (q5065: all 16 binders probed earlier).

## Per entry
- q3907: 0x00BF6930 moved off the row-local Native:: Tree / Patch / Split / LandscapeMap / EngineMap stand-ins onto the shared headers;
  self leaf now the PDB spelling 0x00BDC980 already binds (narrows that split). Test unchanged (self-contained host copy).
- q4251: 17 more binders of ~CDataStream 0x00993B10 moved from row-local CMemoryDataInputStream / CDataInputStream / CBaseClassNonCopyable
  models onto engine/CMemoryDataInputStream.h (mds.py / mds2.py transformers in tools/). Side respells, all onto the PDB / majority
  spelling: 0x006F9F90 OpenMemorySource PAX->PBX (0x00994570), 0x00ADFDC0 ReadToStream->CopyTo (0x009964E0, PDB CopyTo, already bound by
  0x00AE3C30), 0x00BDE860 SetPositionAssumeWithinSourceChunk QAE->IAE (0x00993C10, PDB). 4 left (July candidates-header rows).
- q4510: name proven (Transfer<CDefIndex>); 0x005CC300 self rename as an explicit specialization on engine/CPersistContext.h; binder
  0x005C3A95 respelled in place (its local CTCInventoryWeapons model's two def-index members long -> CDefIndex, the Ego_r type).
- q12165: 0x00C22840 self rename (element CSoundEvent, forward-declared only). Class key V follows the shared libpk headers (CSoundEventGroup.h
  `class CSoundEvent`); the Get<CSoundEvent> row 0x00C13620 binds the U key, and landed CSoundEvent spellings are mixed (6 U / 2 V in
  reloc_bindings): libpk has no PDB, the key is unproven (decision 5 below).
- q4960: 0x00C2C750 respelled: its stand-in CTarget_00c2c750 is now a header-less local `class PKCDeferredBank` (declaration of DeleteData
  only) in the shape of the self row 0x00C14A50; `this` is passed as reinterpret_cast<CData_00c14a50*> (both classes are stand-ins for the
  one libpk data object). New local model of a header-less class: no header anywhere under rebuild/include, sibling 0x00C14A50 has the same
  shape, no base at all (rule condition 3 is vacuous). Coordinator: flag if that does not satisfy the narrow rule.
- q5108 (blocked, finding): a port of 0x00C1ABB0 onto libpk/PKCDefaultFileManager.h grades RELOCATION_MATCH PASS (draft row, funclet
  0x01212379 and the header opt-in in work/pulled/), but it is NOT handed back: the opt-in would declare the factory as returning
  `CSBAgents*`, and the 12-byte object it builds (ctor 0x00C32FC0, vtable 0x012B4F4C next to CDefaultAudioFile::Close 0x00C32FA0; the
  0x00C32FC0 self row is the stand-in CVtable_012b4f4c) is a libpk audio-file object, not the game class CSBAgents (engine/CSBAgents.h,
  Ego_r 0x10 bytes). Writing that token into a shared header would bake a row-local stand-in there. Needs the class named first.
- q5202 / q5204: in-place respell of the row-local CPersistTraits model (value by reference), the Ego_r.pdb AAV spelling.
- q6204: self 0x00BEB490 renamed to the PDB ctor and ported onto engine/CWaterPatchMesh.h; binder 0x00BF5A30 ported, renamed from
  CWaterPatchMesh_Methods::CreateFromStream to CWaterPatchMesh::CreateFromStream (descriptive, no PDB name; no landed referencer) with a
  new header opt-in. 0x00BF3B60 left (q.py DIFFER 825v1480 at HEAD). Spelling count at 0x00BEB490: 2 -> 1 (old self leaf + the
  CWaterPatchMesh_Methods stand-in ctor -> the PDB ctor); 0x00BF3B60 binds no ctor spelling (stand-in free function CreateWaterPatchMesh in
  its candidates header), so no split.
- q6280: 0x00EE9590 self rename onto engine/CExpression_FishScript.h (both PDBs + vtable slot 4).

## Descriptive names (no PDB)
- PKCFrallocStatic<CSoundEvent>::Return (0x00C22840), CWaterPatchMesh::CreateFromStream (0x00BF5A30): kept from the catalog wording; the class tokens are evidenced (libpk trace strings / header notes / callee bodies).
- PKCDeferredBank / CData_00c14a50 (0x00C2C750 binder) are the self row's existing descriptive / stand-in tokens, not new names.

## Flags lines
- 0x00BDE860: new `// Flags: /d2inlT300` (the shared ReadConstData is plain inline; without the flag it stays out of line, DIFFER 688v752).
  mkpay.py ignores Flags lines: tools/mkpay2.py reads them (use it for this row's recheck).

## Pick-any / one-definition checks
- 0x00BF6930: new COMDATs GetPatch / GetSplitBranch / PeekWorldX/Y (TreeNode) and CEngineMap PeekWorldX/Y: header inlines; 0x00BF6130 /
  0x00BF6160 are not landed rows; the GetPatch / GetSplitBranch bodies are the HEAD local PeekPatch / PeekSplit shapes (compared against
  the HEAD object only, not against retail; neither address is a landed row, so no owner body to match).
- q4251 rows: new ??1CDataInputStream@@UAE@XZ / ??1CMemoryDataInputStream@@UAE@XZ implicit-destructor COMDATs; their table lines at 0x00993B10
  are stand-in witnesses (the address is ~CDataStream; nf181 rule), and 185 landed rows already emit the same copies.
- 0x005CC300 / 0x005C3A95: ??0CDefIndex@@QAE@J@Z (CDefIndex.h inline, no owner). The 2-arg Transfer<CDefIndex> is defined strongly only by
  0x005CC300 (no other landed definer; 0x0074DE50 defines the vector overload).
- 0x00BEB490: ??_GCWaterPatchMesh@@UAEPAXI@Z is identical to the HEAD copy (cmpbody: bytes and relocs equal). Note: the landed row at its
  retail address 0x00BEB4D0 is a byte-twin stand-in (?ScalarDeletingDestructor@CEngineScreenEffectOutlineGlow@@QAEPAV1@I@Z) whose body
  differs from this compiler copy (retail inlines ~CWaterPatchMesh): pre-existing, queue it. New CMovableResource.h inline COMDATs.
- 0x00BF5A30: new ??0C2DCoordI@@QAE@XZ (header inline).
- 0x005CC300 one-definition: of the 26 landed rows that define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (whose inline 2-arg Transfer<T>
  would emit a pick-any Transfer<CDefIndex> copy), none mentions CDefIndex: no LNK2005 against the new strong specialization.
- No GameCODA SDK hit: CSoundEvent, PKCFralloc, PKCDefaultFileManager, PKCDeferredBank, CDefaultAudioFile and CSBAgents appear in no
  .lib / .h / .pdb under $S/sdk/gamecoda/tree (CODAApiDbg.lib included), so the libpk names stay descriptive / library-trace names.

## Shared-header warnings (recheck_final.log)
- Every WARN SHARED_HEADER_AVAILABLE in recheck_final.log is a local model the HEAD row already has (class-definition diff HEAD vs new over
  all 27 rows; e.g. 0x00EE9590 keeps its HEAD local CPersistContext declaration, 0x00521240 its CASuspendableProcess / CPersistContext /
  CLevelLoader / CThingManager models). Strict mode would reject them; they are pre-existing, not added here. The one new local class is
  PKCDeferredBank in 0x00C2C750 (header-less, see q4960). WARN RELOC_NAME lines are pre-existing label disagreements of untouched callees.

## Tests changed
- rows/00bdc180_test.cpp (stubs for the shared stream classes), rows/005cc300_test.cpp (rewritten on the shared headers, links the row),
  rows/00c22840_test.cpp (element token respelled). Other tests unchanged (self-contained).

## Funclets / moved files
- No funclet handed back (0x01212379 stays with its pulled-back parent). Self renames that may move parent files: 0x00BF6930, 0x005CC300,
  0x00C22840, 0x00BEB490, 0x00BF5A30, 0x00EE9590.

## Decisions for the coordinator
1. q5172: approve a descriptive CSoundEvent*-returning name for 0x00C21DF0 (self leaf ?IsBusy@CSoundEventSet@@QAEJXZ, it really pops a free
   event) so 0x00C11F00 can become AcquireEvent honestly.
2. q12165: NF182's held 0x00C239B0 draft is stand-in -> stand-in (count-neutral); the true fix is a polymorphic opt-in of
   libpk/PKCFrallocStatic.h (vptr: Return slot 0 / Take slot 1 in place of m_0) and 0x00C239B0 as the pool constructor.
3. q4892 / q4893: need CSoundEvent member classes (+0x38 built by 0x00C34320, +0x78 vtable 0x012B37DC) named before a header port.
4. q5057: a second source for the CSoundEventManager ctor's three count parameters (MMM -> JJJ).
5. q12165 class key: 0x00C22840 is respelled with the V key (shared libpk headers declare `class CSoundEvent`), but the landed Get row
   0x00C13620 binds the U key (?Get@?$PKCFrallocStatic@UCSoundEvent@@@@QAEPAUCSoundEvent@@K@Z) and landed spellings are 6 U / 2 V. libpk
   has no PDB: pick one key for CSoundEvent and respell the other side.
6. Quarantine dependency: the q3907 TreeNode opt-in (FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_GETEDGESTRIP) includes the quarantined
   _quarantine/CEngineLandscapeMap.h for PeekWorldX/Y. If that header is not acceptable in a shared opt-in, 0x00BF6930 needs it promoted first.
7. q5108: name the 12-byte object of ctor 0x00C32FC0 (likely the libpk default audio file; vtable 0x012B4F4C) so PKCDefaultFileManager can
   declare the factory 0x00C1ABB0 in its shared header (draft ready in work/pulled/).
8. 0x00BEB4D0 (NAME_FIXES): the landed row there is a byte-twin stand-in (CEngineScreenEffectOutlineGlow scalar deleting dtor); the
   retail function is ??_GCWaterPatchMesh@@UAEPAXI@Z (inlines ~CWaterPatchMesh). Queue a port.
