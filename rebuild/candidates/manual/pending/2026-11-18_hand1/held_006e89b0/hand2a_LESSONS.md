- 0x00918A40: retail block boundary with no branch after an inlined GetTC store (`mov eax,[eax+4] ; pop esi` | `mov ecx,eax ; call`) and a tested GetTC: the original had duplicated `return X();` arms (early `if (!GetTC(&p)) return X();` and a positive `if (ok) return X();`) that VC7.1 cross-jumps; a second presence test always leaves CSE temps (`mov edx,[eax+30h] ; mov ecx,mask ; test ecx,edx`).
- 0x00680050: Ego_d's thing_object_def.obj holds only CThingObjectDef::Validate, so the TU's c1 symbol ordinals come only from its include list (the include-order lever acts on a real property of that file).
- 0x006E89B0: a float/double TYPE difference can show up only as an x87 latency, with the c2 DAG dump identical. A double local that
  never spills leaves no qword operand, so byte counts and operand widths cannot see it. When a join's fmul readiness is the residue,
  try retyping the float locals feeding that add as double (one at a time; watch that the compare still folds as a dword fcom).
- A donor's per-line byte counts cannot tell `a += b` from `b += a`: read the disassembly when the binary exists.
