# Chunk 812 lessons (GOTCHAS one-liners)

- MovieBase dtor residue (chunks 810 / 812): retail expands the implicit `CScriptGameResourceObjectMovieBase` dtor (call 0x006E7AB0 + GROBase vptr 0x0126008C + call 0x0099A430). Plain header = COMDAT 0x006E7B80 call; MOVIEBASE_FORCEINLINE = extra MovieBase vptr store. Still open.
- A row-local `inline ~CScriptThing() {}` also expands inside a forced inline parent dtor (~CIsThingAlive); retail calls it there but expands it at the plain temporaries (nested-inline gap, 00cf4ec0).
- VC7.1 cross-jumps identical post-loop `if (IsActiveThreadTerminating()) return;` blocks of sibling branches; do-while(0), TRetVoid `_Fail_Val` returns and for(;;) vs while do not stop it (00d13580).
- `__forceinline ~CTimer` expands every exit; retail's budget expands only some (2 of 3 in 00d0d910). Use it as a probe, not a fix.
- Distance and float compares: write the operand order retail loads (`IsDistanceBetweenThingsUnder(GetHero(), thing, d)`, `GetDistance(Me,Home) < GetDistance(Hero,Home)`). A swap costs 16-30 lines.
- A by-value struct with a user ctor / operator= but an implicit dtor lets VC expand the dtor at one exit and call the COMDAT at another, as retail does (TeleportData, 00d13580).
- To share a frame slot between two locals (and the exit pad), give the second its own block scope `if (x == 4) { ... }`, not `if (x != 4) return;` (00ea59e0).
- In a long quest Main, `angle = rand()...` as its own statement; folded into the call expression VC loads Game first and pins it in ebx (00d2aa40).
- `offset += GetHero().GetPos()` in place matches retail's X/Y/Z store-back; a temporary sum does not (00d2aa40).
- Catalog name of 0x00D15140 (CMainGameComponentInit ctor) is wrong: it is TeleportData::TeleportData (24-byte layout, four CCharStrings at 0/0xC/0x10/0x14).
