# Chunk 1012b lessons (GOTCHAS-ready)

- **LoadGameState shape (0x004A3200 / 0x004A34F0 / 0x004A5E90).** Write the three outcomes as one `if (anyDir) { if (found) {...return true;} } else if (found && PathExists && IsPathFile) {...} else {scan...}` chain. Do not use a separate `if (!found) return false;`.
  - With the chain, the not-found any-directory exit falls out to the shared `return false`. Its path-destructor block is then the same IL block as the scan arm's fall-out.
  - The separate return gives that exit its own pre-cross-jump destructor. That has two effects: (a) c2 hoists `lea ecx,[path]` above the `je` (AutoLoadRespawn manual copy, retail 0x004A6117 `je 4A6242h`); (b) path gets an 11th IL reference, which ties its frame slot with fullPath+heroState in AutoLoadRegionLeaving.
  - The chain matches the landed 0x004A3200 too. One TU, one body.
- **c2's frame-slot final sort is K&R quicksort** (c2.dll 0x107AE0C4; comparator 0x107AE9C2 = the /GS-buffer flag, then density w*1000/size descending, ties 0).
  - Pivot: the middle element is swapped to the front. Partition: Lomuto, with swap only on a strict `<`. The recursion is left side, then a loop on the right side.
  - $A/tools/kr.py emulates it. It reproduces the traced orders. Use it to find which IL-weight change flips an equal-density pair before trying spellings. On a tie the result depends on the whole slot array, not on the pair alone (0x004A34F0).
- **IL weights count pre-cross-jump references.** Adding or removing one early exit inside a local's scope (an extra `return`, or a `break` out of a `do { } while (false)`) changes that local's frame weight, and the code can stay identical after cross-jumping. Look for the natural structure that does this (above) before reaching for a device.
- **EA handlers that unpack a CGameEventDataPackage use the typed readers.** `package.GetUHUGE(); x = package.GetFloat(); ...; (EGameAction)package.GetSLONG()` (FableWin-only inline members) gives retail's packing of the read-value frame slots: the uid shares a slot with the action. The `GetData(sizeof v, &v)` form does not (0x0062BD80: masked diff 66 -> 40).
- **Catalog extent:** 0x0062B070 EAOpenHeroInformationScreen is 891 bytes. The 1872-byte extent swallows four more CWorld EA handlers (0x0062B3F0, 0x0062B4D0, 0x0062B550, 0x0062B5B0).
