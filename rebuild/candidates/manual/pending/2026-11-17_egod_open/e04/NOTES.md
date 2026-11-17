# e04 egod lane notes

## 0x00C52C40 CCpuInfo::GetCyrixModelName (catalog label CEngineInternalPrimitiveParticleGroup::GetRenderLayerMask is propagated) -- OPEN
- Not an Ego_d row: the real function is the libpk CPU namer (strings MediaGX / 6x86 / GXm / 6x86MX / Cyrix III; caller DetectCpu
  0x00C53164). Ego_d's GetRenderLayerMask is retail 0x00BBBDF0 (egod_tu). Ego_d has no CCpuInfo record. Name stays descriptive
  (library code, light naming) as the landed siblings GetIntelModelName 0x00C52AD0 / GetCentaurModelName 0x00C52C90.
- Residue: family-5 arm `mov eax,[ecx+0Ch] ; cmp eax,2 ; ... cmp eax,4` (0x00C52C66..74), ours ecx. Family-6 arm uses ecx in both.
- Tried (19 grades, all inert or worse): case order 4/6/5; if/else-if per arm; explicit `return "<unknown>"` per arm; a name
  local with returns or assignments (72..74 bytes); `!Model`; default label; unsigned Family / Model, signed Stepping; Family
  if-chain (78); nested switch on Model (sub/je); /GS, /Os, /Oy-; a whole-TU prefix (GetIntelModelName + GetAMDModelName
  bodies before the row, full DetectCpu struct with long / unsigned Model).
- The twin AMD namer 0x00C52BB0 (pending/2026-11-08_linux_hand859) has the same tie and found it moves only with the arm's first
  compare constant (4 -> eax): a c2 record/hash tie. Next idea: c2prio trace of the Model temporaries' priorities / keys.

## 0x0091C0D0 CAIStateGroup_NymphFloatAround::IsRunnable -- RELOCATION_MATCH (1 grade), test PASS
- Matched at the first grade once ported onto the shared headers (CThing HasTC / AddTC, CAIStateGroupBase MEMBERS + GETCREATURE,
  CAIBrain GETCREATURE): `if (!GetCreature().HasTC(TCI_NYMPH)) GetCreature().AddTC("CTCNymph", false, 0); return GetCreature().HasTC(TCI_NYMPH);`
  Ego_d lists no locals (one expression per statement). No flags. Class declared locally over CAIStateGroupBase.h (no shared header).

## 0x00871FD0 NCameraModes::CStringAndCageBase::UpdateXYVars -- RELOCATION_MATCH (8 grades), test PASS
- The residue of five earlier passes (fld [esi+0ACh] v the first result's `mov eax,[esp+4]` swapped at 0x00871FF5) was the shared
  inline helper's text: `return ((current_val*weight+new_val)/(weight+1.0f));` (outer parentheses = PAREN tuple, c2 scheduling).
  Found by fwsize flagging GetRollingAverage<float> as DIFF (ours 37b v FableWin / Ego_d 45b) and then trying helper texts.
  New opt-in FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE_PAREN (hdr/engine/NCameraModes_CTools.h): a top-level block after the class's
  static assert (nothing moved), used with the existing ROLLING_AVERAGE + ROLLING_OUTOFLINE (inline out-of-class body); every other user
  regraded RELOCATION_MATCH with the parenthesised text (0x00697CC0, 0x0084EAF0, 0x008FCC60).
  Row text unchanged from the draft (Ego_d's function-scope float new_cage_radius). No flags.
- fwsize note: ours /Od 37b v FableWin 45b for GetRollingAverage<float> and 160b v 171b for the row (Ego_d debug length 171 too);
  no text tried reproduces FableWin's /Od size (named result 42b, double 1.0 42b): FableWin is a later revision. The release match
  is the judge.

## 0x0057AEB4 CTCHeroStats::GetFavouriteWeapon -- RELOCATION_MATCH (4 grades incl. 2 compile fixes), test PASS
- Ported the local-model draft onto shared headers (CTCHeroStats SERIALISE + PERSONALITY branch, CTCBase, CThing PeekTC, CTCInventoryWeapons,
  CTCInventoryBase PERITEMDATA, CTCInventoryItem KINDS, CTCWeapon DAMAGE, CWeaponDef, CThingBaseDef, CGameDefinitionManager, text bank)
  with Ego_d's locals and scopes. Two facts closed it: (1) the existing shared opt-ins FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
  + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (visible noinline bodies: the def handles stay in edi/ebx, the old "def register / frame
  0x2c v 0x28" residue); (2) `for (long item = 0; item < items.size(); ++item)` (Ego_d long; unsigned compare = retail's sar/je + jb).
- Member note: retail reads per-item +0x0C = NumberOfKills in the shared header (Ego_r/Ego_d field order NumberOfKills, TimeWielded),
  though Ego_d names the local time_wielded_weapon. Coordinator: the source likely read NumberOfKills (a source-level oddity), or the
  header's CPerItemData is off by a word; the bytes decide +0x0C. No flags (pragmas optimize("y", off) / ("s", on) as the TU).
- text_bank: Ego_d function scope; declared after the if block (retail loads PMainGameComponent only at the end).
- Visible bodies graded at their own addresses from this file: GetSubDef<CWeaponDef> 0x0042B1A6 RELOCATION_MATCH; the shared
  GETDEF_GLOBAL_BODY text as GetDef<CThingBaseDef> 0x0042AF3C is DIFFER(101v98) (retail re-reads the def handle and drops the final
  null test; the landed 0x0042AF3C is a local-model text). Coordinator: the row's match needs only the body's visibility; the shared
  GetDef body text itself is not retail's for this instance (existing opt-in, used by many landed rows; not changed here).
- NumberOfKills settled: 0x005BE8D3 (GetPerItemDataForItem) tail-jumps to CCategory::GetPerItemDataForItem 0x005BE8B1, which returns the
  item's PPerItemData object (+8) -> the row's +0x0C read is NumberOfKills in the shared layout (see NAME_FIXES.tsv).

## 0x004B2890 CQuestManager::PostBoastTransfer -- RELOCATION_MATCH (27 grades over 2 passes), test PASS
- Pass 1 parked 4 rename hunks with a `last` local Ego_d does not list (without it end() was reloaded after __find). Pass 2: GOTCHAS wf10 --
  make BOTH __find (existing FABLE_RETAIL_STLP_FIND_NOINLINE_BODY) and its comparison callee CBasicString<char>::Compare (existing
  FABLE_RETAIL_CBASICSTRING_COMPARE_BODY) visible: VC7.1 then knows the find writes no memory, keeps end() in edi with no `last`, and
  the argument / key registers become retail's. Ego_d structure verbatim (it at function scope, `continue` before boasts, nested blocks).
- Same-TU bodies graded at their own addresses from this file: GetAcceptedBoastsForScript 0x004B1960 RELOCATION_MATCH (after the row,
  Ego_d order), __find 0x004B8E40 RELOCATION_MATCH, Compare 0x00411570 MATCH. No flags.

## 0x00BA9830 CEnginePrimitiveManagerMeshBase::GetReflectionTexture -- RELOCATION_MATCH, test PASS, strict headers clean
- First matched (8 grades) on the landed sibling GetDiffuseTexture's local models; that version failed the shared-header policy
  (vland WARN SHARED_HEADER_AVAILABLE x10). Re-ported onto the shared headers with the existing opt-ins of the landed 0x00B9AC30
  (FABLE_RETAIL_CGRAPHICINFOFRAME_GETTEXTURE_INLINE = FableWin ?GetTexture@CGraphicInfoFrame@@QBE_NPAPBVCTexture@@KK@Z, Ego_d
  `GetTexture(dest, mipmap_level, get_flags)`; PEEKGRAPHICINFOFRAME; STATICBLOCKS_TEXTURE_HELPERS; MESHBASE_ANIMATEDMESH declaration):
  RELOCATION_MATCH at the first grade. Ego_d locals verbatim; `texture` declared first; FableWin inline GetReflectionMapBankIndex().
- The lever for the old "push ebx sunk" residue: `if (frame_info->GetTexture(&texture, ...)) return texture; return
  frame_info->GetBlankTexture();` (bool GetTexture with an out pointer), not a texture-returning inline. No flags.

## 0x005AD305 CTCInventoryTrade::ApplyTattooCard -- RELOCATION_MATCH (4 grades), test PASS
- Ported the local-model draft onto the shared headers using the landed same-TU BuyItem 0x005AF64E's opt-in / include set and TU pragmas
  (optimize("y", off), ("s", on)), plus CTCHeroMorph (TATTOOTHINGS + new ADDTATTOO), CTCAppearanceMorphBase, CTattooDef MEMBERS, display
  engine accessors; Ego_d's names and scopes verbatim (ptc_morph_const PeekTC, two `unsigned long count` index loops, Textures set).
  First grade 603v620; adding the existing FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (+ CTATTOODEF_TYPENAME) matched.
- The include list is BuyItem's (some headers unused here); the test cannot kill a mutant on `if (ptc_hero_morph)` inside the
  RemovesAllTattoos loop: ptc_hero_morph and ptc_morph are the same component (interface 3), so it is never null there.

## 0x0067E290 CDegradableDef::Transfer -- OPEN DIFFER(273v273), 16 grades (pass 1)
- Ego_d: no locals (parameter `transfer`); the TU's debug forwarder CPersistContext::Transfer<enum EEngineGraphicType>(name, val) is the
  named macro form (43 bytes), release TLC drops the name (the existing FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY one-argument body).
- Residue: register renames in the scalar Transfer arms from the second one on: retail 0x0067E2EB `mov eax,[ebp] ; ... push eax` (save)
  and 0x0067E2F9 `mov edx,[esi+24h] ; push edx` (load), ours edx / eax; the third bool arm then ecx/edx v retail eax/ecx (and a
  reordered `xor/mov cl/push` before the stream load); the sort call 0x0067E370 pushes the comparator word through edx (ours eax).
- Tried, inert: enum TransferBinaryOut texts (named `long temp` matches FableWin's 13i/33b /Od size but not the release residue,
  implicit conversion), the enum transferred as `(long&)` / reinterpret_cast / `(unsigned long&)`, Transfer<T> case order swapped,
  `default: break;`, if/else-if (274), FABLE_RETAIL_CPERSISTTRAITS user ctor (+/- EMPTY_DTOR / COPYINIT: 276).
- Pass 2 probes (2): a visible noinline generic CPersistTraits<T>::TransferBinaryIn (the enum's 0x00632470) and visible
  GFSerialiseVectorBinaryIn / Out<CDegradableInfo> stubs: residue unchanged. Ego_r has only the named Transfer(name, val[, default])
  forms, but retail's inlined arms (binary modes only, CheckCRC with the pooled empty key) fit the one-argument release body.
- Earlier passes (hand222/609/763/1018/1020) parked the same rotation. Next: c2 scheduler replay (hand892 tools/sched) to find the one
  tuple that shifts the rename round-robin in the GraphicType arm region.

## 0x00954F40 CAIStateGroup_MazeTeleport::Update -- RELOCATION_MATCH (11 grades over 2 passes), test PASS, strict headers clean
- Ported off the parked local model (frame-order residue) onto the shared headers: CThing GetTC<CTCBoss> (EXPANDED; its key shares
  ptc_boss's frame word, as retail), CTCBoss::GetBossPhase / SetPhaseStartPos (FableWin inline), CThingSearchTools FIRSTROUGH
  GetPFirstThingInRoughArea<CThingFilter_HasScriptName, CThing> (the draft's "HasDefGlobalIndex" filter name was wrong: the filter holds a
  CCharString; landed binding + Ego_r instance name), C2DBoxI(C3DVector::ToC2DCoordI(), 200), NCreatureHiding::GetValidPositions, inline
  GFRandom, CCreatureAction_TeleportOut, CAIStateGroupBase PERFORMACTION / PEEK / GOTOBED_WORLD.
- Levers in order: Ego_d's declaration order (`CThing* pMarkerThing;` first, `pMarkerThing = 0;` right before the search: zero store
  at 0x00954FC4), the filter built from `marker_name` by the implicit char* -> CCharString conversion (retail re-addresses the temporary
  for the copy ctor), FABLE_RETAIL_C3DVECTOR_PACKED (owner exception; PhaseStartPos +0x22). Flags /GS.
- Headers: new engine/CAIStateGroup_MazeBase.h (Ego_d class record; opt-in FABLE_RETAIL_CAISTATEGROUP_MAZEBASE) and opt-in
  FABLE_RETAIL_CAISTATEGROUP_MAZETELEPORT in engine/CAIStateGroup_MazeTeleport.h (the class : CAIStateGroup_MazeBase).

## 0x00F3ABF0 CRangeCompressor::CalcCompressionScript -- OPEN DIFFER(605v605), 17 grades (pass 1)
- Ego_d: params script, block_ptr, element_count, element_size, block_size; function-scope count, count1, count2, block_buffer,
  sub_script, script_entry; three blocks each with only `long n` (the >4 column loop, the values loop, the sub-script copy loop).
- Ported (differ/00f3abf0.cpp): `long count = 0;` at the top (retail zeroes [esp+10h] before the >4 test), column loop `for (long n
  ...; n += 4)` with `block_size - n > 4 ? 4 : block_size - n`, ==3 as two recursive calls summed, values loop as one ternary store,
  count1 = CalcBestCompressionForBlock, count2 = lower + upper half, copy loop `for (long n = 0; n < sub_script.size(); n++)`.
  Code shape is retail's everywhere; residue: `this` takes ebx (retail ebp) and the other callee-saved value of each region ebp
  (19 hunks). Three separate if/else stores in the values loop: 30 hunks; a named `value` local: 19; the old draft (named
  source / value / remaining / half / total, none in Ego_d) keeps this=ebp but places `remaining`'s init early (3 hunks).
- Tried, inert: min spellings, `count = count +`, while loop, count1/count2 reused in the ==3 arm, branch order of the final test,
  split declaration (17 grades), then pass 2 (+7): bisect of the draft against the Ego_d port -- values loop (ternary) and tail
  (no half local, long j) are neutral (3 hunks kept); the >4 loop alone flips `this` to ebx: with only Ego_d's `n` the strength-reduced
  `block_size - n` temporary loses the colouring tie to `this`; a separate decrementing counter (`left` / the draft's `remaining`, not
  in Ego_d) keeps this=ebp but its init sits before the `n < block_size` guard (retail after, 0x00F3AC0C). FableWin /Od 226i / 658b
  (= Ego_d's 658 debug bytes) v the Ego_d loop text 224i: the loop has ~2 more /Od instructions than `n`-only. Next: a loop text with
  only `n` named that reaches 226i (FableWin) -- or a c2 colouring trace (hand892 c2prio_op.py).
- differ/00f3abf0.cpp = the Ego_d port with ONE non-Ego_d local (`long left = block_size;` counted down in the column loop): 1 placement
  hunk (retail inits the counter after the `n < block_size` guard). Probe: a visible noinline stub of CalcBestCompressionForBlock
  (0x00F3A890, before the row in retail) moves the allocation (603v605) -- its real body visible is untried (not landed).
