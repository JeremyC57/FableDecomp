# Chunk 651 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CDefPointer null test: `if (!p.IsNull()) return f(p->x); return v;` (non-null path first) gives retail's `test eax,eax` on the IsNull read and a separate `operator->` re-read; the inverted early return lays the blocks out the other way (0x006817C0).
- Byte-identical if-arms only cross-jump when they are register-identical: a named `const unsigned long index = ...;` local per arm does it (0x0070BF50).
- Safe linked-list scans: write the scan helpers to re-read the stored node (`s->Current = s->Next; ... s->Next = s->Current->Next`), not the parameter -- otherwise VC7.1 keeps a `mov ecx,eax` copy of the node; a list-side RemoveScanInfo (store through the list `this`) lets the empty-list path thread into the no-previous branch; the scan-info resets at its end are dead and vanish when the scan info dies right after (0x00446330 vs 0x00A0D6B0).
- Local declaration order sets the order of the zero-init stores VC7.1 sinks to the first use: retail's `bestPriority = 0; found = false; scan.Started = false` needed the scan-info local declared after the other two (0x00446330).
- x87 order for `pos += jitter * k` with Z dropped: retail is the in-place `jitter *= 0.25f; jitter.Z = 0.0f; pos += jitter;` (fld/fmul X then Y, fxch, fadd [pos.X], fadd [pos.Y], pos.Z self-copy); a C3DVector(x, y, 0) temporary evaluates Y first (ctor args right to left) (0x00918B90).
- A global read before a call with the arithmetic after it (`mov ebp,[g]; mov ebx,edx; call; add ebx,3; imul ebx,ebp`) is two named locals read before the call: `r = Random(6); fps = g_FramesPerSecond; end = GetWorldFrame() + (r + 3) * fps;` (0x00918B90).
- A by-value float parameter of a same-file inline helper gives the argument a temporary home that later locals share (the `const float&` argument then points at it); a function-scope local keeps its own slot (0x0078A690).
- `const float& t = cond ? a.x : a.y;` gives the address select (`add eax,10h; fld [eax]`); a plain float ternary gives `fld/jmp/fld` (0x00490494).
- A C2DVector(x, y) constructor for a position built as `Y = h - f(); X = ...` reproduces retail's order (Y computed first, X stored first) (0x00490494).
- GFVXToAX / GFVYToAY (0x009E1E10 / 0x009E1E40) are `__fastcall float(float)`: the callee pops the float; declaring them cdecl adds `pop ecx` (0x00490494).
- Tolerance literals: the prior's 0.00277778f (0x3B360B6A) was really 0.0027777f (0x3B360A13); ud.py-style masks that treat every >=0x400000 immediate as an address hide float immediates -- check `push imm32` floats by value (0x00918B90).

Review at landing:
- 0x00446330 needed 14 probes; it reuses the landed 0x00A0D6B0 local CAInputProcess / CLinkedListScanInfo model next to engine/CAInputProcess.h (header debt), and its StartScanAtHead zeroes Current/Next/Previous on an empty list where the 0x00A0D6B0 copy returns early -- both copies should converge on one shared model.
- 0x00446330's RemoveScanInfo now ends with the scan resets; check 0x00A0D6B0 still matches if the two models are unified.
- Already-landed 0x0094BB20 (CAIStateGroup_MinionHawkingFromStall::Update) uses an invented test-less `~CDefPointer() { Object->Release(); }` and a noinline stand-in body for CDefClassBase::GetSubDef -- not the engine header; 0x00918B90 shows the same test-less normal-path release, so the real cause is still open.
- Already-landed 0x00B677D0 (CEngineLandscapeRenderer::SetupForegroundStates) uses `__forceinline` on the render-state setters (not PDB-attested).
- engine/CIVCountedPointeeBase.h's FABLE_RETAIL_DEF_CLASS_BASE branch uses FABLE_STATIC_ASSERT without including rebuild_abi.h (include it first).
- 0x0070BF50: our anonymous-namespace hash is ?A0x9ae88543 (retail A0x22a65229); the leaf prefix grades it, the landing must bind the PDB spelling.
- 0x00446330's CProcessedInput ctor writes the four CGameEvent fields through a GameEvent(i) accessor because CProcessedInput.h flattens CGameEvent[4] to bytes.
