# NOTES (agentNF32 name-fix pass, tags land1011fe .. land1011fi)

- The queue file holds 27 entries (the task line said 44; QUEUE.tsv has 27 data lines). Result: 18 done, 9 partial (2 of them BUSY), 0 rejected.
- 31 changed rows in rows/ (22 self-row renames in RENAMES section 1, 21 caller/callee respellings in section 2). FINAL_GRADES.txt: all 31
  regraded once more in one loop with the final overlay -- 3 MATCH, 28 RELOCATION_MATCH. No test was changed. vt.py on every row's
  unchanged catalog test: 26 print `_TEST PASS`; 0x00416225 / 0x00493911 / 0x004DF885 / 0x006590BF print their catalog PASS_<addr>
  and 0x006590A0 prints PARITY_OK (vt.py looks only for `<addr>_TEST PASS`); all are self-contained models that do not link the row.
  Most grades on one row: 2 (+1 final): 0x00668610, 0x0075BA30, 0x0075AD50 (each a missing header opt-in on the first compile).
- One overlay opt-in: engine/CSurface.h FABLE_RETAIL_CSURFACE_GETBYTELENGTH (hdr/README.md).
- Descriptive-by-library (STLport; both PDBs are Dinkumware builds): 0x00493911 (_List_base<const CMessageEvent*>::clear), 0x004428F0
  (vector<long>::resize, const& form), 0x005B16FF (_Rb_tree_iterator<pair<const long,long>, _Const_traits>::operator++), 0x004DF885
  (map<long,long>::~map; FableWin carries the same mangled ~map since the class-template arguments mangle alike). Only types the matched
  callers bind are used.
- 0x004DF885 (folded 5-byte thunk): named by what it jumps to and who calls it -- `jmp 0x004DD2B1`, the tree destructor body bound by the
  landed map-destructor stand-ins (~CLongLongMap 0x0059EC48, ~CLongMap 0x005A30A1, ~CContainer_004dd2b1 0x004E27E0); callers 0x006E7D50
  (CGameScriptInterface::ScriptTimers, Ego_r layout map<long,long> +60) and 0x006CF530. The row now binds 0x004DD2B1 as
  ??1?$_Rb_tree@J..@std@@QAE@XZ (map<long,long>'s tree); the address is ICF-folded, so its own row/name is left to the coordinator.
- 0x009F1D20: both PDBs name it ?InitializeMemorySystem@?A0xcf4a15dd@@YIXXZ (anonymous namespace of the allocator TU). The row is now
  __fastcall (?InitializeMemorySystem@@YIXXZ), matching its only binder 0x009F20B0. The anonymous namespace is not spelled: its ?A0x hash
  is per TU and an unreferenced internal function is not emitted, so separate row TUs cannot share it. PARTIAL.
- 0x007044B0: 0x00704A80 (CGameScriptThingManager ctor) still binds ?DeleteData@@YIXPAVCGameScriptThing@@@Z: its TU models a NON-template
  local `struct CCountedPointer` (used in SetImp's signature, 0x007046D0), which clashes with the template name; needs the move onto
  engine/CCountedPointer.h + CScriptThing.h (NF31 recorded the same gap).
- 0x006924B0 GetRandomAnimFromName (protected static, KI): ~50 landed sources reference the address; the 8 reloc_bindings spellings are free
  stand-ins (GetAnimationFromName 0x0062DAD0, GetTableAnimation 0x0062D890 / 0x00842A20 (the last with a `long` parameter), FindAnimation
  0x0069F0F0, GetRandomLoadedAnimation 0x00718650, GetAnimationEntry 0x007E7FD0, GetRandomLoadedAnim 0x007ED450). The `K` (protected)
  spelling can only be called from a CCreatureActionBase-derived member, so each caller needs its local action model put on
  engine/CCreatureActionBase.h: authoring-sized, not attempted.
- 0x008CE1B0: binder 0x008C2550 (StrikeResponseJackOfBladesIntoRush::FinishAction) keeps CCountedPointer_CCreatureActionBase: its TU
  defines a same-TU noinline stand-in ctor (0x00695880) for retail's frame packing; moving it onto engine/CCountedPointer.h
  (FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR) is a re-model. The other declarations in 0x008CE1B0's TU (the 0x008CE040 / 0x008CE100
  ctor forms) still use CThingCreatureBase* -- other addresses, not in this queue.
- 0x004428F0: 15 other binders spell the address with stand-ins (LongArray, CRetailVector_long, CArray<int>, CArrayLong, CArray<long>,
  PrimitiveIDs, CRetailVector<long>, CPODVector<unsigned long>, CLongArray, CLongVector x2, vector<long> (Dinkumware form, 0x005C19A3),
  vector<unsigned long> x2; 0x004433F0 already right). Several are other element types folded onto the same body (ICF), so each needs its
  own type check: left.
- 0x00416225: other binders of this one-instruction COMDAT: ?GetFrame@CWorld@@QAEJXZ (0x006E28A0), ?GetFrame@CMainGameComponent@@QAEJXZ
  (0x0049DBE0), ?GetWorldFrame@CMainGameComponent@@QAEJXZ (0x006E37D0), ?GetFrame@CMainGameComponent@@QAEKXZ (0x00445A60). The
  CMainGameComponent ones are probably GetCurrentServerFrame; the CWorld one may be a folded CWorld getter. Not respelled (outside the entry).
- 0x00668610: binders 0x00668D90 (CCountedPointer_00668610::Reset stand-in class) and 0x0069F0F0 (ActionPointer / ActionPointerInfo
  stand-ins, DeleteAction) left; spelling the template member there would need a new local CCountedPointer model. 0x0069EF80 / 0x00741DB0
  already had a local template CCountedPointer<T>: it got the static DeleteData member and the count block takes the deleter as an argument
  (same bytes).
- 0x0076B120: binders 0x007C2480 (?push_back@CVectorParamVector@NParticleEngine@@..) and 0x008F7950 (NDust008f7950 stand-in) left.
- 0x006C0330: only the row's local CThing model member was respelled (ScriptName -> ScriptData at +0x78; the shared header has ScriptName
  +0x74 / ScriptData +0x78, retail SetScriptName `lea ecx,[eax+74h]`, GetScriptData +0x78 in the matched 0x007E8590). The move onto
  engine/CThing.h was not attempted (the row also models GetInterface / CInterfaceMap locally).
- 0x0075AD50: moving its CRigidBodyConfig onto the nested CRigidBody::CRigidBodyConfig changes every callee taking it; those were given
  their PDB spellings too (all private in Ego_r + FableWin; ECollideState nested): ComputeForces 0x00759D30, CalculateVertices 0x00759EC0
  (also bound so by 0x0075B000), CheckForCollisions 0x0075AAF0, ResolveCollisions 0x0075A7A0, Integrate 0x00759F80, and Simulate itself.
- 0x0079CE80 / 0x0079DE00 now call 0x0066BD00 as ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z (engine/CTCSoundPlayer.h;
  landed binder 0x008DD6E0) instead of the stand-in CTCSound::PlaySound(ulong,long,ulong,long,long).
- 0x007E8310: no shared header for NTCMessages; the CMessageBase / CMessageOnSetThingOwner model is the landed 0x007E8590's (copied, not
  new). The scalar deleting destructor row 0x007E8350 still models a global `struct CMessageOnSetThingOwner` (not in this queue).
- Local models: no NEW local model of a class with an engine header. Kept pre-existing ones: 0x00971B00 (CAIStateGroup_JackOfBladesBase),
  0x006C0330 (CThing), 0x009F2FC0 (CPixelFormat, IDirect3DSurface9), 0x006590A0 (CTCVillage), 0x00659C32 / 0x0065C08A (CTCVillage),
  0x0069EF80 / 0x00741DB0 (template CCountedPointer, gained a static member), 0x0075AD50 (CTCPhysicsRigidBody), 0x0079D110 / 0x0079E300 /
  0x0079EB30 (CTCBulletTime / CTCPhysicalShieldSpell), 0x0042B312 (CDefPointer: a converting operator= with the same body was added).
- Made-up names: none. No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in any row.
- Repair pass (verifier findings on 0x0075AD50): the Simulate access fix (private, Ego_r 0x00769E60 + FableWin 0x025C0C40) was outside
  the queue (scope widened from land1011fi/0075ba30); its landed binder 0x0075AEC0 CTCPhysicsRigidBody::Move now declares Simulate
  private and binds ?Simulate@CTCPhysicsRigidBody@@AAEXM@Z (RELOCATION_MATCH; RENAMES section 2). The landed self row ComputeForces
  0x00759D30 now emits ?ComputeForces@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z (Ego_r 0x00768D60 + FableWin 0x025C1000),
  the spelling its caller 0x0075AD50 binds (RENAMES section 1; moved onto engine/C3DVector.h + engine/CRigidBody.h, the local C3DVector /
  CRigidBodyConfig stand-ins removed; its local CTCBase / CTCPhysicsRigidBody models are pre-existing). The landed self row
  CalculateVertices 0x00759EC0 emitted the public spelling; it now emits the private one both PDBs and its binders 0x0075B000 /
  0x0075AD50 use (RENAMES section 1). ResolveCollisions 0x0075A7A0 already emits the private spelling (engine/CTCPhysicsRigidBody.h
  opt-in). Integrate 0x00759F80 and CheckForCollisions 0x0075AAF0 are not landed.


## Coordinator note at landing (2026-11-12)
Counts after the repair pass (the figures above describe the first pass): 34 changed rows, 25 self-row renames (RENAMES section 1), 23 caller / member respelling lines (section 2); final grades 3 MATCH + 31 RELOCATION_MATCH. Queue: 18 done, 9 partial (0x006C0330 moved from done to partial/PORT by the verifier: only the local CThing model was respelled), 0 rejected.
