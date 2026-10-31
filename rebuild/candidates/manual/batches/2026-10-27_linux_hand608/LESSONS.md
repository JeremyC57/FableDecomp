# Chunk 608 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A virtual call whose argument is a by-value temporary copy: read the object pointer into a named local first
  (`T* p = obj.Member; p->Virt(Temp(a, b));`). VC7.1 then loads ecx before the copy and copies through eax/edx as retail does.
  The inline `obj.Member->Virt(...)` copies through three registers and loads ecx last (0x0080FFB0, 45 -> 0 lines).
- Re-prove parked names before tuning. 9 of 10 parked rows here had a wrong own symbol or callee spelling: wrong class keys in the
  row's own mangled name (0x00808AA0 struct v class, 0x00864FF0 ABU v ABV), nested parameter types (0x00881690),
  const/access/reference (0x009AD5F0 IBE + stream by reference, 0x00813010 virtual + const CThingAICreature*), and inherited members
  declared on the derived class (0x00945BD0 Terminate / SetSeat / AIAction_DoNothing).
- PDB inline accessors fix register colouring here too: GetTextBank / PeekCutSceneMainFont / GetSubtitleRenderer / GetPCreature
  moved 0x00844600's font into edx (63 -> 17 lines). Real STLport list/vector templates plus PeekWorld / PeekGlobalDef moved
  0x00864FF0's this into edi (48 -> 9).
- CTCBase accessor addresses: GetDefinitionManager 0x006869D0, GetWorld 0x00686A70, PeekWorld 0x00686A80 (Ego_r donor order;
  the last two bodies are identical). CWorld::GetFrame 0x0049D870 is a static __fastcall (SIJXZ), not "GetWorldFrame".
- CAnimFilterBase has a virtual destructor in slot 2. Modelling it gives retail's base-vtable store on the second filter temporary
  (0x007F8C20); the first temporary's store is dead and dropped, as in retail.
- A loop index that retail homes in a dead argument slot: declaring it after the argument's last use puts it in that slot, but
  VC7.1 then folds its zero with the strength-reduced offset register (`xor ebx,ebx ; mov [i],ebx`) where retail stores an
  immediate (0x0074A4E0).
- Scheduling-only residues that no spelling moved (park after 2-3 grades): rep movsd setup v adjacent constant stores
  (0x00881690), pipelining of by-value argument copies (0x00808AA0; /Oa and /Ow do not help), callee-saved register swaps
  (0x00844600, 0x009AD5F0), a dead pointer chain loaded into ecx rather than eax (0x00945BD0).

Review at landing:
- 0x0080FFB0 matches with filler virtual slots v00..v74 on the local CTCPhysicsBase (the header flattens the vptr); slot 75 =
  SetRHSet is proven (donor order + body).
- NAME_FIXES.tsv lists 19 callee addresses whose landed or bound spellings are wrong. Most affected: the CAIStateGroup_UsesSeat
  landed rows (MoveToSeatAndSitDown QAE_NJ), the CCompressorZlib users (GetWorkspaceSize/Init, landed 0x009967E0 / 0x00996A40),
  GetFrame (landed as GetWorldFrame in the CTCCreatureGroup family), and AnimExists/GetAnim/Init.
- The landed CTCCreatureGroup family (0x00813100 etc.) declares members with CThing*. The PDB has `const CThingAICreature*`
  for AddNewMember and ActivateMember; the rest should be checked when those rows are respelled.
- Rows that needed more than 6 grades: 0x00945BD0 (8). 0x007F8C20's differ is longer than the parked file (233 v 221) but has the
  right names; the parked 221-byte file keeps wrong callee names (Set, HasAnimation, GetAnimation).
