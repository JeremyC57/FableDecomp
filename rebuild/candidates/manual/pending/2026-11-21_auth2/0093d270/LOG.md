# 0x0093D270 -- AUTH2 log (continues S/w22/auth1/0093d270/LOG.md; best source unchanged: S/w22/auth1/0093d270/e1.cpp, DIFFER(305v295), residue 6)
1. Ego_r check (advisor step 1): GetProximityForNavigation is in NONE of ghidra_out/egor_pdb_names.tsv, egor_signatures.tsv,
   egor_publics_noproc.tsv, egor_obj_functions.tsv, fp_egor.tsv (no out-of-line copy in Ego_r; the Ego_r.pdb type stream is not on this
   machine); struct_layouts_egor.tsv has only the member (float ProximityForNavigation). Ego_d types.txt (VC10 build): LF_ONEMETHOD
   GetProximityForNavigation public, type 0x27EB2 LF_MFUNCTION return float (0x0040) BY VALUE, 0 args; the member is private
   (offset 56 in Ego_d's later layout); no LF_FRIEND near it. -> no PDB source for a reference return; rule question unchanged.
2. c2path probe (advisor step 2), harness: p25R c2path.py (pending/2026-11-20_c2_symbol_order/tools_c1harness) run through hand3c's
   tools/sched host/common (S/w22/auth2/c2/). m2 mini as is: GetP compiled FIRST (start ?GetP, then f1, f2), both decisions heuristic
   path, accepted. Forced budget path (S/w22/auth2/c2/c2force.py: clears bit 2 of the callee's +0x69 at the decision; diagnostic only):
   both decisions go `budget` (tot 69 / 59, sz 20, lim 1000), accepted -> the object's code is BYTE-IDENTICAL to the base: the
   `mov [esp+8],eax` return-value home stays in f1 and f2. With the callee already compiled, the budget DECISION path leaves the bytes identical; compile ORDER (callee compiled after the caller) is UNTESTED (`start ?GetP` precedes f1/f2 in both traces). (b) is narrowed, not refuted.
