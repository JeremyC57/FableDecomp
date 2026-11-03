# 00ee0c50 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 5. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00ee0c50/NAMING_EVIDENCE.md

# Original identity and native confirmation

The exact leaf is `?Main@CTH_Trigger_Bowerstone@CV_TravellingHeroesScript@NScript@@EAEXXZ`, present in both original name sets. Ego_r complete TPI `0x11378` identifies a **class**, private virtual `Main`, private `IsNightTime`, private bool `ThunderInPlay` at +0x1c, protected `ParentClass` reference at +0x14, protected `PMasterData` pointer at +0x18 and a public CEntityScriptBase base. Native allocation 00ee0b50 requests 0x20, installs vtable 012f439c, and stores the parent/data fields at those offsets. Slot 1 points to 00ee0c50; Init 00ee0c10 writes +0x1c. This independently rejects the old gapscan/landscape destructor labels.

The parent class is original Ego_r TPI `0x1134a`; its complete original data layout is preserved. Native retail base is 0x48, the three scripted handles occupy +0x48,+0x58,+0x68, and `ThunderSceneStarted` is +0x79. Native owner constructor/init/allocation establish the retail size 0x8c. Master data `CQ_SunnyvaleMasterData` retains all original members; `PostSavePosition` is +4 with original enum thresholds EGP_ORCHARD_FARM=450 and EGP_ARENA=900. Original procedures/local scopes in both PDBs name GameflowPosition, MyPos, Thunder, Girl, Boy and the failure adapter.

Private shared-header additions declare original `GossipSetup(CCharString,CCharString,CCharString,CCharString)` public (TPI 0x11337, native 00ee07e0), and private `IsNightTime()` (TPI 0x11374, native 00ee13b0). Both exact emitted names appear in both PDBs. The latter is independently corroborated by the supplied landed neighbor and its native time-of-day/termination calls.

The predicate is `NScript::CIsThingAlive`, original TPI 0xde7d, size0x10, public CPredicateBase base and private Thing +4. Native vtable012c32f0 slot0 invokes Thing.IsAlive; its destructor is 00e241b0. CScriptThing::GetPos native004aa980 has the exact early-null-return wrapper retained here. The static protected `NullCoord` spelling is from the original Ego_r PUBLIC32 record in helper_symbols.json; its native address0143e8e0 is used by GetPos. The distance helper's global fastcall spelling is in both PDBs and its native body00cbe3ea verifies alive/position/distance semantics. No offset-only helper names were introduced.

All 114 actual leaf relocations, including all string bytes, match the corresponding native operand and exact known target. After filling only the actual COFF relocation operands from the native bytes, the resulting 1884 bytes equal the native function exactly. Eight Game virtual methods and CScriptThing::GetPos are separately mapped to original names/native table slots. Full actual objdump disassembly and symbol/relocation tables are retained in attempt_02. RELOCATIONS.json contains each operand receipt.

Evidence: evidence/00ee0c50_types.json, 00ee0c50_helper_types.json, 00ee0c50_locals.json, 00ee0c50_helper_symbols.json, 00ee0c50.private.disasm, 00ee0c50_allocator.disasm, 00ee0c50_distance.disasm and 00ee0c50_night.disasm. Header context is immutable history/00ee0c50/attempt_02/hdr_snapshot. All reference/member-access corrections from the 00efdc40 private migration remain selected, and no root header was edited.
