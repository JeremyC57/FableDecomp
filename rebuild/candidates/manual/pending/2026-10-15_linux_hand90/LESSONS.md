# Chunk 90 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Counted def-pointer copy (0x0057A939):** copying a holder's counted `CDefPointer` member into a local must go through an inline holder accessor that returns the pointer BY VALUE (`def = GetDefinitions()->GetHeroStatsDef()`).
  - Copy-constructing straight from the member binds a reference: size TUs emit `add eax,0DCh ; mov ecx,[eax]` and speed TUs a dead `add eax,0DCh`, where retail has `mov ecx,[eax+0DCh]`.
  - AddRef/Release must be inline members of the POINTEE. A `--Object->RefCount` written in the pointer's destructor schedules the `dec` above the value read.
  - Under `optimize("y",off)` + `("s",on)` the accessor must be `__forceinline` (0x0057A90B); otherwise it is called out of line with the frame word as the hidden result.
- **Inline templates under the y-off/size pair (0x0057B3E2):** the inlined out-parameter `GetInterface(T**, type)` template also needs `__forceinline`; plain `inline` is emitted and called. A `xor ebx,ebx` before the bitset test means the caller null-initialises the out pointer.
- **Index loops (0x0057AA5B, 0x0057AAA8):** a vector index loop whose entry test is `test edx,0FFFFFFFCh` and whose exit is `cmp ; jne` is `for (i = 0; i != v.size(); ++i)`. Written `<` it gives `sar edx,2 ; je` and `jb`. With nothing stored in the body, size() and the first pointer are hoisted after the entry test.
- **Size guards (0x0057AB6A):** a size guard whose zero result sits at the END (`sar ; je` to a trailing `xor eax,eax`) is `if (v.size() > 0) { ...; return ...; } return 0;`. The early-out `if (v.size() == 0) return 0;` emits `test ecx,0FFFFFFFCh ; jne` with the zero arm first.
- **Switch into a zero-initialised index (0x0057A8AF/0x0057A8DA):** a dense enum-to-index switch whose case 0 is `xor edx,edx ; sub eax,edx ; je` (the zero register reused for the test) writes into a zero-initialised local (`long i = 0; switch (s) { case 0: i = 0; break; ... } return i;`). Per-case `return` gives `sub eax,0` and a separate `xor eax,eax` default.
- **Global read before a call (0x007BED90):** a global that retail copies into a frame slot before a call (`mov eax,[g] ; mov [esp+8],eax ; call`) and later `fild`s from that slot is read into a named local first. Inside the expression, VC7.1 hoists the call and `fild`s the global afterwards.
- **Statement order inside FPU code (0x0057A82A):** an integer `inc` placed between `fsub` and `fmul` comes from statement order. Put the partial FPU result in a named float local before the `++counter` statement.
  - Counter bumped before the call: the `inc` is hoisted above the earlier store.
  - Counter bumped after a pointer local: the `inc` lands between `fld1` and `fsub`.
- **Message temporary (0x0071E130):** a stack message object whose (out-of-line base) destructor runs right after the send call lives in its own `{ }` block. Declared in the enclosing `if` body, its destructor sinks to the end of the block. The derived class needs no destructor (no vtable store before `~CBase`).
- **`dx*dx + dy*dy` order (0x0057ADF7):** the order needs the 24m rule, `d = dx*dx; d += dy*dy;`. The one-expression form squares the top of stack (dy) first, even with both differences in named float locals.
- **C3DVector copies (0x0057CB4B):** a C3DVector copied through eax/edx into a frame pair has a memberwise user copy constructor (float members copied without arithmetic stay integer moves in VC7.1). The implicit copy is a `movsd` block through esi/edi in a size TU.
- **Two-level type test (0x0057ACC4):** a result with two outcome blocks where the no-flag case comes first is `if (!flag) return a ? 1 : 0; return a ? 2 : 3;`. The ternaries give `setne` and `neg ; sbb ; add 3`.
- **Size pragma and colouring (0x0057ABD6):** `#pragma optimize("s", on)` alone turned the speed colouring (edi = this, edx/eax pushes) into retail's (ebx = this, `push dword ptr [esi+14h]`). Try the size pragma before reordering locals.
- **Out-of-line copies of inline bodies (0x007497C0):** a row with no callers that some vtable slot inlines byte-for-byte is the out-of-line copy of an inline member. Name it from the slot's class (CTCNoise), never from the propagated manifest label.

Review at landing:
- **Rows that needed more than 4 grades:** none. 0057a962 and 0057adb8 stopped at 4. differ/0057a962.cpp holds an UNGRADED form (the by-value accessor that matched its byte-twin 0057a939); re-grade it first, it should land.
- **Local models next to an existing engine header:** CTCHeroExperience (006d8b00, 006d8ca0), CTCHeroStats (all 18 matched rows), and CTCHeroRangedAttacks (unused: 004e30e7 is really CTCDExplosion). Both headers carry PDB offsets that disagree with retail after the first container (12-byte vectors/maps, 4-byte lists). The headers would need a retail-layout variant before these rows can include them.
- **Errors in already-landed sources:**
  - `rebuild/src/compiled/00/57/CTCHeroStats_IsDrunk_0057a874.cpp` uses `engine/CTCHeroStats.h` and compares `BestScoreCoinGolfOakVale` (PDB +0x19C). In retail +0x19C is Drunkenness (PDB +0x1B4), so the bytes match but the member name is wrong.
  - `rebuild/src/compiled/00/57/CTCHeroStats_UpgradeRenownLevel_0057cae9.cpp` is a `__declspec(naked)` `__asm` body (asm_bake) sitting in compiled/.
  - The "CTCHeroExperience::AddSuccessfulBlockParry" label is propagated over many unrelated bodies; this chunk's four were CTCNoise, CTCShotDefinitionBase and an audio-format record. The landed files under that name (00/57, 00/65, 00/6c, 00/6d, 00/c0) deserve an attribution check.
- **Catalog rows holding several functions:** none in this chunk.
- **Lane gaps:**
  - 00c2d800/00c2d820 are link-time-code-generation bodies: callers assume the callee preserves ecx/edx.
  - 00771d50 is 4 dead frame bytes.
