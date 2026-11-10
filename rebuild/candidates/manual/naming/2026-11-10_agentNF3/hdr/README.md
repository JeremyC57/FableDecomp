# agentNF3 header overlay

Full copies of the current repo headers under `rebuild/include/`. Each copy adds `#ifdef` opt-ins only; with none of
the new macros defined, the preprocessed default path is the same as the repo header. Grade the rows with
`FABLE_HDR_OVERLAY=$S/w22/agentNF3/hdr`.

| Header | New opt-in | What it adds | Evidence | Rows that need it |
|---|---|---|---|---|
| `engine/CDefinitionManager.h` | `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX` | `long GetDefClassIndexFromGlobalIndex(long) const; // 0x009ACCA0`, declared in both class branches | Ego_r.pdb and FableWin.pdb `?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z`; the 0x006B36xx environment rows call 0x009ACCA0 on the definition manager | 006b3600, 006b3680, 006b37a0, 006b3800, 006b38d0 |
| `engine/CASoundManager.h` | `FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS` (needs `_BANKLOOKUP`, `_ENABLEFLAGS`, `_SIZE40`) | the nested `CASoundManager::CRegisteredBank` record (sound bank +0, lip-sync bank +8, `CCharString SymbolName` +0x10, 0x20 bytes) and `CVectorMap<long, CRegisteredBank> RegisteredBanks` at +0x30; it splits the existing `_pad_0x27` | Fable Anniversary 360 PDB publics (lib_sound_manager_base.cpp: `CASoundManager::CRegisteredBank`, `RegisteredBanks`), plus the TLC bodies 0x009916F0..0x009918F0, which walk 0x24-byte `pair<long, CRegisteredBank>` records at +0x30 | 009916f0, 00991690, 00991720, 009917e0, 00991750, 00991840, 00992290 |
| `engine/CASoundManager.h` | `FABLE_RETAIL_CASOUNDMANAGER_GETPSOUNDBANK_LONG` | with `_BANKREGISTRATION`, declares 0x00991840 as `GetPSoundBank(long) const` instead of the descriptive `GetBank(long)`, so one address has one name | Anniversary PDB `GetPSoundBank(long)`; the queue entry for 0x00991840 | 00417a58 |
| `engine/CBulletTimeManager.h` | `FABLE_RETAIL_CBULLETTIMEMANAGER_SLOWDOWNMULTIPLIER` | `float GetBulletTimeSlowdownMultiplier() const; // 0x004C5FA0` | Ego_r.pdb and FableWin.pdb `?GetBulletTimeSlowdownMultiplier@CBulletTimeManager@@QBEMXZ`; 0x004C6010 calls 0x004C5FA0 | 004c6010 |
| `engine/CCardPositionsDef.h` | `FABLE_RETAIL_CCARDPOSITIONSDEF_DTOR_NOVTABLE` (VC only) | `class __declspec(novtable) CCardPositionsDef` for the destructor's TU only | the retail destructor 0x004F9CFC stores no vtable of its own (31 vs 25 bytes without it) | 004f9cfc |

Header debt that was seen but left alone (only opt-in additions are allowed here):

- `CASoundManager.h` `_BANKREGISTRATION` still declares the descriptive `GetBank(long)` for 0x00991840.
- The landed CRegisteredBank helpers 0x00992510 / 0x00992630 / 0x00993030 are still spelled as an invented `CSoundBankRecord`. See `NAME_FIXES.tsv`.
