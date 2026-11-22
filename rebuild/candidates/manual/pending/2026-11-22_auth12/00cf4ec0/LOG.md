# 0x00CF4EC0 CFanMarker::Main -- AUTH12 log (residue = DIFFER lengths; masked = mdiff.py DIFFINS)
- c0 (AUTH10 c0 = AUTH1 c0) no flags: DIFFER(6244v6164) (reproduces).
- c0s = c0 + family flag /d2inlS15 (both landed fan Mains 0x00CF2B70 / 0x00CF3D20 use it for the implicit movie dtor): DIFFER(6144v6164), masked 308 (mostly 4-byte frame-offset lines: frame 0xe0 v 0xe4).
  Retail census: 0x004ABE90 (CScriptThing copy) called x1 at the top, 0x004AA840 x7, 0x00CD4470 ~CTimer x1, 0x00CD4450 never, 0x006E7B80 never.
- p1: hypothesis: pure port (ETextGroupSelectionMethod.h, fable_script_distance.h; bare __forceinline -> plain inline: ~CIsThingAlive, CTimer::operator=, GetTime)
00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6176v6164) flags= []
- p2: hypothesis: p1 + /d2inlS15 (family flag)
00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6064v6164) flags= ['/d2inlS15']
- p3: p2 + CTimer operator=/GetTime forced via overlay opt-in FABLE_RETAIL_CTIMER_FORCEINLINE_SET_GET (hdr/engine/CTimer.h; ruling 2026-11-21 (a)), row-local copies removed
p3 00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6096v6164) flags= ['/d2inlS15'] DIFFINS 321 len 6096 v 6164
- p4: p3 + 0x00DE6C20 predicate device (CPREDICATEBASE_NOVTABLE; inline_depth(0) region round the CIsThingAlive dtor/ctor; retail top: no base vtable store, copy 0x004ABE90 and ~CScriptThing 0x004AA840 called inside the expanded predicate)
p4 00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6096v6164) flags= ['/d2inlS15'] DIFFINS 294 len 6096 v 6164
- p5: p4 + Ego_d locals (eglocals): TRetVoid _Fail_Val (BP -69) + 'return _Fail_Val.Return();' at all 22 returns; names OakValeFan / seh_OakValeFan / seh_BowerstoneFan / seh_HookCoastFan / ChatLevel; one 'long Conversation' per switch block (Ego_d blocks len 2811 / 1611), HLStyleConversationIndex only in the loop-1 hit else-arm (block len 530)
p5 00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6096v6164) flags= ['/d2inlS15'] DIFFINS 294 len 6096 v 6164
- p5 VC10 /Od (odall10 fablelib): total 11488 v Ego_d 11940. Line calibration: Ego_d 118-B lines = `if (T) return _Fail_Val.Return();` (ours 18+100);
  712:140 = SetPredicate+NewScriptFrame+if(T)return (82+27+18+13). Misfits: Ego_d ChatLevel lines 9 B (ours `= N` 7 B; 9 = mov/add/mov =
  ChatLevel++) in loop-1 cases 0..12 and loop-2 cases 0..6, case 13 `= 12` 7 B; after loop 2 Ego_d 962:118 (an if(T)return ours lacks) and
  963:278 (ours final while 45).
- p6: p5 + ChatLevel++ (20 sites) + ending `if (T) return; while (!false) { NewScriptFrame(); if (T) return; } if (T) return;` (the generated
  ending of landed 0x00D706E0 / 0x00D2D930). VC10: 962 = 18+100 = 118 = Ego_d; 963 = 13+27+18+100+5+18+97 = 278 = Ego_d; total 11912 v 11940.
p6 00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6088v6164) flags= ['/d2inlS15'] DIFFINS 270 len 6088 v 6164
- p6d: p6 without /d2inlS15 (default S9): readout shows ~CTimer scored with retail's exact per-site split; grade:
00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6180v6164) flags= []
  -> p6d DIFFER(6180v6164), mdiff 102 (best). Exit pads now match retail's grouping (0223/0299/0824, 1121/11c6, 1092/113d/11e2, 1076
  pads); left: movie dtor COMDAT call 0x006E7B80 at 4 loop-2 pad sites + one extra expanded pad (0xef3), frame slot order.
- /d2inlT sweep on p6 (S15 + T30..150): no improvement (T75 = default shape; others much worse). /d2inlS sweep on p4 (S10..30): S10..14
  6112, S15..30 6096.
- Readouts (AUTH10 inl_force hook, no forcing, READOUT ONLY) on p6 at S15 / S14 / S9: inl/p6.json, p6s14.json, p6s9.json.
  Always-inline test fits `c1 - 4 <= S` for every callee in the table (~CTimer c1 14, ctor 17, movie dtor 19/22, CIsThingAlive ctor 20,
  ScriptedThingBase ctor 20, ~CWideScreen 7, GROBase dtor 5/8). At S9 ~CTimer is scored: k0..k10 E (48/19/22/26), k11..k13 c (83/588/588),
  k14 E (73), k15.. c = retail's per-site split exactly. Movie dtor at S9: k0..k7 E (22..65), k8..k14 c (99, 798, 131, 798, 177, 798, 74).
- p7: p6d + the 0x00D17A80 budget-inliner device (/Og- /d2vol10, #pragma optimize("g", on) before Main), one grade:
p7 00cf4ec0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main DIFFER(6340v6164) flags= ['/Og-', '/d2vol10'] DIFFINS 1462 len 6340 v 6164
- p1 masked count: mdiff 2328 (grade after the port DIFFER(6176v6164)).
- ORACLE (not a grade): fv.sh on 00cf4ec0.cpp forcing movie dtor occurrences 8..14 expanded -> 6164v6164, gobj RELOCATION_MATCH.
- Fan-block 5 B misfit, one odall10 round (od/v5.cpp): `> 0` 536, `== true` 537, `!(<=)` 536, `> 0.0` 556, `0.0f <` 556, `!= false` 556: all negative.
