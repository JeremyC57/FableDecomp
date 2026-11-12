## 0x00881A40

# DOSSIER 0x00881A40 NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags (581 bytes)

## 1. Retail shape (xdis 00881a40 0x255)
- __thiscall, `ret 8` (2 stack args), NO frame pointer (`sub esp,14h ; push ebx ; push esi`, then `push ebp ; push edi` only after the
  early-out test), no SEH frame, no `optimize("s")` idioms (imul 38E38E39h size divide inlined 3x, `jl` loop back-edges, `mov edi,edi`
  2-byte alignment pad before loop 1 at 0x00881A9E). => plain speed TU, /Oy on, no pragmas needed.
- Early out: `ScreenFilterKeys.find(entry)` (call 0x008839E0, ecx=this+0xA0, hidden iterator result local, key local = handle stored
  `mov [esp+10h],ebx`) ; `cmp edx,[esi]` vs header ; `je` straight to the pop/ret epilogue (ebp/edi not yet pushed).
- Register plan retail: esi = &args (arg1 reloaded into esi after find), ebx = handle, then ebx = &entry->reset (`add ebx,30h`) for all of
  loop 1; loop 1: ebp = counter i, arg1's stack slot [esp+28h] reused as the byte offset i*9, edi = args._M_start (reloaded every
  iteration as part of `size()`); loop 2: [esp+28h] (arg1 slot) = counter j, edi = byte offset j*9, ebp = &args[j] then the graphic.
  arg2 (handle) re-read from [esp+2Ch] in loop 2 to form entry->reset (`add ecx,30h`).
- size() = (finish-start)/9 recomputed at loop entry and at each back-edge (`for (i=0; i<args.size(); ++i)` with signed compare `jl`/`jle`
  => int counter compared against `(int)size()` or size() returning a signed difference -- STLport vector::size() returns size_type,
  but retail compares signed: counter is `int`, size cast or the comparison is int vs ... check: `cmp ebp,ecx ; jl` = signed).
- Loop 1 body (per element): key = args[i].thing.GetPItem() (0xA01B50, result stored to key local [esp+14h]); it = reset.find(key)
  (0x00883860, hidden result [esp+10h]); `if (it != reset.end())` (cmp vs [ebx] header): edi=&node->second(+0x14);
  GetPItem x3 on it (non-null, `test [eax+91h],1` Dead, `mov ecx,[eax+64h]` PTCGraphicAppearance, null test) then
  SetByPassColorFilter(bypassReset @ node+0x1C = second+8, zero-extended `xor edx,edx ; mov dl,..`);
  then INLINE erase(iterator): `_Rebalance_for_erase(node, header->_M_parent(+4), header->_M_left(+8), header->_M_right(+0xC))` cdecl
  `add esp,10h`; `~CBaseIntelligentPointer` on node+0x14 (0xA01C10, the value's destructor = ~CThingGraphicByPassFlagState);
  `if (node) free(node)` (0xBFEA14); `dec [ebx+4]` (--_M_node_count).
- Loop 2 body: GetPItem(args[j].thing) x2 on ebp (non-null, Dead bit), third GetPItem re-forms `[esi]+edi` (args[j] re-indexed, not a
  cached ref) -> graphic=[eax+64h] in ebp, null test; then local CThingGraphicByPassFlagState state at [esp+18h]: base ctor 0xA01B10,
  vptr 0x01238C6C, SetPItem(0) (i.e. the default ctor's thing pointer is set through SetPItem(0)), `state.bypassReset=false` scheduled
  late; `state.thing = args[j].thing` (GetPItem on args[j] -> SetPItem on state: the ASSIGN form); `state.bypassReset =
  graphic->ByPassColorFilter > 0` (`mov al,[ebp+5Bh] ; test al,al ; setg cl` = inline GetByPassColorFilter() const);
  key = state.thing.GetPItem() into [esp+14h]; `entry->reset[key]` (0x00884C70, ret 4) -> ebx; `*ebx = state` as
  SetPItem(state.thing.GetPItem()) + byte copy of bypassReset (CThingGraphicByPassFlagState::operator= = the PDB's ??4, inline);
  graphic->SetByPassColorFilter(args[j].bypassSet) (byte re-read `[edi+ecx+8]`); state destroyed via 0xA01C10.

## 2. Identity (two sources -- CONFIRMED, manifest label right)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r addr 0x008C27D0) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x02A59710 /
  naming_stragglers FableWin.exe.signatures.tsv): 
  `?ScreenFilterSetFlags@CScreenEffectPriorityStack@NScreenEffect@@QAEXABV?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@PAX@Z`
- Signature: `void NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags(const std::vector<CThingGraphicArgPasser>& , void*)`,
  public non-const non-virtual (QAE). Shared header already declares it as `ScreenFilterSetFlags(const ThingGraphicArgPassers& things,
  void* handle)` (rebuild/include/engine/NScreenEffect_CScreenEffectPriorityStack.h). PDB param names not on disk.
- Caller: only the tail-jump in landed CEnvironmentSpellEffectInterpolator::ScreenFilterSetFlags 0x006B83E0
  (`add ecx,90h ; jmp 0x00881A40`; xref.py shows 006b83e6), rebuild/src/compiled/00/6b/CEnvironmentSpellEffectInterpolator_ScreenFilterSetFlags_006b83e0.cpp.
- Not virtual (no vtable slot).

## 3. Callees
| addr | landed file | landed name | real (PDB/STLport) name | draft spelling |
|---|---|---|---|---|
| 0x008839E0 | 00/88/CRetailTreeulongkey_find_008839e0.cpp | `CRetailTree_ulongkey::find` (invented, "one of 12 bodies") | STLport `_Rb_tree<CStackEntry*,...>::find` = `std::set<CStackEntry*>::find` (out-of-line COMDAT; body shared by 12 instantiations) | `CKeySet::find` local -- fine as model, but use std::set |
| 0x00883860 | 00/88/stdRbtreeCUnprovenKey00883860..._find_00883860.cpp | `_Rb_tree<CUnprovenKey_00883860,...>::find` (placeholder types) | `_Rb_tree<CThing*, pair<CThing* const, CThingGraphicByPassFlagState>, _Select1st, less<CThing*> >::find` (map::find, out of line). Hand back NAME_FIXES: the key/value are now proven by this row | `CResetMap::find` |
| 0x00884C70 | 00/88/STLmap_operator_00884c70.cpp | std::map::operator[] | `map<CThing*, CThingGraphicByPassFlagState>::operator[]` (Ego_r ??A?$map@PAVCThing@@VCThingGraphicByPassFlagState@...), ret 4, out of line | ok |
| 0x0042A34E | 00/42/stdRbglobalbool_Rebalanceforerase_0042a34e.cpp | `std::_Rb_global<bool>::_Rebalance_for_erase` (static cdecl) | same | WRONG: draft `_Rb_tree_rebalance_for_erase` free function (SGI name, label-propagated) |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | CRT free (via STLport malloc allocator deallocate) | ok |
| 0x00A01B10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | CBaseIntelligentPointer() | same | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | GetPItem() const | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` -- returns CBaseObject* | draft returns CThing* (wrong type) |
| 0x00A01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | SetPItem | `?SetPItem@..@@QAEXPAVCBaseObject@@@Z` | draft takes CThing* (wrong) |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ~CBaseIntelligentPointer (virtual) | same | draft non-virtual dtor + fake v000 |
| 0x004BFB00 | 00/4b/NGraphicAppearanceCTCGraphicAppearance_SetByPassColorFilter_004bfb00.cpp | NGraphicAppearance::CTCGraphicAppearance::SetByPassColorFilter(bool) | `?SetByPassColorFilter@CTCGraphicAppearance@NGraphicAppearance@@QAEX_N@Z` | draft drops the NGraphicAppearance namespace (mangling irrelevant for a direct call but name must be exact) |

Inline (no call) but named in PDBs: `?GetByPassColorFilter@CTCGraphicAppearance@NGraphicAppearance@@QBE_NXZ` (the `setg` on +0x5B),
`??4CThingGraphicByPassFlagState@...@QAEAAV0123@ABV0123@@Z`, `??0CThingGraphicByPassFlagState@...@QAE@XZ`, `??1...`, and
`?Reset@CThingGraphicByPassFlagState@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEXXZ` (FableWin/Ego_r) -- almost
certainly the loop-1 restore block (IS_THING_ALIVE(thing) && graphic -> SetByPassColorFilter(bypassReset)), inline here; the same
block is in landed CScreenFilterEntry::Reset 0x00883AE0.

## 4. Members
- this+0xA0: `ScreenFilterKeys` (std::set<CStackEntry*>, 12-byte STLport). Ego_r offset 240 (debug containers bigger); retail +0xA0
  per landed DeleteDisplacementEntry 0x00881860 layout notes (+0x6C DisplacementParamStack, +0x78 DisplacementKeys, +0x84 colour ...).
  Shared header: NScreenEffect_CScreenEffectPriorityStack.h FULL branch (`FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL`) declares
  RadialParamStack[3] .. ScreenFilterKeys in PDB order; verify its sizeof math gives +0xA0 (FULL needs fable_containers.h/fable_map.h with
  FABLE_RETAIL_STLP_LIST / FABLE_RETAIL_STLP_MAP like landed InsertRadialBlurEntry 0x00881690). Default branch is member-less.
- CScreenFilterEntry (handle cast): `reset` map<CThing*, CThingGraphicByPassFlagState> at +0x30 (struct_layouts_egor.tsv:
  CScreenFilterEntry 64 bytes, +48 `reset`; landed Reset 0x00883AE0 uses +0x30, CStackEntry base +0..+0xF, CScreenFilterParams +0x10).
  Header: FULL branch has only a forward decl `struct CScreenFilterEntry;`; PROCESSES branch has CScreenFilterEntry : CStackEntry with only
  Reset(). No shared header models `reset` -- header debt (landed 0x00883AE0 / 0x00884C70 model it locally).
- CThingGraphicByPassFlagState (Ego_r 9 bytes): `thing` CIntelligentPointer<CThing> +0, `bypassReset` bool +8 (draft calls it `bypass`:
  WRONG spelling). Map node: key +0x10, value +0x14 (thing), +0x1C bypassReset.
- CThingGraphicArgPasser (9 bytes, pack 1): `thing` +0, `bypassSet` +8. Shared header opt-in `FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER`
  (needs engine/CIntelligentPointer.h with FABLE_RETAIL_CINTELLIGENTPOINTER (+_COPY) and complete CThing first).
- CThing +0x64 `PTCGraphicAppearance` (NGraphicAppearance::CTCGraphicAppearance*), +0x91 bit0 `Dead` bitfield: engine/CThing.h
  (IsDead(), GetTCGraphicAppearance() inline accessors exist). IS_THING_ALIVE<T>: engine/ThingAlive.h, opt-in FABLE_RETAIL_IS_THING_ALIVE
  (`if(thing!=0){ if(!thing->IsDead()) return true;} return false;` -- on a CIntelligentPointer needs operator->/compare; landed
  Reset 0x00883AE0 instead spells it as nested ifs on GetPItem()).
- CTCGraphicAppearance +0x5B `ByPassColorFilter` signed char: engine/NGraphicAppearance_CTCGraphicAppearance.h line 98 (private-ish
  block); neither GetByPassColorFilter nor SetByPassColorFilter is declared in that header (grep: no hits) -- add or declare locally.
- CIntelligentPointer<CThing> vtable 0x01238C6C: engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER (+ _ASSIGN: `item=
  other.GetPItem(); SetPItem(item)`, _COPY). Its default `CIntelligentPointer()` is EMPTY in the shared header, but retail state ctor
  calls SetPItem(0): CThingGraphicByPassFlagState() must init `thing(0)` (CIntelligentPointer(T*) -> SetPointer -> SetPItem) or the
  landed 0x00884C70 local model `CIntelligentPointer() { SetPItem(0); }`.
- Classes the old draft models LOCALLY that have shared headers: CThing (engine/CThing.h), CTCGraphicAppearance
  (engine/NGraphicAppearance_CTCGraphicAppearance.h), CBaseIntelligentPointer/CIntelligentPointer (engine/CIntelligentPointer.h),
  NScreenEffect::CScreenEffectPriorityStack (+CStackEntry, CThingGraphicArgPasser) (engine/NScreenEffect_CScreenEffectPriorityStack.h),
  STLport set/map/vector (stlport/fable_map.h, fable_containers.h).

## 5. Callee visibility
- set::find 0x008839E0, map::find 0x00883860, map::operator[] 0x00884C70: STLport inline members emitted out of line (COMDATs) in
  this TU. fable_map.h: plain `find` is inline-bodied (`iterator(_M_find(key))`); FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE gives the
  template form VC7.1 called out of line (chunk 890). operator[] under FABLE_RETAIL_STLP_MAP has its body visible (out of line in retail
  anyway; the visible body lets VC see the key only read -- relevant to the key-temp slot [esp+14h] shared by both loops).
- erase(iterator): EXPANDED inline here (rebalance+destroy+free+--count). fable_map.h FABLE_RETAIL_STLP_TREE_ERASE +
  FABLE_RETAIL_STLP_TREE_ERASE_BODY gives exactly that body (`_Rb_global_inst::_Rebalance_for_erase(..)`, `_Destroy`, `deallocate`,
  `--_M_node_count`). Check deallocate emits `if(p) free(p)` (retail `test edi,edi ; je`).
- Same TU neighbours (CScreenEffectPriorityStack.cpp, 0x00880A40..0x00884FE0): InsertRadialBlurEntry 0x00881690, InsertDisplacementEntry
  0x00881780, DeleteRadialBlurEntry 0x00881820 / DeleteDisplacementEntry 0x00881860 just before; InsertScreenFilterEntry /
  DeleteScreenFilterEntry (PDB names) probably right after 0x00881C90 (next function, `sub esp,60h`). CScreenFilterEntry::Reset
  0x00883AE0 and CThingGraphicByPassFlagState ctor/??4/Reset are inline (not called).
- GetPItem/SetPItem/ctor/dtor are in another TU (0x00A01Bxx) -- not visible.

## 6. Sibling landed rows (copy idioms)
- 00/88/NScreenEffectCScreenEffectPriorityStack_InsertRadialBlurEntry_00881690.cpp: `#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL`,
  FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_STLP_MAP, FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY; includes stlport/fable_containers.h,
  stlport/fable_map.h, engine/CMainGameComponent.h, engine/NScreenEffect_CScreenEffectPriorityStack.h; `#pragma auto_inline(off/on)`
  around a copy ctor only. No Flags line.
- 00/88/NScreenEffectCScreenEffectPriorityStackCScreenFilterEntry_Reset_00883ae0.cpp: same `reset` map walk with the restore block --
  `#pragma pack(push,1)` + `#define FABLE_RETAIL_CINTELLIGENTPOINTER` + engine/CIntelligentPointer.h; local STLport tree; nested
  `if (state.thing.GetPItem() != 0) { if (!(((CThing*)state.thing.GetPItem())->m_flags91 & 1)) { CThing* thing = (CThing*)
  state.thing.GetPItem(); if (thing->m_pGraphicAppearance) thing->m_pGraphicAppearance->SetByPassColorFilter(state.bypassReset); } }`
  (three GetPItem calls = retail loop-1 shape).
- 00/88/STLmap_operator_00884c70.cpp: the map<CThing*,CThingGraphicByPassFlagState> model, pack(1), `CIntelligentPointer(){SetPItem(0);}`,
  `CThingGraphicByPassFlagState() : bypassReset(false) {}`.
- 00/88/NScreenEffectCScreenEffectPriorityStack_DeleteDisplacementEntry_00881860.cpp: the keys-set find 0x008839E0 with a derived-entry
  key temporary homed in the arg slot; retail stack layout comments.
- 00/88/NScreenEffectCScreenEffectPriorityStackCScreenFilterProcess_OnErase_00880f50.cpp: FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES.
- 00/6a/NScreenEffectCScreenEffectPriorityStackCThingGraphicArgPasser_CThingGraphicArgPasser_006a2f00.cpp, 00/6a/stdvector_Minsertoverflow_006a3950.cpp:
  ARGPASSER users.

## 7. Old draft's likely model errors
- Parked DIFFER(591v581) on register allocation (ebx/ebp swap). Probable roots: (a) draft's loop-1 restore is open-coded with a
  draft-local IS_THING_ALIVE taking a reference; retail likely calls inline `it->second.Reset()` (?Reset@CThingGraphicByPassFlagState,
  PDB) -- try that member; (b) loop 2 state is built by default ctor `CThingGraphicByPassFlagState state;` then `state.thing =
  args[j].thing` -- matches; but the bypass is `graphic->GetByPassColorFilter()` (inline const accessor, returns bool) -- draft spells
  `> 0` directly (same code, but accessor inlining can shift regalloc); (c) map assign via the PDB's ??4 (user-defined/implicit
  operator= doing ASSIGN form then bool copy) -- draft relies on CIntelligentPointer::operator= only; fine; (d) the draft casts
  `void* entry` to CScreenFilterEntry* three times; retail forms &entry->reset once in ebx for loop 1 but re-reads the arg in loop 2 --
  consistent with casting per use; (e) the find key: retail stores the handle into a separate key local ([esp+10h]) before the find
  (conversion temp void* -> CStackEntry* const&), draft passes `(CStackEntry*)entry` -- matches.
- Wrong types/names: GetPItem/SetPItem take/return CBaseObject* (draft CThing*); dtor is virtual (draft non-virtual + fake v000);
  `bypass` -> `bypassReset`; `_Rb_tree_rebalance_for_erase` -> `_Rb_global<bool>::_Rebalance_for_erase`; SetByPassColorFilter lives in
  NGraphicAppearance::CTCGraphicAppearance; vector/args should be std::vector<CThingGraphicArgPasser> (ThingGraphicArgPassers typedef)
  not a fake CArgVector; local CThing/CTCGraphicAppearance/CIntelligentPointer/CScreenEffectPriorityStack stand-ins must become shared
  headers.
- Loop form is right: two `for (int i=0; i<(int)args.size(); ++i)` loops indexing `args[i]` (size recomputed each test; elements
  re-indexed, no cached element reference in loop 2's third GetPItem / bypassSet read).

## 0x00492973 (not landed)

# DOSSIER 0x00492973 -- CSubtitleRenderer::SetText (full 4-caption wide overload), 568 bytes

## 1. Retail shape (xdis 00492973 0x248)
- thiscall, `ret 28h` = 10 dword args (font, text, name, b1, b2, b3, flags, time, bool, bool).
- `push ebp; mov ebp,esp; sub esp,1Ch; push esi` ... `leave; ret 28h` => `#pragma optimize("y", off)` + `#pragma optimize("s", on)`
  (same as every landed CSubtitleRenderer row: 0x00492E4B, 0x00492BAB, 0x0048F6BD, 0x0048F939, 0x0048DDF6). No SEH, no /GS cookie
  (Clear 0x00492BAB and Pause 0x0048DD94 carry `// Flags: /GS`, this body has no cookie read -> no /GS flag needed).
- Frame 0x1C = the CSetTextInfo local only ([ebp-1Ch], sizeof 0x1C: 4 strings + flags + time + 2 bools). Everything else is homed
  in DEAD ARGUMENT SLOTS: CCharString temps at [ebp+0Ch]; hero pointer local at [ebp+0Ch] (passed by ref to IS_THING_ALIVE);
  GetTC key 0xCB at [ebp+0Ch]; found list at [ebp+0Ch] with its default allocator temp at [ebp+0Fh]; filter (0x11) at [ebp+14h].
- Early branch: `if (DisplayingText) { CSetTextInfo info(text,b1,b2,b3,flags,time,bool1,bool2); TextInfoQueue.push(info); }`
  then `jmp` to the common epilogue (pop esi only; ebx/edi pushed only on the main path) -> source is if/else or early return.
- ebx = &Text (+0x5C) is cached and reused for the Preload vcall; edi = 0x0122D70E ("" char literal) reused for both CCharString temps.
- Tail loop is UNROTATED: `mov edi,[ebp+0Ch] (sentinel); mov esi,[edi]; jmp test; body: push [esi+8]; mov ecx,ebx; call RemoveMessage;
  mov esi,[esi]; test: cmp esi,edi; jne body`. Manager in ebx (loaded BEFORE the list ctor), sentinel in edi.

## 2. Identity (label OK modulo doubled class)
- Mangled leaf: `?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@1111KM_N2@Z`
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x0049BBF8) + egor_signatures.tsv; the landed caller 0x00492E4B forwards its 10 args
  here (`call 0x00492973`), and landed Clear 0x00492BAB calls it with (`*PFont, info.Text, L"", info.Button1Text, ...`).
  Manifest label `CSubtitleRenderer::CSubtitleRenderer::SetText` has a doubled class prefix; real name CSubtitleRenderer::SetText.
- Signature: `void SetText(const CFontBank&, const CWideString&, const CWideString&, const CWideString&, const CWideString&,
  const CWideString&, unsigned long, float, bool, bool)` -- public, non-const, returns void.
- Callers (xref.py): 0x00492E27(.. ->0x00492E4B landed), 0x00492EA6/0x00492ED1, 0x00492F36/0x00492F69, 0x00492FA2/0x00492FB4,
  0x00493017/0x00493049, 0x004930A5/0x004930D7 (Update), 0x005EAE56, 0x006DD853, 0x00713392, 0x00892AF2.
- Parameter -> member mapping from bytes (IMPORTANT, not the obvious one):
  [ebp+10h] (arg3) -> NameText +0x60 ; [ebp+14h] (arg4) -> Button2Text +0x68 ; [ebp+18h] (arg5) -> Button1Text +0x64 ;
  [ebp+1Ch] (arg6) -> Button3Text +0x6C. Arg3 ("name") is NOT passed to CSetTextInfo (ctor gets text, arg4, arg5, arg6).
  Clear passes L"" as arg3 and info.Button1Text/2/3 as arg4/5/6. Header opt-in SETTEXT names params button0_text/button1_text/...
  (no PDB param names found). Draft already maps arg4->Button2Text, arg5->Button1Text: keep it.

## 3. Callees (landed file / landed name / PDB name; FLAG = draft spelling wrong)
- 0x0048DDF6 CSetTextInfo ctor: 00/48/CSubtitleRendererCSetTextInfo_CSetTextInfo_0048ddf6.cpp; PDB
  `??0CSetTextInfo@CSubtitleRenderer@@QAE@ABVCWideString@@000KM_N1@Z`. FLAG: draft models CSetTextInfo as a top-level struct; it is
  the NESTED class CSubtitleRenderer::CSetTextInfo (Clear 0x00492BAB models it nested).
- 0x004941C0: 00/49/stddequeCSubtitleRendererCSetTextInfo_pushback_004941c0.cpp = std::deque<CSetTextInfo>::push_back (out of line);
  source spelling is the inline `queue::push(x)` -> `c.push_back(x)`. FLAG: draft's `CSetTextInfoQueue::push` stand-in; use
  std::queue<CSetTextInfo, std::deque<...> > as Clear does (TextInfoQueue opaque [0x28] in header -> reinterpret_cast like Clear).
  fable_deque.h offers FABLE_STLPORT_DEQUE_PUSH_BACK (+ _AUX_NOINLINE).
- 0x0043E3D0 ~CSetTextInfo: 00/43/CSubtitleRendererCSetTextInfo_CSetTextInfo_0043e3d0.cpp, `??1CSetTextInfo@CSubtitleRenderer@@QAE@XZ`.
- 0x0099B800 CWideString::operator=(const wchar_t*) (00/99/CWideString_operatorwchartconst_0099b800.cpp); Clear declares it
  `const CWideString& operator=(const wchar_t*)` -- FLAG draft returns `CWideString&` and takes `const unsigned short*` with a cast.
  Literal L"" = 0x0122D70C.
- 0x0048F6BD `?GetTextFromTextTag@CSubtitleRenderer@@AAE_NABVCWideString@@AAV2@@Z` (private, returns bool) -- landed 00/48/..._0048f6bd.cpp.
- 0x00492381 `?ProcessEmbeddedGraphicTags@CSubtitleRenderer@@AAEXXZ` (private) -- NOT landed.
- 0x00491AD2 `?ProcessEmbeddedHighlightTags@CSubtitleRenderer@@AAEXXZ` (private) -- NOT landed.
- 0x0099EBF0 CCharString(const char*, long = -1) / 0x0099EAE0 ~CCharString (landed 00/99). Literal "" = 0x0122D70E. Args are temps:
  `SetButton1OverrideText("")`.
- 0x0048F7C0 / 0x0048F881 `?SetButton{1,2}OverrideText@CSubtitleRenderer@@QAEXABVCCharString@@@Z` (landed; header opt-in
  FABLE_RETAIL_CSUBTITLERENDERER_BUTTONOVERRIDE declares Button2 only).
- 0x00449970 `?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ` returns CPlayer& -- FLAG draft returns CPlayer*.
- 0x00487DC0 `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` (const) -- FLAG draft `GetHeroThing`/CThingCreatureBase.
- 0x0043DBD0 `??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z` (__fastcall, ptr by const ref) --
  FLAG draft instantiates IS_THING_ALIVE<CThing*>; must be <CThingPlayerCreature*>.
- 0x00410DE0 std::bitset<274>::test (CThing::UsingComponents +0x20) -- FLAG draft `CBitset Present`.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) via the inline
  CThing::GetTC(ETCInterfaceType) -- FLAG draft `CInterfaceMap`/`GetTC<T>(long)`; copy Clear's CThing::GetTC model verbatim.
- 0x0048DD94 `?Pause@CSubtitleRenderer@@QAEX_N@Z` (header opt-in FABLE_RETAIL_CSUBTITLERENDERER_PAUSE).
- 0x0048F939 `?InitPlayerMode@CSubtitleRenderer@@AAEXXZ` (private) -- FLAG draft `ProcessPlayerModes`.
- 0x00493BC2 std::list<const CMessageEvent*>::list(const allocator&) out of line (landed as a placeholder
  global_ListNodeCreateCircularSentinel_00493bc2.cpp, `ret 4`, arg = &allocator temp [ebp+0Fh]).
- 0x00493EAB `??$FindAllMessages@VCMessageEventFilter_IsOfType@@@CMessageEventManager@@QBE?BJABVCMessageEventFilter_IsOfType@@AAV?$list@PBVCMessageEvent@@...@Z`
  -- CONST member, returns `const long`, output is std::list<const CMessageEvent*>&. FLAG draft non-const, returns long, custom list.
- 0x00493B5B `?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z` (opt-in FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE, decl only).
- 0x00493BDD list<...>::~list out of line (std_list_T_00493bdd_dtor_00493bdd.cpp; placeholder name). fable_containers.h has
  FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE.
- vcall [PFont vtbl+0x10] = slot 4 `CFontBank::Preload(const CWideString&, bool) const` (engine/CFontBank.h line 41, 0x009A0B20) --
  FLAG draft `LayoutText`. Called as `PFont->Preload(Text, true)` (push 1; push &Text).
- Globals: 0x013B86A0 = CMainGameComponent::PMainGameComponent via inline static Get() (naming wave 18; draft's g_pMainGameComponent
  is the old stand-in) ; +0x1C PPlayerManager (scoped_ptr<CPlayerManager>), +0x24 PWorld; CWorld +0x60 PMessageEventManager_Data
  (engine/CWorld.h, inline GetMessageEventManager() under FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER).
- Message type 0x11 = MESSAGE_EVENT_TEXT_FINISHED_2 (Clear's enum) -- filter CMessageEventFilter_IsOfType(type).

## 4. Members touched (engine/CSubtitleRenderer.h, retail offsets, struct_layouts_egor.tsv names)
+0x24 TextCharactersShowing long (`and [esi+24h],0`) ; +0x28 TextGroupHasEntriesRemaining bool ; +0x2C TextGroupIndex long (`or -1`);
+0x30 TextGroupTag_PStringData (CWideString) ; +0x34 TextInfoQueue opaque [0x28] (queue<CSetTextInfo,deque>) ; +0x5C Text_PStringData;
+0x60 NameText ; +0x64 Button1Text ; +0x68 Button2Text ; +0x6C Button3Text (all `X_PStringData`, reinterpret_cast<CWideString&>
as Clear does) ; +0x78/+0x79 Button1Alpha/Button2Alpha =0xFF ; +0x7C DisplayFlags ; +0x80 PFont (CFontBank*) ; +0x88 MaxDisplayTime ;
+0x8C TextTimer (fldz) ; +0xDD AllCharactersDisplayed ; +0xE5 DisplayingText ; +0xE6 UpdateWhilePaused ; +0xE7 UpdateTimer.
Header opt-ins available: FABLE_RETAIL_CSUBTITLERENDERER (IsDisplayingText inline), _PAUSE, _BUTTONOVERRIDE, _CLEAR, _SETTEXT,
_SETTEXT_CHAR (declares this 10-arg overload), _INTERACTIVE, _INCUTSCENE, _GRAPHICTAG, _CTOR, _DTOR. Clear uses the
`#define CSubtitleRenderer CSubtitleRendererLayout` + derived-class trick to add the nested CSetTextInfo + member decls.
Other headers: engine/CWorld.h, engine/CPlayer.h, engine/CPlayerManager.h, engine/CThing.h (UsingComponents +0x20, Components +0x44),
engine/CMessageEventManager.h, engine/CFontBank.h, engine/CTCHeroReceiveItems.h (no members past PickedUpItems modelled).
- Hero component byte: `mov byte ptr [GetTC(0xCB)+0x26],1` = CTCHeroReceiveItems::DisplayingItems (Ego_r +0x32; retail is Ego_r-0xC,
  same shift as Clear's ForceClear Ego_r +0x33 -> retail +0x27). FLAG draft `CHeroControls::Frozen`. Interface 0xCB =
  TCI_HERO_RECEIVE_ITEMS (Clear).
- Classes the draft models LOCALLY that have shared headers: CSubtitleRenderer, CThing, CPlayer, CPlayerManager, CWorld,
  CMainGameComponent, CMessageEventManager, CFontBank, CVectorMap (draft's CInterfaceMap), CTCHeroReceiveItems (draft CHeroControls).

## 5. Callee visibility
- Same TU (CSubtitleRenderer.cpp, defined EARLIER): Pause 0x0048DD94, CSetTextInfo ctor 0x0048DDF6, GetTextFromTextTag 0x0048F6BD,
  SetButton1/2OverrideText 0x0048F7C0/0x0048F881, InitPlayerMode 0x0048F939, ProcessEmbeddedHighlightTags 0x00491AD2,
  ProcessEmbeddedGraphicTags 0x00492381. All called out of line in retail.
- COMDATs emitted after the TU (0x00493xxx/0x00494xxx, unaligned odd addresses = template/inline instantiations of this TU):
  list ctor 0x00493BC2, list dtor 0x00493BDD, RemoveMessage 0x00493B5B (out of line here! inline in 0x0089AF30's TU),
  FindAllMessages 0x00493EAB, deque::push_back 0x004941C0. Sibling 0x0089AF30 note: "the sentinel stays in edi across
  FindAllMessages only when that template's body (and push_back's) is compiled earlier in the TU" -- they gave those bodies
  under `#pragma auto_inline(off)` before the row. Consider the same for FindAllMessages / list ctor / dtor / RemoveMessage here.
- Opt-ins: FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE, FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE (fable_containers.h);
  FABLE_STLPORT_DEQUE_PUSH_BACK (fable_deque.h); FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE; FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER.

## 6. Sibling landed rows (copy idioms)
- 00/49/CSubtitleRenderer_Clear_00492BAB.cpp -- next function in retail; THE template: Layout-rename include trick, nested CSetTextInfo,
  std::queue/deque stand-ins, CThing::GetTC FABLE_FORCEINLINE over CVectorMap::LowerBound, CPlayer::GetPControlledCreature,
  IS_THING_ALIVE(hero), CMainGameComponent::Get(), `// Flags: /GS` (this row needs none), pragmas y-off + s-on.
- 00/49/CSubtitleRenderer_SetText_00492E4B.cpp, _00492F69.cpp -- forwarding overloads; y-off + s-on.
- 00/48/CSubtitleRenderer_Pause_0048DD94.cpp -- includes engine/CSubtitleRenderer.h directly.
- 00/89/CGameScriptInterface_RemoveAllCutSceneSkippedMessages_0089AF30.cpp -- the FindAllMessages(IsOfType)+remove idiom, with
  list<const CMessageEvent*> model and auto_inline(off) callee bodies.

## 7. Draft model errors vs disassembly
- Body control flow is right (DIFFER 572v568, tail only). Residue: retail homes the found list in the dead `text` slot [ebp+0Ch]
  (allocator temp at [ebp+0Fh]) and the filter in [ebp+14h]; the draft gets list +8/filter +0xC; and retail's removal loop is
  unrotated (jmp-to-test) with sentinel in edi, manager in ebx.
- Draft list/ctor: retail calls an out-of-line list ctor taking `const allocator&` (default-arg temp) -- draft's CAllocatorTag stand-in
  should be the real STLport `std::list<const CMessageEvent*>` with `explicit list(const _Alloc& a = _Alloc())`.
- Loop: retail iterates begin()..end() via iterator (`it = head->_M_next` loaded via `mov esi,[edi]`), calling RemoveMessage(*it);
  write `for (std::list<const CMessageEvent*>::iterator it = found.begin(); it != found.end(); ++it)` with a real STLport iterator
  (or a while loop) -- draft's raw-node for loop rotates.
- FindAllMessages is const and returns `const long`; manager pointer is read as `Get()->PWorld->PMessageEventManager_Data` (ebx)
  before the list is constructed -- keep a named manager local declared before the list.
- Names wrong in draft: GetHeroThing->GetPControlledCreature, ProcessPlayerModes->InitPlayerMode, LayoutText->Preload,
  CHeroControls::Frozen->CTCHeroReceiveItems::DisplayingItems, IS_THING_ALIVE<CThing*> -> <CThingPlayerCreature*>,
  CBitset Present -> std::bitset<274> UsingComponents, CInterfaceMap -> CVectorMap Components, g_pMainGameComponent ->
  CMainGameComponent::Get(), top-level CSetTextInfo -> CSubtitleRenderer::CSetTextInfo, local CSubtitleRenderer -> shared header.

