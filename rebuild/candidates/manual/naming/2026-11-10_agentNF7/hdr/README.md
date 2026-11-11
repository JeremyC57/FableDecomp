# agentNF7 header overlay

One header changed, as an `#ifdef` opt-in only (the default path is byte-identical to the repo copy):

- `engine/CTBaseSingletonNoDefaultAlloc.h`: new opt-in `FABLE_RETAIL_SINGLETON_SET` adds the public static `__fastcall`
  `Set(T*)` (FableWin.pdb `?Set@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIXPAVCQuestManager@@@Z`, also the CPlayerGui
  and CDriveManager instances; no out-of-line copy in retail). Body: `PClass.Reset(item)` -- retail CWorld::PostInit 0x004A6550
  resets the PClass slot itself (`mov ecx,offset 0x013B89FC ; call 0x004A9A10`, CCountedPointer<CQuestManager>::Reset).
  Used by row 0x004A6550 only.

Grade every row in `../rows/` with `FABLE_HDR_OVERLAY=$S/w22/agentNF7/hdr`. No other landed includer defines the new macro, so
they are unaffected.
