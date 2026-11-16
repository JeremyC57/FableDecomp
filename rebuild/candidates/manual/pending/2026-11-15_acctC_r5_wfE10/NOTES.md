# wfE10 NOTES (agent wfE10, acctC lane) -- summary
No row matched (no src/, test/, payload.json, oracle.tsv). Every row now COMPILES on today's rebuild/include + hdr/ (all five drafts were
SRC_FAIL or stale before) and has a graded differ/ source:
  0x007A1B20 DIFFER(2828v2840) 10 hunks, registers equal   | 0x00507C30 DIFFER(7661v7661) 2 lines (one swapped reload pair)
  0x00502E90 DIFFER(13933v13622) inline-budget sites       | 0x00795300 (= CTCUnholyPower::FrameUpdate) / 0x0078FD60 DIFFER(9548v9268) twins
Tools in tools/: gv.sh (grade with prepended defines), sd2.py (structural diff, `m` masks registers), cmpcalls.py (call targets by
address via reloc_bindings), slotmap.py (retail -> ours frame offsets), port_optin.py (copy an opt-in block from a stale header).
Coordinator decisions: see the end of this file.

# wfE10 NOTES (agent wfE10, acctC lane)

## 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate -- DIFFER(2828v2840), 10 hunks, registers equal (differ/007a1b20.cpp)
Identity: Ego_r.pdb/FableWin.pdb ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z
(Ego_r 0x007A7370); extent 0x007A1B20..0x007A2638 incl. the 5-entry jump table, `ret 0Ch` (player by value = 8 bytes + seed).
Re-authored on today's shared headers (old draft used hdr_STALE). Overlay: CTCWillLightning.h BEAM_FRAMEUPDATE (method decls),
new CSpecialAbilitiesLightningSpellDef.h, CTCSoundPlayer PLAYUNATTACHEDCRITERIA, CHitParameters SETCREATEDUST (hdr/README.md).
What moved it (2752 -> 2832/2840, call list now identical by target):
- FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE (retail expands both Inserts, calls _M_insert_overflow 0x0076ADA0; landed sibling
  CTCLightningNode::FrameUpdate 0x007F2DC0 uses the same opt-in), FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE (retail calls erase 0x007A38B0;
  landed same-TU erase row), CCountedPointer dtor plain inline (NOT VISIBLE_CALLED_DTOR: retail expands it in the end loop).
- named `CCharString criteria = PDef->EndAttackeeSound.GetString();` in its own block for both sound calls (GetString before pushes).
- `m_tpParticleHandle.Get() && !m_tpParticleHandle->IsDead()` spelled out (retail `test eax,eax` + `test [eax+91h],1`) and
  `CTCDParticleEmitter* emitter = 0;` (retail passes ebp=0 when GetTC fails). These gave the ebp zero / ebx=0x10 constant registers.
- C3DVECTOR_NONZERO_SQUARED_XYZ + C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ (squares shared between IsNonZero and Normalise, summed X,Y,Z):
  the plain-hit copy (0x007A2030..) now matches.
Open residue (retail address order):
1. frame 0xbc v ours 0xb4: retail keeps the spilled &m_lChildren (0x007A2573 `mov [esp+2Ch],esi`) in its own slot; ours packs it
   into the LIGHTNING_START pair slot 0x14; one more 4-byte slot missing (retail strings at 0x40 / 0x44). Tried: particle block in its
   own nested block (worse), one function-scope iterator, a named children reference (neutral).
2. decapitating-hit copy (0x007A1E53..0x007A1E9B): retail squares Z,Y,X (fstp X then `fld st0; fmul st1`) and compares with no fxch;
   ours squares X,Z,Y as in the plain-hit copy (which matches). Tried GetSquaredMagnitude / paren / ZYX spellings on that copy only.
3. CLOSED: `CThingGameObject* struck = static_cast<CThingGameObject*>(m_tcpTarget.Get());` between SetMoveBackOnHit and
   SetCreateDustParticleEffectOnHit (the old draft's order) puts the target GetPItem between the two stores as retail: 27 -> 10 hunks.
5. frame detail: retail criteria strings at [esp+40h] (attached) / [esp+2Ch] (unattached), ours both at [esp+14h]. Tried: reference-bound
   temporaries (neutral), pairs at function scope (frame 0xcc, worse), one criteria at case scope (2876, worse).
4. end-loop head pad (`lea ecx,[ecx]` v `mov edi,edi`) -- follows from the frame.
Grades used: ~35.

## 0x00507C30 CWorldMap::LoadFromFile -- DIFFER(7661v7661), 2 unmasked lines (differ/00507c30.cpp)
The H40i draft failed to compile on today's headers (its opt-ins lived only in pending/2026-11-11_linux_handH40i/hdr). Re-based as
additive opt-ins on copies of TODAY's headers: CWorldMap.h _LOADFROMFILE, CThingManager.h _SETCURRENTTHINGUID + _WORLDLOAD,
CThingMapManager.h _INIT, CRegion.h _LOADFROMFILE, CLevelLoader.h _CTOR, CEngineManager.h _CLEANUP (asserts pinned).
Name fix: the draft's descriptive CThingManager::InitialiseLoadedThings is ?ActivateThings@CThingManager@@QAEXABV?$vector@PAVCThing@@...@Z
0x0051E2F0 (Ego_r.pdb name + landed row CThingManager_ActivateThings_0051e2f0.cpp + landed caller 0x005223F0); spelled so now.
Bindings re-checked: CLevelLoader ctor (Ego_r ??0CLevelLoader@@QAE@AAVCThingManager@@AAVCWorldMap@@AAVCWorld@@@Z) and
CEngineManager::Cleanup (Ego_r 0x005A5930 right before AddEngine 0x005A5950 = retail 0x00640490 before AddEngine 0x006404D0).
Residue: ONLY the swapped pair of reloads at the parse-loop exit 0x00509395 / 0x0050939C (retail `mov ebp,[esp+1D0h]` (file) then
`mov edi,[esp+0CCh]` (buffer data); ours the other way round). Register-masked: 0 hunks, call targets unchanged.
Tried this session: k = 1..7 unused `float uK = 0.0f;` at entry (no change: not a symbol-count tie). Earlier agents' c2prio
measurement (line 3 of the source) says the order follows the split candidates' original ids; untried: c2prio_f.py (wfcommon C44).
Grades used: 10.

## 0x00502E90 CWorldMap::PopulateRegionWithVillagers -- DIFFER(13933v13622) (differ/00502e90.cpp)
The chunk-879 draft was SRC_FAIL on today's headers. Re-based (all additive, hdr/README.md): CThingAICreature.h _RELATIONS (inline
Set/Get Father/Mother/Spouse, SetHome/WorkBuilding unless CTCWIFE_MARRY), CThingCreatureBase.h _GETSEX (GetSex 0x00661FF0; GetCreatureType
via today's _CREATURETYPE), CThingCreatureDef.h _SEX_TYPED (Sex / CreatureType typed ESex / ECreatureType inside _MEMBERS), CThingBuilding.h
_RESIDENCY (GetIsAvailableForResidency const, the bound spelling), CTCVillage.h _GAYVILLAGERS, CWorld.h _POPULATE
(PopDontPopulateNextLoadedRegion), CWorldMap.h _POPULATE (decl), stlport/fable_map.h _TREE_ERASE_KEY_CALLS (equal_range / erase(first,last)
noinline with their STLport bodies, erase(key) forceinline in tree and map: retail 0x005034BE expands erase(key) and calls both halves).
Today's names: CMAINGAMECOMPONENT_WORLDSEED -> _SAMPLEBANK_WORLDSEED, CTHINGBUILDING_SIMDEF -> _PEEKSIMDEF, CWORLD_PEEKDEFINITIONMANAGER;
ECreatureType from engine/ECreatureType.h (HUMAN_CHILD / HUMAN_ADULT; the draft's local CREATURE_TYPE_* enum removed).
Residue (call list by target otherwise equal): (1) servant CreateRelevantPersonAndUpdateStats site expanded in ours, called in retail
(retail calls only at the servant and apprentice sites); (2) female push_back called (0x00517A10) in ours, expanded in retail
(_M_insert_overflow); frame / register colouring as in the chunk-879 notes. Tried: scoped inline_depth(0) on the servant call (13837,
apprentice site then expands and operator-> calls appear), /d2inlT60..80, /d2inlS20/30 (T70 13625v13622 but 315 hunks).
Grades used: ~20.

## 0x00795300 -- real name CTCUnholyPower::FrameUpdate (catalog said CTCDivineWrath) -- DIFFER(9548v9268) (differ/00795300.cpp)
Identity: rtti-vtables CTCUnholyPower vtable 0x0126BFDC slot 9 -> 0x00795300 (CTCDivineWrath slot 9 -> 0x0078FD60), Ego_r.pdb
?FrameUpdate@CTCUnholyPower@@UAEXXZ; acctB dossier (ref/00795300/notes/DOSSIER.md). NAME_FIXES.tsv has the row.
Twin: 0x0078FD60 CTCDivineWrath::FrameUpdate differs ONLY in 11 constants (RemoveTC 0x86/0x85, SpellExclusionChannel +0xB0/+0xA8,
LoopingSound +0xC4/+0xBC, AddDamageWithAttribute 0xA/9, CEventHitBySpecialAbility 0x13/0x12): one source text, two rows.
Re-based the acctB draft (its hdr overlay was stale): copied the purely-additive stale headers (CCreatureAction_BuildUp/Release/
ReleaseAndLoopUnholyPowerSpell, CTCDExperienceOrb, CTCUnholyPower, CTCWillResponse); ported blocks into today's CThingCreatureBase
(_LEAVEORBS), CHitParameters (_SPECIALABILITY), C3DVector (_SUBTRACT_NAMED_RESULT, as the first branch of the subtract chain),
C2DBoxI (_RADIUS_INLINE, first branch of the radius-ctor chain); GETSTATE_INLINE = today's CWILLBUTTONHELPERTIMER_STATE_INLINE.
Dropped CINTELLIGENTPOINTER_ASSERT_ASSUME / CDEFPOINTER_ASSERT_ASSUME (__assume in operator->: compiler-steering, not retail text).
Residue: see line 2 of the source. First structural fact: ours makes the zero register (edi) at entry, retail first at 0x0079570C
(release start) -- every early `test reg,reg` becomes `cmp reg,edi`, the cancel-sound arms cannot tail-merge (ours pushes edi before
the GetString call). Grades used: 8.

## 0x0078FD60 CTCDivineWrath::FrameUpdate -- DIFFER(9548v9268), 145 reg-masked hunks (differ/0078fd60.cpp)
Identity: rtti-vtables CTCDivineWrath slot 9 -> 0x0078FD60; Ego_r.pdb ?FrameUpdate@CTCDivineWrath@@UAEXXZ. The H40 draft (ref/) is
SRC_FAIL with a large header debt; instead the row is GENERATED from the twin 0x00795300 source by renames (UnholyPower -> DivineWrath,
ST_BUILD_DIVINE_WRATH / HERO_ABILITY_SKILL_DIVINE_WRATH_SPELL strings verified with rstr.py at 0x0126BF90 / 0x0126BF40,
HERO_ABILITY_DIVINE_WRATH_SPELL (18), damage attribute 9, TCI_DIVINE_WRATH 133). It grades EXACTLY like the twin (9548v9268,
145 hunks), confirming one source text. Overlay twins: CTCDivineWrath.h _IMPLICIT_INFO_DTORS / _INLINE_ACCESSORS (the class has no
DarkFacesEffect: 200 v 208 bytes), CCreatureAction_{BuildUp,Release,ReleaseAndLoop}DivineWrathSpell.h _CCREATUREACTION_DIVINEWRATH
(renamed copies of the UnholyPower opt-ins; ctors Ego_r 0x008F57C0 / 0x008F5810 / 0x008F5870 names, retail 0x008BD170 / 0x008BD1C0 /
0x008BD220). Any fix found for one row applies to both. Grades used: 1.


## Decisions for the coordinator
1. NAME FIXES (NAME_FIXES.tsv): 0x00795300 is ?FrameUpdate@CTCUnholyPower@@UAEXXZ (catalog: CTCDivineWrath::FrameUpdate);
   0x0051E2F0 is CThingManager::ActivateThings (H40g draft's descriptive InitialiseLoadedThings).
2. Overlay edits that touch existing lines (preprocessed default text unchanged): C2DBoxI.h and C3DVector.h get a NEW first branch in an
   existing #if chain (old `#if`/`#ifdef` line becomes `#elif defined(...)`); CThingCreatureDef.h wraps the two opt-in lines Sex /
   CreatureType inside the existing _MEMBERS opt-in in `#ifdef _SEX_TYPED ... #else <old> #endif`. Accept, or move to separate names.
3. stlport/fable_map.h _TREE_ERASE_KEY_CALLS is an inline control in a shared header (noinline equal_range / erase(range), forceinline
   erase(key)) evidenced by retail 0x005034BE; it is a header opt-in, not a per-row pragma.
4. The acctB CINTELLIGENTPOINTER/CDEFPOINTER _ASSERT_ASSUME (__assume in operator->) opt-ins were dropped as compiler steering.
5. 0x00502E90 is an inline-budget residue (owner rule 2026-11-08 allows /d2inlT/S or scoped pragmas): no N found yet that fixes both
   the servant CreateRelevantPersonAndUpdateStats call and the female push_back expansion.

# SUCCESSOR (wfE10-2) -- appended; earlier sections above stay valid unless contradicted here
## 0x007A1B20 -- now 8 hunks, frame 0xbc = retail (differ/007a1b20.cpp)
- CLOSED frame / string slots: the two EndAttackeeSound criteria are IMPLICIT conversion temporaries
  `sound->PlayAttachedCriteria(PDef->EndAttackeeSound, true, false, false, -1)` / `PlayUnattachedCriteria(PDef->EndAttackeeSound, m_vEndPos, false, false)`
  with the existing per-TU opt-in FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (forwarder expanded, GetString 0x00415DD0 called: checked
  in the obj; without the opt-in the call goes to ??B 0x00415D70 = wrong target). Evidence: Xbox Anniversary PDB (S_REGREL32 list of
  CTCWillLightning::CLightningBeam::FrameUpdate: pTCLightning, lbChild, ptc_elec, hit_direction, hpParams, ptc_entity_events x2, hit_params,
  sound_player x2, vtStartPos, vtEndPos, upParams, lbChild) has NO criteria local. Explicit unnamed `.GetString()` puts GetString after the pushes.
- Open: decap copy x87 square order only (retail 0x007A1E68 fstp X, squares Z,Y,X; ours fst X, squares X,Z,Y like the plain copy which matches)
  + end-loop pad (follows: retail decap block 4 bytes longer). Tried (no change / worse): decap-only spellings `C3DVector d; d = a - b;` (2860),
  `d(a - b)`, `-(b - a)` (2844), `d(a); d -= b` (needs SUBTRACT_ASSIGN; 3016), `C3DVector(a) - b` (3016), inverted IsNonZero if (10 hunks),
  `d = d.Normalise()` (neutral). Comparator log (tools/lk on tmp/b1.cpp -> tmp/lk_b1.json): decap direction float records ids 796/797/798,
  plain 923/924/925 -- the same relative pattern, so the difference must come from records retail creates differently in the decap copy.
## 0x00507C30 -- still DIFFER(7661v7661) 2 lines, but the call target is now right (differ/00507c30.cpp, line 2 has the c2prio measurement)
- The old source called vector<char>(unsigned) (bound to 0x00411910) where retail calls 0x00410F30: q.py hid it. 0x00410F30 = CArray<char>(unsigned)
  out of line (NAME_FIXES.tsv), with a visible body: new opt-in FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE (hdr/README.md). The body-less
  existing SIZE_CTOR_OUTOFLINE grades 6 masked hunks (buffer not kept in edi, `mov esi,1` constant): c2 then assumes &buffer is captured.
- With it, the named `text`, `buffer[...] = 0` and no-text spellings all grade the same 2 lines (they used to break it). k=1..7 dead long
  locals at entry: no change. c2prio: the reload order = ascending split-piece id; buffer local (#2) -> #62 (free list), file #65 -> #295,
  this #67 -> #296+. Retail needs the buffer piece between file and this. Not closed.
## 0x00502E90 -- unchanged DIFFER(13933v13622) (successor tries)
- inline_depth(0) around BOTH the servant and the apprentice CreateRelevantPersonAndUpdateStats calls: 14080 (operator-> of peopleDef and the
  other inline args become calls; retail inlines peopleDef-> ([edi+68h]) and calls CThing::GetPos 0x004C73D0 at that site). /d2inlT90/110/130,
  /d2inlS40/60: 14120 / 14534 / 15372 / 14162 / 14115, call diffs 18..32 (worse than baseline 7 diff regions).
- Reading of the budget: ours expands the servant site (more headroom there than retail) and then has none left for the female push_back;
  retail must have spent more IL BEFORE the servant site. fwsize (tmp/fw_v0.txt) lists the earlier helpers whose FableWin bodies are larger
  (CThingAICreatureInit ctor/dtor +9i, CIntelligentPointer ctors +9i, CDefPointer operator-> assert bodies): the place to look next.
## 0x00795300 / 0x0078FD60 twins -- DIFFER(9472v9268), 139 masked hunks (was 9548 / 145)
- Zero register now made late as retail (ours 0x00795724 v retail 0x0079570C): the GetAllThingsInRoughArea<> body must be VISIBLE
  (existing opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY + CTCMAPWHO / _SEARCHID, includes CTCMapwho.h / NThingMap_CThingMap.h before
  CThingSearchTools.h with `class CTCMapwho;` first, as the landed CWorldMap::IsLineOfSight 0x0063E6F0). Same lesson as 0x00507C30: c2 then
  knows &things is not captured, so the dead `_M_data = 0` store of the list ctor before malloc goes (retail 0x00795516 has none).
- Cancel-sound arms: both must be implicit conversions `PlaySoundW(PDef->CancelSound[SpellLevel], false)` (the 2nd arm's temporary is built
  BEFORE `push 0` in retail; explicit .GetString() pushes first). Arm 2 must call GetString 0x00415DD0, arm 1 expands it. Default header:
  arm 2 calls ??B 0x00415D70 (wrong); new FORCEINLINE_FORWARDER: both expand; + scoped inline_depth(1) on arm 2: no change; inline_depth(0):
  calls ??B and expands operator-> / vector[] (wrong). Untried: a budget flag (/d2inl*) with FORWARDER, or arm 1 through a different helper.
- Remaining after that (cmpcalls): extra PlaySound call (no tail merge), CEventDied AddEvent duplicated, retail calls list dtor 0x006F09D0,
  CIntelligentPointer copy 0x0078E5B0 and IS_THING_ALIVE in the spell-over loop where ours expands them; frame 0x248 v 0x230.
- Xbox Anniversary PDB locals of CTCUnholyPower::FrameUpdate (weak third source; that build reworked the function): creatures, search_area,
  ptc_hero_stats, findit, something, pspecial_effects, effect, objects, ptc_experience, pdef, hit_param, point_of_hit, direction,
  ptc_entity_events, finalaction, effectname, info, effect_pos, it, ptc_nymph, ptc_cmm, ptc_reward, ptc_deed_log, pcreature_special_effects,
  pmy_enemy_tc, focus -- usable for local naming; no cancel-sound criteria local (consistent with the implicit conversion).

## Decisions for the coordinator (successor additions)
6. New overlay opt-ins: engine/CArray.h FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE (0x00507C30) and engine/CDefString.h
   FABLE_RETAIL_CDEFSTRING_FORCEINLINE_FORWARDER (twins) -- both per-TU inline controls with byte evidence (hdr/README.md); both are new first
   branches of existing #if chains (default text unchanged; no FABLE_STATIC_ASSERT in either header).
7. NAME FIX (probable): 0x00410F30 is ??0?$CArray@D@@QAE@I@Z, not the landed descriptive vector<signed char>(unsigned) leaf (NAME_FIXES.tsv).
   The landed 0x00507C30-era differ sources calling vector<char>(unsigned) would have bound the wrong address (0x00411910).
8. 0x007A1B20 uses the existing FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (implicit criteria conversions; Xbox PDB has no criteria local).
- (0x00507C30 more) `long size = (length == -1) ? file.GetLength() : length;`, `long size = length; if (size == -1) ...`, a named
  `size = file.GetLength() + 1` for the buffer: all the same 2 lines (insensitive with the visible-noinline ctor).

# THIRD AUTHOR (wfE10-3) -- appended
## 0x00507C30 (wfE10-3)
- NEW FACT (c2prio_f.py on probe/r3.cpp -> tmp/prio_r3.txt): local NAMES change c2's original candidate ids (hash-order walk with a free list):
  renaming to the Xbox-PDB names (file_length, map_id, map_uid, map_posx, map_posy, Sea, loaded_on_player_proximity, level_name,
  level_filename, level_script_name, parsed_item, buffer->text) swapped the ids of mapY (5 -> 70) and mapUID (70 -> 5); the buffer-data
  local stays #2 and still gets a free-list piece (#104) below file (#295). Grade unchanged (2 lines). 12 names for the `file` param: no change
  (param ids fixed). Dead `long uK = 0;` k=1..4 before the length test and before the parse loop: no change.
- Model: pieces are handed walking parents in ascending original id; the local (#2) always comes first and takes the free-list top, so
  the reload order file < local < this needs file to get a smaller piece (free list) than the local, i.e. file walked before the local.
## 0x00502E90 (wfE10-3) -- inline trace (tools/inl.sh = C26 c2 inliner trace, tools/inl/an.py) measured; the model is now explicit
- Trace of differ (tmp/inl_p0.txt): T=74, expand iff sc < 74. Servant site = decision 481 (sz 336, a 99, g 8342, pen 0, sc 29 -> expanded);
  apprentice 521 (g 8913, a+g > 9000 -> step penalty 168, sc 151 -> called). Fill-loop push_backs: male 569 sc 124, FEMALE 584 sc 97,
  children 599 sc 159, all with pen 25 (a+g > 9000) -> all called. Without pen they would be 75.6 / 59 / 95 = exactly retail's P O P.
- probe/lc1.cpp: `bool listCreatures = true;` before the servant loop, passed at the servant + apprentice sites only (instead of the literal
  `true`): the constant-argument savings vanish (sav [40,0,..], c1 296, sc 214) -> both CALLED, g at the fill loop drops by ~494 -> pen 0 ->
  male called / female EXPANDED / children called: the key-call pattern (tools/kc.sh) now equals retail's EXACTLY at those sites
  (`| C | R | R | C C P C C O C C P`). Grade 14064 (longer): side effects -- the fill-loop CThingAICreatureInit ctor (0x004FACD0) sites
  538/553/566 now expand (retail calls all five), a 0x00512210 helper expands at two sites (retail calls), one Create call lost (cdiff #118).
- Contradiction found: retail CALLS all five fill-loop init ctors (needs the step penalty there: g+17 > 9000) yet EXPANDS the female push_back
  a few decisions later (needs NO penalty, or a higher site frequency f > 0.281 v 0.2142). With identical male/female branch structure
  (retail 0x00504A01.. / 0x00504BB2.. are the same shape) the plain model cannot give both: some cost input of the female site differs in
  retail (its f, or its callee: e.g. `females.insert(females.end(), creature)` would fold to the same bytes with a different price). Untried.
- tools: kc.sh (key-call pattern from the last obj), cdiff.py (call targets by address v retail), fd.py (first differing instr), anch.py.
- (wfE10-3) differ/00502e90.cpp now uses the SHARED engine/CThingAICreatureInit.h (+ existing opt-in FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY) instead of a row-local class model: same grade 13933v13622. Other row-local classes remain (CVillagePeopleDef: no shared header, the landed 0x004FF750 does the same; CSimBuildingDef, CTCDummyVillager, CTCDNone: check engine/ headers before landing). The lc1 probe (variable listCreatures) is saved as differ/00502e90_lc1_probe.cpp (a probe, not a candidate).
- (wfE10-3) More measured constraints for 0x00502E90 (from the p0 / lc1 traces): retail CALLS SetHomeBuilding (0x00512210) at two house sites
  where p0 expands it with sc 72 (pen 39, g 9662): retail needs sc >= 74 there, i.e. a+g about 730 LARGER than p0 (p0 already includes the
  494 of the expanded servant). So retail's accumulated inline size g is larger all along (>= +560 before the servant site, >= +1200 v lc1 by
  the house loop): some helper expanded at many sites has a bigger IL body in retail (candidates from fwsize tmp/fw_v0.txt: CIntelligentPointer
  <CThingBuilding/CThingAICreature>(T*) ctors and copy ctor +9i, CThingAICreatureInit ctor/dtor +9i, Set{Home,Work,Father,Mother,Spouse}
  +10i). The female push_back must ALSO have a different price (it is expanded although a step penalty applies everywhere late): unexplained.
  `females.insert(females.end(), creature)`: called (worse). Next: helper-text level (bigger-IL but same-code bodies of the frequent helpers).
## Decisions for the coordinator (wfE10-3 additions)
9. No row matched in this pass; no header / opt-in changes (hdr/ untouched), no new names. differ/00502e90.cpp switched from a row-local
   CThingAICreatureInit model to the shared header + existing FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY (same grade).
10. 0x00502E90: if a variable `listCreatures` local at the servant/apprentice sites is ever proposed, it is a FITTED inline-price device
   (no evidence in retail beyond the decisions); it fixes those sites but not the row (see the wfE10-3 section).
11. Grades used this pass: about 45 (007a1b20 11, 00507c30 22, 00502e90 12) + 3 c2 traces (c2prio x1, inliner x2).

# FOURTH AUTHOR (wfE10-4) -- appended
## 0x00507C30 CWorldMap::LoadFromFile -- RELOCATION_MATCH, test PASS, vland RELOCATION_MATCH PASS, addmatch done (src/00507c30.cpp)
- What closed it: device d (owner acceptance test d) -- 3 dead release asserts `FABLE_RETAIL_ASSERT(...)` (overlay fable_assert.h with
  FABLE_RETAIL_ASSERT_DEAD_ARM) right before `CMapInfo& info = Maps[newMap];` in the EndMap block. Their dead arms' IL records re-key c2's
  split-piece numbering (c2prio trace tmp/507_prio_a.txt: the reload order at the loop exit = ascending piece id; pieces are handed out
  from a LIFO free list in a hash-order walk), so the exit reloads become ebp(file) / edi(buffer data) / ebx(this) as retail 0x00509395.
  Conditions FITTED (any condition grades the same); count measured 0..7 at that site, only 3 matches. k unused int locals 1..7 at 5 sites:
  no effect (unused locals are dropped before ids are assigned).
- WARNING (fragility): the matching count/position is hash-walk sensitive: every TU change (an added #include of engine/CPlayerGui.h or
  engine/CExceptionBase.h, a renamed declaration) moved it (2 at entry -> 4 at EndMap -> 3 at EndMap). A future header change that adds
  symbols to this TU may need a re-sweep: tools/507swa.py <src> endmap,after,tail,entry (strips the asserts, writes N=0..7 variants).
- Names fixed in the row (two sources each): 0x0041A060 NResourceDirectoryNames::GetLevelDir (Ego_r 0x0042B8E0; landed CWorldMap::
  SaveMapsToString 0x004FB990 declares it so) was a descriptive GetLevelsDirectory; 0x00438570 CPlayerGui::PeekGuiDef (Ego_r 0x0044B230,
  header opt-in FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF) + CPlayerGuiDef::RegionGraphFileName (+0xA94, FABLE_RETAIL_CPLAYERGUIDEF_CLASS) replaced
  a raw +0xA94 cast of a descriptive GetPlayerGUIDef; 0x009BA390 CStringParser::ReadLineAsString (Ego_r 0x00B0EE30; FABLE_RETAIL_CSTRINGPARSER
  _READLINE) replaced ReadAsStringUntilWhitespace (WRONG TARGET 0x009BA4A0 hidden by q.py masking); globals g_bUseCompiledWorldFiles /
  g_WADBuildEnabled / g_bUseCompiledGlobalThings (landed spellings) replaced GLoadBinaryWorld / GSaveBinaryWorld / GLoadGlobalThings...;
  NProfileTimer from fable_profile.h; CGenericException : CExceptionBase (shared engine/CExceptionBase.h; ThrowInfo __TI2, landed
  CWStringParser::ReadAsFloat model) instead of a base-less local class. The buffer is `CArray<char> text` (Xbox PDB local `text`).
- Header changes: none (existing opt-ins only; fable_assert.h already in hdr/).
- Test: test/00507c30.cpp embeds the row's function text verbatim over a mock engine (3 runs: text world, binary world, missing quote);
  a mutant without `info.Used = false` fails.
## 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate -- RELOCATION_MATCH, test PASS, vland RELOCATION_MATCH PASS, addmatch done (src/007a1b20.cpp)
- What closed it: device d -- ONE dead release assert right after the decapitating hit's `C3DVector direction = ...`:
  `FABLE_RETAIL_ASSERT(direction.X == direction.X && direction.Y == direction.Y && direction.Z == direction.Z);` (NaN check; overlay
  fable_assert.h + FABLE_RETAIL_ASSERT_DEAD_ARM). The dead arm's IL reads the components and changes the x87 stacker's choice for that
  copy only: retail 0x007A1E68 fstp X / squares Z,Y,X, now ours too; closes the 4-byte gap and the end-loop pad (all 8 hunks).
  Measured: asserts with no direction read (1..7 at function / case 3 / clock / before either copy's IsNonZero): inert; `direction ==
  direction`, IsNonZero(), Y-only and Z-only NaN tests: no; X-only and the 3-component test (1..4 copies): match. Condition FITTED.
- Rest of the source is the wfE10-2 differ unchanged (opt-ins listed in hdr/README.md wfE10 section); line 3 now says `ret 0Ch`.
- vland RELOC_NAME warnings (labels, not our spellings): 0x0079F730 is ?AllChildrenInactive@CLightningBeam@CTCWillLightning@@QAE_NXZ
  (Ego_r 0x007A4D20 / FableWin 0x026B2D00; the label 'CLightningBeam::AllBeamsInactive' mixes it with CTCWillLightning::AllBeamsInactive);
  0x0129BA3C DELTA_NOTIONAL_ZERO (label g_NormaliseEpsilon); 0x007A3200 CCountedPointer<CLightningBeam> dtor.
- Test: test/007a1b20.cpp embeds the row's function text verbatim over a mock engine (all 5 states, both hit kinds, zero direction,
  blocked hit, end sounds, child erase); the mutant with a +Y zero-direction fallback fails.
## Decisions for the coordinator (wfE10-4)
12. Both matches use device d (dead release asserts through the overlay fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM), counts and
    conditions stated in each source header (FITTED conditions): 0x00507C30 three asserts before Maps[newMap] in EndMap; 0x007A1B20 one
    NaN-check assert after the decap direction. fable_assert.h is an acctC overlay header (not in rebuild/include): it must land with them.
13. 0x00507C30 name fixes in the row (see its section): NResourceDirectoryNames::GetLevelDir, CPlayerGui::PeekGuiDef +
    CPlayerGuiDef::RegionGraphFileName, CStringParser::ReadLineAsString (was a WRONG CALL TARGET 0x009BA4A0 hidden by q.py), landed global
    spellings, shared CExceptionBase. NAME_FIXES.tsv: 0x00410F30 CArray<char> ctor, 0x006BC4A0 CRegion::AddMap, 0x0050BE40
    CCountedPointer<CAFile>::DeleteData, 0x0079F730 CLightningBeam::AllChildrenInactive.
14. No header / opt-in changes in this pass.

# FIFTH AUTHOR (wfE10-5) -- appended
## 0x00502E90 (wfE10-5) -- DIFFER(13572v13622), call list by target = retail except one cross-jump merge (differ/00502e90.cpp)
- NEW evidenced fix: the erase(key) at 0x005034BE: our hand count loop `for(i=p.first;i!=p.second;++i) ++n;` survived as a 30-instruction
  node walk; STLport 4.6 _tree.c's own `n = distance(p.first, p.second)` (with FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE) lets VC7.1 drop it
  as retail does. New nested opt-in FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE in hdr/stlport/fable_map.h (hdr/README.md).
- Inline set now = retail at every key site (tools/502kc.sh: I=init ctor C=Create P=push_back R=CreateRelevant H=SetHomeBuilding |=DP)
  with scoped `#pragma inline_depth(0)` (device b) around: servant + apprentice R calls (servant def indices hoisted to named locals
  like the apprentice/shop sites, so peopleDef-> stays inline), the 5 fill-loop `CThingAICreatureInit init(...)` decls, the
  `candidates(males)` copy, the 3 house-loop SetHomeBuilding calls (back() hoisted to a named local: retail inlines back()).
  Trace (tools/inl.sh, tmp/inl_502p2d.txt): ours g stays below retail's all through the late part (retail needs pen>0 at the vector
  copy, the house SetHomeBuildings, the fill-loop ctors) -- the per-site pragmas are the device for that.  /d2inlT72/73 instead of the
  house/candidates pragmas: map::find (0x00513420) then expands (pen falls with T) -- rejected.
- Remaining: (1) teacher-site R expansion: ours 4 Creates (the fallback `else if (maleDefIndex)` copy not cross-jumped into the first
  male copy: ours pushes edx/eax in the first copy's overflow path, retail ecx/edx -> retail jmp 0x00503810); shopkeeper site 2 Creates
  v retail 3. (2) frame 0x19c v 0x194 (pre-existing, also in the base); 223 masked hunks, mostly slot/colour.
- Anniversary PDB locals (weak) of this function are in tmp/anniv_syms.txt line 75020 (men_shuffled = candidates, ptc_talk, init x14).
## 0x00795300 / 0x0078FD60 twins (wfE10-5) -- DIFFER(9363v9268), 104 masked hunks, call list = retail but one dtor (differ/*.cpp)
- tools/795patch.py applies the same edits to either twin (both grade 9363v9268 / 104).
- (1) Visible CDefClassBase::GetSubDef<T> body (text of the 0x00502E90 row) + existing FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME and
  _GETEFFECT_BODY (landed same-TU CTCUnholyPower ctor 0x007941D0): retail keeps creature_effects in ebp after GetSubDef 0x00669580
  (0x00796CA9); ours had a zero register in ebp there (cmp ebp,reg / AddEvent of CEventDied tail-duplicated). Fixed both. 9405 -> 9363.
- (2) Stamina cancel arm: `{ CCharString sound = PDef->CancelSound[SpellLevel].GetString(); PlaySoundW(sound, false); }` (FITTED): the
  GetString call 0x00415DD0 (an explicit call at f 0.03 is refused, sc ~92) with the temporary built BEFORE `push 0`, and the arms
  tail-merge (arm 1 jmp into arm 2's PlaySoundW). Implicit conversion: GetString expands at both arms (nested decisions under the
  forceinline forwarder get f=1.0, sc 17); inline_depth(0) suppresses the forceinline forwarder (??B called); inline_depth(1) inert.
- (3) Device d: three dead `FABLE_RETAIL_ASSERT(PDef->DamagePerHit[SpellLevel] >= 0.0f);` right after `float damage = ...` before the
  spell-over loop. Their condition's inline calls (CDefPointer operator->, vector[]) are expanded by c2's inliner although dead, so g
  rises and the step penalty reaches the loop-top decisions as in retail: IP copy 0x0078E5B0, IS_THING_ALIVE 0x006961F0, bitset test
  (0x00410DE0), SetLeaveExperienceOrbsOverride 0x0078DDB0 x2 called. 0, 1 and 2 asserts leave them expanded (measured).
  LESSON: dead release asserts with inline-call conditions are an inline-BUDGET lever (they add g without code).
- Open: frame 0x240 v 0x230 (16 bytes); release-loop ~name (VICTIM_EFFECT) cross-jumped in ours (18 v 19 ~CCharString calls); the
  scoped inline_depth(0) on the spell-over copy/IS_THING_ALIVE (tried before the asserts) also calls the tree iterator operator->.
- (wfE10-5, later) + new opt-in FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST (FableWin four-field initialiser as the first branch of
  the box-ctor chain; 9363 -> 9349) + existing FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY (visible by-name GetDef<T> body: the
  HERO_ABILITY def stays in esi across AddTrainableStatExperience and its release loses the null test, retail 0x007959B2) -> 9303v9268,
  85 masked hunks. Tested and rejected: __assume(Object) in CDefPointer operator-> (new opt-in tried in a scratch copy only, not kept:
  9365 / 211 hunks, and the null test stayed -- it was the reload, not the assert). Assert count re-measured after each change: 3 (4 equal).
- Twin source edits are all in tools/795patch.py (run on ref drafts 795_base / 78f_base = the old differ sources).
- (wfE10-5) Twin probe, NOT kept: one shared `CThingFilter_OverlappingCircle circle` for the whole release part (both GetAllThingsInRoughArea
  calls) gives 9277 (and 9247v9268 with 4 asserts, 88 hunks, call list still right) -- shorter than retail, frame still 0x240: retail most
  likely builds the circle twice. Frame 0x240 v 0x230 open (our /FAs slot table: tools/795fr.sh; this at retail 0x38, ours 0x4c).
- (wfE10-5) 0x00502E90 differ now also declares noGuards before noGhosts (retail loads NumGhosts +0xA4 into ecx, NumGuards into eax at
  0x00502F5B; registers now equal there, same hunk count). k = 1..7 unused locals at entry: byte-identical (inert).
- (wfE10-5) Landing blockers still in 0x00502E90: row-local class models with shared headers (CShopDef, CSimBuildingDef, CTCBuyableHouse,
  CTCThingOwner, CTCTalk, CTCDummyVillager); twins: row-local NEntityEvents::CEventDied etc. (check engine/ headers before landing).
## Decisions for the coordinator (wfE10-5)
15. New overlay opt-ins (additive; default text unchanged; hdr/README.md): stlport/fable_map.h FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE
    (STLport 4.6 text, row 0x00502E90) and engine/C2DBoxI.h FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST (FableWin text as a new first
    branch, twins). Matched rows 0x007A1B20 / 0x00507C30 regraded after both: still RELOCATION_MATCH.
16. Devices in the open rows (none matched): 0x00502E90 uses 10 scoped `#pragma inline_depth(0)` regions (+2 hoisted locals for the
    servant def indices and 3 for back()); the twins use 3 dead asserts (device d) and one FITTED named-temporary block. If the owner
    prefers, the 502 pragmas might be replaceable by device-d asserts (the twins showed asserts move g): untested.
17. __assume(Object) in CDefPointer operator-> (acctB idea) re-tested in a scratch header: worse (211 hunks), not kept.
- (wfE10-5, last) Twins now use the SHARED opt-in FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (engine/CDefinitionManager.h, FableWin
  0x01CCD0F0 text, noinline) instead of a row-local template body: identical grade 9303v9268 / 85. (0x00502E90 still carries the older
  row-local GetSubDef<T> body: switch it to the same opt-in before landing.) FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY: inert.
- Grades this pass (wfE10-5): 0x00502E90 ~45 + 2 inliner traces; twins ~80 + 1 inliner trace.

## wfE10-6 re-verify (2026-11-15) -- 0x00507C30 / 0x007A1B20
- Assigned again as "parked" by the coordinator, but both were already closed by wfE10-4. Re-verified against today's rebuild/include +
  hdr/ overlay (after the other agent's additive opt-ins): q.py RELOCATION_MATCH both (no flags); vt.py PASS both; read-only vland
  RELOCATION_MATCH PASS both (WINS 2); payload.json source/test equal to src/ and test/. Nothing re-authored.
- vland RELOC_NAME warnings on 0x007A1B20 for 0x007A3200 (CCountedPointer<CLightningBeam> dtor) and 0x0129BA3C (DELTA_NOTIONAL_ZERO) are
  Ghidra-label disagreements only: rebuild/oracles/reloc_bindings.tsv already binds those exact spellings to those addresses. The other
  warnings (0x0079F730, 0x00410F30, 0x006BC4A0, 0x0050BE40) are already in NAME_FIXES.tsv.

## wfE10-7 (successor) -- 0x00795300 / 0x0078FD60 twins
- NEW (measured): (1) the two `CThingFilter_CanBeHitByEpicSpell can_be_hit;` named objects -> value-initialised temporaries in the
  Bind2 argument, `CFilter_Bind2<...>(CThingFilter_OverlappingCircle(pos, radius), CThingFilter_CanBeHitByEpicSpell())`, with the row-local
  class WITHOUT the std::unary_function base (as the landed 0x006A8D30 row models it; POD, so `T()` zero-fills its byte): gives retail's
  `mov byte [esp+0x1f],0` stores (0x00795579 / 0x00795A19) and frame 0x230 (was 0x240). (2) behaviour fix: the keep test of the
  spell-over erase loop is `offset.GetSquaredMagnitude() > reach * reach + 5.0f` (retail 0x00796294 `test ah,5 ; jnp reset`: keep only
  when strictly greater); the old `<=` inverted the branch. (3) the hit sites: retail builds `GetPos() - dir` X first with each store
  right after its fsub and no copy (0x00795B30.., 0x0079665C.., 0x0079704x) = the landed SpiritAttack 0x00854DD0 idiom: DEFAULT
  operator- (drop FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT) + visible noinline 9-arg CHitParameters ctor (HIT_CTOR + HIT_CTOR_BODY +
  CTOR instead of SPECIALABILITY; new opt-in CHITPARAMETERS_EPICSPELL_SETTERS supplies the two setters). Named result + ctor body: the
  temporary is still copied (z through ecx), 9340. (4) device-d assert count must be re-swept after each change (tools/asw.sh).
- (wfE10-7) Best now differ/00795300.cpp = differ/0078fd60.cpp: DIFFER(9284v9268), 63 masked hunks, call list = retail but the one cross-jumped
  ~CCharString (tools/795patch7.py turns a wfE10-5 source into it). Matched rows 0x007A1B20 / 0x00507C30 regraded after the CHitParameters.h
  addition: still RELOCATION_MATCH.
- (wfE10-7) Measured but not kept: Xbox-PDB local names (creatures, search_area, something, hit_param, direction, effectname, ...; tools/xren.py):
  byte-identical (local names do not move these ties). point_of_hit named local: no change. SQUAREDMAGNITUDE_XYZ/ZYX/none: identical.
  Normalise opt-ins: PAREN_SQUARES_XYZ best (fixes dir site 3); site 2 (0x007965C2, retail Z,Y,X with `fst/fmul`) still X,Z,Y.
  k unused locals at entry: inert. Dead-assert count re-swept 0..20 on each base: 8 best (spell-over loop top called as retail).
- (wfE10-7) Sweep-2 `++it` arm placement (retail 0x007961D9 right after the test, ours last before the latch): FIXED by writing the test as
  `if (!IS_THING_ALIVE(creature))` (or IS_THING_DEAD) -- the bool-returning inline function changes c2's block order -- but that site's
  extra inline layer then makes the sweep-2 map erase (0x00797A60, retail calls) expand and the bitset test call 0x004118C0 (9335, 81
  hunks); no assert count 0..20 or asserts before the loop restores it; `creature == 0 || ...` / `!creature.Get() || ...` keep the old
  placement. Open lead: an inline control for the erase only (scoped inline_depth(0) also un-inlines the iterator copy: 9291).
- (wfE10-7) Inliner traces (tools/inl.sh, tmp/inl_d7.txt = best source, tmp/inl_t2.txt = IS_THING_ALIVE probe): sweep-2 erase decision
  sc = c1*(1/f+6)/7 with c1 73: best f 0.6152 -> sc 79 > T 74 (called, = retail); probe f 1.2305 -> sc 68 (expanded). The bool-function
  test makes the else arm 1/2 instead of 1/4 likely. /d2inlT67 on the probe calls the erase and, with 4 dead asserts, gives frame 0x230,
  retail x87 orders at the range test and dir site 2, and no block hunk (9202v9268, 66 hunks) -- but small-c1 helpers then lose their
  forced inline (map::find wrapper c1 9 called at 0x007955F2 / sweep 1) and HasTC 0x004118C0 is called where retail calls
  bitset::test 0x00410DE0 (the latter is in the probe at default T too). T68 = T67; T70/72 re-expand the erase. Rejected; kept as
  differ/00795300_isalive_probe.cpp. Next idea: a spelling of the sweep-2 dead/null test that keeps f(else) = 1/4 (two branches) but
  lets c2 put the ++it arm first (retail), or find why retail's erase arm, not the ++it arm, falls into the latch.
- (wfE10-7) 0x00502E90 not reopened this pass (no measured lead beyond the wfE10-5 note: device-d asserts as g lever might replace some of its
  10 inline_depth pragmas -- untested; the six row-local class models are still landing blockers). Grades this pass: twins ~200 + 2 inliner traces.
## Decisions for the coordinator (wfE10-7)
18. New overlay opt-in engine/CHitParameters.h FABLE_RETAIL_CHITPARAMETERS_EPICSPELL_SETTERS (two inline setters, additive; lets a TU use
    HIT_CTOR + HIT_CTOR_BODY instead of SPECIALABILITY). Matched rows regraded: still RELOCATION_MATCH.
19. Behaviour fix in the twins: the spell-over keep test is `!(mag > reach*reach + 5.0f)` (retail jnp); the wfE10-5 source had `<=` (NaN differs)
    and an intermediate draft `>` (inverted).
20. Twins' row-local CThingFilter_CanBeHitByEpicSpell no longer derives from std::unary_function (matches landed 0x006A8D30 model; needed for
    the value-initialised temporary's zero byte). No shared header exists for this class.
21. Twins still use device d (8 dead asserts, count re-swept) and one FITTED named temporary (stamina cancel sound).
