# Chunk 712 (NA055) -- 10 of 10 byte-exact (RELOCATION_MATCH), one opt-in branch, one new header (2026-11-02)

Rows (`verify.log`: WINS 10, all PASS; eight of ten labels wrong): CScratchBuffer::Uninitialise 0x009F4950, CMeshDataBank::FrameEnd 0x00A23AD0
(its only caller loops over CEngineResourceManager's MeshBanks[3]), the NProfileTimer `dynamic atexit destructor for 'LogFile'` `_$E2`
0x0122A790, and seven destructors named from their vtables / ??_G callers: ~C3DMeshFileEngineMaterialChunk 0x00A9D270, NScript::
CV_FisticuffsClubScript::CFisticuffsMember::~CFisticuffsMember 0x00E839F0 (no RTTI: vtable stored only by the landed ctor), ~CThingPhysical
0x006A5D50, ~CTCFishingSpot 0x007B0300, ~CTCScriptedControl 0x00719C70, ~CTCPhotojournalCapture 0x0055D590 and NParticleEngine::
~CPSCUpdateNormal 0x00B0CA90 (the three "CExclusiveAssert" labels were wrong).

Headers (hdr/README.md): opt-in FABLE_RETAIL_CMESHDATABANK (engine/CMeshDataBank.h: the banks typed CResourceBank, FrameEnd declared; layout
unchanged), new engine/CResourceBank.h (: CBaseClassNonCopyable, vtable 0x0129C814, sizeof 0x7C), and `void RemoveFromGlobalTCs();` inside the
existing FABLE_RETAIL_CTCBASE branch of engine/CTCBase.h (a non-virtual declaration). Default text of each unchanged under `cpp`.

Second spellings at landing: CTCFishingSpot::RemoveParticles 0x007B01C0 and NParticleEngine::CPSCUpdateNormal::KillAllDecalEmitters 0x00B0C730 are
protected in both PDBs (`IAE`); the landed own rows and their landed ??_G callers 0x007B06C0 / 0x00B0F990 declared them public, so all four were
respelled in this commit (access only; bindings updated). Recheck of the 67 landed FABLE_RETAIL_CTCBASE / CTCDoor / CMeshDataBank / CResourceBank
includers plus those four rows: 71 of 71 = 6 MATCH + 65 RELOCATION_MATCH, all PASS (includer_recheck_grades.tsv). Queued PRIORITY (NAME_FIXES_QUEUE.tsv, c712): three landed rows whose
own names are wrong (0x0051DFE0 labelled `$E2` is CThingManager::RemoveFromPhysicalList; 0x005283C0 / 0x00718EC0 labelled SortTreeRecursively are
STLport `~_List_base` instantiations), CInitBaseClass::Uninitialise 0x0099A360 (three stand-ins), the STLport list clear 0x00B0F630, ~CThingPhysical
bound as a CEngineSubPrimitiveGameMesh stand-in by 0x005297D0 (whose comments describe another function -- recheck it), and CMeshDataBank::FrameEnd
bound as a CResourcePool stand-in by 0x00B3B870. The agent's 25 NAME_FIXES rows are queued as c712; labels in ghidra_out/labels_chunk712_2026-10-27.tsv.
