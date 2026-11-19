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
