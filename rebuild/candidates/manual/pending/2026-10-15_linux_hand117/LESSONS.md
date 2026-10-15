# Chunk 117 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A leading `mov eax,ecx` in a member that then works on `this` through eax is a `T& f() { ...; return *this; }`
  (C2DVector::Normalise 0x00A14540): the result register holds `this` throughout.
- x87 vector maths with intermediates kept on the FPU stack in source order = C2DVector/C3DVector value operators
  with a user constructor; `len * 0.25f` must take a NAMED float `len` or VC distributes the constant into each
  square (0x00A57A40); `a < b` evaluated rhs-first in retail is written `b > a`.
- `ret 4`-only stream/chunk members with a mangled catalog leaf (`AAVCDataInputStream`) need the parameter types
  declared `class`, else the leaf matches nothing and q.py silently grades the largest TU function (0x00AAE440);
  the inlined stream `Read` needs `__forceinline` in a non-size TU.
- Branch polarity of inlined helpers is source-visible: empty-array accessor `if (size != 0) return begin; return
  (T*)&array;` gives `mov edi,begin ; jne ; mov edi,this`, and `if (size <= remaining) copy; else overflow;` gives
  `cmp size,[rem] ; jg overflow` (0x00AA48D0).
- A virtual call through a counted-pointer member that fetches the vtable into eax (not edx) wants the member typed
  CIVCountedPointer<T> with an inline `operator->` (0x00A3C0C0).
- `new T(src)` storing the base vtable, copying a ref-count word, then the derived vtable = the IMPLICIT copy
  constructor of a CIVCountedPointeeBase-derived class (0x00A3C260); its release `dec [ecx+4] ; jne ; call [vt+4]`
  is an inline member of the pointee.
- A re-indexed vector slot loaded as `mov eax,[ecx+ebx] ; add ecx,ebx ; test eax,eax` is a REFERENCE local to the
  element inside the `if`; testing `v[i].p == 0` in the condition gives `lea ecx,[ebx+eax] ; cmp [ecx],0` (0x00A42FC0).
- Placement-constructing N elements in a count-down loop (`mov ebp,count ; ... dec ebp ; jne`) is an inline helper
  taking the count BY VALUE; a member count in the loop test is re-read after every constructor call (0x00A896A0).
- The STLport vector member structure (`_Vector_base(const allocator&)` + `_STLP_alloc_proxy`) also fixes
  `xor eax,eax` / `mov ecx,arg` scheduling around the three zero stores in a plain (non-polymorphic) constructor
  (C3DGroup2 0x00AC4150) and in an inlined chunk constructor (0x00ABD0D0).
- A single-vs-many write with two tail calls of the base WriteToFile is an if/else with ONE shared call after it;
  an early `return` in the first arm moves `out` from ebp to esi (0x00ABC390).
- `"lit" + GetA() + "lit" + GetB()` with two leading copy-constructor calls and the copies destroyed after the sum =
  by-value inline getters (temporaries); a final copy into the hidden result after the temporaries die = a named
  result local (`CCharString info = ...; return info;`, 0x00ABFE80). `?B` in the mangled return = `const CCharString`.
- CRGBColour members in chunk records are one dword (`unsigned long m_argb`); a 4-byte char struct packs them at
  byte offsets (0x00ABFCF0).
- Inline-asm floor helpers (`fsub [0.5] ; fistp ; fild ; fadd [1.0]` with 0.5f/1.0f stored as frame locals and a frame
  pointer) are asm_bake-only: three rows here (0x00A52650, 0x00A526D0, 0x00A4DEA0) -- same family as 006472b2.

Review at landing:
- No row needed more than 4 grades. Rows that used all 4: 004fab40, 0066c880, 00ac6600 (parked), and 00a42fc0 (matched
  on the 4th).
- Local models next to existing engine headers (all because the PDB header sits 8 bytes high, the retail chunk base is
  0x2C): C3DMeshFileGroupChunk (00abae50 parked, 00abaee0), C3DMeshFileClothChunk (00ac6600, 00ac66e0),
  C3DMeshFileHelperPointChunk (00aa9da0), C3DMeshFilePrimitiveChunk (00abc390, 00abd0d0), C3DMeshFileMaterialChunk
  (00abfcf0), C3DMeshFileEngineMaterialChunk (00ac5260), C3DAnimFileXSequenceChunk (00aa48d0, header keeps the array
  opaque); C3DAnimation2 (00a52650, PDB 4 bytes high); C3DAnimationInfo (00a269c0 follows the landed TouchAnimation
  model); CRegionMinimap header IS used (0066c880 parked).
- Rows named away from the catalog (check the leaf/name the lander uses): 0066c880, 00cbd670, 00a737b0, 00a73840,
  00a42fc0, 00c1cc10 (see NOTES "Label corrections"; Leaf lines in the sources).
- Already-landed sources: rebuild/src/compiled/00/ac/C3DMeshFileClothChunk_GetChunkInfo...00ac62b0.cpp and
  rebuild/src/compiled/00/aa/C3DAnimFileMovementVectorChunk_GetChunkInfo...00aae0f0.cpp are a bogus shared body
  (`return CCharString((const char*)0x122d70e, -1)` named GetStrikeEventHelperName) -- worth re-checking what they
  really land.
- No catalog row in this chunk holds several functions.
