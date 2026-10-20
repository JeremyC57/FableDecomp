# Chunk 204 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A 16-float setter whose stores walk the member linearly while the loads stride the argument block is the private
  `SetD3DMatrix(D3DMATRIX&, 16 floats)` inlined with the arguments transposed. Sixteen direct stores in the body let
  VC7.1 reorder the eax/edx pairs (0x00988490).
- A compiler-generated copy constructor (POD matrices as `rep movsd`, C3DVectors word by word, a member with a user copy
  ctor called in place) needs only a placement-new copy site in the TU (0x00B38600).
- A by-value inline `Clamp(v, lo, hi)` spills the value. Retail's in-place stack-top clamp is `Clamp(float& v, lo, hi)`.
  A two-float struct argument moves the next frame word. The bounds' register order (lo through ecx) comes back only
  when the bounds are named float locals of the `if` block (0x00872050).
- A CCharString whose text and length both come from ONE null test, with "" in edx and the length/text in edi/ecx, is
  written null-case-first with both values assigned in each arm. A preloaded `text = ""`, or the non-null arm first,
  moves them into ebx/edi (0x0048DE8A).
- A literal handed to a `const CCharString&` parameter that retail re-addresses with `lea` after the ctor is an implicit
  conversion even when the -1 length is pushed; it lands in the dead parameter slot (0x0048DE8A, 0x0049319C).
- Two byte stores at a fade object's +0x0C/+0x0D (Opening = false, then Closing = true) are an inline method of the
  CFadeInFadeOutBase member, not two class flags (0x0049319C).
- One x87 compare (`fucompp` of `Max != 0`) shared by two if/else arms that both test it reproduces from the plain source;
  VC7.1 CSEs it (0x004930D7). The neighbours in that TU use y-off/s-on, but this body has no frame pointer: only
  `optimize("s")`.
- Constructor arguments of a `new T(...)` that build temporaries get the construction-flag word (zeroed at entry, set to
  3, tested bit by bit). A by-value inline getter's copy lands in a dead argument slot (0x00A5D510).
- Two returns of the same by-value class through its inline copy ctor (vtable store, Data, add-ref) share one tail;
  write both returns naturally (`return *cached;` / `return *created;`) (0x00A5D510).
- A `this` call whose result is discarded (`GetCamTools();`) survives because the callee is out of line; keep it as
  a statement (0x00872050).
- Entity-script vtables have no RTTI, but the factory record names the class: the factory pushes the size and the
  registration string sits beside it (0x00DC71C0 "UndeadSpawnCutscene" -> vtable 0x012DA8AC). Quest labels copied
  between sibling Main rows are common.
- `strcmp`-shaped CCharString equality used as an `if` condition: `__forceinline` is needed in size TUs (else it is called
  out of line), and even then the lane materialises `sete al` (the chunk 188 gap) (0x0048F7C0).
- In a test, a function returning a class by value from a named local runs one extra copy plus a destructor under
  VC7.1 (no NRVO) but not under g++. Reference counts that the test checks must not depend on that copy (0x00CBE87F
  test).

Review at landing:
- 0x009A6370 matches only with `volatile bool WindowsQuit` (probe/009a6370_volatile_RELOCATION_MATCH.cpp). Retail
  re-reads the flag right after testing it, so volatile is probably the original. Decide whether the lane rule allows it.
- 0x00A5EC00 is named CPixelShader::CPixelShader here (RTTI vtable 0x0129A054). The landed family row
  CBaseIntelligentPointer_CBaseIntelligentPointer_00a5ec00.cpp names the wrong class.
- Landed 0x0048DAA2 (CGuiControlListBox::SelectEntryFromName) binds `??BCCharString@@QBEPBDXZ` to 0x0099AF10. That body
  returns the empty WIDE literal 0x0129A8E0, so it is the CWideString accessor. Chunk 204's 0x009B9EE0/0x009B9FC0 bind the
  same symbol to 0x0099E4C0 (the real CCharString one), which shows up as RELOC_CONFLICT.
- 0x013B8790 has two landed spellings (g_pGameUI_013b8790 x8, g_pPlayerGui x7); this chunk uses g_pPlayerGui.
- 0x009D58D0 has two landed spellings (CreateEntry x2, GetAsyncEntry x1, which is the PDB name); this chunk uses GetAsyncEntry.
- 0x00A3E330 is a catalog row holding six functions (SKIP).
- engine/CShaderDataBank.h and engine/CStreamingFontDataBank.h carry PDB offsets (Shaders +0x174, Fonts +0x1EC). Retail
  is +0x110 / +0x178 because the bank bases are 0x10C / 0x164 bytes, so both rows use local models.
  engine/CSubtitleRenderer.h's Button*OverrideText (+0x60) and TextTimer (+0x7C) are +0x70 / +0x8C in retail: the
  landed rows use +0x70.
