# Chunk 495 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 495) `return m_arr.m_data[i]->Get(j)` loads j first. Retail's `mov eax,[ecx+4]` first comes from a named element
  local: `T* e = m_arr.m_data[i]; return e->Get(j);`. A named data-pointer local or an inline operator[] does not do it
  (0x00C34E20).
- (chunk 495) A float product retail emits as `fld [arg] ; fmul [pool]`, scheduled before the earlier dword copy, is a
  literal constant: `value * (1.0f / 330.0f)`. An `extern const float` global, in either operand order, gives
  `fld [global] ; fmul [arg]` after the store (0x00C3D390).
- (chunk 495) A toggle setter's `mov al,[esp+4]` (and not `mov dl`) comes from each if-arm reading the state word itself
  (`if (on) { f = m; if (f & bit) return; m = f | bit; } else { f = m; ... }`), which VC merges into one load. A shared
  read before the `if` picks dl. A choice between two immediates, `mov eax,A ; jne ; mov eax,B`, is
  `mask = A; if (!cond) mask = B;`; a ternary gives neg/sbb/and/add (0x00C417F0, 0x00C41840).
- (chunk 495) A `??_E` whose array arm calls `eh vector destructor iterator` (0x00BFEED8) needs /EHsc; without it VC7.1
  inlines the destroy loop. When both frees go to the libpk class deallocator, the root class must declare
  `operator delete[]` as well as `operator delete`. Without it the array arm calls global `??3` (0x00C3D9A0).
- (chunk 495) Passing a member of a selected pointer directly, `Open(p->m_x)`, loads it into ecx before `mov ecx,esi`.
  Retail's eax load comes from a named local (`void* s = p->m_x; Open(s)`) (0x00C556E0).
- (chunk 495) libpk: a member store and an x87 store through ecx right after a cdecl call (`call ; fstp [ecx+18h]`) is an
  LTCG register contract. Park it after one grade (0x00C42FB0).

Review at landing:
- 0x00C3D9A0 binds 0x00C3C370 under two names: `??3CPKObject@@SAXPAX@Z` (bound) and the new `??_VCPKObject@@SAXPAX@Z`.
  vland reports this as RELOC_DUPLICATE. Retail folded the identical operator delete / delete[] bodies, so the duplicate is
  real, but the bindings table needs an alias policy for it.
- Catalog rows holding several functions, to re-split:
  - 0x00C35460: four functions at 0x00C35460 / 0x00C35470 / 0x00C35480 / 0x00C35490.
  - 0x00C3C4B0: two functions at 0x00C3C4B0 / 0x00C3C4D0.
- Landed rows with wrong identities: 0x00C35170, 0x00C34DD0, 0x00C42AB0, 0x00C42AC0, 0x00C5A1D0, 0x00C3D7B0 and
  0x00C4BFA0. NOTES.md "Label corrections" has the real identity of each.
- 0x00C42AB0 is landed as a free `__fastcall` function with a dummy edx parameter. This chunk calls it as a thiscall
  member, CPtrArray<CTableEntry*>::GetAt. The bytes agree, but the binding should be unified.
- The landed StreamReader model (0x00C426F0) lacks the vptr; see NOTES.md "Header debt".
- No row needed more than 4 grades. 00c34e20 used exactly 4 (3 probes), 00c3d030 used 4 and is parked.
