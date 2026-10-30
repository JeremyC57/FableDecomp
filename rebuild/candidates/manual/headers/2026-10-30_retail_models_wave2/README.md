# Retail class-model migration, wave 2 (2026-10-30): 711 landed rows onto shared retail headers, bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv; REVERTED.tsv (14); HEADERS.md (evidence per slot/member); NEXT.md (wave 3 plan and dry-run
counts); LESSONS.md.

Headers: CParentDefClassBase.h retail branch (RTTI : CDefClassBase, vtable 0x01230CBC, 25/25 slot names probe-matched;
overrides at slots 10-16, 21, 22, 24; InstantiationName +0x25, SubDefInfoMap +0x29, size 0x39 packed; ctor 0x00430370, dtor
0x00430300, copy ctor 0x0046EE98, operator= 0x00431ED0); CIVCountedPointeeBase.h retail branch gains the FableWin inline
AddRef/Release/GetRefCount (IVRefCount mutable: mutable vs const_cast compile identically, chosen to match landed models);
new CDefPointer.h (Object +0, the FableWin member set, __forceinline: retail never calls a CDefPointer member out of line;
one template reproduces both release byte shapes when Release holds the pointee in a local; the ENullDef ctor is omitted,
its enumerator is in neither PDB).

Rows by family: GetSubDef<T> 151 (invented vNN slots gone; slot 14 = const GetPSubDefInfo), CBaseClass exact shape 231,
CBaseClass + interface 7, CBaseClass implicit-copy rule 6, local CParentDefClassBase 224 (12 also given their PDB
Transfer/Copy/GetSizeofClass overrides), interface-only CGameScriptInterface 91, 0067c980 respelled (Copy virtual: leaf UAE).
Agent recheck 711/711 PASS (10 MATCH, 701 RELOCATION_MATCH); 708 byte-identical to HEAD, the other 3 expected (the
respelling; two rows differing only in compiler-internal $L jump-table labels). Coordinator independent spot regrade after
cherry-pick onto 87d9df757: 80/80 PASS (coordinator_spot_recheck.log); bindings unchanged, multi-check 0.

Rule found (LESSONS.md): retail never inlines the CBaseClass ctor outside its own source file; the 193 immediate CBaseClass
vtable stores in landed rows are each overwritten at once by the CScriptThing vtable, i.e. the implicit copy ctor.

Reverted: 8 CParentDefClassBase rows change bytes (004773f1, 004585e0, 0045767e, 00458d3f, 00458994, 00459e46, 00458b9d,
0045a0ee; 004585e0 pads the base itself so its fields move; the rest not yet looked at), 0045784b / 004575fc (`typedef long
CDefString` clashes), 004564b5 (two-parameter CVectorMap), 004585bd (unattributed abstract aggregate), 00d36140 / 00f0b960
(local `struct std::vector` vs the forward header's `class`).

Open: the def-chain copy members (CParentDefClassBase operator= / copy ctor, CDefClassBase operator=) are out-of-line calls in
64 size-optimised files but inline in 5 /O2 rows, the same question as the CScriptThing copy; NEXT.md has the experiment.
Wave 3 dry runs: mig_cgsi.py 462 rows, mig_bc.py --allow-v00 1,088, mig_bc_copy.py 169; DCB clusters, 35 local-CResource rows
and 16 SFB hand edits still deferred.
