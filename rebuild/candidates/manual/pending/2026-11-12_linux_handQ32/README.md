# Chunk Q32 (continuation of Q29's open rows, one row at a time, Opus 5.5)

6 of 6 matched and landed in landing eb, in 49 grades, 0 probes, about 100 tool calls:
- 0x00EB3BA0 CMurderTwistSheriffGuard::AttackHero: the row Q29 left one byte off after 44 grades. IsValid (0x00CD23B9) sits at an
  unaligned address among COMDATs, so it is an inline member emitted out of line: its real body goes in the row file as
  `inline __declspec(noinline)`, with `seh_Guards[ID]` written twice.
- 0x00BD2A90 CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings: FABLE_RETAIL_STLP_MAP (`map::operator[]` body visible
  but called out of line) fixed two swapped frame slots.
- 0x00958B20 CAIStateGroup_MinionReactToCrime::OnReplace (vtable 0x0128C580 slot 10).
- 0x00465C54 CCombatAbilityBlockDefBase::Transfer: new STLP_TREE_CTOR_DTOR_OUTOFLINE; flags `/d2inlS39`.
- 0x00722740 CTCSmokeGenerator::FrameUpdate: a vector iterator loop. Its first hand-back had a local CDefinitionManager stub, and it
  was sent back and fixed onto the shared header before landing.
- 0x009FC950 CGraphicFrameData::InitialiseAsSurface: stores Width, Height, Pitch in that order.
Lesson: three of the six "compiler tie" residues were facts about whether a callee's body is visible to the compiler; the other
three were model errors that the move onto the shared headers fixed.
