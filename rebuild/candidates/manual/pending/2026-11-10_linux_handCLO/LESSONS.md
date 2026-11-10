# agentCLO lessons (0x005B8743)

- Start from a matched sibling's TU devices (CTCInventoryWeapons::ConstructScreenPC 0x005C9A95): optimize("s"), same-TU GetSubDef/GetDef
  bodies under auto_inline(off). The CDefPointer capture is by far the largest diff (3,880 -> 177 lines in one step).
- A same-TU GetSubDef<T> body needs T::StatGetTypeName(); add it as a header opt-in from FableWin.pdb + the landed GetSubDef<T> literal.
- Retail pushing an immediate `1` in one path and a register in another for a bool parameter = the call is written twice (if/else arms,
  tail-merged). A bool expression argument always materialises in `al`.
- A 4-byte slot shift that starts at one local and runs to the end of the frame: look for an empty-class temporary (allocator default
  argument). If the callee body (here STLport _List_base(const _Alloc&)) is visible, VC7.1 homes the temporary on a dead arg byte
  instead of a fresh slot -- even though the callee is still called.
- Loop shape: a reference local to a vector element lets VC7.1 strength-reduce to a count-down loop; indexing through the outer vector
  every iteration keeps the up-counting IV and the repeated size computation.
- Do not trust landed labels for STL instantiations near 0x005BAxxx-0x005BCxxx: several are named after unrelated callers
  (CActiveFile_OnReadFinished, CDisplayManager::CopyBackBufferToTexture). Bind by what the call site constructs / destroys.
- Grep forbidden tokens as substrings: purity.py tests `"naked" in text`, so a variable called `naked` in a test trips it.
