- (agentC17, 0x00BA5260) Extern GLOBAL NAMES move c2 record-order ties: respelling made-up globals to their PDB names (NEngineInternal::GP*)
  flipped both an x87 product order and a bookmark test/or form. Fix every global/callee name BEFORE fitting any count-sensitive residue.
- (agentC17) A same-TU callee body only removes a pinned-store residue if ITS callees do not capture either: SetupStaticBlocks 0x00BA50A0
  matched only once the out-of-line-at-TU-end GetDiffuseTexture/GetBumpTexture bodies were visible (their `bool& failed` no longer escapes).
- (agentC17) Bookmark test/or form key (hand892: IL-tuple count) measured by DELETION after the site: removing one inline expansion after
  the cull site fixed it; deleting plain calls did not. Calibrate with deletions before/after the site to learn which side counts.
- (agentC17) fwsize finds evidence-backed IL changes that are code-neutral: SetEnableFadedShadowFactor 25i->24i (member read, no Peek call),
  PeekMesh 9i->10i (through PeekRenderInfo), GetAppropriateMipmap 75i->78i (C3DBoundingSphere::GetCenter). Use them as natural 'units'.
- (agentC17) Dead `const T r = x;` rulers count as IL units; unused locals and single-use named locals do not (forwarded): do not expect
  a named local to replace a ruler.
