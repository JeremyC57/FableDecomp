# NF129 notes (PRIORITY slice 10, q3967 .. q6512)

## Descriptive names
None. Every name in this hand-back is a PDB spelling (Ego_r.pdb + FableWin.pdb; lane wchar_t spelling G).

## Flags lines
None added.

## Funclets
None found.

## Shrink-only reading
A rename/respell counts as shrink-only when every other landed referencer of the address already binds the new spelling (probe
of every REFS row, work/probe.tsv, plus the exe scan). New spellings were taken only for whole families:
- 0x009A90B0 DeleteData<CRetailBank> (row 0x009A9D10): HEAD passed a cast integer (no binding), no other referencer; ALLOWNEW.
- 0x009D1EE0 Reset<CDiskFileWin32> moved with 0x009A9B60 (q6177 family) so DeleteData 0x009A9030 keeps one spelling.

## Leaf-only / self renames of rows outside the entry addresses
0x0099EF70 (SetUseFastExtend member), 0x009D1EE0 (q6177 family), 0x009BA8C0 (own leaf pointer -> reference, PeekNextItem /
ReadNextItem, Ego_r + FableWin; no landed referencer binds a different spelling), 0x00594F36 (LoadGame onto
CNewFrontendGameComponent.h; its one referencer 0x00599E3F already binds it).

## Stand-in -> PDB spelling on a callee (address spelling count does not grow)
0x0099EC30 in row 0x00A63C00 (NativeStringCopy -> ??0CCharString@@QAE@ABV0@@Z, 63 binders already); 0x0099E4B0 in row 0x00DC0300
(CCharStringFmt -> ??0CCharString@@QAE@XZ); 0x0099F1F0 in 0x00417568 / 0x00A63C00 / 0x00DC0300 (-> Sprintf).

## New local models
None. In-place respells only (existing local CCharString / CPersistContext / CSystemManager / CStringParser / CDataInputStream /
CWideString models). Local models dropped in favour of shared headers: 0x009B9BA0, 0x009A9B60/BB0/BF0/C80/D10, 0x009D1EE0,
0x009D5820, 0x009BAB50, 0x00A641A0, 0x009EC430, 0x00594F36, 0x009A0CF0 (fable_render_texture.h view), 0x00F37170 (local
`struct CTexture : CTexturePdb` and CScratchTexture), 0x00DC0300 (CCharStringFmt), 0x00A63C00 (NativeStringCopy, free Format).

## Debts left in place
- 0x004FE030: `script.SaveToFile((CAFile&)file)` keeps the HEAD cast (its local `file` type is a stand-in); binding is the claim.
- 0x00AE8810: its call of 0x0099E5F0 stays on the HEAD spelling, because the row's own parameter is not a CAFile& and the PDB
  carries only the 0-argument revision of its own member; q4626 is blocked on that (decision (b)).

## Blocked rows
- 0x00995D30 (q4249 self, port): DIFFER(136v138) after 3 forms; ours `add eax,[esi+18h]`, retail `mov ecx,[esi+18h]; add ecx,eax`
  (draft work/00995d30_blocked.cpp).
- 0x0099EF90 (q3967): needs a new fable_string.h opt-in (MakeOwnCopyOfDataOrEmptyString as IAE); decision (c). The NAME_FIXES line
  records 0x0099EF90 = ?Reserve@CCharString@@QAEXJ@Z.
- 0x006BEBF0 (q4933) and 0x009C7A60 (q4285): the wrong spelling lives in a namespaced local CWideString model; decision (a).

## Tests
- 0x009B9BA0: the landed test is a standalone model of the body (it does not link the row's symbol), unchanged.
- New tests rows/009a0cf0_test.cpp and rows/00594f36_test.cpp: the landed tests declared the old stand-in classes and linked the
  row's old symbol (LINK_FAIL in the first final run, work/recheck_try1.log). The new tests drive the shared classes over zeroed
  raw storage with fake vtables (SetTexture slot 0x41; SetQuit slot 4) and a test-defined CWideString::operator= probe; same checks
  as the landed tests.
- Other rows reuse their landed tests (vland PASS, recheck_final.log).

## Decisions for the coordinator
(a) Is de-namespacing an existing namespaced local CCharString / CWideString model (NGameText::, N006bebf0::, N_0099af00::) an
    accepted in-place respell? It unblocks 0x009C7A60, 0x006BEBF0, 0x009F2DD0 and most of the q4458 census.
(b) 0x00AE8810: may its own parameter become CAFile& (the PDB only has the 0-argument revision) so its SaveToFile call can take
    ?SaveToFile@CCharString@@QBEXAAVCAFile@@@Z?
(c) A fable_string.h claim for an IAE MakeOwnCopyOfDataOrEmptyString opt-in (row 0x0099EF90 / q3967).

## Rule slips
- One compile (the 0x009BA8C0 grade and its pdiff) ran while vland batch 2 was running.
