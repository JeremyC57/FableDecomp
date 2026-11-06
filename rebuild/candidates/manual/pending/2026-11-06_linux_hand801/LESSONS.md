# chunk 801 lessons (GOTCHAS one-liners)

- Same-TU `#pragma auto_inline(off)` + `__declspec(noinline)` stand-in bodies for out-param callees (writing only the out params) tell VC7.1 the caller's locals are uncaptured: def pointers stay in registers and `vector.size()` hoists out of loops as retail does (0x7A7D20 560 -> 86 lines, 0x850A80 585 -> 383).
- Retail can expand one instance of a helper inline while the others stay calls (the last GetEffectFromString, one CVectorMap::LowerBound): model that instance with a separate `__forceinline` helper (0x8DDAD0 precedent).
- A block scope around one search changes VC7.1's frame packing (0x7A7D20 0x150 -> 0x140; 0x833C00 0x78 -> 0x88 = retail by unscoping only the line block): try scopes per block when the frame size differs.
- `C3DVector(a,b,c)` and the header operator- compute Z first; memberwise assignment computes X first. A named C2DCoordI with an inline box ctor converts X first; an out-of-line C2DBoxI(const C2DCoordI&, long) converts Y first whatever the spelling.
- VC7.1 narrows a bit-16 bitset test to a byte load (`mov cl,[eax+2Ah] ; and cl,1`); retail keeps `mov ecx,[eax+28h] ; shr ecx,10h`. No HasTC spelling tried (shift-mask, named word, const&, forceinline, std::bitset::test) avoids it.
- Epilogue cross-jumping: VC7.1 merges identical return tails into the FIRST copy, retail into the LAST (0x7DB6A0 case 0x1c, 0x850A80 case 5); return-v-break reorders change which copy wins.
- The q.py base flags have no /Gr: free callees that retail calls with ecx/edx must be declared `__fastcall` explicitly.
- A `#define FABLE_RETAIL_*` opt-in must come before the first include that pulls its header in (transitively), or the branch is silently skipped (0x7A7D20 a8 SRC_FAIL).
- pack(1) class models need explicit pads where the PDB has alignment holes (CTCBattleCharge `_pad3b`, `_pad6e[2]`); assert offsets.
- vland RELOC_NAME on a stand-in global: respell it as the landed static member + inline accessor (g_framesPerSecond -> CMainGameComponent::GetConstantFPS()); name-only.
