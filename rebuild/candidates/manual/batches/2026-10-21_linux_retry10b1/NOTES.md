RESUME: (pending)
- 0072a9b0 / 0072aa00 / 0072aa50 / 0072aaa0 / 0072c680 / 0072a970 (CTCSpecialAbilities::Setup*): lane gap (shared advisor OPEN, 30 probes): retail `mov ecx,[esi+18h]` right after the CCharString ctor, before `push 0 ; push 1 ; lea eax,[esp+0Ch]`; ours loads it after the pushes; every form that moves it also changes the condition code.
