# Chunk F1 (Fable 5.2, rapid fire): chunk Q30's seven open rows

7 of 7 matched (136 grades, about 90 tool calls) and landed in landing eg. Seven independent evidence verifiers checked them
(EVIDENCE_VERDICTS.md): five clean, two with small findings that the coordinator fixed before landing (NOTES.md, last section).
- 0x00878BE0 NCameraModes::CProjectileWeapon::GetPosFromParams: `return pos / 1.0f + offset + ZAxis * height_param;` -- the unit
  divisor is INFERRED and marked so (see the source header for the nine graded spellings).
- 0x00877720 NCameraModes::CTools::GetStandardThings: the landed sibling 0x00878390's idiom.
- 0x009E3B60 CPixelFormat::ConvertPFDToRGB: GetXPosShift as local-reference field sums; pfd.GetLong() read inside each channel.
- 0x00AD9770 CVertexGroupBlockList::RemoveAllSubsets: callee bodies visible (IsProperSubsetOf, vector operator=, _M_insert_overflow).
- 0x00BC2480 CEngineInternalPrimitiveParticleMesh::Update: three float scales, in-place `*=`, grouped GetRight, element-wise negated
  row stores.
- 0x00801920 CTCIdleScheduler::RegisterIdleStateGroup: GetDef / GetSubDef / const tree find bodies visible; the period looked up twice.
- 0x00407CD0 CUserProfileManager::RedefineControlPassive: AssignedInputsPassive[i] spelled at every use; GetDirection body visible.
Made-up names (NOTES): GDefaultIdlePeriod (global 0x013BAF1C), IsSameControl (file-local helper), CPFDColour::Long.
