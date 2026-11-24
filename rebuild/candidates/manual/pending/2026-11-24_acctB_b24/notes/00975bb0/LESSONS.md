- (B24A7, 0x00975BB0) x87 sum-term order of CSE'd products follows the TEMP keys ((temp# << 6) & 0xFFFF, i.e. mod 1024), not the source
  order. A free-list XX temp versus fresh ZZ / YY decides it, and the count of temps over the WHOLE function shifts every site alike, not
  only positionally. Measure with il2.sh (dump2 + pp2, 2 min). The cure that fits the evidence was dead release ASSERTs, which FableWin's
  +734i row size supports.
- (B24A7) FableWin uses Dinkumware std (bitset _Xran / _Tidy): FableWin sizes of STL members are no evidence for STLport texts.
- (B24A7) The B24B3 ghash.py matched nothing here (it is uncalibrated outside the CTimer bodies). Sizes stay the only FableWin check.
- (B24A7) A #pragma optimize / inline_depth around a member template (or an explicit instantiation) does not reach the instantiation.
  An explicit SPECIALISATION `template<> inline __declspec(noinline)` inside the pragma does (CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26 pattern).
