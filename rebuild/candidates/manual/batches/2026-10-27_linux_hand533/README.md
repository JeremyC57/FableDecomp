# Chunk 533 (agent533, 2026-10-28): 4 of 10 re-queued large functions landed (514-1022 bytes)

Re-queue chunk of 10 rows of 514-1022 bytes (0x009F73C0..0x00A35B70): navigation (A*, quad tree, steering), physics-mesh
collision, quaternion slerp, right-handed sets and the mesh data bank. One Opus 5.5 agent at high effort.

- **Landed:** 3 rows with `vland.py --land`, all RELOCATION_MATCH (0x00A15140 CNavigatorManager::GetNextNeighbour,
  0x00A1C700 the CSteeringAgent ctor, 0x00A27350 ~CMeshDataBank), and 0x00A22590 CQuaternion::SetSlerp as grade
  `retail_asm` (payload_retail_asm.json; dword `fistp` at 0x00A22624 / 0x00A22692 / 0x00A226FC, twin of the landed
  SetSlerpFromIdentity / CalculateSlerpWeights). Tests PASS under g++ and VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE.
- **Parked (6, differ/):** 0x009F73C0 CVsyncCallbackTimer::OnPostSwapScreens, 0x00A18B50
  CPhysicsMesh::HasCollidedWithTriangleAt, 0x00A1A7E0 GetIntersectionPointsBetweenNodes, 0x00A1CEB0
  CNavigatorAStar::Initialise (one store position), 0x00A229E0 CRightHandedSet(const C3DAngles&), 0x00A35B70
  GetRotationMatrixForTransformFromTo (frame slots, x87 order, hidden-pointer pair return).
- **Identities:** all 10 row names proven (Ego_r / FableWin donor order plus signature); 9 of 10 catalog labels were wrong.

Name fixes (NAME_FIXES.tsv, 19 rows, deferred): 0x00A26900 CMeshDataBank::CloseMeshes (bound `?h1@@YIXPAUS@@@Z`),
0x00A9D730 / 0x00A9D740 C3DAnimationManager::Initialise / Uninitialise, ~CBankFileAsync 0x009D6490 and ~CResourceBank
0x009FC690, STLport 5-argument `_Rb_tree::_M_erase` at 0x00A2DC20 / 0x00A21640, 0x00A14F70 GetNodeAt (const, byte layer),
0x00A55F90 CMatrix3x4::SetFromVectorAndAngle, 0x00A22F80 CRightHandedSet::Rotate, 0x00A155C0
CNavigatorQueueManager::RemoveNavigatorFromQueue, 0x00A76240 CNavBlockedQuadTreeNode::operator= (landed on a placeholder).
