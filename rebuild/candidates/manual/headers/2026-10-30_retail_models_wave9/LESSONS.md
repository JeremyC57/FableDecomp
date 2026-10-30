# Wave 9 lessons (class-model migration, 2026-10-30)

1. **Read the zeroing order: it separates two members that look identical.**
   - ReleaseData and ~CCountedPointer run the same release, then zero both words. ReleaseData zeroes Info then Data
     (expanded in 0x00A45330); the dtor zeroes Data then Info (732 out-of-line COMDATs).
   - Writing the dtor as `{ ReleaseData(); }` gave the right leaf bytes at extra sites, but VC7.1 then called a
     `ReleaseData` COMDAT where retail calls the dtor COMDAT: a wrong callee name, caught only as RELOC_RENAMED
     (`??1 -> ?ReleaseData`). The new name was unbound, so bind_new would have added it silently.
   - Before choosing how a body is split into members, scan .text for every out-of-line copy of each candidate body
     (work/ byte scan in HEADERS.md); a member that never appears out of line is always expanded.
2. **Dead stores tell you which member a body is written over.** Reset 0x008AB2B0 keeps only `mov [esi+4],0` after the
   release; ShareData keeps neither store. Both are ReleaseData expanded, with the stores that later code overwrites
   removed. A model that writes the stores by hand (`Info = 0; Data = data;`) gives the same bytes here, but a third
   caller can differ.
3. **Test header variants on a stratified sample, not on the failures.** Three dtor models were compared on 955 rows (every
   non-SAME row plus 300 random rows with dtor / copy / assign / ShareData members), about 5 minutes per variant with
   mcheck -j 4. The model that fixed the most failures was not the one with the right callee names.
4. **The true model reproduces most forced / out-of-line scaffolding.** On the shared plain-inline header, 79 of 81 rows
   with `__forceinline` pointer members and 166 of 181 rows with out-of-line declared members stay SAME. The exception
   is a function where retail calls several pointer members out of line at once (CreateActiveEntityScript: all 85
   DIFFER) and a called operator= (4 rows).
5. **A regex over `CCountedPointer<...>::` must balance its angle brackets.** `CCountedPointer<[^;{}()]*>::` also matched
   `CCountedPointer<CActiveEntityScriptBase> CEntityScriptBinding<T>::CreateActiveEntityScript`, a return type followed
   by another class's member. That excluded 245 rows as "out-of-class member"; the balanced pattern keeps 47.
6. **Grep the row for asm before a mechanical pass.** A row whose pointer model was migrable carried a landed
   `Retail-asm` block (0x00E81B40, fld / fistp). Its catalog entry has no grade field, so the grade check missed it.
   mig_cp.py now skips any row that contains an inline-assembly keyword, `Retail-asm`, `retail_asm` or `asm_bake`.
7. **A const accessor named in the PDB is the cleanest fix for a const-only call.** Retail calls the const GetMainPlayer
   QBE 0x00449960 through `g_pMainGameComponent`, and FableWin.pdb has `CMainGameComponent::PeekPlayerManager() const`
   returning `const CPlayerManager&`. Spelling the call through that accessor (an inline body on the row's stand-in)
   made all 12 such rows SAME on the shared header, with the QBE callee kept. Wave 8 had to revert these rows.
8. **MovieBase: destruction form follows the ctor (wave 8 data, reread).**
   - All 35 AB0-only rows expand the MovieBase ctor in the same function; all 26 INL-only rows do not.
   - This points at a per-function expansion budget, not at the dtor model. Every dtor model tried still expands
     0x00DF2D30 fully (HEADERS.md).
9. **Lane cost.** mcheck -j 4 did 2,778 rows in about 15 minutes. With the libsig3 vland on the lane, `vland
   --recheck-landed` ran at about 8-10 rows a minute (wave 8 had 17-50). Plan regrade batches around the other agent.
