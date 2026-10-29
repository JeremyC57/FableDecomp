# fam_opy8.py <file>...: CCharString::operator+= returns const CCharString& (Ego_r.pdb + FableWin.pdb:
#   ??YCCharString@@QAEABV0@ABV0@@Z 0x0099F0A0, ??YCCharString@@QAEABV0@PBD@Z 0x0099F100, ??YCCharString@@QAEABV0@D@Z 0x0099F180).
# Respells in-class declarations `CCharString& operator+=(` and out-of-class `CCharString& CCharString::operator+=(`.
# Idempotent (an already-const declaration is left alone).
import sys, re
PAT = re.compile(r'(?<![\w:])(?<!const )CCharString(\s*)&(\s*)(CCharString::)?operator\+=(\s*)\(')
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
    n = PAT.sub(lambda m: 'const CCharString' + m.group(1) + '&' + m.group(2) + (m.group(3) or '') + 'operator+=' + m.group(4) + '(', t)
    if n != t:
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(n); print('edited', p)
    else: print('skip', p, 'no non-const CCharString operator+=')
