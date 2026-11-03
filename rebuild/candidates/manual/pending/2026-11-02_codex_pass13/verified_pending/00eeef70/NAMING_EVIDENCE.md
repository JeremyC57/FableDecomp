# 00eeef70 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 4. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00eeef70/NAMING_EVIDENCE.md

# 00eeef70 identity and verification

Original leaf: `?Main@CTestQuestCard1Script@NScript@@UAEXXZ`, present in both original PDB name sets. Native owner vtable 012f5a78 slot 2 points to this address. The independently named constructor at 00eeeea0 and Alloc at 00eef540 store that same vtable; the registered script literal is TestQuestCard1. Original Ego_r TPI 0x117c1 establishes CLevelScriptBase<NScript::CEmptyData> and the single public member Accepted4SecondBoast. Native stores place that bool at +0x48; Alloc requests 0x4c bytes, rather than the debug donor's 0x60. The new shared owner header preserves the full retail layout.

Both original procedure records name the CScriptThing local `chest`. The literal MagicChest and native by-value lookup verify its role. The original WaitForBoasts member's exact public, nonvirtual spelling is present in FableWin and its type in Ego_r; the native thread constructor stores 00eef510 alongside the WaitForBoasts literal. This callback is empty in retail. Its name must not be interpreted as proof of behavior that the code does not contain.

The source follows the original boast arguments and objective strings exactly, including the negative reward -5, the three explicit frame yields, all three MsgIsRegionLoaded waits, and the termination guard inside Accepted4SecondBoast's otherwise empty branch. Game calls use the actual shared interface declarations. GetThingWithScriptName is the one-string const overload at slot 72, not its two-string overload. The final SetQuestAsCompleted arguments are true, true, false. The original scalar allocation and deallocation thunks are verified from MSVCR71 imports.

The assigned full extent is 1428 bytes, ending in ret at 00eef503. Twelve alignment bytes precede the next callback at 00eef510. The first complete source shape matches all bytes with relocated operands masked. A fresh independent RTM rebuild confirms that result. Every one of 87 Main relocations is checked against its actual native call/data target or exact terminated literal. The spawned-function vtable's seven symbols, including the scalar-deleting fallback behind VC's weak external, and its 15-byte completion-store override are checked. All 79 CScriptThing vtable slots are also checked against the original PDB slot map and native pointers.

The standalone C++03 test pastes the selected Main body. Its 144 native-event plans and 1266 scenarios cover null/dead/live chest handles, thread allocation failure, immediate/delayed message readiness, all terminating exits, exact boast and objective arguments, state reset, and balanced handle/string lifetimes. The actual empty callback is tested separately from deliberate external-state injection that exercises the dormant boast-flag guard. Both GNU C++03 and VC7.1 /GR- print exactly `00eeef70_TEST PASS`. Production byte/ABI checks remain separate from dependency doubles.
