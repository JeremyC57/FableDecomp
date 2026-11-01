# Chunk 662 -- 10 of 10 landed (2026-11-01, NA007 of the never-attempted pool)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA007): nine rows under one propagated "_Dest_val<..C3DSkeletalMorphTarget..>"
label plus the twin-less 0x004D1DB0. Landed via `coordinator/land_chunk.sh 662` (vland --land 10/10 RELOCATION_MATCH + PASS, multi-check 0;
every row on its first shape): 0x00A0BD40 ~CASoundBank, 0x00AF6570 NParticleEngine::~CComponentInstance, 0x00A32690 ~C3DMeshVolume,
0x009F81C0 ~CGraphicDataBankInfo, 0x004D1DB0 ~CSubtitledMessage (STLport vector text inside CArray / CVectorMap), 0x00A0A5D0
_Destroy<CMeshDataBankInfo> (/Gr), 0x006966D0 _Destroy<pair<const CCharString,CCountedPointer<CActionEventBase>>> (/Gr), 0x00695F40 its
non-const ~pair, 0x00CB9700 ~pair<CCharString,CCountedPointer<CEntityScriptBindingBase>>, 0x00CBA6C0 its _Destroy (/Gr). No row parked.
At landing the coordinator respelled 0x009F88C0 (landed under the ~CGraphicDataBankInfo name, now also 0x009F81C0's) to
`std::_Destroy<CGraphicBankManager::CGraphicDataBankInfo>`: file std_Destroy_009f88c0.cpp, CompilerFlags /Gr, recheck RELOCATION_MATCH + PASS.
3 NAME_FIXES queued (c662). Labels: ghidra_out/labels_chunk662_2026-10-27.tsv (15). Emission-order-only splits (swap together if a
caller contradicts): 0x00695F40 / 0x00695F80, 0x00CB9700 / 0x00CBA6C0. Header debt: engine/CMeshBankManager.h has no nested
CMeshDataBankInfo. exp/ holds the two emission-order experiments. Lessons in GOTCHAS (1 line) and LESSONS.md.
