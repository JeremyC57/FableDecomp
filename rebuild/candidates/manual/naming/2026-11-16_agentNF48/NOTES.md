# NF48 notes (round 7): STLport vector members reached through local stand-in containers

Resumed after a container restart (the first run had 48 of 87 worklist rows with a status). Earlier statuses and grade logs were
trusted; their grades count against the budget (STATUS.tsv `grades_used` comes from work/grades.log; the three rows blocked
without a change count the one probe compile of their repo copy from the start of the round).

## Counts (from STATUS.tsv / APPLIED.tsv / recheck_final.log)
- Worklist rows (87): 83 ported, 2 not-a-binder, 2 blocked, 0 deferred (no NF47 lock was hit).
- Extra rows ported as followers of self respellings (not in the worklist, locked as NF48): 0x00891530, 0x00747B00, 0x00E81D10.
- Entries (16): 13 done, 3 blocked (e1 and e28: 0x009CEAE0; e67: 0x007E7C90; the row left and the model to port first are in
  APPLIED.tsv).
- recheck_final.log: one vland run over all 86 changed rows (rows/*.cpp, four with a restated _test.cpp): 86 RELOCATION_MATCH +
  PASS. Grades: 102 in all, at most 3 on one row.

## Naming: descriptive-by-library
Retail is STLport 4.6.2; Ego_r.pdb and FableWin.pdb are Dinkumware builds. Every `std::vector<T>` member spelling here is the
STLport one the shared stlport/fable_containers.h produces (allocator template argument, `const T&` resize, `size_type` = unsigned
int, protected `_M_insert_overflow`). Element types come from the Ego_r.pdb member types of the callers (struct_layouts_egor.tsv)
or from the landed callee rows; no template argument was invented. Where a PDB name exists for the member (clear<CScriptThing>,
clear<CCharString>), it was cited.

## Self respellings (RENAMES.tsv section 1, 16 rows)
- New names this run: 0x008AD5C0 vector<CScriptThing>::clear, 0x00485FE8 vector<CCharString>::clear, 0x0043332C
  vector<CCharString>::resize(size_type, const CCharString&) (all three emitted by explicit instantiations on the shared header), and
  0x009D3FB0 CArray<CCharString>::DeallocateMemory (was labelled ~CArray<CCharString>: Ego_r.pdb and FableWin.pdb both name
  ?DeallocateMemory@?$CArray@VCCharString@@@@QAEXXZ, and retail calls 0x009D3FB0 only from CBankFile::CloseEntries 0x009CD850, which
  already binds that spelling, and AllocateBuffers 0x009CEAE0; never from a destructor). Its body is defined in the row on
  engine/CArray.h's both-PDB declaration set (`if (capacity() > 0) { clear(); CArray<T> empty; swap(empty); }`, byte-derived).
- Leaf-only respellings (catalog name already right): 0x005F9DF8, 0x004B2B70 (now const, Ego_r), 0x00747890 (private const, Ego_r),
  0x008A8F20, 0x00E54990 (namespace X_00e54990 gone), 0x008AA270 / 0x008AA4D0 / 0x00891530 (free __fastcall stand-ins -> the
  CGameScriptInterface virtuals of engine/CGameScriptInterface.h), 0x008A8440 (the CGameScriptInterfaceMethods adapter -> the
  CGameScriptInterface virtual).
- From the first run: 0x00443F90, 0x004E4D3B, 0x00AF72D0.
- Name checks: none of the new leaves is a Leaf of another landed row, and rebuild/oracles/reloc_bindings.tsv binds each new
  spelling only at its own address.
- File moves for the coordinator (the catalog name changes): 0x0043332C (was CArraystdpairlong_pushback_0043332c.cpp),
  0x00485FE8 (unattributed_ApplyPair00485fe8_00485fe8.cpp), 0x008AD5C0 (unattributed_CallWithPair008ac9d0_008ad5c0.cpp),
  0x009D3FB0 (CArrayCCharString_CArray_009d3fb0.cpp).
  No funclet or test includes any changed row by path (grep of rebuild/src/compiled and rebuild/tests for every file name).

## Callers of the respelled rows (callers.py on every self-changed address, plus reloc_bindings.tsv for every section-1 address)
- **0x00443F90 (first run's respelling) is partial:** eight landed rows outside this slice still bind their own stand-in spellings
  for it (wrong before and after; the first run did not check them): 0x0064E1E4, 0x0064ED06, 0x0064F6DC, 0x007E57C0, 0x00AFE9B0,
  0x0043B050, 0x0072DC70, 0x00BC65D0 (CRetailHandleVector / HandleArray / Handles / CRetailVector / CScreenEntryVector / CHandleVector
  ::Resize-style stand-ins). They are a queue entry of their own (resize(size_type) 0x00443F90; FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
  on engine/CEnginePrimitiveHandle.h, as 0x00602E30 / 0x0064F659 did). 0x004E4D3B, 0x00AF72D0, 0x00891530, 0x009D3FB0 (bar the
  blocked 0x009CEAE0) and 0x008A8440 have no stale binder.
- 0x0043332C: 0x00E54990 and 0x00E81D10 now bind the new spelling. **0x00433273 GFSerialiseVectorBinaryIn<CCharString> still binds
  its own stand-in `?resize@?$vector@VCCharString@@@std@@QAEXIABVCCharString@@@Z` (it was already wrong; it is not made worse).**
  Porting it changes its own leaf (Ego_r adds the allocator: ??$GFSerialiseVectorBinaryIn@VCCharString@@@@YIXAAVCMemoryDataInputStream@@
  AAV?$vector@VCCharString@@V?$allocator@...@Z), which pulls in its callers 0x004331F9 / 0x0049B8D0
  (CPersistContext::TransferVector<CCharString>), and needs a forceinline one-argument resize opt-in (the row's comment: 153 bytes
  without it). Left for the coordinator; the lock was released unchanged.
- 0x00485FE8 / 0x008AD5C0: the only landed caller, RunCutsceneMacro 0x00CBFB7D, already binds both new spellings.
- 0x005F9DF8: 0x005F6E40 and 0x005EEDE2 bind the new spelling.
- 0x004B2B70: 0x00891530 ported (follower).
- 0x00747890: 0x00747B00 ported (follower: it called a made-up GetAttackAvailability(bool)); the second call 0x00747CD3 is in no
  landed row.

## Header overlay (hdr/README.md)
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE, FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE.
- engine/CTCAIScratchpad.h: FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS.
- Repo opt-ins used: FABLE_RETAIL_STLP_VECTOR_ASSIGN, _ERASE_RANGE_OUTOFLINE, _RESIZE_OUTOFLINE, _RESIZE_ONE_DECL, _PUSHBACK_OUTOFLINE,
  FABLE_RETAIL_STLP_VECTOR (reserve), FABLE_RETAIL_STLP_COPY_NOINLINE, FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL,
  FABLE_RETAIL_STLP_FILL_CHAR, FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE (first run), FABLE_RETAIL_CARRAY.

## Inline controls (owner decision 2026-11-08, EXTRA_RULES inline-budget section)
- 0x00EDD060 and 0x00D18B20 replace their former lever (a hand-written `__forceinline DestroyInline()` copy of the STLport body plus
  a declaration-only `Destroy()` on a local std::vector model) with the shared header's destructor under
  FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE and a scoped `#pragma inline_depth(0)` region around the merged end, the lever the landed
  twin 0x00EE6A40 already uses. They are new inline_depth regions in these two rows; line 3 of each says `// Inline-budget: ...`
  with the evidence (plain inline ~vector: the COMDAT is called at every exit, 0x00EE6A40 graded DIFFER(246v441) that way).
- 0x00EE6A40 keeps its pragma and Revisit marker; its forceinline destructor is now the header opt-in. 0x00CB5B90 marks the
  forceinline destructor opt-in the same way.

## Blocked rows (blocked/port)
- 0x009CEAE0 (e1, e28), 2 real grades (work/attempts/009ceae0_a1.cpp SRC_FAIL, _a2.cpp DIFFER 373v375; row left unchanged): ported
  onto CBankFile.h's typed members (FABLE_RETAIL_CBANKFILE_FILENAMEINDICES / _ADDENTRYMULTIPLEFILES), engine/CArray.h DeallocateMemory
  (out of line for CCharString / CRuntimeData through explicit-specialisation declarations, as the landed 0x009CD850) and STLport
  resize, every vector binding came out right, but 2 bytes differ: the row's stand-in CRuntimeData has a member-wise zeroing
  constructor for resize(n, CRuntimeData()), engine/CBankFile.h's CBankFile::CRuntimeData is a POD without one. Prove and add that
  constructor to the header first.
- 0x007E7C90 (e67, first run): see STATUS.tsv (needs 0x005ED940 identified).
- 0x008A8440 (e163, formerly listed blocked) was ported in one grade: GetScriptThing returns the header CScriptThing, so the local
  projection model and the adapter went; push_back<CScriptThing> 0x008ADF90 and ~CScriptThing 0x004AA840 now bind their PDB spellings.

## Bonus respellings (RENAMES.tsv section 2)
Changing the container type also corrected these bindings: AreAllThingsInVectorDead 0x00CBED00 (0x00EE6A40),
KillAllThingsInVector 0x00CBED82 (0x00ED2A50), _M_insert_overflow 0x0049C370 / 0x00433530 (0x00747890, 0x008A8F20, 0x004B2B70,
0x00A62120), _M_fill_insert 0x004333B3 (0x0049B860), pGlobalScriptDef 0x0143E90C and vector<bool>/<CCharString> resize
(0x00E54990), vector<bool>::resize 0x00467686 (0x00D347B0), the three CTCAIScratchpad gossip maps and their operator[] 0x007AEBE0
(0x008AA010 / 0x008AA270 / 0x008AA4D0), push_back<CScriptThing> 0x008ADF90 / ~CScriptThing 0x004AA840 /
GetScriptThing 0x004C7E50 (0x008A8440).

## For the coordinator (name issues seen, not changed)
- 0x0143E90C has four spellings in reloc_bindings.tsv (`CDefPointer<const CScriptDef>` per engine/fable_script_def_global.h,
  `CScriptDef*`, an NGlobalScriptDefs one and g_pScriptDef). 0x00E54990 uses the header's; 0x00D347B0 / 0x00E81D10 keep `CScriptDef*`.
- 0x013BAE44 is also bound as `g_gossipVillagesMap` by some landed row; Ego_r order and 0x008A9F50's notes make it
  CTCAIScratchpad::Gossip (Villages is 0x013BAE50).
- 0x00747B00 IsAvailableForAttack is const in Ego_r.pdb (?IsAvailableForAttack@CTCStealth@@QBE_NXZ); the row keeps QAE.
- 0x008ACA90's comment lists 0x008AD5C0 among its callers; 0x008AD5C0 is vector<CScriptThing>::clear (comment only).
- Local models left in ported rows (pre-existing, outside these entries): CTestQuestVector (0x004A08D0, its erase is inline in
  retail while the TU's other erase is out of line), CVector<long> / CVector<SAbilityEntry> (0x005EEDE2), CRetailVector<SDangerThing>
  (0x00747890), local CScriptThing / CGameScriptInterface / CCharString classes in several rows, CTBaseSingletonNoDefaultAlloc in
  0x00891530.

## Rule slips
- Three Bash calls exited non-zero: the first run's inline lookup (KeyError 'size', read-only) and, this run, a port script's own
  leftover-name assert (port_ee6a40.py) before it wrote anything, and a trailing `ls` of a lock directory that did not exist (exit 2).
  Nothing was graded or changed by any of them.
- 0x008A8F20 spent one grade on the stale leaf (DIFFER on the wrong function), 0x00CB5B90 one SRC_FAIL (duplicate placement new),
  0x008AA010 two SRC_FAILs (header prerequisites). All within budget.

## recheck_final.log warnings
- STANDIN_LEAF on 0x0043332C, 0x004E4D3B, 0x008AD5C0, 0x009D3FB0: the catalog names still carry the old names; RENAMES.tsv section 1 gives the
  new module/name. STANDIN_LEAF on 0x00D2D900 / 0x00D61C00 / 0x00DC2540: their own made-up deleting-destructor leaves are
  pre-existing and outside these entries (only the 0x008AC970 callee was respelled).
- SHARED_HEADER_AVAILABLE / RELOC_NAME / RELOC_ALIAS warnings are about pre-existing local models and other callees of these rows.

## Disclosures
- RENAMES.tsv section 2 is complete for this run's rows (worklist targets plus every incidental binding found by probing the
  original sources); for the first run's 45 rows it lists the worklist targets only. Their incidental respellings (for example
  0x00442350 _M_fill_insert and the std::list<long> members in 0x0064F659, the 0x00462B4E / 0x00462CB0 calls in 0x00462B15, the
  extra vector<T> operator= targets in 0x00478132) are in the row sources but were not enumerated by a full relocation diff.
- 0x00D2D900 still includes engine/CFrontEndScreen.h under a renaming macro (`#define CFrontEndScreen CFrontEndScreenPdb`) and
  models NUISystem::CFrontEndScreen locally; pre-existing, untouched here, but the row is in this hand-back.
- Several tests restate the old stand-in shape and pass because they are self-contained (0x00E54990, 0x00747B00, 0x008AA010 /
  0x008AA270 / 0x008AA4D0, 0x008A8440, 0x00891530, 0x00E81D10). Restated: 0x008AD5C0, 0x00485FE8, 0x009D3FB0 (+0x004E4D3B first run).
- RELOC_NAME warnings read: the vector symbols agree with their labels' element types (0x0047857D 'Vector<CAbilityTrackDef>::Assign',
  0x00CD3CCB propagated, 0x0043332C the catalog name this hand-back respells); the rest are pre-existing callees outside these entries
  (AutoTinyCompositeDestructor_00cbd510 = CScriptBase::~CScriptBase in 0x00D61C00 / 0x00DC2540, ~CFrontEndScreen in 0x00D2D900, the
  0x008A8440 g_HeroPerceptionManager / GetPerceivingThings stand-ins, etc.). None is on a gossip-map or pGlobalScriptDef symbol.
