# 0x00D05A60 NScript::CQ_BanditCampBossBattleScript::CAssassin2::Main (AUTH30) -- all seven gates pass; held for one ruling
Route: Codex's draft (c0) matched its own bytes but defined strong copies of landed functions (Alloc 0x00D05910): new LNK2005s. The fix
keeps Main inline (out-of-line Main gives DIFFER 1365v1400) and replaces the strong definitions with an anonymous-namespace helper that only
constructs the entity, so the TU emits ??_7CAssassin2 (and through it the inline Main) with no strong duplicate. Source: 00d05a60.cpp (= g4.cpp).
Opt-ins (overlay): FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_TYPED_CTORS_PASS29; repo opt-ins as #defined at the top.
Gates:
1. bytes: RELOCATION_MATCH 1400 (meas.sh with the overlay; name_gate's own regrade also gives RELOCATION_MATCH).
2. test: 00d05a60_test.cpp (Codex's 00D05A60 fixture on shared headers, with its embedded copy of Main removed): vt.py with the row object PASS,
   without it LINK_FAIL, so Main comes from our object. Fixtures derive from the shared-header classes; the rest are link stubs.
3. pick-any: pickany_vs_retail all EQ, pk_all all EQ (no NEQ, so no reach flip). Only strong symbol: the helper (no retail address).
   The movie ??_G reached through the vftable (46 B v 87) is identical to HEAD rows 0x00EA2720 / 0x00E50FD0 (head/): debt, not new.
4. vftables: ??_7CAssassin2 0x012C8D28 7/7 slots agree, count OK; every other emitted table agrees (vtchk).
5. Ego_d: locals and scopes match exactly (seh_me, TalkedTo, _Fail_Val; speech_movie and PauseMyEntitiesPlease in each of two blocks).
6. purity: crawl/purity.py classify = genuine; retail addresses only in comments.
7. names: NAME_GATE.txt = NAME_GATE PASS (every relocation agree_pdb or split_fixable against older landed stand-ins; no new disagreement).
Ruling needed: (a) the scaffolding helper (nf86 / nf88 anonymous-namespace precedent; not a retail function) and (b) Main inline in the TU.
Not run: addmatch.py (left for after the ruling, so nothing reads as a landed match).
