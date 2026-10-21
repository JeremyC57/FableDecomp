# Chunk 251 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An address-taken counted/def pointer releases differently unless the out-of-line callee (GetDef/GetSubDef) has a same-TU `#pragma auto_inline(off)` stand-in body: VC7.1 then knows the address does not escape and keeps the def in a register (`dec [reg+4]` releases; 0072f170, 007066a0, 0071da70).
- Fast-normalise sums: write `(x*x + y*y) + z*z` so only the squares CSE with a nearby GetLength, not the whole sum (006702e0).
- C3DVector with a user copy ctor: VC drops a dead Z copy but keeps 12-byte stack slots; member-wise `pos.X = ...; pos.Z = ... + 0.1f` copies match retail where the aggregate copy does not (0074baa0, 0074e6e0).
- Two independent trigger flags: `bool a = true; bool b = true; if (x < interval) b = false;` gives retail's spilled-flag ternary (`jge; xor cl,cl; jmp; mov cl,[slot]`) (0074baa0).
- Locals in separate `{}` blocks share stack slots: put each short-lived local in its own block to get retail's packed frame (007849c0).
- The const& idiom also works for component pointers: `CTCX* const& log = GetTC(...)` gives `mov eax,[eax+4]` ... `mov ecx,eax` (007849c0). It is NOT universal: when retail loads a VALUE early (007f1930's joystick device `mov ecx,[edx+eax*8]` before the struct stores) a const& keeps only the address (`lea`); a plain named value local declared before the struct build matches.
- EH constructors need `// Flags: /EHsc`; an empty polymorphic base plus the parent as the derived's first member keeps the argument load after the this-spill (00c14e40).
- An `__int64` member right after a vptr pads the vptr to 8 bytes; model 64-bit UIDs as `unsigned long[2]` when retail has them at +4 (0072f170).
- `if (P.GetPItem()) { T* t = P.GetPItem(); ... }` (null test, then a separately fetched named local) matches the double GetPItem call shape (0072f9e0).
- `Day = Max(Day, today)` taken by reference and written identically in both arms tail-merges like retail (0072f9e0).
- Inline list push_back that reads the old tail once: `SNode* prev = pos->_M_prev;` (007066a0).
- Serialise bodies: an inline `IsSaving()` keeps the save path first; build a CCharString into a named local before assigning to a vector element (RHS first) (00706f40).
- A FindChild result converted to the CObserver base with NO null test means the source dereferenced it into a reference: `CComponent& ok = *screen->FindChild(...)`; `&ok` then converts with a bare `add esi,4` (0055ef00).
- A by-value CCharString return used as `mov eax,[eax]` (the returned pointer) AND destroyed after later locals: pass it into a `__forceinline` helper by const reference (`Helper(GetString())`); naming it (or binding a `const CCharString&` local) reads the slot instead (00561450).
- An empty allocator passed as a default argument `A()` stores a zero byte when the struct is an aggregate; give it an empty user ctor to drop the store (and the register shuffles it causes) (00561450).
- `data == 0 ? "" : data->str` puts the "" constant before the branch (`mov ecx,IMM; je; mov ecx,[eax]`); `data ? data->str : ""` gives a jmp-around form (00561450). Use the GetPString spelling.
- Retail's inline strlen loop (`lea edi,[eax+1]; mov dl,[eax]; inc eax; test dl,dl; jne; sub eax,edi`) is the /Oi intrinsic: declare `extern "C" unsigned int __cdecl strlen(const char*)` with `#pragma intrinsic(strlen)`; a hand-written loop compiles differently (00561450).
- Argument keys built before ANY argument push are named locals (block-scoped when retail destroys them before the next test), not temporaries (temporaries push the ctor's eax) (00561450).
- Inline-asm fistp rounding (0.5/1.0 in ebp-frame locals) is the asm_bake family: park it, do not burn probes (006cc800).

Review at landing:
- 00c14e40 is a constructor under the CTCDiceGame label; the class attribution needs confirming (PDB CTCDiceGame is 24 bytes; the vptr is 0x012B29F8).
- Scoreboard +0x60 naming conflict: landed OnOkClicked 0x0055EE20 says PScoreboardScreen, landed ctor 0x00562EC0 / ResetScoreboard 0x00562E70 say PConnectingMessage; 0055ef00 follows the ctor.
- 00561450's `OpenUploadRequest` helper is a descriptive, force-inlined construct (no such PDB method); the reply string type is the PDB's std::basic_string<char> modelled as a 12-byte malloc-backed LHString.
- The 1.0f pool at 0x01237F00 and 0x00686AE0's +0x34 navigator manager (006cc6b0) are names from this chunk only.
