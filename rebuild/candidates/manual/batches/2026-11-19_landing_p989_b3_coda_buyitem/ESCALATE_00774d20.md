# ESCALATE 0x00774D20 CTCShop::BuyItem(long) (hand4d, 2026-11-19)
Coordinator rule (2026-11-19): >= 60 grades and two straight non-improving ideas. This row: 60 grades (49 by the first author, 11 this
session: 1 baseline regrade, 5 include-order variants, 5 declaration / condition-shape probes; none moved the residue). Status
ESCALATED in STATUS.tsv.

## Best source
- `/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand4d/src/00774d20.cpp`, no flags.
  DIFFER(1149v1149), 34 lines differing (sd.py count), every one a register-role / cascade difference; the instruction stream is
  otherwise retail's.
- Identity proven: Ego_r.pdb ?BuyItem@CTCShop@@QAEXJ@Z (tc_shop.obj #41) + FableWin.pdb 0x0261E960; the landed caller
  CTCInventoryTrade::SellHighlightedItem 0x005A3436 binds it (NAME_FIXES: replaces the label CTCShop_SellHighlightedItem).

## Residue (taken from the last probe e3_subdef_nested.cpp, which grades identically to the best source: same 34 lines; retail left, ours right; ** = differs; `call` lines differ only because our object is unrelocated -- the grader masks them)
```
  00774d71  8b4304                 mov eax, dword ptr [ebx + 4]            | mov eax, dword ptr [ebx + 4]
  00774d74  8b4870                 mov ecx, dword ptr [eax + 0x70]         | mov ecx, dword ptr [eax + 0x70]
  00774d77  8d542424               lea edx, [esp + 0x24]                   | lea edx, [esp + 0x24]
**00774d7b  33f6                   xor esi, esi                            | xor edi, edi
  00774d7d  52                     push edx                                | push edx
**00774d7e  89742428               mov dword ptr [esp + 0x28], esi         | mov dword ptr [esp + 0x28], edi
**00774d82  e819e3d9ff             call 0x5130a0                           | call 0x774d87
  00774d87  8d4c2444               lea ecx, [esp + 0x44]                   | lea ecx, [esp + 0x44]
  00774d8b  51                     push ecx                                | push ecx
  00774d8c  8bcd                   mov ecx, ebp                            | mov ecx, ebp
**00774d8e  e8ed340000             call 0x778280                           | call 0x774d93
  00774d93  d900                   fld dword ptr [eax]                     | fld dword ptr [eax]
**00774d95  d805d8de2201           fadd dword ptr [0x122ded8]              | fadd dword ptr [0]
  00774d9b  8d542444               lea edx, [esp + 0x44]                   | lea edx, [esp + 0x44]
  00774d9f  52                     push edx                                | push edx
  00774da0  8bcd                   mov ecx, ebp                            | mov ecx, ebp
  00774da2  d918                   fstp dword ptr [eax]                    | fstp dword ptr [eax]
**00774da4  e8d7340000             call 0x778280                           | call 0x774da9
  00774da9  8b4c2424               mov ecx, dword ptr [esp + 0x24]         | mov ecx, dword ptr [esp + 0x24]
  00774dad  d98184000000           fld dword ptr [ecx + 0x84]              | fld dword ptr [ecx + 0x84]
  00774db3  83c008                 add eax, 8                              | add eax, 8
  00774db6  d800                   fadd dword ptr [eax]                    | fadd dword ptr [eax]
  00774db8  8d542444               lea edx, [esp + 0x44]                   | lea edx, [esp + 0x44]
  00774dbc  52                     push edx                                | push edx
  00774dbd  8bcd                   mov ecx, ebp                            | mov ecx, ebp
  00774dbf  d918                   fstp dword ptr [eax]                    | fstp dword ptr [eax]
**00774dc1  e8ba340000             call 0x778280                           | call 0x774dc6
  00774dc6  d94008                 fld dword ptr [eax + 8]                 | fld dword ptr [eax + 8]
**00774dc9  d81ddcde2201           fcomp dword ptr [0x122dedc]             | fcomp dword ptr [0]
  00774dcf  dfe0                   fnstsw ax                               | fnstsw ax
  00774dd1  f6c405                 test ah, 5                              | test ah, 5
  00774dd4  7a0f                   jp 0x774de5                             | jp 0x774de5
  00774dd6  8d442444               lea eax, [esp + 0x44]                   | lea eax, [esp + 0x44]
  00774dda  50                     push eax                                | push eax
  00774ddb  8bcd                   mov ecx, ebp                            | mov ecx, ebp
**00774ddd  e89e340000             call 0x778280                           | call 0x774de2
**00774de2  897008                 mov dword ptr [eax + 8], esi            | mov dword ptr [eax + 8], edi
  00774de5  8b5304                 mov edx, dword ptr [ebx + 4]            | mov edx, dword ptr [ebx + 4]
  00774de8  0fb7828c000000         movzx eax, word ptr [edx + 0x8c]        | movzx eax, word ptr [edx + 0x8c]
  00774def  8d4c2420               lea ecx, [esp + 0x20]                   | lea ecx, [esp + 0x20]
  00774df3  51                     push ecx                                | push ecx
  00774df4  50                     push eax                                | push eax
  00774df5  8bcb                   mov ecx, ebx                            | mov ecx, ebx
**00774df7  89742428               mov dword ptr [esp + 0x28], esi         | mov dword ptr [esp + 0x28], edi
**00774dfb  e8d01bf1ff             call 0x6869d0                           | call 0x774e00
  00774e00  8bc8                   mov ecx, eax                            | mov ecx, eax
**00774e02  e83561cbff             call 0x42af3c                           | call 0x774e07
**00774e07  8b7c2444               mov edi, dword ptr [esp + 0x44]         | mov esi, dword ptr [esp + 0x44]
**00774e0b  57                     push edi                                | push esi
  00774e0c  8d4c2420               lea ecx, [esp + 0x20]                   | lea ecx, [esp + 0x20]
  00774e10  51                     push ecx                                | push ecx
  00774e11  8bcb                   mov ecx, ebx                            | mov ecx, ebx
**00774e13  89742418               mov dword ptr [esp + 0x18], esi         | mov dword ptr [esp + 0x18], edi
**00774e17  e8b41bf1ff             call 0x6869d0                           | call 0x774e1c
  00774e1c  8bc8                   mov ecx, eax                            | mov ecx, eax
**00774e1e  e8bd8b2300             call 0x9ad9e0                           | call 0x774e23
  00774e23  8b4c2420               mov ecx, dword ptr [esp + 0x20]         | mov ecx, dword ptr [esp + 0x20]
  00774e27  8d542410               lea edx, [esp + 0x10]                   | lea edx, [esp + 0x10]
  00774e2b  52                     push edx                                | push edx
**00774e2c  89742418               mov dword ptr [esp + 0x18], esi         | mov dword ptr [esp + 0x18], edi
**00774e30  e86be2d9ff             call 0x5130a0                           | call 0x774e35
  00774e35  84c0                   test al, al                             | test al, al
**00774e37  7475                   je 0x774eae                             | je 0x774eaf
  00774e39  8b4c241c               mov ecx, dword ptr [esp + 0x1c]         | mov ecx, dword ptr [esp + 0x1c]
  00774e3d  8d442414               lea eax, [esp + 0x14]                   | lea eax, [esp + 0x14]
  00774e41  50                     push eax                                | push eax
**00774e42  e8c5e6e1ff             call 0x59350c                           | call 0x774e47
  00774e47  84c0                   test al, al                             | test al, al
**00774e49  8b6c2410               mov ebp, dword ptr [esp + 0x10]         | mov edi, dword ptr [esp + 0x10]
**00774e4d  7463                   je 0x774eb2                             | je 0x774eb3
  00774e4f  8b4c2414               mov ecx, dword ptr [esp + 0x14]         | mov ecx, dword ptr [esp + 0x14]
  00774e53  db4128                 fild dword ptr [ecx + 0x28]             | fild dword ptr [ecx + 0x28]
**00774e56  d89da4000000           fcomp dword ptr [ebp + 0xa4]            | fcomp dword ptr [edi + 0xa4]
  00774e5c  dfe0                   fnstsw ax                               | fnstsw ax
  00774e5e  f6c441                 test ah, 0x41                           | test ah, 0x41
**00774e61  7a4f                   jp 0x774eb2                             | jp 0x774eb3
  00774e63  8bd1                   mov edx, ecx                            | mov edx, ecx
  00774e65  8a423c                 mov al, byte ptr [edx + 0x3c]           | mov al, byte ptr [edx + 0x3c]
  00774e68  84c0                   test al, al                             | test al, al
**00774e6a  7446                   je 0x774eb2                             | je 0x774eb3
**00774e6c  56                     push esi                                | push 0
  00774e6d  8d442438               lea eax, [esp + 0x38]                   | lea eax, [esp + 0x38]
  00774e71  50                     push eax                                | push eax
  00774e72  8bcb                   mov ecx, ebx                            | mov ecx, ebx
**00774e74  e8c7fcffff             call 0x774b40                           | call 0x774e7a
  00774e79  8b4c2438               mov ecx, dword ptr [esp + 0x38]         | mov ecx, dword ptr [esp + 0x38]
**00774e7d  8b742434               mov esi, dword ptr [esp + 0x34]         | mov ebp, dword ptr [esp + 0x34]
**00774e81  2bce                   sub ecx, esi                            | sub ecx, ebp
  00774e83  b8398ee338             mov eax, 0x38e38e39                     | mov eax, 0x38e38e39
  00774e88  f7e9                   imul ecx                                | imul ecx
  00774e8a  c1fa03                 sar edx, 3                              | sar edx, 3
  00774e8d  8bca                   mov ecx, edx                            | mov ecx, edx
  00774e8f  c1e91f                 shr ecx, 0x1f                           | shr ecx, 0x1f
  00774e92  03ca                   add ecx, edx                            | add ecx, edx
**00774e94  7409                   je 0x774e9f                             | je 0x774ea0
**00774e96  56                     push esi                                | push ebp
**00774e97  57                     push edi                                | push esi
  00774e98  8bcb                   mov ecx, ebx                            | mov ecx, ebx
**00774e9a  e811ddffff             call 0x772bb0                           | call 0x774ea0
**00774e9f  85f6                   test esi, esi                           | test ebp, ebp
**00774ea1  740f                   je 0x774eb2                             | je 0x774eb3
**00774ea3  56                     push esi                                | push ebp
**00774ea4  e86b9b4800             call 0xbfea14                           | call 0x774eaa
  00774ea9  83c404                 add esp, 4                              | add esp, 4
**00774eac  eb04                   jmp 0x774eb2                            | jmp 0x774eb3
**00774eae  8b6c2410               mov ebp, dword ptr [esp + 0x10]         | mov edi, dword ptr [esp + 0x10]
**00774eb2  8b15a0863b01           mov edx, dword ptr [0x13b86a0]          | mov edx, dword ptr [0]
  00774f0f  8bc1                   mov eax, ecx                            | mov eax, ecx
  00774f11  8b4804                 mov ecx, dword ptr [eax + 4]            | mov ecx, dword ptr [eax + 4]
  00774f14  894c2430               mov dword ptr [esp + 0x30], ecx         | mov dword ptr [esp + 0x30], ecx
**00774f18  8b4c2444               mov ecx, dword ptr [esp + 0x44]         | mov ecx, esi
  00774f1c  33ff                   xor edi, edi                            | xor edi, edi
**00774f1e  e8ad4ae6ff             call 0x5d99d0                           | call 0x774f22
  00774f23  8bf0                   mov esi, eax                            | mov esi, eax
  00774f25  81fe12010000           cmp esi, 0x112                          | cmp esi, 0x112
  00775043  db434c                 fild dword ptr [ebx + 0x4c]             | fild dword ptr [ebx + 0x4c]
  00775046  d8442444               fadd dword ptr [esp + 0x44]             | fadd dword ptr [esp + 0x44]
**0077504a  e8219a4800             call 0xbfea70                           | call 0x77504e
  0077504f  89434c                 mov dword ptr [ebx + 0x4c], eax         | mov dword ptr [ebx + 0x4c], eax
**00775052  db85b0000000           fild dword ptr [ebp + 0xb0]             | mov ebx, dword ptr [edi + 0xb0]
**00775058  d85c2444               fcomp dword ptr [esp + 0x44]            | mov dword ptr [esp + 0x30], ebx
**0077505c  dfe0                   fnstsw ax                               | fild dword ptr [esp + 0x30]
**0077505e  f6c441                 test ah, 0x41                           | fcomp dword ptr [esp + 0x44]
**00775061  0f8ac7000000           jp 0x77512e                             | fnstsw ax
**00775067  8b8db4000000           mov ecx, dword ptr [ebp + 0xb4]         | test ah, 0x41
**0077506d  894c2430               mov dword ptr [esp + 0x30], ecx         | jp 0x77512f
**00775071  db442430               fild dword ptr [esp + 0x30]             | mov ecx, dword ptr [edi + 0xb4]
**00775075  d9442444               fld dword ptr [esp + 0x44]              | mov dword ptr [esp + 0x30], ecx
**00775079  d8d9                   fcomp st(1)                             | fild dword ptr [esp + 0x30]
**0077507b  dfe0                   fnstsw ax                               | fld dword ptr [esp + 0x44]
**0077507d  f6c441                 test ah, 0x41                           | fcomp st(1)
**00775080  7506                   jne 0x775088                            | fnstsw ax
**00775082  d95c2444               fstp dword ptr [esp + 0x44]             | test ah, 0x41
**00775086  eb02                   jmp 0x77508a                            | jne 0x77508f
**00775088  ddd8                   fstp st(0)                              | fstp dword ptr [esp + 0x44]
**0077508a  8b9db0000000           mov ebx, dword ptr [ebp + 0xb0]         | jmp 0x775091
**00775090  2bcb                   sub ecx, ebx                            | fstp st(0)
**00775092  41                     inc ecx                                 | sub ecx, ebx
**00775093  894c2430               mov dword ptr [esp + 0x30], ecx         | inc ecx
**00775097  db442430               fild dword ptr [esp + 0x30]             | mov dword ptr [esp + 0x30], ecx
**0077509b  d985bc000000           fld dword ptr [ebp + 0xbc]              | fild dword ptr [esp + 0x30]```
Register roles from 0x00774D7B:
| value | retail | ours |
|---|---|---|
| CSE'd zero (CDefPointer null stores, MaxStock 0.0f store, the `false` of GetFreeSaleItemPlaces) | esi, live to `push esi` 0x00774E6C | edi, dead after 0x00774E2C, so `push 0` |
| def (parameter, loaded 0x00774E07) | edi, dies after AddSaleItem 0x00774E97; reloaded from its arg slot at 0x00774F18 | esi, kept to 0x00774F18 (`mov ecx,esi`) |
| pshop_def | ebp from 0x00774E49 (and 0x00774EAE) | edi |
| places begin | esi (0x00774E7D) | ebp |
Cascade: the donation block (0x00775052..) keeps MinGoldForDonation in ebx in retail (this in ebp); ours spills MaxGoldForDonation
once more; the epilogue destructor order then differs only in registers.

## Ideas tried (levels) and outcomes
First author (49 grades; NOTES.md "0x00774D20 ..." sections): Ego_d locals / scopes / construction order of the five CDefPointers
(matches retail's null-store order); same-TU price bodies; IS_THING_ALIVE / GFPower spellings; find NOINLINE / DECL; operator[]
DECL_ONLY (flips this/Stock by making def escape -- rejected); SellItem body; coordinator levers 1-5 (scope blocks, VC6 for (N/A),
zero constant spellings, MinGoldForDonation text, GetFreeSaleItemPlaces real body = batch 9); STLport map / tree / CDefPointer /
CDefinitionManager opt-ins (11, none helps). Best 1154 -> 1149v1149 / 34 lines.
This session (11 grades, all ndiff 34 exactly):
1. Include order (owner lever 2026-11-19): CTCShop.h first; the inventory headers first; CMainGameComponent / CPlayerManager / CPlayer
   first; CTCHeroStats / CTCHeroExperience first (all 34); the search headers first (SRC_FAIL, order-dependent headers).
2. Declarations: `long bought_price = 0;` after ptc_inventory_base; tc_hero_experience declared before phero (both 34).
3. Condition / escape shapes: `*places.begin()` for `places[0]`; the two `&&` gates as nested ifs (both 34).
(Donor executables FableWin.exe / Ego_r.exe are not on this machine, so no donor disassembly this session; Ego_d.pdb types were
resolved with llvm-pdbutil: places is a plain std::vector value, the CDefPointers are values.)

## Top 3 untried hypotheses
1. Measure c2, do not guess: the allocation order of the three webs (zero CSE, def, pshop_def) is a priority tie. Dump c2's web /
   priority data with the repo's c2 tools (pending/2026-11-09_linux_hand892/tools; MATCHING_PLAYBOOK "regime-1 priorities") for ours
   and for one probe that changes only def's use count, and turn the first wrong decision into an inequality. The decisive fact is
   that retail's def web STOPS before the GetInventoryType(def) use at 0x00774F18 (reloaded from [esp+44h]), while ours extends to it.
2. def's alias class: `&def` escapes into the out-of-line map operator[] (0x00778280, same-TU template COMDAT) at the top. The first
   author's operator[] DECL_ONLY probe flipped this/Stock "by making def escape", i.e. def's escape status steers this tie. Try the
   operator[] / find / lower_bound REAL bodies visible but out of line (noinline + body, the e01 recipe) so c2 has mod/ref facts for
   `const long&` -- the state retail's TU most likely had (the bodies are in the CTCShop TU as COMDATs).
3. Same-TU callee AddSaleItem (0x00772BB0, landed with its own opt-ins and /d2inlT116) defined above BuyItem (noinline) -- the only
   same-TU callee inside def's retail web not yet made visible; its mod/ref on `const GoodsPlace&` (places memory) and on `this` may
   end def's web where retail ends it. Watch that it must not drag its inline-budget flag into this row.

## Coordinator window analysis (no compile), 2026-11-19: register-allocation split choice (NOT parked)
I compared what each side holds in esi/edi/ebp across the deciding window 0x00774E97..0x00774F18. The values are the same on both sides, in a different permutation:
| value | retail | ours |
|---|---|---|
| zero | esi | edi |
| def | edi | esi |
| pshop_def | ebp | edi, after the zero web ends |
| places begin | esi, after the zero web ends | ebp |

Both sides keep all four values in registers. Each side shortens one web, and they pick different webs:
- **Retail** keeps the zero web alive through the `false` argument (`push esi` at 0x00774E6C). It ends def's web after AddSaleItem (`push edi` at 0x00774E97) and reloads def from its argument slot at 0x00774F18.
- **Ours** ends the zero web at 0x00774E2C and pushes the immediate `push 0`. It keeps def in esi up to 0x00774F18.

Every other instruction is identical. So the difference is c2's allocator choosing which web to split under the same register pressure: register choice, a compiler residue under ASM_RULES rule 2. No source shape keeps a value in a register on one side that the other side keeps in memory or recomputes (the immediate `0` and the def reload offset each other).

CORRECTION (owner, 2026-11-19: no parking; EXTRA_RULES "Never park on register colouring": measure the first wrong decision in c2 and turn it
into an inequality). The row stays OPEN. Next work, in order:
1. Read docs/pipeline/MATCHING_PLAYBOOK.md (regime-1 priorities, measured c2 rules and dead ends).
2. Bring up the c2 web / priority dump tools (pending/2026-11-09_linux_hand892/tools) on this machine. Dump ours and one probe that changes
   only def's use count. Find the first decision where the allocator splits def instead of the zero web, and write the inequality.
3. Prove the lever with a throwaway diagnostic build, then search source shapes for the natural spelling: hypotheses 2 (map bodies visible
   but out of line, which sets def's escape class) and 3 (AddSaleItem body visible) are the leading candidates.

## c2 measurement (c2-tooling author, 2026-11-19 session 3) -- the first wrong decision, as an inequality
Tools: w25/hand4d/c2work/ (README in NOTES.md "session 3"): c2prio8 (+ --pen penalty sources), wi.sh what-if wrapper, klist / pieces /
pbreak / bdv readers, objq (grade a what-if .obj). All what-ifs below are c2 hooks on an unchanged source; no grades spent.
- Decision K25 (allocation order): #13 = places._M_start (prio 22, key 123) chooses EBP. Costs: esi 400 = 100 x spill 4 of the def piece
  #68 (lines 112..126; esi-only because bought_price took edi at K17 and the phero temp #53 holds ebp), edi 601, ebp 0. Retail = esi.
- Proof A: --forcereg K25=esi -> every instruction up to the donation block is retail (ndiff 34 -> 6): zero esi to `push esi`, def edi
  (the def piece is re-split to 112..118, line 126 reloads from [esp+44h]), pshop_def ebp, places esi. The 6 left = residue 2 below.
- Proof B: --forceprio pshop_def piece = 23 (coloured before #13; it takes ebp, then #13 takes esi at cost 400 < edi 1201) -> the same
  ndiff 6. With 22 (a tie; #13 wins on key 123 v 71) -> no flip (ndiff 34).
- INEQUALITY: prio(pshop_def piece after the K17 split; base 14) > prio(places begin; 22) [or the esi penalty of the def piece gone].
- Residue 2 (donation block, independent of residue 1): retail reads MinGoldForDonation at 138 as a plain `fild [ebp+0B0h]` and starts
  the Min CSE web at 141 (`mov ebx,[ebp+0B0h]` 0x0077508A); ours CSEs Min from 138 (ebx). Diagnostic dg_vol138 (volatile read at 138,
  diagnostic only) gives retail's 141 shape and raises the pshop_def piece to 22 (+8: one more reference in block 141..149), still a tie.
  So the source needs BOTH: (i) Min not CSE'd from 138 to 141 (gives +8), (ii) +1 more for pshop_def (or -1 for places begin).
- Measured inert (no flip of K25 / residue 2): the AddSaleItem body visible before BuyItem (v_addsale: ndiff 34; with /d2inlT116 121);
  map operator[]/find bodies are already visible and out of line in ours (template COMDATs compiled before BuyItem); clamp spellings
  (std::min 62, `?:`, reversed compares, `value -= Min-1` statement as FableWin, Max+1-Min, implicit conversion: all 34).
- c2 facts on residue 2: c2 never rematerialises a CSE temp from its source address (mini w_press2: a spilled CSE temp goes to a stack
  slot), and the clamp store to `value` is not the kill (Max is CSE'd 139 -> 141 across the same store in retail). So retail's 138 read is
  a DIFFERENT IL expression from the 141/144 reads. Retail scan (c2work/scanfild.txt: fild [r+d] then mov r2,[r+d], 63 hits, 43 in
  landed rows) found only store-killed or unsigned-conversion cases, no landed analogue.
- ASK: the FableWin.exe / Ego_d text of BuyItem lines 136..144 (how MinGoldForDonation is read in the compare); none of the exes is on
  this machine. Best guess: the compare reads Min through a different expression (e.g. a separate accessor or a cast), which is also
  what supplies residue 1's missing +1.

### Coordinator checks (c2-tooling author, 2026-11-19)
1. Kill ordering (retail): from `fild [ebp+0B0h]` 0x00775052 to the first `[ebp+0B4h]` read 0x00775067 there is only fcomp / fnstsw /
   test / jp: NO call, NO store. The clamp store (0x00775082) comes AFTER the Max read, and Max still survives it to 141. Reading stands:
   the 138 Min read is not killed, so it is not the same CSE expression in retail.
2. Composition what-if (dg_vol138 + --forceprio pshop_def piece #32 = 23; runs/dgv_p23): K22 pshop_def ebp, K25 places esi, and EVERY
   instruction up to 0x00775052 is retail. The 19 lines left are: the volatile read itself (3, a diagnostic artefact); the `mov ecx,esi`
   slot (also in k25esi); `lea edx,[eax+eax]` v ours `mov [esp+2Ch],eax ; add eax,eax` at 0x007750FC (max_morality*2); and the
   eax/edx vptr rotation in four destructor tails. The last two look like the expression-temp rotation shifted by the volatile's extra edx
   temp, but that is NOT proven: a third decision cannot be excluded until a natural Min shape exists. The inequality is complete for the
   register-allocator part (first block exact).
3. Weights: every block has w = 1 (no loops), so "weight" = K. With the Min shape (dgvb pass 2) the pshop_def piece (22) is: +2 b115(K1),
   +4 b135(K2), +10 b135-138(K5), +4 b139(K2), +40 b141-149(K5), +4 +4 destructor b152 (K1 each), minus live-through: K1 blocks b119 x2,
   b123 x2, b131, b135 x2, b152 x4 (each -1), K2 blocks b119 x2, b119-123, b135 x4, b152 x2 (-2 each), K3 b118, K4 b152, K5 b116-117
   and b135 (map, GetTC). Places begin (22) = b116-117 K5 +10, b118 K3 +6, b119 K2 +4 (test/free with c0), b119 K1 +2.
   +1 sources: any K1 live-through block of pshop_def losing its only reference (-> K0), or pshop_def referenced in a K1 live-through
   block (+3 net), or one fewer candidate in b116-117 / b118 / b119 (places -2 / -1).
4. Ego_d line table (tmp/egod_buyitem_lines.tsv) against our VC7.1 /Od per-line sizes (c2work/od/b.cod):
   phero 854/121 29/29, if 855/123 42/40, 859 `bought_price = 0` 7/5, **860: a second 7-byte init with no counterpart in ours** (a
   null/zero init of the next local: ptc_inventory_base?), PeekTC 861/126 28/28, GetBoughtPrice 863/127 15/15, profit 866/128 23/20,
   `if (profit>0)` 867/129 6/6, experience 869/130 49/40, AddTrainable 870/131 14/14, donation if 874/135 63/58, value 876/136 20/17,
   GoldDonated 877/137 26/20, **Min compare 878/138 30/26; lines 879-880 no code; clamp `if` 881/139 26/23; 882 no code (brace line);
   assignment 883/140 17/16** -> the compare and the clamp are separate statements with nothing between them, the clamp is a braced
   two-line if. After the clamp Ego_d has 11 code lines (885 35, 886 46, 888 33, 890 33, 892 23, 893 11, 894 10, 895 17, 896 11,
   897 50, 898 17, gaps at 887/889/891) where ours has 9 (44, 26, 19, 55, 9, 16, 19, 11, 49): the morality math is a different text
   (two more statements; a 17-byte statement AFTER SetMorality-sized 897; 894 is too small (10) to hold a call). Tested:
   `ptc_inventory_base = 0` (line 860 guess) -> inert (ndiff 34; with the Min diagnostic, places/pshop_def still 22/22).

## RESOLVED (2026-11-19, c2-tooling author): RELOCATION_MATCH 1149, from the Ego_d line-table fit (coordinator lead)
Probes (no-ATTEMPTS harness c2work/qv.sh unless noted; all also in ATTEMPTS.log at the end):
- v_f1: donation block per Ego_d 883..890: braced clamp, `value -= MinGoldForDonation - 1;` FIRST (885), max_value as Max + 1 - Min (886),
  `log_value = GFPower(value, ...)` (888 / 890 the same 33-byte shape) -> ndiff 32: residue 2 GONE (retail's `fild [..+0B0h]` at 138 and
  the Min reload at 141 fall out). c2: pshop_def piece 22 v places 22 -> still the K25 tie (as the inequality predicted, +8).
- v_f1c: + Ego_d 842..844 ONE `if` (4-term && chain: both GetSubDef tests, price, displayable) instead of two nested ifs -> ndiff 2: the
  K25 permutation is gone (block structure changes the priorities). Left: `mov ecx,esi` one slot late in the morality tail.
- Tail: 10 paren / order variants; `1.0f - (ptc_stats->GetMoralityPercentage())` (p6) and `(morality_adjustment * log_value)` (q1) give
  RELOCATION_MATCH (a PAREN tuple moves the scheduler's region cut). Kept q1, marked FITTED (Ego_d's two extra tail lines 894 / 898 are
  not reproduced). Inert: `ptc_inventory_base = 0` (line 860 guess).
- Final src/00774d20.cpp also defines STLP_TREE_INSERT_UNIQUE_HINT_BODY + STLP_MAP_ORIGINAL_INSERT_BODY (STLport _tree.c bodies, "visible
  in every retail TU that includes <map>"; RELOCATION_MATCH with and without; needed for the behaviour test to link map::operator[]).
- Coordinator decision 2026-11-19: keep q1 (Inferred line now names p6). One-shot test of Ego_d 894 as `morality = 1.0f - morality;`
  (c2work/var/v_l894.cpp): DIFFER ndiff 4, not adopted. 898 guessed debug-only (byte-neutral, not gradeable). Re-graded final src
  RELOCATION_MATCH; strict vland PASS again; b12 refreshed.
