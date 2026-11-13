# Coordinator's ideas for the continuation on 0x007F2DC0 (leads to measure, not facts)

Retail's prologue: `mov ecx,[ebx+24h]` (PDef) / `cmp ecx,edi` / `je` (null) / `fld [ecx+40h]` (LifeTime) /
`mov eax,[ebx+28h]` (Timer, loaded BEFORE the `LifeTime > 0` branch) / `fcomp 0.0` / `mov [esp+18h],eax` / ... / `jne` /
`fild [esp+18h]` / `fild [FPS]` / `fmul [ecx+40h]` / `fcompp`.

So Timer is read AFTER the PDef null test but BEFORE the `LifeTime > 0.0f` test, and kept as a copy. That is the shape
of an argument evaluated at a call site of an expanded inline whose body holds the whole lifetime test:
1. an inline member of the def taking the timer BY VALUE, e.g. `PDef->IsLifeTimeOver(Timer)` /
   `bool CLightningDef::HasExpired(long timer) const { return LifeTime > 0.0f && timer > LifeTime * FPS; }` -- look in
   the Ego_r / FableWin name sets and the FableWin per-TU COMDAT list for CLightningDef / CTCLightningNode inline members
   (and for sibling Defs with a LifeTime: the same helper may be landed elsewhere, e.g. the landed
   CCreatureAction_SpiritAttack::FrameUpdate 0x00854B90 the notes cite);
2. a by-value parameter of a DIFFERENT integer type than the member (long member -> int / unsigned parameter, or an
   unsigned member -> long parameter): a conversion node at the inline boundary may stop c2 forwarding the copy;
3. a free inline helper `GFIsTimeUp(long timer, float seconds)`-style with both arguments evaluated at the call;
4. `Timer++` / post-increment used as a value is NOT it here (the increments are elsewhere), but check whether the
   branch loop's `(*it)->Timer++ % ...` idiom has a twin on the node's own Timer.
Check each as a free peek on the prologue only (does the `mov [esp+18h],eax` copy survive?) before any grade.
