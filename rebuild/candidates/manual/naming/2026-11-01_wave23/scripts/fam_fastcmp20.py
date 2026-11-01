# fam_fastcmp20.py <ov> <rows.txt> [tag]: naming wave 20, family fastcmp -- the descriptive CCharString::FastCompare (0x009E6530) is
# ??MCCharString@@QBE_NABV0@@Z (FableWin.pdb; the CCompareString comparator of the landed chunk-686 sort rows): every row that declares
# `bool FastCompare(const CCharString&) const` on its CCharString model is respelled to `operator<` (declaration, calls as `a.operator<(b)` ->
# kept textual, comments). Kept only when the object loses exactly the FastCompare symbol and gains exactly the operator< one.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F
ov, rf = sys.argv[1], sys.argv[2]; tag = sys.argv[3] if len(sys.argv) > 3 else 'fastcmp'
OLD = '?FastCompare@CCharString@@QBE_NABV1@@Z'; NEW = '??MCCharString@@QBE_NABV0@@Z'
def transform(t, a):
    if 'FastCompare' not in t: return None, 'no FastCompare'
    t2 = re.sub(r'\bFastCompare\b', 'operator<', t)
    # `x.operator<(y)` is legal; a comment marker records the respell once
    t2 = t2.replace('bool operator<(const CCharString& other) const;      // 0x009E6530', 'bool operator<(const CCharString& other) const;      // 0x009E6530 ??MCCharString@@QBE_NABV0@@Z (FableWin.pdb; was the descriptive FastCompare -- naming wave 20)', 1)
    return t2, 'FastCompare -> operator<'
def check(S0, S1, a):
    return F.diffcheck(S0, S1, lambda s: s == OLD, lambda s: s == NEW, need=[NEW])
F.run(ov, F.rows_from(rf), transform, check, tag)
