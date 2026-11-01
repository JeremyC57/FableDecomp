# Chunk 653 -- 3 of 10 landed (2026-11-01, increased-attempts + retail-fidelity brief)

Cut 2026-11-01 from the fixable bucket (residues 6-8 bytes). Landed via `coordinator/land_chunk.sh 653` (vland --land 3/3
RELOCATION_MATCH + PASS, multi-check 0): 0x00C4DC40 `CAudioBuffer::Create` (OpenAL wrappers as inline members of the landed
CAudioContext model; 3 probes), 0x0089F300 `CGameScriptInterface::CreateCreatureNearby` (engine/CScriptThing.h + CGameScriptInterface.h;
same-file noinline const `CVectorMap::LowerBound` keeps the pointer callee-saved; 10 probes), 0x00712340
`NTCScriptedControl::CActionTalkBase::GetCameraSetupPositions` (exact Ego_r leaf; the prior's camera scales were on the wrong cases
and the relocation mask hid it -- `lane_tools/const_check.py` now checks __real@ constants; 5 probes).
Parked in `pending/2026-10-27_linux_hand653/differ/` (STATUS.tsv has residue + probe counts): 005ebfa0 CameraUseCameraPoint 321v329,
00c5a9d0 CCodaVoice::SetModulation 382v382 (catalog ReadFromFile was wrong), 00896b60 KickOffQuestStartScreen 437v445, 00666310
CThingCreatureBase::OnCreate 915v908 (header debt: the 0x15c..0x15e bitfields are flattened in engine/CThingCreatureBase.h), 00984f60
FleeUpdate 856v863, 00e37e40 CM_AssassinTrigger::Main 1104v1103 (behaviour bug in the prior fixed), 00e4aa00 CBanditLeader::Main 1362v1356.
9 NAME_FIXES queued (c653). Labels: ghidra_out/labels_chunk653_2026-10-27.tsv (12). Lessons in GOTCHAS (2 lines) and LESSONS.md.
