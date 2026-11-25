g0: baseline = hand1014 draft as-is (expect DIFFER 278v264)
g1: Ego_d-shaped port (locals order anim_to_ignore_iterator, found_anims, index, start_it; for-loop scope it; two returns, index from GFRandom)
g2: g1 + visible out-of-line _M_insert_overflow body (OVERFLOW_OUTOFLINE instead of DECL_ONLY): retail does not reload it (edi) or finish (esi) after the call/store, so c2 must see the callee body (sibling 0x005DCB00 rule)
g3: g2 + visible out-of-line reserve body (STLP_VECTOR_RESERVE): retail keeps found_anims._M_finish in esi across the loop, so c2 must see reserve does not capture the vector
g4: g3 + anim_to_ignore_iterator declared first without initializer and assigned Anims.end() after the vector (Ego_d: decl order puts it first; line 1006 = vector ctor 15 B, 1007 = 19 B assignment, 1008 = reserve 10 B); retail zeroes the vector before reading end()
g5: g4 + STLport _vector.c reserve form (RESERVE_ALLOCATE_AND_COPY + STLP_VECTOR_C, as landed owner 0x0077E580): retail reserve copy 0x005E15E0 has the push ecx word and capacity()<n compare; leaf must stay RELOCATION_MATCH
final: 005dcc00.cpp = g5 + header comment (strict headers)
mutation controls (test check, not grades): mut1 key == -> <=, mut2 drop avoided assignment
g6: FableWin.exe (/Od, same source) order: ASSERT(AnimExists) first, vector ctor, THEN anim_to_ignore_iterator constructed from Anims.end() (EH state 4 after the vector's 3), reserve(5); anim_entry is a pointer ([ebp-4Ch] = &it->second, then *anim_entry to the filter, ->BankIndex); assert before the empty return; index initialised from GFRandom. Replaces g4's declare-then-assign fit.
final2: 005dcc00.cpp = g6 + header comment; test F fixed (key 7 past end)
mutation control (not a grade): mut3 drops the end() test
