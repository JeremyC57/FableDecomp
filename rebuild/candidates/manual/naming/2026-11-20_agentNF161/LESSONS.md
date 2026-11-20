# NF161 lessons
- vland needs the lane environment (`source $S/wine/env.sh`); without it verify_and_land.py looks for the Windows manifest path and dies.
- Protected static data (PMemoryAllocator) reached from non-members: use the PDB public inline accessor (`CMemoryAllocator::Get()`), not
  a friend or a cast; it inlines away and keeps RELOCATION_MATCH.
- Out-of-line template members that retail calls (CSmallVector::push_back 0x00A7DBF0): add a declaration-only opt-in, mutually exclusive
  with the inline-body opt-in, so the row binds the call instead of expanding it.
- Private members in tests: linker `/alternatename` from an extern "C" __fastcall stub (thiscall = fastcall + spare edx) keeps the test
  off a local class model.
- By-value returns of shader handles (CVertexShader) on the shared header drag the inlined ~CShaderBase vptr restore in/out; 3 forms
  did not converge -- block early.
- pass.py must match 0x-uppercase STATUS keys.
