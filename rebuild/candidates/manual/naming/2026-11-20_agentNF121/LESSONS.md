# NF121 lessons (one line each)
- A family entry with many REFS rows is mostly done already: one HEAD recheck with a per-object binds dump (work/probe.sh) cut 52 rows to 9 wrong binders + 4 self leaves before any edit.
- The exe-scan REFS list can carry byte coincidences (0x004804DE: `or [ebp+60h],10h ; push 0` = 60 10 6A 00): a REFS row whose object binds nothing at the address is not-a-binder; disassemble the hit to say so.
- Pointer -> const reference respells are byte-neutral; make the local model's helper loop and begin()/end() const in the same edit or the const member fails with C2662.
- A regex respell of `Fn(arg)` must skip other overloads with the same name (IsCarryingThing(long) got a stray `*`): diff every transformed row before grading.
- Read the self row's old leaf from a HEAD object (objdump -t defined externals) when the row has no `// Leaf:` line; the entry's wrong column may not list it (0x006A1490 was QAE, not the queued QBE).
