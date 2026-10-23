# Chunk 309 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- `return n > 0 ? true : false;` gives retail's bare `setg al` tail. A plain `return n > 0;` adds `xor eax,eax` before it (008751d0).
- A named bool for the final compare (`bool r = count == 4; return r;`) has the same effect: no `xor eax,eax` before the cmp (00e3e320).
- A __fastcall callee returning a non-POD class (one with a user ctor, e.g. CDateAndTime) takes the hidden result pointer in ecx. Retail's `lea ecx,[local]; call` needs that user ctor (004091c0).
- An implicit `const wchar_t*` -> CWideString conversion builds the temporary before the argument pushes. A named local gives `lea` where an inline temp gives `mov edx,eax` (004091c0, 00e0f1d0).
- An x87 `a < b` emits `test ah,5; jp`, and `a <= b` emits `test ah,0x41`; pick the operator from the retail test mask.
- A global epsilon that retail loads from memory (0x0129BA3C) must be an extern variable even when squared. A literal folds to an immediate .rdata constant (008751d0).
- An __int64 member after the vptr makes VC7.1 pad to 8; retail uses `#pragma pack(push,4)` around such classes (00837570 UID offsets).
- A script WAIT on a bool getter written `!= false` reproduces retail's `test al,al; setne bl` (00e0f1d0, 00e480d0).
- Byte-identical copies in different scripts get the same propagated label. Find each copy's callers (xref.py) and name it after the caller's class, so one symbol never binds two copies (00e44a40 / 00e53670).
- VC7.1 test harness quirk: a function-pointer typedef returning a template UDT fails with "Incompatible calling conventions for UDT return value". Return void* in the test stand-in (00e5d4f0 test).

Review at landing:
- Rows that needed many grades: 004091c0 (/GS, parked after 5 variants), 004093e0, 008751d0, 00e3e320, 00ef25e0, 00e6a0b0.
- 00e0f1d0, 00e3e320 and 00e53670 are landed under corrected names, so their catalog labels differ (see NOTES Label corrections). 00e3e320 carries a `// Leaf: IsHeroDressedAsLady` line.
- 00d043a0's catalog row holds two functions: CheckForSecondAreaDoorHelp + jump table + 0x00D04760. Trim the oracle and land via verify_land_jumptable.py.
- Script sources use local stand-in structs (GameCalls vtable with filler slots, CScriptThing, CTimer) next to the engine headers, following the landed script rows. Check whether rebuild/include/engine has a CGameScriptInterface header these should switch to.
- 00e569d0 DoConversation calls `handle->IsPerformingScriptTask()` with a null handle when the speaker is neither boy0 nor girl0 and no conversation is active. This is faithful to retail, and the test avoids it.
