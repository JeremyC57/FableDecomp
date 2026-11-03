# 00d43450 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 23. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00d43450/NAMING_EVIDENCE.md

# 00d43450 identity and evidence

The original function is `NScript::CQ_GuildTrainingScript::CWillDummy::Main`, Leaf `?Main@CWillDummy@CQ_GuildTrainingScript@NScript@@EAEXXZ`, private virtual. Original Ego_r TPI110ab/110ca supplies complete owner and nested data and method access; both name sets contain the exact Leaf. Procedure/local extraction is `evidence/00d43450_function_locals.json`: Ego_r stream24 record0xf6a00, FableWin stream23 record0x1219e0. Original locals are initial_angle float, Conversation long in each feedback scope, and TRetVoid _Fail_Val in FableWin.

Independent retail evidence: factory00d43360 allocates0x1c, constructs CEntityScriptBase with parent.Game+0x40, installs vtable012ce364 and writes ParentClass+0x14/PMasterData+0x18. Vtable slot1 is00d43450; surrounding functions are that class's empty Init and OnPredicateFail plus scalar deleting destructor, followed by the independently named CPreMeleeMaze factory. Native body reads Me.GetAngleXY, tests lightning ability11, increments master WillScore+0xac, waits on WillTrainingStarted+0x9f, and uses the WillHelpTimer member+0x60. These behaviors and layout uniquely agree with original CWillDummy and CQ_GuildTrainingScript.

Extent2168 is exact: [00d43450,00d43cc8) ends RET00d43cc7, followed by8 int3 bytes and next factory00d43cd0. Exact Leaf occurs once in selected COFF symbol table section13 and strict extraction succeeds. The frame remains0xdc in both native and selected object.

New engine/CQ_GuildTrainingScript.h includes every original owner data member; donor base0x5c becomes retail0x48, yielding owner0x68 and WillHelpTimer+0x60. Nested class retains the original protected reference ParentClass and PMasterData pointer, with the original private virtual methods. Shared base opt-ins select original Game and MasterScriptData references. These original base TPI records are in evidence/00efdc40_base_types.json and00efdc40_reference_types.json, independently checked here against the native factory loads.

Global0143e90c is exact `CDefPointer<const CScriptDef> pGlobalScriptDef`, not a raw pointer. Original PUBLIC32 and DATA32 records plus complete CScriptDef TPI3500 are in evidence/00d43450_global_types.json. Original public float GUI_WillDummySpinTimer is donor+0xfd4, retail+0xf18; native00d4393c loads that precise field, and the existing complete shared header models the retail definition size0x1088. pDSTGame is the original CGameScriptInterfaceBase pointer, independently loaded by native timer dispatch. Native double constants at01238010 and012316f0 equal0.25 and0.5 and match both selected eight-byte COFF constant payloads.

## Historical parked review (superseded by root23 below)

RELOCATIONS.json audits all94 selected relocations independently, including every literal and16 virtual method identities from native game/thing tables and original names. Three object timer calls replace native inline operations and are explicitly classified as mismatches without invented target addresses. All other original direct callees are verified against native call targets. No same-offset relocation or byte parity is claimed for this DIFFER row.

The exact selected Main body passes311 scenarios under GNU64 and actual VC7.1 /GR-. Checks include lightning versus melee, priority when both messages are present, score, cooldown feedback, both original speakers, four angle changes, target disable/restore, the original global spin delay, termination at100 distinct checks in3 scenarios, and temporary destruction. Selected grade18 is DIFFER(2136v2168): three timer expansion decisions and first positive-feedback Conversation register allocation remain after source-shape exhaustion; direct GetTime reduced the earlier40-byte residue to32.

## Current main verified selection

# WillDummy final identity and annotation evidence

Root rows/00d43450/NAMING_EVIDENCE.md and original TPI/procedure evidence remain the declaration authority. Main is the original private virtual Leaf, both original name sets agree, own native vtable012ce364 slot1 points to00d43450, and exact extent2168 ends RET00d43cc7 then8CC. Every actual94 COFF operand is independently resolved at its native position; strings and double constants are compared by full payload.16 real game/thing slots are read from native vtables and checked against exact original symbols. Global pDSTGame is original PUBLIC32/DATA32 in evidence/00df1830_original_publics.json; pGlobalScriptDef original CDefPointer type/public is in evidence/00d43450_global_types.json. No global type was inferred from spelling alone.

Original CTimer assignment0360a190 and GetTime0360a200 are independently body-proven by the private original donor and exact public names; existing source implementations agree field-for-field and call-for-call. Their stronger expansion is selected under newest Owner header-helper conditional preference corrections/docs/EXTRA_RULES.md:100-104, consistently with root-accepted timer helpers. Ordinary forms and source contexts were exhausted in1–18 and preserved. This is a documented category application; these exact methods are not named in the clause examples. The original source keyword is unproven and is NOT inferred from native expansion. No compiler-generated method is newly authored.

Previous parked conclusion is superseded by the exact original generated loop form. Root independently verifies the final selected source, all target operands and both test compilers.
