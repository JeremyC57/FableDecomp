# Chunk 634 -- 1 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 634` (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0):
CEffectFilter::SetLowPass(long, float, unsigned long) 0x00C74AB0 (catalog label CAudioChannel_ComputeShelveFilterCoefficients was
propagated; the body filds a long first argument and the coefficients are a second-order Butterworth low-pass). The lever was the
landed sibling SetHighPass 0x00C74BB0's: `frequency = pow(...); frequency *= reference;` as two statements puts the unsigned
sample-rate test before the fmul. At landing the caller CEffectPart260::Configure 0x00C783F0 was respelled (SetShelf(float,float,ulong)
-> SetLowPass(long,float,ulong), fields LowPassCutoff/LowPassReference, member LowPass) and rechecked RELOCATION_MATCH; its binding
row replaced. Nine rows stay DIFFER in `pending/2026-10-27_linux_hand634/differ/` (00c14f90 PKCPlaybackTemplate dtor lea two slots
early -- real libpk member classes fixed the +0x0c zero store --, 006afaf0 CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject
(PDB spelling) XY-sum operand order, 0076e4d0 GetLastWeaponTrailPoints push_back copy interleave, 0076c8a0 CTCEnemy ctor ebp/edi swap
-- now on the shared CDefPointer.h model with real GetDef/GetSubDef bodies, bytes identical to the FindDef_* stand-ins --, 00ac7200
MakeOrthogonal dot z term, 00c72d00 CAudioSource::BindSound callee-saved assignment, 00946190 CAIStateGroup_PrepareMeal::Update
ecx/eax, 00a55f90 SetFromVectorAndAngle operand order + zero stores, 00547050 CScrollingComponent::UpdatePosition load order).
Lessons: VC7.1 picks x87 operand order itself (park "x87 canonical operand order" fast); real member classes fix `p = 0` after
`delete p` in dtors; CDefPointer.h + real GetDef bodies retire the FindDef_* stand-ins at equal bytes. 18 NAME_FIXES + 3 review rows
queued (prefix c634): PDB spellings for CTCBase ctor/GetDefinitionManager/GetWorld, CDefinitionManager::GetDefNameFromGlobalIndex,
CDefStringTable::GetString, CFactionManager::GetPFaction, RemoveMode, four CAIStateGroupBase protected members; libpk descriptive
respells (PKCPlaybackTemplate for vtable 0x012B29F8 -- owner call --, CPKListImpl dtor 0x00C19250 with 17 spellings, CFlagOverride::ApplyTo).
Labels: ghidra_out/labels_chunk634_2026-10-27.tsv.
