# Chunk 315 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A user `__forceinline` dtor (CTimer) plus `#pragma inline_depth(0)` around returns splits the dtor copies: inline at plain early returns, out-of-line (0x00CD4470) at the pragma returns and closing brace (00e02e50).
- Script wait loops: put the first IsActiveThreadTerminating check at the top of `for(;;)`, not as a guard before the loop plus a check at the bottom. This fixed the register choice (00e02e50).
- A function-final `if (IsActiveThreadTerminating()) return;` folds down to a bare IsDone call (00d2c810).
- Script handle locals of exact type are devirtualised: CScriptThing::GetPos (slot 6) and the float slot 10 inline as `PImp.Data == 0 ? default : PImp.Data->Get()`. Write the empty-handle default FIRST; `Data ? call : default` reverses the branch (00e4e0e0).
- An inline CCountedPointer::operator= must read the source pair (`T* d = o.Data; Info* i = o.Info;`) BEFORE the `Info != i` self-compare. It also needs `__forceinline` on both it and the CScriptThing operator=; otherwise VC7.1 calls an out-of-line implicit operator= (00e4e0e0).
- A float compare whose retail computes the RIGHT-hand distance first: write it as `dist(Me,hero) > dist(blockWay,hero)`. VC7.1 evaluates the left call first and `>` gives `test ah,5; jp` like `<` with the operands swapped (00e4e0e0).
- `CDefPointer::operator->()` (an inline accessor) gives `mov eax,[this+off]; push; lea ecx,[eax+field]`, where `.m_pDef->` gives `mov ecx,..; add ecx,field` (00847520 / 00846710).
- A by-value CIntelligentPointer argument built from a temporary is constructed directly in the argument slot (`sub esp,8; mov ecx,esp; ctor`) only when the class declares a copy constructor. Without one, VC7.1 builds a temporary and copies it (00847520).
- 0x006925C0 takes no arguments. The `push frame+1` before it belongs to the outer constructor, so count the 8 constructor arguments by `ret N` before trusting the pushes.
- CWideString/`L""` under these flags: wchar_t is not native, so declare `CWideString(const unsigned short*)` and cast the literal.
- `$S/w17/xdis.py` takes the length in HEX. A decimal length silently over-reads into the following functions and inflates x87/call counts.
- Writing `if (a && b) {...} else if (c) {...}` form fixed a mis-structured `if (a) { if (b) } else if (c)` where retail's failed b falls through to the c test (00847520).

Review at landing:
- 00e4e0e0: the class name in the source is a placeholder (CTollBanditScript). The leaf HandleBanditToll is kept; confirm the entity class from the 0x00E4B5B0 Main before binding the symbol.
- 00846dd0 / 00847520: same-length register-priority misses, routed to the R&D register round. Their sources are complete reconstructions and good bases.
- Ten "not reached" rows hold skeleton sources only (decode notes in the headers). They are budget parks, not lane gaps.
