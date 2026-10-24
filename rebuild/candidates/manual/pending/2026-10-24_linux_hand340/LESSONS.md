# Chunk 340 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Catalog `vfunc_N` leaf matches no symbol** and makes q.py grade the largest function in the TU. Always add `// Leaf: ?Method@Class` on its own line. Name the method from FableWin.pdb plus the vtable slot.
- **Interface lookup forms → retail shapes:**
  - The out-parameter `GetInterface(&p, k)` with the in-place `it = last` pick gives `mov eax,[eax+4] ; mov ecx,eax` (008d8fe0).
  - HasTC plus a returned one-ternary GetTC gives `mov ecx,[eax+4]` (008d8fe0, 008d9480).
  - `T* p = 0; if (HasTC(k)) p = GetTC<T>(k);` gives the `xor ecx,ecx` preload followed by an unconditional use (008da3b0, 008e10e0).
  - An untested `GetInterface` result reads the never-written out variable on the absent path (008d8960, 008dc6c0, 008dffd0, 008e7600).
- **Several lookup keys sharing one frame word:** when retail reuses one `[esp+N]` for two LowerBound keys and ours gives each its own, a same-TU `#pragma auto_inline(off)` const LowerBound stand-in merges them (008d87c0).
- **`bool` from a BOOL-ish call:** `return f(...) ? true : false;` gives `test al,al ; setne al`; `!= false` and `!!` give neg/sbb (00870de0).
- **pack(1) placement:** pack only the classes the PDB shows packed (CIntelligentPointer members at odd offsets). Packing the action temporary as well moves it in the frame (action at +7; 008e8940).
- **Named locals that pin the schedule:**
  - `CThingCreatureBase* creature = ParentThing;` before a call through it (008d6540).
  - `CAnimateThing& animate = tc->GetAnimation();` before a push (008d8960).
  - `CCombatManager* m = GetWorld()->CombatManager;` so GetWorld runs before the argument push (008d8ea0).
  - A named `C3DVector position` for the exact frame size (008e07f0, 0x13c).
- **Inline world RNG** `seed = GFROR13(seed * 0x24A1 + 0x24DF)`, written as an inline helper with two statements (store, then the call result stored back) (008d6580). Open-coded in the caller, VC7.1 schedules the seed update before an unrelated `_ftol` and spills the seed to the frame.
- **Free `__fastcall` returning a class by value:** the hidden return pointer goes in ecx, the first argument in edx, the rest on the stack, and the callee ends with `ret 4*n`. The caller builds the temporary in a dead parameter slot (008e9110 → GetStrafeAnimationName 0x008E8E50). Declaring the callee `CCharString __fastcall F(const A&, const A&, E)` reproduces the call exactly.
- **Odd `range = -lo - hi` arithmetic** (`neg edi ; sub edi,eax`) with a negative float constant (-1.5f) is plain source arithmetic. Write `-minFrames - (long)(f * -1.5f)` literally; do not "simplify" it (008d6580).
- **`if (n) --n; else if (cond) Recover();`** gives retail's tail-duplicated epilogue (two `ret`s) (008e10e0).

Review at landing:
- **Local base models.** The sources use local CModeBase / CModeStandBase models (retail 0x1c / 0x68) instead of `engine/CModeStandBase.h`, because the generated header is flattened and does not expose the CTCCreatureModeManager::CModeBase base members (ParentThing +4, Active +0x18). Check whether a retyped header now exists.
- **Landed-name conflict on 0x0073A680.** Landed sources call it both "GetDisplayEngine" (propagated) and CModeBase::GetWorld. This chunk uses GetWorld.
- **Signature conflict on SetSuspendAnimation 0x00833A10.** Landed sources disagree on the second parameter (`long` vs `CModeBase*`). This chunk passes `(long)this` where retail pushes the mode.
- **Catalog rows naming the derived class for an inherited method.** 008d6580 is CModeSitBase::OnActivate, not a CModeSitChair method; label it with the base class when landing.
- **Rows needing 2 to 4 grades.** 008d8ea0, 008e8940, 008d87c0, 008d8960, 008e07f0 and 008d6580 each matched within the 4-attempt budget. None needed more.
