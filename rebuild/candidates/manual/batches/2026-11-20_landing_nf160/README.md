# Landing nf160 (Account A, 2026-11-20): engine resource / texture / input names (41 rows)

Agent NF160 (round 10) handed back 22 entries (10 done, 2 done-already, 10 blocked); its files are in `naming/2026-11-20_agentNF160/`.

- 41 rows. q6213: 14 rows (the NPlayerGui draw classes and others) replace the local CRetailHandleVector stand-in with STLport
  std::vector<CEnginePrimitiveHandle>, so 0x00443F90 binds ?resize@?$vector@VCEnginePrimitiveHandle@@...@std@@QAEXI@Z.
  q4300: 11 input rows respelled onto engine/CInputEvent.h. 0x00499D90 StopProgress ported onto the shared headers.
- Self renames (6): 0x009FA280 CTexture::Initialise, 0x00A04110 CInputEvent::SetAsJoystickButtonReleased,
  0x00B40230 CEngineWorldMap::OnGameMapUnloaded, 0x00B41DB0 CEngineMap::SetMapVisibility, 0x00B42390 / 0x00B423B0
  CEngineWorldMap::SetMapVisibility / SetMapVisibilityAllMaps.
- Headers (add-only declaration opt-ins, `orig` equals HEAD): engine/CEngineMap.h, CEngineWorldMap.h, CLocalDetailCacheMap.h and
  engine/_quarantine/CEngineLandscapeMap.h (its 26 existing sizeof / offsetof asserts are now line-pinned).
- Advisor decision (owner delegated), the `??1CDrawBase@NPlayerGui@@QAE@XZ` COMDAT: seven q6213 rows now emit the implicit
  destructor of their row-local CDrawBase model as a pick-any COMDAT. Six (0x0064E24F, 0x0064E83F, 0x0064E8DC, 0x00651904,
  0x00654258, 0x006542A4) are identical to the copy the landed 0x0064E1E4 already emits (8 bytes, checksum 3859706184), so they
  are accepted. 0x00658240's copy is 51 bytes: it has no size pragma, so the vector destructor inlines. Two pragma placements did
  not change it (`logs/cdrawbase_fix_attempt_form1.log`), so it lands as handed back with a DEBT line. Nothing binds the symbol and
  the link adds no clash; the port onto engine/NPlayerGui_CDrawBase.h (destructor out of line) removes all eight copies.
- Recheck (`logs/recheck.log`): 1 MATCH + 40 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0
  created; includers 256 good, 0 bad. tablecheck: 0x004A96F0, 0x00F36F40 and 0x00687B30 show one more spelling each. Those
  spellings appear in the same text at HEAD (0x006542A4, 0x00F37440, 0x00689C40) but their bindings were missing from the HEAD
  table. All three are queued.
- Link (`logs/link_landing.log`): 2 LNK2005, ??0CGameEvent(long) and CEnginePrimitiveHandle::Clear, the same at HEAD
  (`logs/link_control_head.log`); Clear is newly queued as DEBT.
- Queued: the preload_bump_texture_compiled.h CResource header claim (q11728 / q11729 / q11867 / q11868), the 0x00499880 port
  (q4293 / q6169), and the rest of the blocked rows (see APPLIED / STATUS).
