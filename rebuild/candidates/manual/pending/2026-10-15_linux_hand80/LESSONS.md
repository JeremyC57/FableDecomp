# Chunk 80 lessons (candidates for docs/pipeline/GOTCHAS.md)

- A bitfield flag update shows as `mov eax,[mem] ; or eax,N ; ... ; mov [mem],eax`, with an unrelated `lea` scheduled between the load and the store. The source is a one-bit bitfield assignment (`m_bitK = 1`); `m_flags |= N` compiles to `or dword ptr [mem],N`.
- An implicit (compiler-generated) destructor stores no vtable. That holds for CIntelligentPointer temporaries and members (only `~CBaseIntelligentPointer` runs) and for intermediate bases such as CTCActionUseBase, whose out-of-line copy at 0x004DA967 has no store either.
  - Inside a `__forceinline virtual` deleting destructor, VC7.1 calls such an implicit base destructor OUT OF LINE.
  - To inline it, give the base an explicit `__forceinline ~Base() {}` plus `__declspec(novtable)` (0x0072A730).
- A member function returns ANY struct through a hidden pointer, even a 4-byte POD of bitfields. Bit code such as `shr eax,8 ; not al ; and al,1` on the call result in eax means a scalar return: `!((flags >> 8) & 1)` (0x007EAF20).
- A float member copied with `fld [old] ; fstp [last]`, while the float argument is moved as a dword, is a NAMED float local:
  `float previous = cur; cur = arg; last = previous;`.
  A direct member copy, or a copy through an inline float getter, compiles to integer moves (0x007F8D60).
- Vector components loaded Z, Y, X onto the x87, then squared from registers (`fld st(i) ; fmul st(i+1)`) and cleaned up with `fstp st(3)` plus two `fstp st(0)`, come from an inline `Magnitude(float x, float y, float z)` taking the components BY VALUE.
  - A POD vector copy goes through the frame instead.
  - A memberwise copy constructor loads X first (0x006627A0).
- When every `bool : 1` bitfield in a byte is initialised to a constant in the init list, VC7.1 folds the eight bit operations into one byte store; equal byte values share a register (`mov al,4`) (0x00723FD0).
- The PDB's unaligned CIntelligentPointer members (+0x11, +0x21, +0x39 in the PDB, the same in retail) need `#pragma pack(push,1)` around the IP class definitions (0x0079BDF0, 0x007A50F0, 0x004CBFB0).
- Two frame strings, the first default-constructed, the second copy-constructed from it, with the first destroyed BEFORE the call that takes the second by const reference: an inline helper that returns a NAMED local by value (VC7.1 has no NRVO) (0x007E8160).
- Argument order around a by-reference member: `const T& m = Get()->member;` puts the `add eax,N` before the last argument load, a `T* p = Get();` local puts it after the last push, and the inline call pushes everything first (0x006CA7F0).
- A free `__stdcall (a, b, ctx)` whose only reference is a `push offset` into a C-style registration call (0x00C49A2F) is a stream callback. An address-taken local in it is homed in a dead argument slot (0x00C497E0).
- Ego_r.pdb mangled names (`ghidra_out/egor_pdb_names.tsv`) resolve propagated labels and `vfunc_N` slots quickly: grep `@<Class>@@`. Static members in this code base are `SI` (`__fastcall`).
