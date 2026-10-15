# Chunk 908 (skipped-row ledger: the two functions of catalogue row 00d9f1b0, and the EH prefix row 009f5d70)

- 00d9f1b0 is not CMC_Minion::OnPredicateFail: that name belongs to the 8-byte thunk at 0x00D9F1A0 (`mov ecx,[ecx+14h] ; jmp 0x00D9F1B0`,
  ParentClass at PDB +0x14). `this` here is the CQ_MinionCampScript (PDB fields at retail offset - 0x14: NumberOfMinions +0x50,
  MaxNumberOfMinions +0x54, Min/MaxMinionSpawnPeriod +0x58/+0x5c, MinionSpawnTimer +0x70). Named OnMinionKilled (descriptive).
  `if (--NumberOfMinions < 0)` with the thread-terminating bail-outs; GetTimer is CGameScriptInterface slot 90 (+0x168).
- 00d9f200 StartMinionSpawnTimer (descriptive): SetTimer(handle, (long)((max - min) * r + min)) with `float r = rand() * (1.0f / RAND_MAX)`
  as a NAMED local (kept on the FPU stack, `fmul st(1)` then `fstp st(0)` after the cast); the constant 0x0129C81C is 1/32767 folded
  from the source. The expression form (no local) gives `fmulp` and 68 bytes.
- 009f5d70 CInputManager::LoadAllEventPackages(const CVersion&, CAFile&): the EH function the 121-byte row was a prefix of. Per-iteration
  `try { CInputEventPackage package(&file); LoadedEventPackages.push_back(package); } catch (...) { break; }` with push_back declared
  throw() reproduces the states (0 before the constructor, -1 after the destructor), the funclet and the continuation; only the home
  store of `this` differs (retail at entry, lane after the header Read). Parked in differ/ after 4 attempts.
- The chunk 145 oracle still carried the 160-byte multi-row for 00d9f1b0 and shadowed the split piece (75v149) until it was removed.
- Callee labels: 0x009F5690 "CInputEventPackage::Save" is the constructor from a CAFile; 0x009F6A70 "CCombatWheel::ResetRings" is
  ~CInputEventPackage; CAFile's vtable 0x0122ECCC is all __purecall, the slot names come from CDiskFileWin32 (0x0122D06C):
  3 Read(void*, long, CASuspendableProcess*), 7 GetPosition, 9 GetLength.
