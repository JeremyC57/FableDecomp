# Landing nf177 (Account A, 2026-11-20): returned render / particle / TC entries (32 rows; 0x009A80B0 held)

Agent NF177 (round 10) took the entries NF172 / NF173 returned plus their name fixes: 28 entries, 13 done, 5 done-already, 1 rejected,
8 blocked, 1 deferred. Its files are in `naming/2026-11-20_agentNF177/`.

- 32 rows land. Self renames (15): CBankFileManager::GetDevBankFilePath / GetDevHeaderFilePath / GetRetailHeaderFilePath, the four
  NPlayerGui CDraw* ClearEffects, CTCWeapon::IsUsableBy 0x005DAB10, NParticleEngine::CParticleEmitter::Set2DPosition 0x00AED090,
  CEngineLandscapeMap::OnGameMapUnloaded 0x00BDBFF0, CMemoryAllocator::NewFrame 0x009F26D0, CEngineScreenEffectRadialBlurRenderer::
  CaptureEnvironmentParameters 0x00B5BD70, COpinionReactionManager::GetToleranceToBeingHit 0x00701940, and the D1 family nf172 left:
  the const tree find 0x00BE0930 with CEngineStaticMapFileBlockManager::IsBlockLoaded const 0x00BE0480 and its five callers. q6927 is
  done as the port NF173 was told to make (0x009D56C0 onto the shared NBankFileManager headers; its CCountedPointer<CRetailBank> dtor
  copy is byte-identical to the owner 0x009A9D60). File names follow the self renames.
- Headers (add-only opt-ins, `orig` equals HEAD): fable_retail_banks.h (5), engine/CBankFile.h (1), engine/CTCWeapon.h (ISUSABLEBY),
  engine/CEngineStaticMapFileBlockManager.h (ISBLOCKLOADED), candidates/landscape_background_lod_compiled.h (ISBLOCKLOADED_CONST;
  pin_asserts line-pinned one existing assert, generated name unchanged).
- Held: 0x009A80B0 (q4391's last binder, CBankFileManager::BankExists). Its port graded RELOCATION_MATCH, but its object newly emits
  `std::less<CCharString>::operator()` with a 43-byte body against the owner row 0x0042AAB3's 41 bytes (a size-optimised TU). That is
  the case the advisor's pick-any rule (EXTRA_RULES 2026-11-20) holds. The row is back at HEAD (`logs/recheck_after_hold.log`: all 33
  objects of the tree, 3 MATCH + 30 RELOCATION_MATCH, PASS; table rebound), the draft is in `naming/.../held/`, and q4391 is requeued.
- Pick-any check (`logs/pickany_check.txt`): every other symbol a row newly defines for another address is byte-identical to its owner
  (bitset::test, the list / tree iterator comparators against the folded 0x0041C820 / 0x0041C830 stand-ins, CBasicString operator<,
  _M_incr, the CRetailBank counted-pointer dtor). HasTC 0x004118C0 has no landed row.
- Coordinator calls: 0x005DAB10 reaches 0x005DA7F0 through `static_cast<const CTCWeapon_Getters*>(this)` to keep that address's landed
  stand-in spelling (it already has three); accepted as the least-bad form, with the 0x005DA7F0 family respell queued. q5054 is deferred
  under the vendor rule (GameCODA, LIGHT renaming only); NF177's SDK-shaped proposal CSalContext::AllocateVoice is recorded. q11918 is
  rejected: 0x00B4B830 is the private UninitialiseTextures and the virtual OnReleaseDefaultPoolResources is 0x00B4B880 (fix queued).
- Recheck (`logs/recheck.log`): 3 MATCH + 30 RELOCATION_MATCH, every test PASS (before the hold). rebind multi-check 1 (baseline);
  split gate 0 created; includers 203 good, 0 bad; owner recheck good. tablecheck: nine addresses shrink, none grows.
- Link (`logs/link_landing.log`): 3 LNK2005 against 4 at HEAD (`logs/link_control_head.log`); the CVectorMap::LowerBound const clash is gone.
