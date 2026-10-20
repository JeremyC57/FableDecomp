# Chunk 229 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A nested success path (`if (a) { if (b) { ...; return true; } } return false;`) keeps the callee-saved pushes at entry and puts `mov al,1` before the shared `xor al,al` exit. Flat early returns shrink-wrap the pushes; do/while(0)+break shares the exit but puts the false block first (0x009F35D0, 0x007EACB0; it also fixed the frame of 0x006C06D0).
- When retail pushes the arguments above a type test and both arms jump into one call, write the call in BOTH arms (`if (t) x = f(a(i), 1); else x = f(i, 1);`). The identical pushes hoist and the calls cross-jump; a call after the `if` does not (0x0048F6BD).
- `const long* items = arr->m_pFirst; items[i]` puts the element base in the dead pointer's register (esi); `arr[i]` picks eax (0x0048F6BD).
- Retail `test ah,1 ; je <else>` on an x87 `<` test is the source `!(t >= x)`; a plain `t < x` gives `test ah,5 ; jp`. The value of `(m += step)` stays on the FP stack for the test (`fst` + `fld st(1) ; fcompp`) (0x00646B12).
- A frame copy of an operator result (`fstp [tmp] ; mov ecx,[tmp] ; mov [named],ecx`) is a named local ASSIGNED after its declaration (`C2DVector d; d = a - b;`); `C2DVector d = a - b;` builds it in place (0x00646B12).
- A by-value STLport list iterator built with `push ecx ; mov eax,esp ; mov [eax],esi` needs the iterator's user copy ctor; without it VC emits `push esi` (0x00646B12).
- `IsScripted = p && p->IsScripted ? true : false;` materialises in cl (`mov cl,1 / xor cl,cl`); the bare `&&` goes through ecx (`mov ecx,1`) (0x006BF9E0).
- A hidden-pointer camera result followed by a field copy needs the object pointer as a named local (`const T* thing = GetObservedThing();`). Without it VC hoists the constant push (`push 3F800000h`) above the GetObservedThing call (0x00871EC0).
- A counted-pointer temporary released and nulled inside `if (Data)` keeps the null store (`and [ebp-4],0`) when the slot is reused later and drops it at the end: `__forceinline ~P() { if (Data) { Data->Release(); Data = 0; } }` (0x0048F6BD).
- Same-TU stand-in callee bodies under `#pragma auto_inline(off)` let VC share the frame word of an out-pointer and home tags in dead argument slots (0x004D1360; this also fixed the frame of 0x007E5680).

Review at landing:
- Rows that took more than 4 grades: 007cdfe0/007cf110 (the __copy twins).
- Local models sit next to existing engine headers in 009b9aa0/009ba1d0 (CStringParser), 0048dc8b/0048f6bd (CSubtitleRenderer), 0079c750 (CTCBulletTime) and the parked 00ab7b00 (CStaticFontBank). engine/CStaticFontBank.h types +0x04 as `long m_maxHeight`, but it is CFontBank::FontHeight (unsigned long; retail does the unsigned conversion fix-up). 0072e810 does include engine/CTCBuyableHouse.h.
- Already-landed sources:
  - CSurface_CopyFromSurface_009f37e0.cpp contains `__asm`, a purity issue.
  - The landed rows name 0x009C9070 "GetPGroupEntry" and 0x009C7A40 "CGroupEntry::GetText"; on the type-0 (text) path they are GetPTextEntry and the text entry's GetText.
  - The landed 0x009BA1D0-family MoveStringPos form differs from the landed ReadNextString form: two inline variants in one retail TU.
- Stand-in callee bodies: 004d1360 (src) and the parked 006bf9e0 (differ) use them; the landed 0x007E0910 has the same pattern.
- Catalog row with several functions: 007f7870 (FrameUpdate + 0x007F7940), SKIPPED.
- Catalog names corrected in line 1: 004d1360 (`_M_insert_overflow`), 007cf110 (CTCCreatureTintEffect ctor), 0072ed10/007efe70 (OnActionUse), 007f8e20 (FrameUpdate), 006bf9e0 (OnCreate). gall.sh honours the leaves.
