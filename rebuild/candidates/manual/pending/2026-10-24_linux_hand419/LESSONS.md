# Chunk 419 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **vector::reserve with the tag on a live slot.** Retail homes `_M_allocate_and_copy`'s `__false_type()` on the
  still-live `n` argument slot (n is re-read after the calls). This reproduces with a same-TU `auto_inline(off)` +
  `__declspec(noinline)` `__fastcall __uninitialized_copy` that never touches the tag, plus a declaration-only
  `_M_clear()`. Matched first grade (0x00A55500).
- **`_M_allocate_and_copy` needs `__forceinline` on `__uninitialized_copy`.** With plain inline VC7.1 leaves it as a
  separate COMDAT call (0x00A53650).
- **A by-value C3DSkeletalBoneMorph re-copied into the argument area needs C3DVector's memberwise user copy ctor.**
  With the implicit one, VC copies Scaling through an `add esi,8` pointer, 171v169 (0x00A46530).
- **x87 point-to-line distance.**
  - Natural C2DVector/C3DVector inline operators reproduce retail's odd term orders (dot X,Z,Y; lengths Z,Y,X) on their
    own. Write the plain `X*X + Y*Y + Z*Z` and let VC order it.
  - Spelling `line.GetLengthSquared()` at each use, instead of a named local, gives retail's memory-first
    `(len2 * t) * t`; a named float local gives `(t * t) * len2` (0x00A442E0).
- **Lane gap: retail keeps a register copy of a just-stored float local.** Retail emits `fcom`/`fst` with the copy
  reused across the branch; every spelling we tried in this lane gives `fstp`/reload. The two adjacent GF maths rows
  0x00A441E0 and 0x00A442E0 (same TU) both show it. Park after 4 grades unless a new idea appears.
- **MMX with memory operands is a retail inline-asm block.** If `punpcklbw mm,[esp+N]` uses a stored zero dword, there
  is no `pxor`, and every operand is a `movd` from a frame slot, retail wrote inline asm over C locals. The RTM
  include dir (`$S/wine/vc71rtm/include`) has no `mmintrin.h` (only the QFE one does), so an intrinsic probe must
  restate the `__m64` and `_m_*` declarations (0x00A44250).
- **CRC-caching constructors.**
  - `BoneNameCRC = boneName.GetCRC();` in the body, over the ARGUMENT string (retail re-reads it, not the member),
    with `GetCRC() { return CCRC::Calc(0, c_str(), Length()); }` and two ternary accessors. Right-to-left argument
    evaluation then gives the length null test before the data one, and VC duplicates the call tail per arm
    (0x00A44DF0).
  - `CCRC::Calc` is static `__fastcall` (seed in ecx, data in edx, length pushed).
- **An unoptimised microthread TU.** `#pragma optimize("", off)` with the static `__fastcall` member reproduces the
  ebp frame and the `[ebp-4]` spill and re-read (0x00A44660, like its landed neighbours).
- **STLport list find, erase, then a discarded find.**
  - `AgentLists.erase(find(begin, end, x)); find(begin, end, x);` reproduces retail's repeated walk after the free.
  - A model with a split `_List_iterator_base` (member `==`/`!=`), a `_List_iterator` with a user copy ctor and a
    plain inline input-iterator `find` matched first grade (0x00A47D80).
- **Lane gap: a vector erase inside a loop.** `(*it)->erase(i); --i;`, `erase(i--)`, `i = erase(i); --i;` and the
  fully layered STLport erase all compile to the same bytes. The lane makes `pos + 1` an induction variable (a fifth
  register, `agent` spilled); retail does not (0x00A47D10).
- **`return (a - b).GetLengthSquared() < d * d;` gives retail's `mov eax,1 ; ret 4` / `xor eax,eax ; ret 4` bool
  returns** (0x00A4B7B0).
- **Earlier rows of this chunk:**
  - STLport `__introsort_loop` needs an empty comparator class with a user ctor, and `partial_sort` as an inline front
    end over the declared `__partial_sort`.
  - `copy` into a `back_insert_iterator` needs `__forceinline __copy`.
  - C2DLineF `_M_insert_overflow` wants noinline stand-ins only for copy and fill: the trivial `__destroy_aux` stays
    inline and vanishes. The CDataBankTextEntry one needs all three stand-ins.
- **C3DAnimFile::GetRoot() const 0x00A537F0 is a byte twin of the landed C3DMeshFile::GetRoot() const 0x00A32620.**
  Reuse the counted-pointer model verbatim; only the class names change.

Review at landing:
- 0x00A44250: decide whether an MMX inline-asm body qualifies for the `retail_asm` exception (MMX is not on the
  rdtsc/fistp/fnstcw list, but this body is clearly inline asm in retail).
- 0x00A441E0 and 0x00A442E0 share the register-copy residue: check whether this GF maths TU needs a different
  flag or compiler before spending more grades.
- Fix the catalogue row 0x00A43E30: it starts inside 0x00A43DF0. Catalogue 0x00A43DF0 itself.
- Landed names to review:
  - 0x00751E80 declares 0xA441E0 `__stdcall AngleDifference`; the PDB has `__fastcall GFGetSignedAngleDifference`.
  - 0x00A4EBB0 calls 0xA537F0 `GetRootChunk`; the PDB has `C3DAnimFile::GetRoot`.
  - 0x006CE670 and 0x01224320 call 0x013D2834 `g_registeredObstacles` / `g_list_013d2834`; it is
    `CNavigatorExternalInterface::AgentLists`.
  - 0x00A54A90 is `CRetailTree_00a547c0::operator=`; its tree is `_Rb_tree<CCharString, pair<const CCharString,long>>`.
