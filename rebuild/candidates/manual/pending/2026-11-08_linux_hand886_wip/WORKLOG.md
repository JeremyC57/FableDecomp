# WORKLOG 0x0041E6D3 -- NUISystem::CManager::Action  (agent886, 2026-11-08)

## Session 7 (2026-11-08) -- READ THIS FIRST: Codex's fixes merged one at a time (src/ = v28; v23 kept in t7/v23_src_backup.cpp)
State: **v28 DIFFER(35702v35651), frame 0x800 = retail**; code byte-identical to Codex's checkpoint (t7/odiff.py v28 v.
cmp2/codex_action.obj: no hunk). Metrics (t7/m3.py, code to `ret 4`): exact 79.3% / regs-masked 83.6% / regs+frame-masked 98.6%
(v23: 87.0 / 91.5 / 98.4); cmp.py difflib (frame masked) 93.0% (v23 92.7); t5/swap.py noframe 91.5% (v23 91.3).
Each fix graded alone on top of the previous (ATTEMPTS v24..v28, TRACE.md has tool + evidence per step):
- a (v24) FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR (hdr/engine/CCountedPointer.h overlay, hdr/README.md): frame 0x808 ->
  0x800, LOAD_PHOTO texture now shares retail's -268h and holds 8 in ebx (= Codex's "f": no separate source change).
  Exact share fell 87.0 -> 72.7 while regs+frame stayed 98.4: the regime-1 frame ORDER (density sort, ties scrambled) reshuffled;
  the bottom region -78Ch..-6A4h became identical to retail (v23 was +8 there). Kept: frame size is retail's, shapes unchanged.
- b (v25) `CTCInventoryTrade* inventory = PInventoryTrade;` inside the 4 successful-FindChild arms (2008/2010; retail
  `mov ebx,[ebx+54h]` once per arm 0x004254A9): +12 instructions.
- c (v26) `CActionParamSLONG* count` (BUY_BULK / SELL_BULK): `push dword [eax+8]` as retail (0x0041F5D8), 2 sites.
- d (v27) `NumberAugmentations <= 0` (unsigned): `ja` as retail instead of `jne`.
- e (v28) PHOTO_CAPTION `CWideString text = caption->GetText(); OnCaptionEntered(text);`: retail's `lea eax,[text]` re-lea
  (0x00426809); the extra named symbol reshuffled 334 frame offsets and exact rose 72.8 -> 79.3.
None hurt the v23 per-case shapes (regs+frame-masked share rose monotonically 98.4 -> 98.6).
Frame-slot order is now the biggest exact-share lever (exact 79.3 v regs-masked 83.6 v r+f 98.6): t7/fmap.py <obj> pairs our
[ebp+X] with retail's from the masked-equal blocks: 47 of 284 slots identical (v23: 101). Retail's order follows the exact
symbol set (session 3: refs/size ascending from the bottom, ties scrambled), so every added/removed named object moves it.
Tools (t7/): m3.py <obj> (three shares + frame), fmap.py <obj> (slot pairing + summary), odiff.py <a.obj> <b.obj> (our-v-our
instruction diff; swap.py --show missed the BUY_BULK change, odiff did not).

## Session 5 (2026-11-08) -- READ THIS FIRST (parked at v23 = src/ = differ/ minus the 6 Grade lines; t5/v18.cpp, t5/v22.cpp kept)
State: **v23 DIFFER(35697v35651)**, difflib 92.7% of retail instructions (9277/10004; v17/v18 were 92.0%), loose per-case
96.6% (t4/pc.py), frame 0x808 v retail 0x800. Byte count is now ABOVE retail because the 13 bool-setter memory homes (+5 bytes
each) are retail's too; do not read 35632 (v22) as "closer".

Fixed this session (all retail-evidenced, ATTEMPTS.log v19..v23):
- POP_MAP_STATE: `std::map<unsigned long, CUIState>::iterator it;` declared, then `it = map->GetStates().find(state)`; the
  insert loop reuses `it` = retail find temp [-628h] copied into the [-250h] slot; the pair is a named `value_type entry(..)`.
  `iterator it = find()` (v19) shrinks the frame to 0x7FC.
- 2020/2022: `CComponent* list; if (PInventoryTrade->InventoryTradeOpen) list = FindChild("PC_..."); else list = FindChild("UI_...");`
- ASSIGN_SPELL_TO_ITEM_SLOT_PC: `reverse_iterator it = UIActionParams.rbegin(); ++it; ++it;` (home store of the list header).
- CHANGE_CHILD_STATE: `reverse_iterator component = rbegin(); for (i = depth->Value; i > 0; --i) ++component;`.
- 13 bool setters (SET_HEADPHONES .. SET_SHOW_TARGETING_STATUS): `bool enabled = ..->Value > 0; Get().SetX(enabled, false);`
  gives retail's `seta al; mov byte [ebp+7Ch],al; push dword [ebp+7Ch]` (0x0041F245). A named bool local in this regime-1
  function lives in memory; an argument expression stays in eax (`push eax`).
- Tried, no change: SCROLL_* with the C2DVector built in the call (retail fills the delta, then loads GPreviousSelected in the
  shared tail 0x422F30; ours still hoists the GPreviousSelected load above the delta stores).

Entry register roles (the first difference, instruction 6) -- what c2 does (probe/p5/v18.trace via c2prio --trace):
- The CAction* is NOT `action` itself but CSE temp #1395 of `action.Ptr` (kind 3) spanning the switch head and each case's first
  use; killed by stores through pointers (reverse_iterator ++, list walks), not by calls; then rematerialised from [ebp+7Ch].
- Colouring order = priority desc (prio = sum over referencing blocks of w*K*cost minus w*K per live-through block): temp #1395
  prio 1015 is coloured BEFORE game #102 prio 571, so it takes esi and game gets edi. Retail needs game first, or an esi cost on the
  temp. Chooser: cost array 0x10880680 filled in 0x107833F2: an uncoloured neighbour with a copy-relation list (+0x38) adds weight
  to its preferred regs (0x107AA2BD / 0x107AC65E); an uncoloured neighbour with exactly one allowed reg adds spill*100
  (0x10784B8F..0x10784B57); own relations subtract weight (0x1078C062); cheapest allowed reg in table order eax,ecx,edx,esi,edi,
  ebx,ebp wins. Reference counts of both match retail almost exactly, so the cause is elsewhere: candidates are the K / live-
  through sets (bool locals in memory -- v23 changed nothing here; CSE temps such as PInventoryTrade in 2008/2010 (retail ebx);
  const 8 in LOAD_PHOTO (retail ebx)), and [ebp+7Ch] homes (v23: 258 ours v 257 retail; per case t5/percnt.py).

Open, per case (t5/percnt.py `ebp \+ 0x7c\]` and t5/swap.py --show <caseaddr>):
1. Cross-jump direction: retail keeps the GetTC + AddCloseInventoryEvent tail in CLOSE_TELEPORT_MENU (0x41F792) and
   CLOSE_MENU / OPEN_ABILITIES / CLOSE_ABILITIES jump to it; ours keeps the earlier copies. EXIT_LIVE_GUI_IF_LEAF -> 0x41F849.
2. CHOOSE_CLAN: retail `*action->UIActionParams.begin()` with a begin temp at [-678h] and the param in esi (ours 3 [7Ch] uses v 1).
3. LOAD_PHOTO: photoTexture homed in [7Ch], const 8 in ebx (ours neither).
4. ASSIGN_SPELL_TO_ITEM_SLOT_PC 6 v 4 and 2014 7 v 6 [7Ch] uses.
5. Frame 0x808 v 0x800 since v21 (both reverse_iterator homes are retail's): 8 bytes of slots elsewhere (t5/fhist.py).
6. SCROLL_* shared ChangeTargetPositionDelta tail; CWideString == non-null block placement in 2006/2013.

Tools (t5/): swap.py (`SWAP=0 NOFRAME=1 python3 t5/swap.py <obj> [--show caseaddr]`, per-case touched counts), percnt.py
(`percnt.py <obj> '<regex>'` per-case retail v ours counts), fhist.py (frame offset histogram), regcnt.py, roleuse.py,
blk.py (c2 block dump filter), backj.py, loops.py. gr19..gr23.txt are the grade outputs, v19..v23.obj the objects.
Always run t2/pp.sh on the source before gr.sh.

## Session 4 (2026-11-08) (parked at v17; superseded by Session 5)
- Working copy t4/w.cpp (src/ still v14). Fixes so far (no grade yet; objs from the c2 trace runs score loose 94.2%):
  * MISSING CASE found: UI_ACTION_TYPE_RESET_ASSIGNED_SPELLS (211, retail 0x00420ACF): hero GetTC(TCI_HERO_ABILITIES) ->
    CTCInventoryAbilities::ResetAssignedAbilities() 0x005F694D (Ego_r + FableWin ?ResetAssignedAbilities@CTCInventoryAbilities@@QAEXXZ).
  * CTCHero: shared engine/CTCHero.h (FABLE_RETAIL_CTCHERO_TITLE) `hero->WeaponsUsable` (+0x0C; the local model put it at +0x10;
    `AreWeaponsUsable` was an invented name, no PDB entry). CTCQuestCompletionUI Active +0x0C / Quitting +0x1C (were +0x10/+0x20).
  * SEND_OWNED_EVENT, SEND_OWNED_EVENT_FORCE_OBSERVATION, ADD_OBSERVER, REMOVE_OBSERVER, MOVE_COMPONENT, SEND_EVENT read
    action->UIActionParams (retail `lea esi,[edi+4]`, edi = CAction*), not this->UIActionParams (+0x60).
- c2 inline decision instrumented (probe/inl/: inl.py gdb hooks, run.sh <src> <tag> [flags] -> <tag>.json/.obj, an2.py):
  regime 1 (global optimizer off) takes 0x107537a4: cost = sz - sav0 - sav1 + pen - 10 (leaf callee) - 7 (user inline)
  + 4 (site flag 2); accept iff cost < S (9). pen = (sz/2) * N / 9000 when sz > 9 and N > 9000, N = caller IL so far
  ([0x10882b58], 11998 at Action entry, 28462 at its end) + a callee number. Tools: t4/c2dis.py, t4/c2xref.py.
- v15 graded DIFFER(35312v35651), loose 94.2% (first diff instruction 2 = frame 0x804 v 0x800).
- Inline split, what the trace shows (probe/inl/w3.json, an2.py; t4/ocalls.py <obj> [regex] counts our call targets):
  * No uniform N explains retail. Retail rejects `_List_iterator_base::!=` in AUGMENT (2nd case, our N ~12700, cost 2 -- it
    would need N >= 19000) yet inlines `_Rb_tree_base_iterator::!=` (same body, sz 18) in POP_MAP_STATE / 2014 / PLAY_SOUND
    at N 19000..28000 (cost 9..18 in ours); rejects begin (sz 13, cost 4..6 in ours) from case 2005 on but inlines the
    C2DVector(float,float) ctor (sz 18, cost 9..13 in ours) in SCROLL_ACTIVE_LIST_UP/DOWN and SCROLL_LIST_UP/DOWN, the
    C2DExtentsI(long,long) ctor and ~CSurface in LOAD_PHOTO, map<CCharString,K>::begin in PLAY_SOUND, and
    CWideString::operator==(const wchar_t*) (sz 42, cost 88: forceinline in retail, as in chunk 784).
  * Padding probes (probe/inl/pad*.cpp, +6300..7000 IL tuples at entry) reproduce the setters / vector[] / != out-of-line set
    but call begin at 23..27 sites (retail 16), list<CList*>::empty x2 and a pair ctor that retail inlines, and make the
    expanded-in-retail sites worse. /Z7 changes nothing. So per-site devices (EXTRA_RULES owner decision 2026-11-08).
  * Retail out-of-line sites: begin 0x429A85 in 2005..2014 (0x424CA0 0x424E7B 0x424F75 0x42545A 0x4255E0 0x425AC6 0x425C4C
    0x426116 0x426228 0x426315), front()'s begin in 2019..2023 (0x425357 0x42552C 0x4259C3 0x425B98 0x425FC1), CHOOSE_CLAN
    0x426CD4; vector<CCharString>[] 0x4263D2 (2014); vector<long>[] 0x4242F5 0x42488D 0x4248A8 (2004); setters 0x4224B3
    0x422547 0x422928/38/45 0x422A62/72/7F 0x422B0D; list != 0x41EAA3 0x41EBD6 (AUGMENT) 0x420CED (CHANGE_ALPHA);
    postfix -- 0x41F4D8 0x41F504 (ADD_CHILD). LOAD_ITEM_DESC / 2004 / UNLOAD_ITEM_DESC / LOAD_PHOTO / PLAY_SOUND begin inline.
- v16 (t4/v16.cpp): `#pragma inline_depth(0)` ... `#pragma inline_depth()` around each of those statements except the three
  list != (end() is in the same loop condition; needs another spelling), list reference hoisted (`params`) so
  CCountedPointer::operator-> stays inline; front() -> `*params.begin()`; FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ.
  v16 graded DIFFER(35551v35651), pc.py 95.7%; frame 0x808 (an expanded callee's temp still missing).
- Correction: map `!=` is NOT expanded in retail -- it calls 0x0041C830 six times (0x420DBB 0x420E0B 0x420EAA 0x42641A
  0x426B10 0x426B57) exactly like ours; only the LIST != (0x41C7C0) differed.
- v17 (= src/ and differ/ now): per-TU opt-ins for callees retail treats the same way at every site of Action (hdr/README.md):
  CWideStringData text == forceinline (with the existing CWideString one), C2DVector(float,float), C2DExtentsI(long,long),
  ~CSurface, map/_Rb_tree::begin forceinline; _List_iterator_base::!= __declspec(noinline) (#ifdef _MSC_VER). An inline_depth
  region is captured per full-expression, so `it != c.end()` cannot be split by a region (probe/mini/m2.cpp), and a
  forceinline inside a region is still called (m1.cpp). Graded DIFFER(35593v35651): FRAME 0x800 = retail; first difference
  at instruction 6 (entry registers); pc.py 96.1%; calls 98.4% (t4/calls.py --count).
- Remaining (t4/pc.py, largest first): 2004 (27 touched; register/slot choices), POP_MAP_STATE (26), ASSIGN_SPELL_TO_ITEM_SLOT_PC
  (25), the cross-jump tails CLOSE_TELEPORT_MENU / CLOSE_MENU / OPEN_ABILITIES / CLOSE_ABILITIES (~80: retail CLOSE_MENU jumps
  into CLOSE_TELEPORT_MENU's AddCloseInventoryEvent tail 0x41F79F), 2006 / 2013 (expanded CWideString text ==: retail places
  the non-null `std::operator==` block after the case and jumps back; ours keeps it in line), SCROLL_* (retail SCROLL_LIST_UP/
  DOWN and SCROLL_ACTIVE_LIST_UP jump into SCROLL_ACTIVE_LIST_DOWN's call at 0x422F30/0x422F3A), LOAD_PHOTO (ebx = 8 kept for
  the CPixelFormat pushes), GetTC expansions 75 v 73, SetStaticCast 10 v 9, GetDef 3 v 2, 0x40D2F0 2 v 1.
- Next: (1) entry register roles: retail `push edi ; mov edi,[ebp+7Ch]` (edi = the CCountedPointer&, then CAction*), esi = game;
  ours swaps them -- try declaring `game` before the IsValid test or reading `action` into a CAction* local; (2) the GetTC
  surplus (find the two cases where retail calls GetTC fewer times: compare per case with t4/calls.py); (3) cross-jump tails.
- v18 (game declared before the IsValid test): no change. The call-sequence diff of v18 (probe/calls18.txt, t4/calls.py
  --ctx 0) shows every remaining call difference is TAIL MERGING (cross-jumping), not source: retail's OPEN_ABILITIES /
  CLOSE_ABILITIES / CLOSE_MENU jump into one shared GetTC + AddCloseInventoryEvent tail (hence GetTC 73 v our 75),
  EXIT_LIVE_GUI_IF_LEAF jumps to the ToggleLiveGui call at 0x41F849 (0x42161D), SCROLL_LIST_UP/DOWN / SCROLL_ACTIVE_LIST_UP
  share SCROLL_ACTIVE_LIST_DOWN's ChangeTargetPositionDelta call, while ours merges ASSIGN_SPELL_TO_ITEM_SLOT_PC's
  AssignThingPC / AssignThingSwappingPC calls (retail keeps them) and the PLAY_SOUND / 2013 tails differently. The tails differ
  in their registers (esi/edi swapped from the entry on), so the entry register roles are the likely root of most of this.
- Everything else matches in the call sequence except names the binding table does not know (GFStringsAreEqual = 0x401490,
  std::operator== = 0x404280, CPixelFormat(JJJJ) = 0x41C850, ...).

## Session 3 (2026-11-08) -- READ THIS FIRST. State: **v14 DIFFER 35224v35651** (src/ = differ/ minus residue lines; t3/src_v12.cpp, t3/src_v13.cpp kept)
- **c2 internals found (probe/c2/, gdb on a patched c2.dll copy via tools/.../c2probe/c2dbg.py; c2 = 13.10.3077, base 0x10700000):**
  * 0x1077bdd8 counts the function's IL tuples at global-optimizer entry; > 10000 (`cmp [esp+0Ch],2710h` at 0x1077be49)
    sets 0x10882b98=1 / 0x108a91fc=0 = "global optimizer off" (call it REGIME 1). Action counts **14593** (aux 923).
    The same function also counts "aux" (kind 0x17, opcode 0x2ca); the caller sets the index limit 0x10882bc0 = 3*aux+5.
  * frame packing = 0x107855af: gate `cmp esi,ebx` at 0x10785605 (A=packable symbols 0x108a78a0, B=0x108a789c) runs the
    packer 0x10790ad6 (liveness setup 0x107903e3, liveness/alias walk 0x107909b1), which writes the shared offset into
    symbol+0x18 at 0x10790eb5 (unpacked layout first written at 0x1076ee1c). Symbol record: name [sym+4], offset +0x18,
    size +0x1c. Listing printer hook 0x1082bf47 (edi=symbol). Overflow/remap list in 0x1076567d (called 0x10764049):
    at 0x10765b41 ebp = overflow entries; 0 -> 10-entry remap table 0x107505a9, >0 -> table sized from ebp, and the packer
    then sees half-function lifetimes (groups of <= 2).
  * Measured (probe/c2/nl.sh <src> <tag>; NOLIMIT=1 patches the cmp):
    full src 14593 tuples, regime 1, A=549, ovf 0: scalars and NON-address-exposed objects pack (gfull.slots: 400 named
    syms in 157 slots; e.g. all 5 `CCountedPointer<CText> titleText` share -624, 52 `_type` keys share the dead `action`
    home [ebp+7Ch]); address-exposed objects never pack (25 `_deletion`, 17 `_child`, 17 `_event`, 16 `_deleteAction`).
    full src NOLIMIT: ovf 12, deletions still unpacked. cut 0..130 (7875, ovf 0) packs everything (frame 0xC8);
    cut 0..131 (7945, ovf 58) packs nothing.
  * 600-case deletion probe (probe/s3/n600.cpp): 16206 tuples -> 600 deletions in 600 slots; with NOLIMIT -> 1 slot.
- **KEY RESULT: retail is REGIME 1 (> 10000 tuples), not a packed regime-3 build.** probe/c2/work/regime.py <obj> 4224c3
  (loose/strict instruction match of retail cases 0..129): v12 full build 94.5% / 70.1%; NOLIMIT full 91.4% / 51.7%;
  cut130 (global optimizer on) 70.3% / 26.9% (CSE `[esi+4]`, dropped home stores, cross-jumped CONSTRUCT_* tails that
  retail keeps per case at distinct offsets [ebp-4D8h]/[ebp-5ECh]). So cutting Action below 10000 tuples would be WRONG.
  Retail's frame is regime 1's too: 0x8DC-0x800 = 220 ~= 26 x 8 bytes = exactly the unshared deletion objects. Retail
  puts every deletion (and the trade cases' CCountedPointer<CText> locals) at [ebp-248h], inside the regime-1 packed-scalar
  area (ours: -250h/-240h/-23Ch) => in retail the deletion is a TRACKED (not address-exposed) symbol. The open question
  is which retail spelling of the deletion cases leaves the object un-exposed while still calling ctor 0x42BED4, copy
  ctor 0x42CD84 (by-value SetDeletion arg) and member dtor 0x42ABCA out of line. probe/s4/gv.py + runv.sh test spellings
  in a 450-case regime-1 probe.
- **SOLVED the frame blocker (v13):** the deletion is address-exposed in ours only because the overlay opt-in
  FABLE_RETAIL_CDELETION declares CDeletion's ctor/copy ctor as user-declared externals. In retail they are the IMPLICIT
  members (0x42BED4 = `push ecx; ...; call list ctor 0x42ABAF`, 0x42CD84 = memberwise copy; same-TU COMDATs called out of
  line). probe/s4: in a 450-case regime-1 switch the implicit spelling packs 450 deletions into 1 slot with retail's exact
  shape (ctor call, `mov [d],method`, jmp shared tail `mov edi,[esi]; push ecx; push ecx; lea eax,[d]; mov ecx,esp; push eax;
  call copyctor; call [edi+X]; lea ecx,[d+4]; call ~list`), while user-declared (external or same-TU-defined), force-inlined
  helper and by-ref variants keep 450 slots. src v13 = v12 minus `#define FABLE_RETAIL_CDELETION` (no header change):
  frame 0x804 (retail 0x800), every deletion at [ebp-248h] (t3/slotuse.py 0x248 now equal per case), loose 93.8%
  (v12 93.5%), strict 69.0% (v12 69.6%: slots shifted), ours now 430 bytes SHORTER (tails cross-jump; 9869 v 9948 insns).
- **Remaining residue (v13):**
  1. Frame order. Regime-1 layout = unpacked symbols sorted by reference density (refs/size) ascending from the frame
     bottom (14-byte CEngineGraphic temps and the 40-byte `$T` deepest, then 4-byte `$T`/8-byte deltas, `_child` x17 ...,
     the 17 40-byte `_event`s at the top -568..+72), ties scrambled. Same slot SET as retail in the event region, permuted;
     bottom region shifted -4: ours has ONE extra 4-byte unpacked symbol between -1700 and -600 (or retail packs one more).
     Retail also reuses the dead `action` home [ebp+7Ch] 257 times vs ours 186 (t3/offs2.py; t3/shift.py maps retail ->
     ours offsets through the alignment).
  2. Inline split (unchanged from session 2): retail calls postfix `--` 0x42899B (ADD_CHILD 0x41F4D8: `it = end(); it--;`
     with the result temp at [ebp-258h]), begin() 0x429A85 late; ours inlines. These calls also add retail's `$T` symbols.
  3. Block placement: the GetTC(0x6F) CTCInventoryAbilities block lives in UNASSIGN_SPELL in retail (others jump to it);
     ours places it in CLOSE_MENU / OPEN_ABILITIES / CLOSE_ABILITIES (percase.py nets -29 / +21 / +19 / +19).
  4. Entry registers swapped (retail edi=action, esi=game).
- v14: the three `FindChild(InventoryTradeOpen ? "PC_SUBTITLE_INDEPENDANT" : "UI_TEXT_TRADE_NAME_TEMPLATE_NEW")` became
  `if (...) title = FindChild("PC_..."); else title = FindChild("UI_...");` (GOTCHAS 762: retail's push-per-arm is c2
  cross-jumping two whole calls) -> those hunks now differ only by relocations; loose 93.9%, strict 69.1%.
- Inline-cap flags on v14 source (probe/s5/flag.sh / fl2.sh; nl.sh build + regime.py): /d2inlS10, S13, S15, S16 all give
  frame 0x800 (default S14 gives 0x804) but none reproduces a single retail begin()/postfix-- call; S13/S10/S15 inline
  slightly MORE (calls 1569/1572/1567 v 1576), S16 far more (count 15802, loose 91.4%). /d2inlT20/T40 change nothing.
  Not adopted. Working theory: c2's per-function inline growth budget scales with the caller's pre-inline IL; ours inlines
  every begin()/postfix--/setter site, retail runs out from case 2005 on, so ours is either bigger before inlining or its
  expansions are cheaper. Retail's calls (t3/percase.py alignment): begin 0x429A85 x16 (2005+), map != 0x41C7C0 x3
  (AUGMENT, CHANGE_ALPHA), setters 0x41C965/0x41CA3E/0x41C972/0x41C994/0x41CA31/0x41CA52 (SET_*_TO_ASSIGN*), operator[]
  0x40F220 x3 (2004), postfix -- 0x42899B x2 (ADD_CHILD), operator== 0x404280 x2 (2006, 2013).
- Address-exposure detail: retail keeps ADD_CHILD's `it` in the dead `action` home [ebp+7Ch] although `&it` goes to the
  out-of-line postfix -- 0x42899B: a call to an INLINE (same-TU, header-defined) function does not expose the object; a call to
  a user-declared external or non-inline same-TU function does (probe/s4 HDR=vis keeps 450 slots).
- tools: probe/c2/nl.sh (nl4.sh also finds `?f@M@@`), cnt_host.py (PACK=1 packer groups, WALKMIN histogram), addk.py/
  bisk.sh, probe/c2/work/regime.py + regcase.py (retail-prefix match per obj), probe/s4/gv.py (spelling variants).

## Session 2 (2026-11-08) -- READ THIS FIRST; it supersedes "State", "MISSING" and "Next step" below
- **State: v12 DIFFER 35762v35651** (ours 111 bytes long; loose per-case match 93.5% = 9302/9948 retail insns, ours 10019).
  All 213 cases are written in retail address order (2000-2023 from t3/b9..b13.cpp, 3000-4002 from t3/b14/b15.cpp;
  2009/2021/2010/2022 generated by t3/gen13.py). src/0041e6d3.cpp = best; differ/0041e6d3.cpp = same + residue lines;
  t3/src_v8.cpp = the session-1 source. Grades v9..v12 in ATTEMPTS.log.
- Fixed this session: TU pragma is `#pragma optimize("s", on)` ONLY (dropping `optimize("y", off)` gives retail's biased
  `lea ebp,[esp-74h]` frame); CEngineGraphic has its two PDB ctors (non-POD: GetEngineGraphic's hidden pointer goes in ecx and
  callers read the temporary in place, +1.5%); entry is `if (!action->IsValid()) return; game = Get(); if (!game) {...; game =
  Get();}` (overlay CAction::IsValid -> retail `mov al,[eax+0Ch]; test al,al`); 0x5BE8D3 = CTCInventoryBase::GetPerItemDataForItem.
- **THE BIG RESIDUE: frame 0x8DC vs retail 0x800 = VC7.1 frame-slot packing is OFF in ours.** Retail packs disjoint-scope object
  locals into shared slots (every `CDeletion deletion` and the 8-byte CCountedPointer<CText> locals of the trade cases share
  [ebp-248h]); ours gives every object local and temporary its own slot (`/FAs` listing: 43 `_deletion$` symbols in ~40 slots).
  Proven with probes (probe/pk/, all compiled with the row's flags /O2 /Oy /GS + optimize("s")):
  * a probe switch with N cases of `{ C* d = p->FindChild(i); if (d) { CDeletion deletion; deletion.Method = 1;
    d->SetDeletion(deletion); } break; }` packs every `deletion` into ONE slot up to N=360 and packs NONE at N=380/400/600;
    250 cases with 4 extra pointer locals kept live across calls (spilled) also packs none, 270 with 1 extra (enregistered)
    packs -> c2 drops packing above a count of frame-resident symbols (not lines, not blocks, not case count).
  * our src bisected with probe/pk/cut2.py + run2.sh (keep case ranges): cases 0..129 pack, 0..130 do not (case 130 =
    SET_EXPRESSION_ITEM_TO_ASSIGN_PC, the very case where retail stops inlining SetThingToAssign: inline 0x0042247E, called
    0x00422547); 106..211 alone pack. So ours crosses the limit about 60% of the way through; retail, with all 213 cases,
    stays under it -> retail's source has far fewer frame symbols (named object locals + temporaries + inline-expansion temps).
  * `/d2inlS8` and `/d2inlS5` do NOT restore packing (probe/pk/runf.sh; S5 grows the frame to 0x8F8).
  Next: find which of our symbols retail does not have. Candidates: (a) inline expansions that create temps per site
  (GetTC expanded, list begin/!=/++ which retail CALLS late: 0x429A85 / 0x41C830 / 0x429CEA; GFStringsAreEqual 0x401490);
  (b) named object locals where retail uses a temporary or vice versa; (c) by-value CCharString/CCountedPointer temporaries.
  Method: compile with runf.sh (full src, any flags) and count `= -N ; size` symbols before `?Action@...PROC` in f_<tag>.asm;
  retail-shaped edits that cut the count should flip packing (check `_deletion$` slots) and shrink the frame toward 0x800.
- Our frame-symbol table (probe/pk/allslots.txt, from probe/pk/full.asm = v12 src with /FAs): 545 symbols in 265 slots; by name
  `_type` 75 (the expanded GetTC key, all packed at -592 already), `$T` temporaries 147, `_deletion` 25 (+ arrowDeletion),
  `_screen` 18, `_event` 17 (CGameEvent locals), `_child` 17, `_deleteAction` 16, `_entry` 11. The packing limit counts symbols
  before packing, so trimming any of these (fewer named object locals, fewer by-value temporaries) is the lever; check each edit
  with `probe/pk/runf.sh <tag>` (prints `deletion N in M slots` and Action's `sub esp`) before spending a grade.
- Other per-case residues (`python3 $A/t3/percase.py [--show caseaddr]`; slots per case `t3/slots.py [--show caseaddr]`; who uses
  a slot `t3/slotuse.py 0x248`; deepest offset per case `t3/frame.py`): retail CALLS begin() 0x429A85 in the late trade cases
  (ours inlines); in 2019/2023 the FindChild `PInventoryTrade->InventoryTradeOpen ? "PC_SUBTITLE_INDEPENDANT" :
  "UI_TEXT_TRADE_NAME_TEMPLATE_NEW"` argument is a push per arm in retail (ours: a register select then one push); register
  choice at entry (retail edi = action, esi = game; ours swapped) is global and will follow.
- New overlay entries: fable_string.h FABLE_RETAIL_CWIDESTRING_TEXT_EQ_HELPER, engine/NUISystem_CMenuEntry.h
  FABLE_RETAIL_CMENUENTRY_ACTION, engine/NUISystem_CClickable.h FABLE_RETAIL_CCLICKABLE_SHARED_ACTION,
  engine/NUISystem_CAction.h FABLE_RETAIL_CACTION_ISVALID, engine/NInventory_CTCInventoryBase.h
  FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA, fable_definition_table.h (unchanged copy). See hdr/README.md.
- Session-2 local models (migrate to engine headers after the match): CEngineGraphic (packed, PDB sizeof 14, PDB ctors),
  CThingBaseDef, CThingObjectDef, CAppearanceDef, CWeaponDef, CAugmentationDef, CAnimationEntry/CAnimFilterBase/
  CAnimFilterNoGroup, CAnimationSet::GetAnimation 0x5DC340, CPlayerGuiDef, CTCWeapon, CTCPhotojournalCapture,
  CTCHeroOnlineScoreboard, CASoundSampleOwner_Unproven (slot 9 StopSample), NUISystem::CActionParamCCountedPointer<T>, plus many
  method additions to the session-1 CTCInventory* models.
- No behaviour test yet (test/ empty).

## Identity (done)
- Real name `?Action@CManager@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z`
  (Ego_r.pdb 0x0042FEBC donor 34,588 B; FableWin.pdb 0x01AE7AE2; vtable 0x01230134 slot 14 = header decl order).
- True extent 35,651 = code 0x41E6D3..0x426E0E + 6 jump tables (0x426E0E/58, 0x426EF6/34, 0x426F7E/12, 0x426FAE/107,
  0x42715A/23, 0x4271B6/24) ending at 0x427216 (= CreateTickSlider, landed). The catalog extent is right.
- TU: `#pragma optimize("y",off)` + `("s",on)` like the landed neighbours; `// Flags: /GS` (cookie [0x139c8a8] at [ebp+70h]).
- Prologue `push ebp; lea ebp,[esp-74h]; sub esp,800h`; epilogue 0x426DFC `ret 4`.

## State (v8: DIFFER 20848v35651, difflib 47.4%)
- src/0041e6d3.cpp = best source (also differ/0041e6d3.cpp with the residue lines). Every case of t2/order.txt from WIELD
  (0x41E752) through RESPAWN (0x423245) is written, in retail address order (= source order), from batches t2/b1..b8.cpp.
- MISSING (write next, t2/order.txt lines 157-213):
  * UI_ACTION_ADD_MESH_CHILD .. UI_ACTION_UNLOAD_WANTED_ITEM_DESC (2000-2023), 0x423261..0x426490, ~12.7 KB. These are the
    description loaders (GetDef<T> 0x42AF3C / 0x42B040 / 0x42B144 + GetSubDef 0x42AF9E / 0x42B0A2 / 0x42B1A6 / 0x42B248,
    the same-TU def lookups already modelled under auto_inline(off) at the top of src). 2004 alone is 2,728 bytes.
  * CHEAT_MORALITY (3000) .. EXIT_PHOTOJOURNAL_CAPTURE (4002), 0x426490..0x426DFC, ~3.5 KB (photojournal, edit boxes,
    scoreboard, clans, PLAY_SOUND, CLOSE_BOAST_MENU, ...). EXIT_PHOTOJOURNAL_CAPTURE decoded: hero GetTC(TCI_PHOTOJOURNAL_CAPTURE)
    && capture -> ClosePhotojournalCapture().
  Each missing case currently falls to `default: break;`.
- The first difference is still at instruction 1 (frame 0x800 vs ours; it will only settle once every case and local exists).

## How to work (tools in t2/)
- `python3 $A/t2/lst.py <start> <end>`: condensed retail listing with callee names (names.pkl cache).
  NB it labels `[ebx+..]` as this.X even when ebx is not `this` (e.g. `[this.PInputManager]` after `mov ebx,[this.PPlayer]` is
  CThing+0x20 = the TC bitset). It truncates the address to the low bytes: cut -c3- output loses the 0x41/0x42 prefix.
- Fast template diff for repetitive cases: diff a normalised listing against t2/l_pers.txt (the CONSTRUCT_* template), see
  the shell in this session's history: `sed 's/0x4[0-9a-f]*//g;s/\[-[0-9a-f]*\]/[-X]/g;s/L[0-9a-f]*: //'`.
- `python3 $A/t2/vt.py <Class@Ns>` (K0=first N=last env) dumps a retail vtable found through RTTI.
- t2/pdbnames.txt: demangled FableWin/Ego_r names per callee. ghidra_out/fablewin_pdb_names.tsv for member names.
- ALWAYS run `$A/t2/pp.sh` (g++ -E) before a grade: a fatal cl error (C1189 #error, unbalanced braces C1075/C1004) HANGS
  cl.exe under the wine lane until the timeout (10 min) with no output.
- gr.sh grades with the overlay (~1-2 min).

## Decoding rules learned (apply to the missing cases)
- edi = action.Get() (CAction*), esi = game (CMainGameComponent::Get() at entry), ebx = this at each case entry.
- A home-slot store of a list header (`mov [ebp-6xx],hdr`) followed by `[hdr+4]` reads = `std::list<CActionParam*>::reverse_iterator
  it = list.rbegin(); *it; *++it` (overlay stlport reverse_iterator). A plain `end()` + `*--it` has NO home store.
- `begin()` walks (`mov esi,[hdr]`, `mov esi,[esi]`) = forward iterator `*it, *++it`.
- GetTC: FABLE_RETAIL_CTHING_GETTC_EXPANDED; key stored in [ebp+7Ch] when `action` is dead, else in a local; miss path
  reading an uninitialised local = `T* tc; owner->GetTC(id,&tc); tc->...` (no &&); `xor reg,reg` before = `T* tc = 0;`.
- Hero sources: `mov ecx,[esi+1Ch]; call 0x449970; call 0x487DC0` = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
  `mov eax,[MainGame]` reloaded = CMainGameComponent::Get()->...; `mov ecx,[ebx+58h]` = PPlayer; 0x449960 = PeekPlayerManager() const.
- Shared tails (cross-jumped by the compiler, nothing to write): 0x42143C SILENCE_SOUND_GROUPS, 0x423183 AddGameEvent,
  0x42306F SetEvent, 0x4209D7 SetEvent(UI_EVENT_LEFT), 0x422E4E ~CCountedPointer<CAction>, 0x426205 ~CCountedPointer<CComponent>,
  0x41F849 CPlayerGui ToggleLiveGui, 0x4223E9 the two swapping flags = false, 0x422A7D SetComponentSlotToSwap.

## Open problems (the tuning phase, after every case exists)
1. Inline split: same-TU functions defined BEFORE Action in retail address order (CTCInventory/CTCInventoryAbilities setters
   0x41C965..0x41CA52, CMenuEntry::GSetForceProcessing 0x41CAC8, list<..>::begin 0x429A85, postfix -- 0x42899B, != 0x41C7C0)
   are inlined early and called late (e.g. SetThingToAssign inline at 0x42247E but called at 0x422547). That is VC7.1's
   caller inline budget: model them as same-TU out-of-line bodies (not in-class inline) and try `/d2inlS<N>` / `/d2inlT<N>`
   per EXTRA_RULES. Header-inline callees in other TUs (CTCQuestCompletionUI::IsActive) stay inline everywhere; out-of-TU
   ones (CTCQuickAccessMenu::IsActive 0x625930) are always calls.
2. Prologue: retail `mov al,[eax+0Ch]; test al,al` for `action->Valid` (ours `cmp byte ptr`): check once the body is complete.
3. GetDef<T> T for 0x42AF3C (A) and 0x42B144 (C) unproven (A = CDefClassBase now); byte global 0x0137D76C (GTeleportOpenedFromMap,
   provisional); CMenuEntry::GForceProcessing data name 0x013B8AA8 unproven.
4. Local models still used for CTCInventory*, CTCHero*, CTCQuickAccessMenu, CTCInGameMenu, CPlayerGui, CWorld, CQuestManager,
   CTCBoastUI, CTCGamePlayer, CTCTavernGame, CTCQuestCompletionUI, CFrontEndManager, CMenuEntry, CRedefinerList,
   CActionParam2DVector, CHeroAbilityDef: engine headers exist for most (CTCInventory.h, CTCInventoryStats.h, CTCInventoryMap.h,
   CTCQuickAccessMenu.h, CTCInGameMenu.h, CPlayerGui.h, CWorld.h, CQuestManager.h, CTCBoastUI.h, NUISystem_CRedefinerList.h);
   migrate them once the bytes match.
5. No behaviour test yet (test/ empty): write it after the match.

## Next step
Write the 2000-2023 cases (start with 0x423261 ADD_MESH_CHILD, 600 B), then 3000-4002, grade, then the inline tuning.
