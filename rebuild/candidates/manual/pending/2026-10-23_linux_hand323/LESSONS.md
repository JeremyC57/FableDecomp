# Chunk 323 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Chain assignment `Value.X = OldValue.X = StartValue.X = v` stores StartValue first, the rightmost target first (0054bbb0).
- A thumb/position helper repeated in every switch case only matches as `__forceinline`. A plain inline helper is emitted once out of line (0054bbb0).
- In size-optimised ebp-frame TUs (`#pragma optimize("s",on)` + `("y",off)`), CWideString `operator+` and the bank/defs directory getters are `__fastcall` with the hidden result in ecx and the first argument in edx. Declare the literals as `const unsigned short*` (004336bc). In the same TUs a dead parameter slot (`[ebp+8]`) is reused for string temporaries (0048fab6).
- A nested-if error tail (`if (ok) { ... return true; } Error(...); return false;`) lays the error calls out after the main body as in retail. Early-return forms invert the layout (009e55a0).
- `__forceinline` CTimer accessors are needed in large script-thread bodies. Plain inline accessors stay out of line once the body is big (00e998f0, 00e2a320). `#pragma inline_depth(0)` after the first exit keeps ~CScriptThing out of line (00ec0ba0).
- A CRGBColour passed by value is built in place in the push slot when the class has both a body-assign ctor (R,G,B,A) and a user copy ctor.
- With /GS (007ec710), retail gives every return its own epilogue, with the cookie check before the register pops. Neither the RTM nor the QFE-4035 toolset reproduces this from one source, and duplicating the return tails is worse. Treat it as a lane gap until a source form is found.
- `HasTC(x)` results kept as `shr; and al,1; mov [esp+x],al` in a dead stack bool mean the original assigned to a named bool local. Our folded `test byte` form is shorter (0078cd80, 007e9460).
- A bare `fistp` (often `fistp; fild` pairs) is an inline-asm ROUND and is a lane gap; it also forces an ebp frame. Rows: 0079d360, 0078d250, 0041afa0.
- CVectorMap inside CTCBoastingPodium is 16 bytes: a 12-byte vector plus a 4-byte pad (00813d00).
- The fast inverse-sqrt table sits at 0x013CE558. STLport `push_back` is inline with `_M_insert_overflow` at 0x0066E370.
- An 8-aligned frame (`and esp,-8` in an ebp frame) in our build when retail has a plain esp frame means a double/`__int64` local we introduced, for example a `floor()` result kept as double (00ab7c20, 007fe040).
- Retail code using `fld qword [K]; fstp dword [esp]` to pass a double constant to a float parameter means the source passed a non-folded double object (a global double), not a literal (00490494).
- Same-TU `auto_inline(off)` stand-ins for small getters (GetSubDef/GetEffect) can be closer than extern declarations (007a76f0).

Review at landing:
- No row needed more than 4 grades per body.
- Local base models next to existing engine headers: CStaticFontBank (00ab7c20, a POD header with no methods), CSubtitleRenderer (0048fab6/00490494: retail offsets do not fit the PDB header), CTCBerserk, CTCBulletTime and CTCCombat drafts use local layouts. All are parked, so none is landed.
- Landed twin 009df060 (CStateBlockFunction ... Sold) regrades DIFFER in this lane (register tie). Check it before relying on it.
- Lander warnings on matched rows: RELOC_CONFLICT ??0/??1CBaseIntelligentPointer (0099a380/0099a430 here against 00a01b10/00a01c10 in landed 0044a5a0/00692f30). RELOC_NAME warnings: Release -> 00cd2770 is labelled CMemoryDataOutputStream::Clear; ~CIsThingAlive -> 00e241b0; ~CResourcePImp -> 006e7ab0. No RELOC_IDENTITY lines.
- Catalog row 00548b50 holds two functions (split at 0x00548F40). 00c61610 is likely mislabelled (object 0x384 bytes against the PDB's 0x240).
