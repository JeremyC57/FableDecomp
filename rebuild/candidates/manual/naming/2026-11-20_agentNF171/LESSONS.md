# NF171 lessons (one line each)
- Census first: two of the "wrong binder" lists in the entry texts were stale; the HEAD compile probe (195 rows, ~20 min) cut q4477 / q4768 / q5207 to zero or one row.
- REFS hits can be byte coincidences: 6 of 10 q11767 "referencers" were the operand bytes `60 52 6A 00` (`[esp+60h]; push edx; push 0`); disassemble the hit VA before probing.
- A helper script named `dis.py` shadows the stdlib `dis` and breaks capstone's import; name scratch tools uniquely and run them with `python3 -I`.
- The CThing_Retail script-interface rows carry a giant unused local-model preamble; the port is the body only on CGameScriptInterface.h (NSCRIPT) + CScriptThing.h, dropping the preamble (0x00891FF0, 0x0088E1C0).
- `CTBaseSingletonNoDefaultAlloc<T>::Get().M().N(*x.vcall())` hoists the PClass load before the vcall; a named reference for the argument puts it back (0x00891FF0).
- CIntelligentPointer<T>::GetPItem returns CBaseObject*; use the FableWin inline `Get()` for the typed pointer (0x00903920).
- A same-TU inline callee another row owns strongly (PlaceInMapwho 0x0071A710 in MoveInMapwho 0x0071A750): write its body out at the one expanded site instead of defining a second strong copy.
- pass.py-style STATUS updaters must normalise the `0x`-prefixed uppercase address key against vland's lowercase log rows.
