# 0x0085C470 naming and native/donor mapping

Leaf `?Update@CMoveOnCylinder@NCameraModes@@UAEXXZ` is exact in Ego_r.pdb at00846d10 and FableWin.pdb at0288b840. Original FableWin full TPI0x56db0 proves public virtual Update, AxisX/AxisY/AxisZ,Theta,HeightParam. The retail vtable0125a064 slot1 points to0085c470; OnDie0085c450 and OnInit0085c7d0 flank it in the same order as both donors. Original procedure-local records from BOTH PDBs are saved in ORIGINAL_LOCALS.json; original full types in original_types.json.

This migration corrects historical misnomers: +98 is CBase::LeftStickVector; +44 is PCameraModeDef; +a4/+bc are AxisX/AxisZ; +c8/+cc are Theta/HeightParam. CThing+60 is PhysicsTC and PhysicsTC+34 is Radius. World slot13 is PeekWorldMap, GetHeightZBelowPos receives the queued ground_test_pos argument, and0084e1a0 is protected AddAttractorGoTo. CCameraModeDef uses every Ego_r field after original CParentDefClassBase; its native sizeD8/last fieldsC8..D4 are proven by the native body and original field records.

Every relocation including compiler literals is enumerated in RELOCATION_AUDIT.json. The nine callable identities use exact two-PDB spellings; STLport LowerBound uses its native const pair-pointer ABI (already exactly bound004365b0) over the original CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<...> > identity. Existing wrong GetWorld QBE and AddAttractorGoTo QAE spellings at these targets are queued in NAME_FIXES.tsv.

No invented callee/class/global name remains. Literal symbols are compiler IEEE encodings, not invented globals. Self-contained fixture names and recording counters are test-only.
