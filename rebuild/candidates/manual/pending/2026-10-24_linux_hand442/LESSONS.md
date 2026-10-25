# Chunk 442 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Loop with a failure exit (0x00C60A30).** `for (n = head; n; n = n->next) { r = f(n); if (r < 0) return r; }
  return r;` keeps `r` in edi and tests it after the call (`mov edi,eax; test edi,edi; jl`). Any other form changes the
  shape:
  - `break`, or a `goto` past the loop, puts the result in eax and makes edi a zero register (144 bytes).
  - `while (n && r >= 0)` tests at the loop top.
- **List-drain destructor (0x00C60930).** `node = m_head; while (m_head) { m_head = node->next; delete node; node =
  m_head; }` gives retail's `mov ecx,[esi+8]; mov eax,ecx; test eax,eax; jne`. Testing the member while the node is
  re-read is what produces that copy. `while (node)`, `for (...; node; node = m_head)` and `while ((node = m_head) != 0)`
  all drop the copy (96 bytes). `while (m_head) { node = m_head; ... }` re-loads the head and null-checks the delete.
- **Float clamp feeding an intrinsic (0x00C62FB0).**
  - `float c = x > m ? x : m; return (float)log(c) * k;` splits into two `fldln2; fyl2x` paths, and the floor loads
    as a float (`fld dword [m]`).
  - `if (x <= m) x = m;` merges the paths (42 bytes).
  - `return log(m) * k` folds the floor into a qword constant.
- **Float divide by a constant (0x00C61C00).** VC7.1 /O2 compiles `/ 32767.0f` to the same `fmul [1/32767]` as an
  explicit reciprocal, so either spelling matches.
- **unsigned -> float (0x00C61C00).** The conversion is `fild; jge; fadd [4294967296.0f]`. A `fild` of an unsigned
  local with that fix-up means the source type is `unsigned`.
- **Fall-through switch (0x00C626A0).** `case 1: flag = true; case 2: body; break;` makes the jump-table entry for
  case 2 point into the middle of case 1's block, just past the flag store. `case 5: flag = true; default:` makes the
  `ja` default target land after case 5's flag store. Check the pairing by hand: relocation masking hides the
  table.
- **Constructor that allocates (0x00C61B90).** `m_capacity = n; m_count = 0; m_pData = new T[n];` as body
  assignments schedules the count store before the `operator new[]` call. An initialiser list in declaration order
  stores it after the call.
- **x87 values live across a call are not proof of LTCG (0x00C62370).** Our non-LTCG build also kept `acos`'s result
  on the FPU stack across a call to a same-TU noinline `RandomUnit`. The real lane gap there is the hidden return
  pointer that arrives in esi.
- **Hidden thiscall (0x00C61CF0, seen from 0x00C61F60).** A member that ignores `this` still gets `mov ecx,esi` when
  ecx was clobbered. When ecx happens to still hold `this` (0x00C61FB0), the same callee looks like a `__stdcall`
  function. Prefer the thiscall model.
- **Sign-bit test (0x00C609A0).** `long_member & 0x80000000` gives `test eax,eax; jns`; `< 0` gives `jl`.
- **x87 compare direction (0x00C60450).**
  - Positive `x >= y` / `x <= y` give `test ah,41h; jp/jnp` (unordered fails).
  - The negated `y > x` gives `je/jne`.
- **bool `||` return (0x00C60B30).** `return a || b || c;` returning bool gives `mov eax,1` and reuses the callee's
  zero eax. An `if` returning true/false gives `mov al,1`.
- **Reference to a member (EAX setters 0x00C633C0..).** `float& stored = m[slot].f; if (stored != v)` and
  `unsigned long& flags` keep the slot address in a register. 0x00C63D20 instead needs direct member access in both
  the test and the store.
- **One `else if` chain into one local (0x00C62FF0).** VC7.1 merges the four arms into one block behind
  `add edx, offset`.

Review at landing:
- **Label-agreement WARN.** AddSource 0x00C609A0 and InvalidateAll 0x00C60A30 call 0x00C3C360 as
  `CPKObject::operator new` (the class operator new of the libpk root class, vtable 0x012B5868). The address is landed
  or labelled as `CTCHeroStats::InformOfEvent`, a propagated label. The reloc check may warn: operator new versus a
  plain name. 0x00C3C370 is the matching `operator delete`.
- **Two partial models of CEaxContext.**
  - The setters use `probe/eax_model.txt`: a byte-array vptr, members from +4 to 0x3d8.
  - The list rows use `probe/ctx_model.txt`: `class CEaxContext : public CPKObject` with a virtual destructor.
  - The layouts agree at every offset used. Fold both into one engine header if the class gets one.
- **Errors in already-landed sources.** All are byte-identical; only the names and models are wrong.
  - CLight 0x00C630D0 / 0x00C631E0 / 0x00C63270 (SetLightValue's "voice" model), CSkillTable_00c67470 and the
    CDrawGuildSeal labels are this EAX 4.0 context. SetLightValue's field is the reverb environment diffusion.
  - unattributed ResetMessageList_00c60af0 (CMessageOwner) is the context's reset. Its model types 0x00C60A30 as a
    message factory that returns a node pointer. 0x00C60A30 returns the long status of the MarkAllDirty loop, not a
    node.
  - CEventChain_00c60410::DeleteAll 0x00C60410 is CEaxContext::ClearEvents. Its "destroyer" is the libpk free
    0x00C3C370.
  - CConsoleVariable 0x00C60B90 / 0x00C61B20 is the EAX source (vtable 0x012B9344).
  - CShaderRenderManager::OnPostViewportChanged 0x00C60FE0 is CEaxSource::MarkAllDirty.
  - CLandscapeBackgroundPatch `??_G` 0x00C60AD0 / 0x00C60A00 are the context and source-node deleting destructors.
  - CTCHeroStats::InformOfEvent 0x00C3C360 / 0x00C3C370 are libpk operator new and delete.
  - CRegion::SetMiniMapRegionExitTextOffsetY 0x00C61CF0 is the voice-table lookup. It is a thiscall member; the landed
    CHandleOwner_00c61fb0 models it as `__stdcall`.
  - CHandleOwner_00c61fb0::ReleaseHandle 0x00C61FB0 is the same voice-handle object as 0x00C61F60.
  - CVtable_012b93d4 0x00C62E20 is PKRASlaveObject's constructor (owner at +4).
- **Possible folded class at 0x00C48750.** It is a scalar deleting wrapper that calls 0x00C62750, but
  ~CPKDecompressedStream stores no vtable. Another class with a folded identical destructor probably owns that
  wrapper. Do not attribute 0x00C48750 to CPKDecompressedStream.
- **No direct or absolute references to 0x00C61B40** (CEaxSource::Set) were found in the image. It is probably
  unused or exported.
