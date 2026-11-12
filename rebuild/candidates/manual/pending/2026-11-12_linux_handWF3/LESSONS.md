## 0x009DC6E0

Draw2D* vertex rows: a second/third load of the screen RHW float with Z stored as a constant just before it is the inline CGenericVertex::SetRHWFromZ() folded on a known Z, not a plain RHW = g_ScreenRHW (0x009DC6E0); U/V constants stored before the first RHW read stay immediates.

## 0x00761980

- (wf3 0x00761980) An "iterator in a frame slot / constant in bl" residue of a draft with local class models went away by itself on the shared headers (typed STLport iterator, `const CWorldMap&`, PDB const refs): port first, then read the residue.
- (wf3 0x00761980) `bool NShape::CShape::GetStartPoint(C3DVector&) const` (FableWin.pdb only) is inline in retail: `cmp begin,end ; je` + `mov eax,edx` + a 12-byte block copy into the caller's vector is its signature (0x00760910 matches byte-exact with it; no by-value operator- needed).
- (wf3 0x00761980) VC7.1 forwards a struct-assigned C3DVector into later field reads for every spelling inside one basic block (copy ctor, operator=, memcpy, reference out-param, escaped local, array[1]); it keeps the classic `ecx/edx/eax` block copy and reloads the fields only when the reads sit after a join (or under /Op). `/ 1.0f`, `__assume`, `__noop`, volatile and C3DVector alignment 1 do not produce that shape.
- (wf3 0x00761980) VC7.1 does not fold a second `Points.empty()` test that follows an identical one in another block (begin is reused, end is reloaded): a redundant-looking test in a draft stays in the bytes.
- (wf3 0x00761980, pass 2) An inline ctor that retail calls here but expands in a sibling of the same TU must be VISIBLE and out of line in the caller (`__declspec(noinline)` on the in-class body): with a bare declaration VC7.1 schedules the colour temporary's stores before `push &colour` (CEnginePrimitiveLine 0x004C3090).
- (wf3 0x00761980, pass 2) VC7.1 forwards every plain float copy out of a 12-byte struct copy in the same block (even after `/ 1.0f` late folds); a homed-and-reloaded component needs an x87-evaluated stage (PAREN-grouped `(X+other.X)` in operator+, `(float)(x*1.0)`) or a block boundary. Retail has only 16 same-block store/reload sites.
- (wf3 0x00761980, pass 2) The /Od size of an inline in FableWin.exe.signatures.tsv (bytes + instruction count) is a cheap oracle for its body: GetStartPoint 67 bytes / 26 instructions = early return + implicit block operator=.
- (wf3 0x00761980, pass 3) Components of a just-copied struct that retail RELOADS from the frame as dwords (`mov [slot],r ; mov r2,[slot]`, block copy left unsplit) mean another value took a scratch register between the copy and the reads: VC7.1 hands out eax, ecx, edx in rotation in evaluation order and only forwards a value whose register is still intact. Read retail's registers as that rotation, find the load that sits in the gap (here `mov ecx,[edi+4]`, the parent thing) and name it in a local BEFORE the reads (`CThing& parent = GetParentThing();`): 381v389 to RELOCATION_MATCH.
- (wf3 0x00761980, pass 3) Order of memberwise float copies out of a block-copied struct decides the reloads: ascending (X then Y) forwards both, Y before X copies Y register to register and reloads X, Z,X,Y as pure copies reloads X and Y. Use it as a quick probe of the rotation before looking for the missing statement.

## 0x00A89740

TransformPoint residue that is only epilogue scheduling (`mov eax,[result] ; pop esi` a few x87 instructions early, same instructions): spell the inline rows as plain left-to-right sums in retail's evaluation order (`E3k*zz+E2k*yy+E1k*xx+E4k`), not the parenthesised form -- PAREN tuples move the cut (0x00A89740).

## 0x009122A0

0x009122A0: a square that retail loads early and spills to a dead argument slot is an inline GFSqr(expr) (argument evaluated before the object expression); four extra component spills in a final vector sum = named result local returned by value.

## 0x00BE7BE0

- (wf3, 0x00BE7BE0) A pointer stored twice to the same frame slot = two by-value parameter copies of nested inline helpers whose address goes to push_back(const T&); look in FableWin.pdb for by-value `Add...ToRenderList(Q..., bool)` helpers before modelling a local.
- (wf3, 0x00BE7BE0) A store that sits before the branch of a test (`cmp ; mov [slot],reg ; jbe`) says the test is INSIDE the inline helper (after its parameter copy), not at the call site.
- (wf3, 0x00BE7BE0) `je far ... far: add ecx,K ; jmp back` + the element size hoisted into a register = two expanded push_back statements cross-jumped (if/else); a list select `(c ? A : B).push_back` gives `add ; jmp ; add` and an immediate.
- (wf3, 0x00BE7BE0) Unlisted code right after a row's COMDATs can be the unreferenced out-of-line copy of a header inline (0x00BE9030 / 0x00BE9080): xref the push_back callee to find it and read the helper's real body there.
- (wf3, 0x00BE7BE0) VC7.1 expands an inline helper at a constant-argument site but calls it at a variable-argument site; retail expanded both -> FABLE_FORCEINLINE in the shared-header opt-in (inline budget).
- (wf3, 0x00BE7BE0) vector _M_insert_overflow declaration-only vs body visible moves address-taken inline-parameter copies between dead argument slots.

## 0x0081F240

- A "register pressure" residue in a CMap cell-copy row (HeightMap reloaded per access, base spilled to a dead arg slot) is the PDB inline Get/Set*AtIncludingExtraCells helpers (value through an unsigned char param, coords through C2DCoordI temporaries), not raw member expressions; loop counter signed (`jl`) even when the helper param is unsigned long.

## 0x00646580

A callee that returns a class by value and is defined earlier in the same retail TU: put its real body in the row TU (auto_inline off) -- VC7.1 then knows the hidden result pointer does not escape, so the receiving named local's frame slot is reused by later temporaries (row 0x00646580: uv temporary at [ebp-8] instead of its own slot).

## 0x0052C920

a colour difference that retail computes with `mov al,t ; sub al,[c]` for R/G and registers for B/A (stores R,G,A,B) is `CRGBColour d = target; d.R -= c.R; ...` over the Initialise-based copy constructor (FABLE_RETAIL_CRGBCOLOUR), not a 4-argument constructor of the differences (row 0x0052C920)

