# Landing nf266 (Account A, 2026-11-22): the four public AllowsCombinationAnims binders onto the protected PDB run, CastAssassinRush::Initialise (5 rows)

Agent NF266 (q12760 done; q12743 half: 0x008CD470 done, 0x0072CAA0 not started; q12744 / q12659 open). Hand-back: `pending/2026-11-22_nf266_handback/`.
- **q12760 (0x00693B30, 0x00693DC0, 0x0069F0F0, 0x006A0D50):** retail checked first: slot 66 is 0x00692890 in all four retail tables (no
  CCombatActionBase / derived override), so the base's protected `MBE` spelling is right. In-place access change inside each row's local
  CCreatureActionBase model, zero line delta (labels on existing lines; two rows carry their own asserts), for the whole run of slots 56-69
  (both PDBs spell all fourteen `MAE` / `MBE`; engine/CCreatureActionBase.h declares the run protected). HEAD v new: defined symbols identical,
  every function COMDAT byte- and relocation-identical, only referenced symbols change (14 / 14 / 14 / 12 public -> protected pairs);
  vtmap: only slot 56-69 lines change. 0x00692890 2 -> 1 (nf264's slot-66 census: these four were the only public binders; no direct
  callers); the other 13 addresses not grown (0x006DF800 already binds the protected spelling at each). Link: 0 duplicates (control LNK4006).
- **q12743, 0x008CD470** (CCreatureAction_CastAssassinRush::Initialise): new add-only `FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_INITIALISE`
  (`virtual void Initialise();`; Ego_r 0x009047C0 + FableWin 0x029F9410 + retail vftable 0x01284254 slot 12 = 0x008CD470, an override);
  pin_asserts 0; whole-tree equiv_check 0 of 57,125 differ; the six includers rechecked. The row onto the shared headers, first form: its
  Initialise body byte-identical to HEAD; only SetFinalAction moves off the `CCountedAction` stand-in onto the PDB spelling (0x008CEAE0 2 -> 1;
  0x008CD470 its only caller); its new pick-any copies with retail owners EQ (0x00487050, 0x0048C5C0); CTCAssassinRush has no shared header
  (local model kept, counted-pointer member / parameter respelled). Table: the stand-in SetFinalAction line replaced by the PDB one.
- **Recheck** (`logs/recheck.log`, landed tree): the 5 changed rows + 0x00692890, 0x006DF800, 0x008CEAE0 and the six header includers 0x0072D990 /
  0x0096AE50 / 0x009735D0 / 0x008C2550 / 0x008CEF50 / 0x0072CAA0 (14 rows): 14 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF266's questions
1. q12760 scope: the whole protected run (slots 56-69) is accepted: all fourteen spellings are the PDBs' and the header's, and it is
   count-neutral or reducing everywhere (0x006DF800 already binds each protected spelling).
2. q12743: land as it stands (the declaration, 0x008CD470, the two table lines); 0x0072CAA0 (with its three-argument ctor declaration and a
   second equiv_check) to the next agent.
3. NAME_FIXES queued (slot 1 Validate in 0x0069F0F0 / 0x006A0D50 v retail CBaseObject::Validate; slot 3 PerformAction self leaves QAE v the
   tables' UAE; 0x006929C0 / 0x00694D10 stand-in self leaves; CIntelligentPointer<CThing> slot 0 `??_E` v `??_G`; the four entry-1 rows' full
   port; 0x008CD470's standalone test; slot 12 of the CastAssassinRush tables in 0x0096AE50 / 0x009735D0 / 0x008C2550 binding the base
   Initialise where retail has 0x008CD470 (clang-only census; confirm with VC7.1, then the nf264 slot-66 way)). The last one is HEAD debt
   this landing does not change: those slots bind the base symbol, which resolves to the base body as before.
4. q12744 / q12659 to the next agent.
Rule slips (the entry-1 recheck without an outer `timeout 900` on its launch line; one `rm -rf` on a variable path, no-op; 0x008CD470's HEAD
test prechecked after its first recheck): noted; the next brief repeats the rules.
