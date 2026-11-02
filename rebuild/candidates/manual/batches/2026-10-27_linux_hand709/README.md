# Chunk 709 (NA052) -- 9 of 10 byte-exact (RELOCATION_MATCH); the tenth row is a fragment of a landed function, retired (2026-11-02)

Rows (`verify.log`: WINS 9, all PASS): CWorld::EventIsWorldEvent 0x0049D8C0 and CWorld::Unload 0x0049E270 (both through the new opt-in
FABLE_RETAIL_CWORLD), CTCHeroMorph::FrameUpdate 0x0071CD30 (slot 9; "vfunc_9"), ~CCreatureAction_SheatheItemToInventory_ForEnflame 0x0072D390,
~CCachedFile 0x00826380 (the stored vtable has CCachedFile RTTI; the label said CChunkedFile), NCameraModes::CPCMouseLook::
OnCameraInsideObservedThing 0x00873DB0 (slot 3), ~CCreatureAction_DragonTakeOffIntoFlying 0x00953470, NGameText::CGroupEntry::~CGroupEntry
0x009CAE60 and the STLport `pair<CCharString, CCountedPointer<CConsoleInputBase>>` dtor 0x009EEA50 (`/Gr`, as its TU). Every name from the
PDBs; the four `_Dest_val` labels were wrong. The agent stripped inherited `#pragma pack(1)` from three local models (bytes unchanged).

0x00BFEF20 ("CFrameData::ReleaseTexture") is not a function: it is the `__finally` block inside the landed eh vector destructor iterator
`??_M@YGXPAXIHP6EX0@Z@Z` 0x00BFEED8 (96 bytes, covering 0xBFEF20..0xBFEF37); retired in SKIPPED_ROWS.tsv.

Headers (hdr/README.md): new opt-in FABLE_RETAIL_CWORLD in engine/CWorld.h (bases CBaseClassNonCopyable + CIDrawWorld, vtable 0x012390F0,
sizeof 0x198); ResetModes / GetPControlledCreature added to the FABLE_RETAIL_PLAYER branch of engine/CPlayer.h. Chunk 706 had added
IsFirstPersonTargeting to that same branch after this chunk was cut: the coordinator merged both declaration sets (no overlap) and
regraded the nine rows 9/9 against the merged headers (merged_header_regrade.log). Default text unchanged under `cpp`; all 261 landed includers
rechecked: 19 MATCH + 242 RELOCATION_MATCH, all PASS (includer_recheck_grades.tsv).

Second spellings at landing, queued PRIORITY (NAME_FIXES_QUEUE.tsv, c709; each binder needs a class-model change): 0x0084DC10
NCameraModes::CBase::FrameUpdate (two stand-in classes), 0x0049D8C0 (the landed AddGameEvent 0x0041649C uses a `CWorld_Methods` method shim),
0x0072D390 (bound as a CCombatAction stand-in by 0x0072C790), 0x00953470 (bound non-virtual by 0x00951B20). The agent's 15 NAME_FIXES rows
are queued as c709 (three landed STLport pair stand-ins 0x009EEA90 / 0x009EEDC0 / 0x009EF260 among them); labels in
ghidra_out/labels_chunk709_2026-10-27.tsv. Header debt: engine/CCreatureActionBase.h is flat and non-polymorphic (849 landed sources model the
base locally); a FABLE_RETAIL_CCREATUREACTIONBASE branch with vtable 0x01259D24's slots is the fix, too large for one chunk.
