- Wine 'free(): invalid pointer' on every program = /run/user/0 missing (mkdir -p /run/user/0; chmod 700); unset TMPDIR TMP before cl.
- A SIB base/index residue between an imported `_this` leaf and a strength-reduction temp is decided by c2's 16-bit leaf hash: temp
  64n mod 65536 vs local ((n&0x7ff)<<5)^(n>>11); SR temps (kind 15) have a fixed number base, so only the local's number moves, and it
  counts every c1 symbol of the whole function plus inliner imports before it (position-independent). lk.py + fsolve size the window.
- Codex inline devices are often over-fitted: test each inline_depth region / FORCEINLINE alone; 6 of 10 on 00f1eed0 were byte-neutral.
