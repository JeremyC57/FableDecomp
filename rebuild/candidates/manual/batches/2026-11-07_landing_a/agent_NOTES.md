# Chunk 827 notes (six game-logic retries)

All six names were already proven by earlier chunks; none is descriptive. Overlay: hdr/ (README there). Every attempt is in STATUS.tsv /
ATTEMPTS.log.

## 0x00684610 CTCCreatureOpinionOfHero::OnSerialise -- RELOCATION_MATCH (src/00684610.cpp, test PASS g++ + VC7.1)
- Start: chunk 824's parked po17 (`pending/2026-11-06_linux_hand824/differ/00684610.cpp`), regraded unchanged: same 4 lines.
- Same-TU visible bodies (chunk 824's lever, kept): `template <> void CPersistContext::Transfer<long>` (landed row 0x00410BE0's switch body)
  and `Transfer<COpinionTransientOffset>` (landed row 0x00685F80's body), both under `#pragma auto_inline(off)`, copied as chunk 824 had them.
- The fix: Ego_r.pdb names a USER copy ctor `??0COpinionTransientOffset@@QAE@ABV0@@Z` (Ego_r 0x0045DA29). New opt-in
  `FABLE_RETAIL_COPINIONTRANSIENTOFFSET_COPY` declares it (member-wise init list), and the row spells
  `GetDefaultVal<COpinionTransientOffset>() { COpinionTransientOffset value; return value; }` (no NRVO, the same form as
  GetDefaultVal<CCharString>). The hoisted default is then a copy of the body-order-constructed local, which VC7.1 constant-propagates to
  one `xor eax,eax` plus six memory-order stores (retail 0x00684A46 / 0x00684F37), while the load loop's own `offset` local keeps the
  body-order default ctor (0x00684F5E..0x00684F7A). p1 (copy ctor but `return COpinionTransientOffset();`): RVO, no copy, still 4 lines.
  Chunk 824's po18 (the named-local GetDefaultVal without a user copy ctor): 125 lines.
- Relocations: every symbol binds one address. NAME_FIXES.tsv: 0x00410BE0 (three wrong bound spellings), plus chunk 824's 0x00685F80 /
  0x004793F8 lines carried over.
- Label: ghidra_out / manifest correct; only the q.py catalog's `COpinionReactionManager::TransferOpinionData` is stale (LABELS.tsv).

## 0x007C4430 CTCHealLife::StartHealingEffect -- parked DIFFER(1980v1952), 222 lines (chunk 825's source unchanged)
- Residue: the loop's `HealthIncreaseAccum.push_back(0.0f)` expands where retail calls the folded 4-byte push_back 0x00568850. The
  switch-case CCharString temporaries (case 1 / case 2 slots swapped), the GetTC key and the effect / effects slots differ in every form.
- 10 spellings (a1..a10: plain-if loop body, soundPlayer/switch block, declaration orders, loop-local events pointer, radius order, self-cast
  test, default: break) -- 222..364, none closer. Chunk 825's foldable `if (!target) continue;` (a2) outlines the site and reaches 1952
  bytes, but moves this ebp->edi (254).
- Diagnostic only, nothing kept: /d2inlS15 1992/236, S10 1980/222, S20 2036/395, T72 1980/222, T60 1972/510, T50 1988/465. No budget
  setting outlines only the loop site, so this is not just a budget problem.
- A per-site out-of-line push_back is blocked: `_M_insert_overflow` is protected, and the PUSHBACK_OUTOFLINE opt-in moves both sites.

## 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering -- parked DIFFER(2296v2300), 608 lines (was 648; 684 on today's headers)
- Kept e1:
  - The line block (lineColour, linePos, the call) in its own `{}`, and the name-text block unscoped. This is the reverse of chunk
    801's choice, and it lets the colour temporaries share retail's slot.
  - The line top passed as an explicit `C3DVector(linePos.X, linePos.Y, linePos.Z + height)` in the CEnginePrimitiveLine
    arguments. This gives retail's interleaved X / Y integer moves.
- Residue: info.Engine is cached in ebp, where retail keeps info in ebp and re-reads [ebp+34h] at each call, with ebx for the vtable;
  ours keeps bl = 0xff. Also a one-dword frame-slot shift (the flag at retail [esp+48h], ours [esp+44h]).
- Attempts:
  - Flags /Oa 671 and /Ow 684: neither is retail.
  - Named back() reference 688, named unique id 684, engine local 684, named topZ 684.
  - c4 (three PeekPos() calls): 598, but it changes semantics. Diagnostic only, not kept.
  - d1 638, d2 616, e2 / e3 752.

## 0x0094AB00 CAIStateGroup_GoToPub::Update -- parked DIFFER(2018v2067), 246 lines (was 743)
- Kept k1:
  - The row-local `template<class T> __forceinline void ListPushBackInline(std::list<T>&, const T&)` (STLport insert-at-end body:
    malloc the node, placement copy, link before the end node) at the one tables push_back. Precedent: 0x008DDAD0 push_back_inline.
  - A `__forceinline` member template `GetTCLowerBoundInline(ETCInterfaceType, T**)` on the row-local
    `class CThingObject : public CThing`, holding the expanded CVectorMap::LowerBound halving loop. It is used only at the
    table-choice-loop TCI_TAVERN_TABLE lookup; the other eight lookups call 0x0040F020, as retail does.
  - The shared engine/CIntelligentPointer.h replaces the local pointer model (FABLE_RETAIL_CINTELLIGENTPOINTER + _ASSIGN, CThing
    retail branch; `.Get()`).
- Residue:
  - The frame is 0x188, retail 0x190: retail gives each CIntelligentPointer temporary its own slot (0x1c / 0x30 / 0x24) where ours
    share [esp+10h].
  - The tables-node spill is missing.
  - The GFRandom result is in edx (retail eax).
- Attempts:
  - h1 416, h2 369 (the first h2 failed to compile, C2248 protected Components; fixed by making the helper a CThingObject member),
    h3 265.
  - i1..i3 (explicit pointer temporaries) 265; j1..j3 (plain-inline local pointer ctor/operator=) 265; k1 246.
- Note: the forceinline helpers are the per-site lever the brief allows (chunk 801 precedent). If the coordinator judges them
  "invented local copies", the h-less form is chunk 800's 743-line source.

## 0x0066CF00 CRegionMinimap::PopulateRegionMinimap -- parked DIFFER(3236v3248), 659 lines (was 669)
- Kept l7 = chunk 802's source plus the new opt-in `FABLE_RETAIL_CDEFPOINTER_ASSIGN_REREAD` (hdr/engine/CDefPointer.h). This is the
  template operator=<U> in the reread form; retail re-reads the source at 0x0066D03C after the Release. It fixes the inline
  GetDef<CEngineThemeDef> block shape.
- Residue:
  - x (retail ebx) and the theme def pointer (retail ebp) swap registers.
  - The frame is 8 bytes larger (0x1ec v 0x1e4), and the slot layout differs.
  - GetBoundingBoxCentre 0x00A11AE0 still has no relocation binding (no landed row binds it).
- Attempts: l1..l10. Six of them tie at 659 (l1, l2, l4, l5, l8, l9). l3 3208/671, l6 3252/679, l10 698.

## 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams -- parked DIFFER(2005v2073), 481 lines (was 643)
- Kept n4:
  - One `do { } while (0)` round the sky-beam block (line-4 `// Inline-split:` note).
  - A visible row-local `virtual ~CParticleUpdateParameters() { }` body.
  - With these the overflow copy loops and the dtor expand in the sky block as retail does, and the call sequence equals retail's
    except for one call: the sky block's CVectorMap::LowerBound is called, where retail expands it.
- Header debt (not shipped): an opt-in `FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND` in engine/CVectorMap.h makes the non-const
  `LowerBound` `inline` instead of `__declspec(noinline)`. With it, n2 reaches 2053v2073 / 472 lines, but it also expands LowerBound in
  beam loop 2, where retail calls 0x0040F020, so it is further from retail's shape and not kept. The overlay copy was deleted.
- Attempts: m1 1834/591, m2 643, m3 1932/514, m4 700, n1 1957/597, n2 2053/472, n3 (= n2), n4 2005/481, n5 2069/529, n6 2021/522.
- Next: the per-site `__forceinline` lookup helper from 0x0094AB00 applied to the sky block's GetTC.

## Header debt / checks
- hdr/ edits are opt-in only. The default paths were diffed against rebuild/include: textually unchanged, so no includer recheck.
- Retail-asm check: no `__asm` / `naked` / `_emit` / `volatile` in src/, test/ or differ/. No `#pragma inline_depth`, and no `/d2`
  flag in any kept source.
- vland (read-only, final overlay): 0x00684610 RELOCATION_MATCH PASS. Its RELOC_NAME warnings show two stale labels, now in LABELS.tsv
  and NAME_FIXES.tsv:
  - 0x00681BA0 is CTCCreatureOpinionOfHero::SerialiseFlagMask; it is landed under the autoname CParticleMorphs_TransferEntry.
  - 0x00569FE0 is CPersistContext::TransferVector<float>; it is labelled Transfer<CQuestScoreRecord>, possibly a folded COMDAT.
- The other two warnings are cosmetic label spellings: 0x0099E4C0 operator const char*, and 0x004046B0, where the
  `_List_base` dtor binds the label `list::~list`.
