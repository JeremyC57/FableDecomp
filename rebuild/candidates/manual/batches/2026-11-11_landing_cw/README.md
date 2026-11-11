# 2026-11-11 landing cw -- chunk Q13 (rework of the closest residues, Opus 5.5): 3 rows

vland --land: 3 wins (RELOCATION_MATCH), tests PASS. bindings_multi_check is at its NullCoord baseline.
- **0x00DD9520 NScript::CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene.** The inline GetAngleXY slot wrapper returns
  its call parenthesised; the PAREN tuple moves one fstp a scheduling slot later.
- **0x0092ACE0 CAIStateGroup_ApprenticeHero::GoToLibrary.** The look turn speed 0x0138CBA8 (.data, 0.01f) is a file-static
  `static float g_LookAtTurnSpeed = 0.01f;`. Other landed rows declare the same name extern; the relocation binds by name.
- **0x009DD960 CRenderManager2D::Draw2DLine.** The two RHW stores are two plain reads of the screen RHW float placed after the
  Z zeroes, which c2 merges into one load plus a register copy.

No headers. Labels: ghidra_out/labels_landing_2026-11-11cw.tsv. The 23 residues are in pending/2026-11-11_linux_handQ13.
