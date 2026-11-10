# 2026-11-10 landing o -- chunk 900 (chunk 899 follow-ups), 10 rows

Source: `pending/2026-11-10_linux_hand900/`. All 10 matched rows landed (vland --land: 2 MATCH + 8 RELOCATION_MATCH, tests PASS).

- Six DirectShow BaseClasses `adjustor{4}'` thunks 0x00CA6170..90 (CBaseInputPin, renbase.cpp) and 0x00CAA720..40
  (CDynamicOutputPin, amfilter.cpp): chunk 899's hand-written rows with a `// Vendor-proof:` line; the vendor row
  `strmbase-dxsdk-sum2004` was built and `lib_verify.py` gives 6/6 MATCH, each relocation proven against the library
  (needs `FABLE_WINDDK_INC` set to the WinDDK 3790.1830 `inc/wnet`; the VENDOR_PROOF_MISMATCH warnings in the log are that
  variable missing in the landing shell, re-proven after). Names: the vendor objects' IQualityControl vtable order = retail
  0x012BF3AC / 0x012BF748.
- Splits: 0x00C3EC10 / 0x00C3EC20 CCodaSystem::CreateHardwareVoiceA / B (vtable 0x012B5BC0 slots 4 / 5, 11 bytes each);
  0x00F29FF0 CCutsceneDef::Copy (5-byte tail jump) and 0x00F2A000 CCutsceneDef::operator= (203 bytes, the implicit assignment).
  New manifest starts 0x00C3EC20 and 0x00F2A000; BOUNDS_FIX lines were recorded at chunks 897 / 899.
- Headers: new `engine/CCutsceneDef.h` (sizeof 0x9c asserted); `engine/CVectorMap.h` opt-in
  FABLE_RETAIL_CVECTORMAP_ASSIGN_OUTOFLINE (declaration-only operator=, retail 0x006313A0; default path unchanged).

Not landed: 0x005C8459 CTCInventoryWeapons::ConstructScreen parked at DIFFER(3299v3235) (`differ/005c8459.cpp`, with the agent's
CTCInventoryWeapons / CTCWeapon / CTCInventoryItem header drafts under `hdr/`); 0x005C9A95 and 0x005B8743 not drafted.
Header debt: 0x00C3EC10/20 model the +0x1d4 object locally (CAudioContext) where 0x00C3EB60/80 call it CEAXDevice.
Queued (`land1011n`): 0x006313A0 ??4CVectorMap<ulong,CSubDefInfo>, 0x00432EE9 ??4vector<CCharString>.
