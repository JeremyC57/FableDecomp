# NF57 notes (round 7: follow-up entries that need a name settled)

## Triage (before any grade)
- q10424 0x00B7C510: Background. The out-of-line body uses the function statics 0x01437864..0x01437884 and the same call
  sequence as the copy inlined in the landed RenderSeaBackground 0x00B70230; RenderSeaMidground 0x00B6FA40 uses its own
  statics (0x01437840..). FableWin.pdb has ?SetupSeaBackgroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ (0x02D51100).
- q10433: Ego_r order GetThemeForMiniMapAt 0x0067AC30, GetThemeSizeZOffsetAt 0x0067AC50, GetThemeMinCameraSizeZOffsetAt
  0x0067ACC0 (both 0x70 bytes) = retail GetDominantEngineThemeAt 0x0081F910, 0x0081FA20, 0x0081FA90 (both 103 bytes).
- q10411/12: Ego_r 0x00C1C5A0.. and FableWin 0x0346A790.. list const First, const Next, non-const First, non-const Next, the
  retail order 0x00AA7F40 / 0x00AA7FF0 / 0x00AA80E0 / 0x00AA8190.
- q10406/07: STLport std::_Destroy<T*>(T*,T*) stubs (push ecx; lea eax,[esp+3]; push eax; call __destroy_aux).
- q10429: Ego_r / FableWin `dynamic initializer for 'pGlobalScriptDef'` and its atexit destructor (unqualified, class type).
- reloc_bindings.tsv keeps one witness row per (symbol, target): every changed spelling needs callers.py / absrefs.py.

## Resumed run (after the 06:56 stop)
- Coordinator note: 0x0053BF20 / 0x0053CFC2 are NF56's (landing nf56); recorded `not-a-binder: handled by NF56` and q10403 judged
  from NF56's records (self ported by NF56, 0x0053BF20 blocked/port there, 0x0053C332 has no witness: NAME_FIXES).
- The 12 rows ported before the stop were rechecked first (work/vl1.log, all PASS).

## Descriptive names (made up: the original is in none of the sources this lane can read -- Ego_r / FableWin / Ego_d name sets,
## exe strings, the FSE manifest refs/fse_api_manifest.json, rebuild/include/libpk; libpk ships no PDB or RTTI. EgoCore and the
## sibling repos (FableForge, ForgeFSE-retail-shadow, Fable2RE) live on the owner's Windows paths and were NOT searched from here)
- CBankSampleRecordReader (vtable 0x012B4D34; OnRecord 0x00C313C0, out-of-line ctor 0x00C311C0, not landed): copies each
  "LHAudioBankSampleTable" record into the bank's sample map. Was CUnprovenRecordReader_012b4d34.
- CAudioBankTableReader (ctor 0x00C3BB40, Load 0x00C3BB50, LoadCriteria 0x00C3BCB0): the wrapper ReadBankData 0x00C30F60 builds
  on its segment reader to walk the sample-table and criteria segments. Was AudioBankTableReader_00c3bb50 /
  AudioCriteriaLoader_00c3bcb0 (Load / LoadCriteria self rows: NAME_FIXES).
- CSampleIndexMap / SSampleIndexNode: libpk/CBankProcessor.h's own descriptive names (not new), applied to 0x00C3B0E0 / 0x00C3B0C0.
- CDriverIndexMap (proposed only, not applied): the header's driver map, still spelled CIndexMap_00c3abd0; renaming a header
  class is not an opt-in, so q10483 is blocked on it.
- CPKDecompressedStream: the landed descriptive class (dtor 0x00C62750 / Seek / Read) now also names the ctor 0x00C62920.
- STLport instances (descriptive-by-library: retail is STLport 4.6, both PDBs Dinkumware): _Destroy 0x005BC0C6 / 0x004DFBC6,
  __uninitialized_copy / __uninitialized_fill_n / __destroy_aux 0x00AD1650 / 0x00AD1690 / 0x00AD37E0, _Rb_tree _M_create_node /
  _M_insert 0x00769980 / 0x0076A110, _Rb_tree::erase(const key&) 0x008AE420 and _M_erase 0x007AE0E0 (bindings only).

## Settled from two sources (see APPLIED.tsv)
- q10421/22/23: ?InitialiseTextures@CEngineScreenEffect{RadialBlur,Glow,Displacement}Renderer@@AAE_NXZ (both PDBs; retail order =
  Ego_r order InitialiseTextures then SetEffectQuality).
- q10433: GetThemeSizeZOffsetAt / GetThemeMinCameraSizeZOffsetAt are protected const (IBE) in both PDBs; all six rows blocked/port
  (local CMap models).
- q10430: 0x013BAE44 = CTCAIScratchpad::Gossip; its dynamic initializer / atexit destructor (0x0121F160 / 0x01229CA0) now come
  from one definition on the shared headers ($E1 / $E2), the 0x022 pattern of pGlobalScriptDef 0x0122AE7E.
- q10493: IsAudible returns bool: the returned `||` expression reproduces retail's eax materialisation (`mov eax,1`); an
  if/return-true form gives `mov al,1` (DIFFER 57v60). q10494: HasWaveBankSegments returns long (caller tests eax).

## Funclets regraded (parents changed)
- 0x00C30F60 -> 0x0121406C, 0x01214077; 0x00C30E90 -> 0x01214030, 0x01214049; 0x00C47F10 -> 0x01215910, 0x01215918, 0x01215920,
  0x0121592B; 0x00C62920 -> 0x01216442. All PASS (work/vl2.log, work/vl3.log, recheck_final.log).
- File moves the coordinator must repoint: 0x00C62920 (CAudioStreamOwner00c62920_... -> CPKDecompressedStream_...) is included
  by funclet 0x01216442. The other self renames (RENAMES s1 "File move") have no funclet that includes them (grep).

## Gaps found (not fixed; outside the slice or not a name fix)
- 0x00B687D0 BuildThemes: old candidates-header row with no // Leaf:; q.py builds 327 of 1476 bytes and no call to 0x00B6BD40;
  its include resolves only from a pending batch dir. An authoring task.
- 0x00B5BBC0 (bound by the radial ctor as ResetParameters, catalog SetShadowClipVolumesToNeverShadow) is Ego_r 0x00A0DD80 / FableWin
  0x02D1C070 SetDefaultParams (it stores the CurrentParameters defaults); the ctor 0x00B5C460 now binds it (NAME_FIXES for its self row).
- SetEffectQuality (all three screen effects) is virtual UAE in both PDBs: the retail vtables 0x012A2658 / 0x012A27CC / 0x012A3A94
  hold 0x00B5BFD0 / 0x00B5EE00 / 0x00B86B00 at slot 13, the first slot after CEngineComponent's thirteen, so a derived-class
  opt-in (_SETEFFECTQUALITY) carries it; all three rows ported (first form each).
- Rows ported here that keep pre-existing local models of shared-header classes (no new model added): 0x00C313C0 (CBankData),
  0x00C20C10 (CSoundEvent / CSoundEventManager), 0x00865C20 (CThing, C3DVector, CThingCreatureBase, CBaseIntelligentPointer).

## After the advisor review
- 16 zero-grade not-a-binder rows probed (work/nab_probe.tsv): every binding is the settled spelling (the 0x00AD68D0 -> 0x00AD37E0 line
  shows DIFF only because WORKLIST holds no witness for that pair; the bound symbol is the settled __destroy_aux).
- 0x0053C332 (not in rows.txt) already binds NF56's push_back spelling (read-only probe); dropped from NAME_FIXES.
- Moved out of NAME_FIXES (not proven): 0x00C3B160 is probably ?Get@CSampleIndexMap@@QAEPAXK@Z (AddDriver 0x00C2E2B0 calls it on the +0x3C
  sample map; catalog CIndexMap_00c3b160::Get); 0x00C168A0 (the request-name accessor 0x00C47F10 calls) returns const unsigned short*,
  name unsettled (catalog is a propagated tail-jump label).
- Class-key follow-through: reloc_bindings has no other `UCSegmentFile@@` binding and only the Pop self row for `UCCodaFrame@@`
  (NAME_FIXES). `ABUCString@@` remains in 0x00C1EEE0 (??0CNamedItem@@QAE@ABUCString@@K@Z -> 0x00C20230, ?Find@CItemDispatcher@@... ->
  0x00C101D0, which libpk/CBankProcessor.h calls CNamedSet::Find(const CString&, long*)): engine/CString.h keys CString as class; not
  respelled here (not in rows.txt, callee names unsettled).
- 0x00C47F10 still models `struct CCodaFrame` locally (binds only ?GetData@CCodaFrame@@QAEPAFXZ, where the key does not appear), so it
  does not contradict q10495; its local model is a port item.
- Address-suffixed names still in code of rows handed back (pre-existing, outside the entries' callees): 0x00C30F60 CSampleTableReader_012b4d40;
  0x00C313C0 CAutoMapEntry_00c3baa0 / CAutoMapOwner_00c31560 / SBankSampleRecord_00c313c0; 0x00C47F10 CAsyncDecoder_00c48640 /
  CIntelligentPointer_00c16900 / CMailbox_00c48860 / CMailboxQueue_00c48a00..c80; 0x00C497E0 StdcallForward_00c3cde0;
  0x00C62920 AlignUp_00c62920 (inline helper); 0x00C1D950 CAIStateGroup_00c1df70. The others listed by grep are in `Was ...` comments.
