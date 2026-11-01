# 2026-10-27_linux_hand668 — chunk 668 (NA013, never-attempted pool)

Landed 2026-11-01 via `land_chunk.sh 668`: 10/10 (1 MATCH, 9 RELOCATION_MATCH), bindings multi-check 0.
Rows: 0x00B42A90 ~CEngineMap, 0x00991880 ~CASoundManager, 0x008D7A90 / 0x008D7B20 CModeGuildmasterPowerUpShield/Speed::OnDeactivate,
0x00B52870 / 0x00B52900 CEngineWeatherRenderer::FadeRainToTarget / FadeSnowToTarget, 0x00558C70 CNavButton::ChangeState,
0x00A3BD00 CBaseVideoRenderer::QueryInterface, 0x009C2FD0 ~CRenderManager2D, 0x00A75E30 CNavQuadTreeNode::SetUpNeighbours.
Pairs (Fade*, OnDeactivate) share one source each (one retail TU); TU-emitter ctors for CTextureRenderer and CRenderManager2D.
Decision (coordinator under owner delegation 2026-11-01): the retail FadeSnowToTarget copy-paste (reads rain strength +0x5C8) stays, faithful to retail.
NAME_FIXES.tsv (18 rows) queued as c668; labels in `ghidra_out/labels_chunk668_2026-10-27.tsv`; lessons folded into GOTCHAS.
