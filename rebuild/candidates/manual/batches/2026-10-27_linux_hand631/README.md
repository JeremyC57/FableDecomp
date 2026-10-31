# Chunk 631 -- 3 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 631` (vland --land 3/3 RELOCATION_MATCH + PASS, multi-check 0):
C2DLineF::GetInfiniteLineLineIntersection 0x00A57960 (PDB; the four end points copied into C2DVector locals with a memberwise user
copy ctor -- spilled float copies keep their offsets inside the 8-byte struct slot, giving retail's 0x20 frame; cross terms as
x2*y1 and x4*y3), CCodaVoice::SetVibrato 0x00C59C60 (CODA descriptive, class established by 31 siblings; both factors as the
<math.h> float pow overload, which schedules `pop edi` ahead of the last fstp where a hand-written helper does not; the stale
binding ?ApplyStageTransform@CMaterialState_00c59fc0 from caller 0x00C59FC0 was replaced by what that caller already emits),
CDSoundStreamBuffer::CreateLoopingBuffer 0x00C56D70 (gap-scan stub sub_00c56d70; class from the vtable shared with the landed
CreateAndPlay; the Play test as one more early return keeps the shared false block after the first test). Seven rows stay DIFFER
in `pending/2026-10-27_linux_hand631/differ/` (00725b70 frame 0x24 v 0x28 after a same-file noinline GetAllThingsInRoughArea
stand-in closed 205v201, 00aeb600 clone register, 00943c20 scratch register, 008f99a0 LSocket::Receive(vector<char>&) string-ctor
scheduling, 009bbbf0 frame 0x38 v 0x40, 00a5d000 `xor eax,eax` 3 slots late after raw float pointer fixed both x87 operand orders,
00c351b0 CVirtualPlayer::Start ebx/edi -- bound signature's long first parameter is dereferenced as a pointer: identity decision).
17 NAME_FIXES (std_foreach_00726660 functor, GFGetPVillageInterface 0x007EE540 -> CTCVillageMember::GetPVillage, SecondsToMixBlocks
0x00C59B00, CTCBase GetThingManager / PeekThingSearchTools 0x00686AE0 / 0x006AFD40, IsInWorkHours 0x0091A6F0, NParticleEngine
callees, basic_string::assign, CCharString ctor, _DSPROPSETID_EAX20_BufferProperties 0x012BEEA0) + 4 review rows queued (prefix c631).
Labels: ghidra_out/labels_chunk631_2026-10-27.tsv.
