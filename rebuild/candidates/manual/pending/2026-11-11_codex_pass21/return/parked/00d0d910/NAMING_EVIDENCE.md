# Identity and naming evidence

Compiled leaf: `?Main@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ`. The final object symbol table is checked against this exact spelling.

- Ego_r name-set entry: `00d11c90	?Main@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ`.
- FableWin name-set entry: `036979e0	?Main@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ`.

Native vtable 012C8E80 slot 1 points to this Main; the owning class vtable is stored by the corresponding Alloc at 00D071F0. Both original PDB name sets and full owner type records establish the private virtual Main. Native OpenGate 00D0EBC0 is invoked through the original CSpawnedFunc2<CQ_BanditCampBossBattleScript,float,CCharString>; its native task vtable is 012C9918 and SuspendableProcess is 00D0EBA0. See ORIGINAL_TYPES.json and the genuine shared CSpawnedFunc2 model.

Original CEntityScriptBase protected reference Game and protected Me, private CIsThingAlive::Thing, CPredicateBase's unary_function base and protected static const Game pointer, and CLevelScriptBase public reference members were restored from both full original types. Existing default header token streams remain unchanged with the new opt-ins disabled.

The true extent is 4752 bytes including the switch table at 00D0EB90. The selected source emits 4672 bytes: its implicit Movie cleanup pattern/frame match, but two hot Timer cleanup expansions are missing. The 25-scenario complete Main fixture passes both compilers. Native HitNumber starts at zero and the unchanged routine only sets it to one; cases 2/3 are present but not falsely claimed dynamically covered. This row remains PARKED, not relocation-matched.
