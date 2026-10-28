# Chunk 535 lessons

## GOTCHAS one-liners
- Inline CDataInputStream::Read register census (retail exe, ~800 sites): the 64-bit position's hi word is never in edx.
  The counts are 506 `lea ecx,[eax+4];xor eax,eax`, 147 `lea edx,[eax+4];xor eax,eax`, 84 `add eax,4;xor ecx,ecx`, and 0
  `add eax,4;xor edx,edx`. An `add eax,4 ; xor edx,edx` in our output is never retail.
- VC7.1 RTM gives that hi=edx form only when the same function also holds a `cdq ; idiv` (STLport size() over a
  non-power-of-two element under /Os). Removing the idiv, either loop or the write, or using /O2 (imul), gives retail's lea
  form. About 150 equivalent source forms and every /O, /G and /Z flag tried were inert, and so was the QFE compiler. Treat
  the CPersistContext::TransferVectorOfSubComponents 244v245 / 254v255 family as a lane gap; LTCG is the one untested
  route.
- Only `/Oa` (or `#pragma optimize("a")`), or folding the hi compares away (`(__int64)p` on an `unsigned long` parameter),
  reproduces retail's allocation. Both change other bytes, so neither is the source.
- A compile-only harness (cl + objdump plus a mnemonic diff against dis_all.txt) runs about 5 variants per second in the
  Wine lane. It is the cheap way to sweep forms without spending q.py grades (agent535/tools/exp.py).
- C3DVector temporary through the implicit operator=: every user operator=, named local, Set() or reference form folds the
  temporary into immediate stores (0x44 body), and every implicit form gives the same 101-byte body. Retail's partial
  forwarding (temp X reloaded, Y/Z forwarded through edx, dead temp stores sunk to the end) was not reproduced (0x00B2F760).
- Parked sources can rot: 00b2f760's parked file no longer compiled (C2512: no C3DVector default constructor). Compile a
  retry's parked source before reading its old grade.
- Never run `find /`: it walks the shared Wine trees. Search a known directory instead.

## Review at landing
- NAME_FIXES.tsv (16 rows). The landed 0x004F9DF8 binds `??1CCardPositionsDef@@QAE@XZ`; the PDB has only UAE. The shared callees
  0x00404500 / 0x00993EB0 / 0x00993CA0 / 0x00430300 / 0x00430370 / 0x00B59710 carry 3 to 27 invented spellings in
  reloc_bindings.tsv.
- Catalog: the 9 family rows all carry `TransferVectorOfSubComponents<CAbilityButtonGuiDef>`, and 0x00B2F760 carries
  CCompressedMemoryDataInputStream. Respell them to the leaves in NAME_FIXES.tsv.
