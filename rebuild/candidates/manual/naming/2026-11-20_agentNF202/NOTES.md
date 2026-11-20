# NF202 notes (round 10, remaining queue q12195..q12271)

No made-up names: every new spelling is a PDB name or an STLport header spelling (descriptive-by-library, no PDB holds STL instances:
0x0053C9BC, 0x00579D97, 0x008F9F50, 0x00485AA7).

## Ports / respells
- q12195 0x006E5950: self row moved off its local CScriptConversationManager model onto engine/CScriptConversationManager.h (new opt-in
  FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_ADDLINETOCONVERSATION, hdr/README.md). Caller 0x00890750 also dropped its local
  CScriptConversationManager model for the header; its test was rewritten onto the header (stub with the PDB signature, manager in raw
  storage so the out-of-line ctor 0x006E6150 is not linked). 0x00890750 still models CScriptThing / CGameScriptInterface / the world
  locally (vland WARN SHARED_HEADER_AVAILABLE, pre-existing, unchanged).
- q12208 / q12212 0x0053C9BC: ported onto stlport/fable_containers.h exactly as the landed 0x0042ABCA (DTOR_IN_LIST + CLEAR_OUTOFLINE,
  explicit instantiation). New pick-any COMDATs (implicit ~_List_base / ~_STLP_alloc_proxy / ~allocator and deallocate for CList*):
  no table line, no owner row. Test unchanged (standalone stand-in model, does not link the row).
- q12213 0x005CC1E8: Leaf line written in full; object unchanged; catalog name fix in RENAMES s1.
- q12220..q12223: in-place respell (Add<X>SubChunkType -> Register<X>SubChunkType) in the row-local C3DAnimFileLoader models of the four
  self rows and the callers 0x00A999B0 / 0x00AA4710 (engine/C3DAnimFileLoader.h already spells them Register<X>; the port onto it is
  separate debt). Tests unchanged (standalone).
- q12224 0x00A9CF40: ported onto engine/CCountedPointer.h (explicit instantiation, as 0x00A94DF0). New pick-any
  ?ReleaseData@?$CCountedPointer@VCChunkedFileChunk@@@@QAEXXZ and ??0CCPPointerInfo: no table line; no catalog / Leaf owner of
  ReleaseData / ~CCountedPointer<CChunkedFileChunk> (grep of auto-re-candidates names and Leaf lines).
- q12225 0x009EA3A0: GetFormattedName on the new partial mode FABLE_RETAIL_CCONSOLECOMMANDBASE_GETFORMATTEDNAME (hdr/README.md);
  `Name + OPEN_BRACKET` with FABLE_RETAIL_CCHARSTRING_CONCAT_STRING; OPEN_BRACKET stays in the anonymous namespace (retail TU-local), so
  the rewritten test declares its own anonymous OPEN_BRACKET and stubs.
- q12230..q12232 (and q12220-family siblings): the VillageTask CCountedPointer<T> families moved whole: the 44-byte body is
  ReleaseData (self rows, FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA + explicit instantiation), the ??_G rows use RELEASEDATA_DECL_ONLY and
  an anonymous-namespace `delete p` helper to emit ??_G, the Clear rows lost their CValue stand-in. ~CCountedPointer is the 5-byte jmp
  COMDAT; every copy (Torch / SecuritySweep / CarryCrate, all rows) is byte- and relocation-equal (cmpbody). The table lines
  ??1?$CCountedPointer@VCVillageTask<T>@@@@QAE@XZ -> 0x0065D574 / 0x0065D5CC / 0x0065D5F8 go stale (those addresses are ReleaseData;
  the ??1 is the jmp thunk): coordinator table action. The landed NF180 Patrol family (0x0065D507) has the same jmp-thunk shape and is
  still spelled ~CCountedPointer: same fix, not in my slice (not filed as a row change).
- q12235 0x00579D97: _Rb_tree<EHeroTrainableStatType, pair<.., SExpSpendUndo>>::_M_erase on stlport/fable_map.h
  (FABLE_RETAIL_SEXPSPENDUNDO_ONLY). All 4 new pick-any copies with owners rechecked under the 2026-11-20 rule (cmpbody bytes AND relocs
  equal): clear<long> = 0x0042A168 (owner defines it as ?Clear@List@@, compared by body), ~_List_base<long> = 0x0042AB73,
  ~SExpSpendUndo = 0x005797FA, pair ??_G = 0x005799DE.
- q12240 0x00871D30: GetZCoord on engine/NCameraModes_CStringAndCageBase.h + NCameraModes_CBase.h (two new opt-ins) and the shared
  engine/CThingGameObject.h (retail chain FABLE_RETAIL_CTHING .. CTHINGGAMEOBJECT). A first version declared a local empty
  `class CThingGameObject : public CThing { };` like its sibling 0x00871D50; selfcheck flagged it NEWLOCAL and it was replaced by the
  header include before hand-back (same grade, same pairs). 0x00871D50 still carries that local shape (pre-existing).
- q12247 0x00AC1460 / q12251 0x00ABFE50: on engine/CCountedPointer.h / C3DMeshFileMapChunk.h. 0x00ABFE50's ??_7 and ??_G copies equal
  0x00AC0C00's (bytes and relocs) and the retail 0x00AC1960 bytes; the 0x00AC1960 self row is a free-function stand-in
  (?C3DMeshFileMapChunk_CompositeScalarDeletingDestructor) defining no ??_G, filed in NAME_FIXES.tsv.
- q12252 / q12257 0x008F9F50: ~_List_base<LSocket*> on fable_containers.h + engine/LSocket.h; every pick-any copy equals 0x0122A200's
  (bytes and relocs). 0x0090AA40 (local std::list model, binds ~list) is the remaining port.
- q12256 0x013BAE1D: data references respelled to CTCAIScratchpad::BettingActive in 0x0088FB40 / 0x0088FB50 / 0x007E4E30
  (engine/CTCAIScratchpad.h + ETCInterfaceType.h; 0x0088FB40/50 keep their stand-in free-function self leaves: their own names are the
  CGameScriptInterface virtuals, outside this entry). 0x00691560 / 0x007E5230 / 0x007E50C0 / 0x007E5680: local CTCBase / CThing /
  CTCBetting models clash with the header (ports).
- q12261: dead externs in 0x008ED440 / 0x008ED2B0 respelled g_DefStringTable -> GDefStringTable (unused; objects unchanged).
- q12262 0x004115A0: self row on fable_string.h (COMPARE_BODY + TEXT_EQ_COMPARE_ZERO); 88 bytes MATCH; its pick-any Compare copy =
  0x00411570 (bytes and relocs). The TEXT_EQ_INLINE 20-byte ??8 debt in other TUs (q12265, 0x00D13580 ...) is a header decision.
- q12269 0x00A98600: port tried (work/00a98600_try1.cpp) and failed: the header's ~CCountedPointer runs on the resize temporary
  (77 v 32); retail has no destructor code. Left for whoever owns the CCountedPointer header story.
- q12271 0x00485AA7: self row on fable_containers.h (FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE needs rebuild_abi.h for FABLE_FASTCALL);
  test rewritten on the header but prints the kept PassPattern `LISTBOX_SELECT_OK` (build_candidates.ps1 line 49418). Coordinator may
  respell that PassPattern. vland WARN RELOC_NAME labels 0x00405BA0 Std_Move_Backward (forge autoname); a later label file already says
  std::__copy_trivial.

## Pick-any recheck (coordinator rule 2026-11-20: relocs must match too)
Every pick-any COMDAT my rows newly define that has a table owner was compared with tools/cmp2.py (cmpbody with per-object symbol
names): all `bytes equal True relocs equal True` (0x004115A0, 0x00579D97 x4, the six VillageTask ??1 jmp copies, 0x008F9F50 x5,
0x00ABFE50 ??_7 / ??_G vs 0x00AC0C00). Copies the HEAD objects already carried were compared with HEAD: unchanged except the intended
ones (??1 44 -> 5 bytes in the ReleaseData families; the 0x00A999B0 / 0x00AA4710 ctors' relocations follow the Register<X> respell;
0x00ABFE50's vtable now equals 0x00AC0C00's). Nothing held.

## vland warnings (expected)
- WARN STANDIN_LEAF on renamed self rows: the catalog still carries the old name until RENAMES s1 is applied.
- WARN SHARED_HEADER_AVAILABLE on in-place-respelled rows: pre-existing local models, same at HEAD.

## Needs header claim
- q12207 0x0072AB40 (CTCSpecialAbilities slot 1 persist hook) must bind the PRIVATE static ?CurrentCreatureGlobalDefNum@CTCSummonSpell@@0JA:
  engine/CTCSummonSpell.h needs `private: static long CurrentCreatureGlobalDefNum;` plus `friend class CTCSpecialAbilities;` (friend in a
  shared header). Unblocks 0x0072AB40 only.

## Blocked ports (census for the next pass)
- q12201 / q12202: unchanged from NF186 (ctor owners 0x00DEF6A0 / 0x00EE02C0 define ??0 strongly; the ??_G rows need the ctor in their TU).
- q12214 / q12216 0x00BEB4D0: retail body disassembled (APPLIED). Prerequisite splits: 0x00BDA0D0 bound as
  ?InsertNode@LandscapeLayerList@@QAEXH@Z (0x00BEAAA0), ?DeleteVertexBuffer@LandscapeMemory@Native@@QAEXPAVCVertexBufferWin32@@@Z
  (0x00BE6090), ?FreeBlock@CEngineLandscapeMemoryManager@@QAEXPAX@Z (0x00BEAA00), catalog DeleteVertexBufferBlockWhileBusy; both PDBs
  have public ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z and private
  ?DeleteVertexBufferBlockWhileBusy@...@@AAEXPAVCVertexBufferWin32@@@Z (which one is 0x00BDA0D0 is not settled here). Global 0x01436EA8 is
  bound under 5 spellings (g_pLandscapeRenderer / g_pEngine_01436ea8 (two types) / g_pEngineLandscape / g_pEngineGlobals_01436ea8).
- q12226..q12229: the CCountedPointer component through the multi-type binder 0x0042F50B (0x00403750 has no thunk and is a real dtor;
  0x0042918C locked by NF201; the RELEASEDATA opt-ins are per TU, not per T).
- q12244 identity / authoring; q12245 ??_G one definition (ctor 0x00957170); q12255 binder 0x0089B780 whole-row port; q12257 0x0090AA40.
- q12259 0x00EE2A80 and q12265: the last pGlobalScriptDef binder, gated on the q12262 ??8 header decision.
- q12271 binders 0x0047DBB8 / 0x0059EC48 / 0x00587A41 / 0x0047E07F (CComponentVector / CRetailVector / CPODVector models).

## Deferred
- q12256 0x0069E980: lock owner NF200. q12261 0x005DCF70: lock owner NF203.
- q12219 0x009F03B0: lock owner NF199; binder 0x00CB5B90 lock owner NF201 (its q12191 is the same address). Rule slip: I drafted and
  graded the self row before reading its lock (not in my rows.txt). Draft kept in work/009f03b0_draft.cpp (RELOCATION_MATCH PASS), not
  in rows/. Coordinator: whichever agent owns it, the self rename is count-neutral (no landed row binds RemoveSection), but per the
  older brief rule it waits for 0x00CB5B90; please decide.

## Locks
- Claimed then released unused: 00def6a0, 00ee02c0, 00a9ad00 (owner files now read `released`).

## Coordinator decisions
- Self renames that leave stand-in binders behind (brief 2026-11-20 covers only "every referencer already binds the new spelling"):
  0x004115A0 (about 20 rows still bind ??8CBasicStringChar, plus X_004fe630 / N006d41f0), 0x00485AA7 (4 binders on CComponentVector /
  CRetailVector / CPODVector), 0x008F9F50 (0x0090AA40 binds ~list). Each adds no spelling (the new one was already bound) and none
  defines the old self leaf any more. Kept in rows/; move them to work/ if the older "self rename waits for its binders" rule applies.
  0x009F03B0 (deferred, NF199) is the same question.
- Table actions: drop stale reloc_bindings lines ?g_PlayAnimationDefaultFlag@@3_NA / ?g_bScriptAnimationFlag@@3_NA (q12260) and the
  ??1?$CCountedPointer@VCVillageTask<T>@@@@ -> ReleaseData-address lines; add the missing ?pGlobalScriptDef@@3PAVCScriptDef@@A line
  (0x00EE2A80) so the table is a full witness.
- q12262 header decision for the TEXT_EQ_INLINE 20-byte ??8 copies; q12207 friend in engine/CTCSummonSpell.h.
- build_candidates.ps1 PassPattern of 0x00485AA7 (kept LISTBOX_SELECT_OK).

## Rule slips
- 0x009F03B0: edited and graded before reading its lock (NF199's); draft moved to work/.
- Two Bash calls exited non-zero: the first q12256 precheck loop (CDefPointer.h #error, exit 2) and an xdis.py call with a hex length
  (exit 1).
- 0x00871D30 briefly carried a new local CThingGameObject model (selfcheck NEWLOCAL); replaced by the header before hand-back.
