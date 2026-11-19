# hand4f notes (2026-11-19)

## Matched
- 0x00B6DF90 / 0x00B6DFD0 -- NOT byte twins (offsets 0x138 / 0x144): CEngineWaterRenderer::GetIceForegroundShadowedVertexShader /
  GetIceForegroundColbuffShadowedVertexShader (reloc_bindings.tsv binds both under caller RenderIceForeground 0x00B751F0; Ego_r
  Engine_Water_Renderer.obj #28/#29, FableWin #23/#24). The catalog label CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader was a
  byte-shape propagation from the landed 0x00BF5C60. FableWin /Od 0x02D4CE30: a single `return arr[(unsigned)n < size()-1 ? n : size()-1]`
  (the parked draft's if/return/return was the residue). Shared header with existing opt-ins FABLE_RETAIL_CENGINEWATERRENDERER +
  _FOREGROUNDHELPERS (declares the three private getters). MATCH at the first grade. No header changes, no flags.
  Test: `#define private public` (common test idiom), raw zeroed image with the vector words at +0x138/+0x144.

- 0x005DC340 CAnimationSet::GetAnim -- RELOCATION_MATCH. Residue was a dead `mov edi,ecx` (this kept across FindFirstAnimWithName,
  never read) + no end/key test in the loop. 15 probes of dead `this` uses (release dead-arm asserts in/before the loop, `(void)` compares,
  empty ifs, folded `cond || true` loop conditions, unused end local, unreachable trailing returns, same-TU bodies of GetNoAnims /
  AnimExists / FindFirstAnimWithName -- the last inlines FindFirst, wrong) all left DIFFER(56v66) or worse. The FableWin loop
  (`it != Anims.end() && it->first == start_it->first`) falling off the end keeps both tests. FableWin loop + `__assume(0)` after it:
  RELOCATION_MATCH. Evidence the original had an unreachable marker there: FableWin /Od shows no code after the loop (an __assume emits
  none at /Od), Ego_r's VC10 release body drops the same two tests, and retail's dead edi says the end() test existed at register
  assignment. Written as the landed CPersistContext idiom `#ifdef _MSC_VER __assume(0); #endif`.
  DECISION FOR THE COORDINATOR: the brief's device list says "no __assume"; the acctC ban measured __assume as an ASSERT form. This is
  not an assert stand-in but the function's unreachable end, the same as the landed `default: __assume(0)` rows. If the owner rejects
  it, the row has no other known C++ spelling (a noreturn call / for(;;) emit code).

- 0x0053848F / 0x005384DD -- NUISystem::CList::AddActionOnInactive / AddActionOnSelected (second was mislabelled; Ego_r ui_list.obj #28/#29,
  members +0x1AC/+0x1B0). RELOCATION_MATCH on the shared engine/NUISystem_CList.h (FABLE_RETAIL_CLIST) with the ui_list TU pragmas
  (`optimize("y", off)`, `("s", on)`, as the landed same-TU rows). The parked draft's residue (dead `push ecx ; and [ebp-4],0`) is gone
  with the shared STLport list; the last residue (new object in eax v retail edi) closed with the existing opt-in
  FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE: list's default ctor inline, _List_base(const allocator&) out of line -> 0x00535140 is
  the _List_base ctor, not list::list (NAME_FIXES.tsv). Helper bodies graded at own addresses (0x00535140 / 0x00535710) under /O2:
  RELOCATION_MATCH. 3 grades.

- 0x00554030 / 0x005698A0 / 0x004C5B90 -- the std::map operator[] family: RELOCATION_MATCH at the first grade each once moved onto the
  shared stlport/fable_map.h with the landed 0x00553E60 opt-in set (STLP_MAP + STLP_TREE_INSERT_UNIQUE_HINT_BODY +
  STLP_MAP_ORIGINAL_INSERT_BODY). Owner classes from NUISystem_CTable.h / CTCHeroOnlineScoreboard.h /
  NGraphicAppearance_CTCGraphicAppearance.h. 0x004C5B90 also needs FABLE_RETAIL_DEF_CLASS_BASE (CIVCountedPointeeBase Release/Delete),
  CTCGRAPHICAPPEARANCE_MOVEMENT + EFFECT_ENTRY_PASS23, the entry ctor body `{}` (FableWin 0x01D07550) and a derived-class typedef for
  the protected entry type (explicit-instantiation access check). Names fixed for 0x005698A0 / 0x004C5B90 (FableWin.pdb symbols).

## Open (owner wrap-up order 2026-11-19: not started)
- 0x006CA8C0 (138 bytes) -- UNTOUCHED, 0 grades. Catalog: CEngineTextureMorphSet::AddEngineMorph (Ego_d record: this, long original_texture;
  debug length 111); an older parked draft calls it CTextureMorphs::AddMorph. IDENTIFY first (Ego_r / FableWin object tables, callers,
  reloc_bindings). Prior best DIFFER(128v138) (ref/006ca8c0/prior_006ca8c0.cpp, local model): in the in-place push_back arm retail stores
  MorphID from the zero register (edi) and omits the temporary's blends free, with its own `++finish; return finish-1` tail; the draft
  merges both arms. Best next idea: port onto shared headers (engine/CEngineTextureMorphSet.h if present, stlport vector) in FableWin
  /Od statement order with Ego_d locals; check STLP vector push_back opt-ins (PUSHBACK_NOINLINE etc.) and same-TU helper bodies.
- 0x00A0CA90 (187 bytes) CWParsedItem::InitAsFloat -- UNTOUCHED, 0 grades. Prior best DIFFER(172v187)/(176v187) (ref/00a0ca90/prior_00a0ca90.cpp
  on engine/CWParsedItem.h): x87 allocation -- retail keeps integer part and fraction on the FPU stack and the divisor in the push-ecx
  frame word (fld/fmul/fstp per digit, fdiv from memory, fild of the digit before the multiply). Best next idea: copy the landed narrow
  twin 0x009B9770 CParsedItem::InitAsFloat (hand4e: FableWin order + Ego_d locals + retail literal types closed it at grade 1; whole part
  `*= 10.0f`, fraction `*= 10.0`) and widen it (unsigned short text); read each fmul operand size in retail.
- coordinator 2026-11-19: b1-b3 landed; __assume(0) as unreachable END accepted (EXTRA_RULES) with #elif __GNUC__ __builtin_unreachable(); and // Inferred:.
