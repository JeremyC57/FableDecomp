# fam_i2w8.py <file>...: 0x00405F00 is GFIntToWideString (Ego_r.pdb + FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z;
#   the only `CWideString __fastcall (long)` GF*ToWideString in both PDBs, Ego_r 0x00404F90; body formats L"%d"). Renames the
#   identifiers IntToWideString / LongToWideString (declarations, calls, comments) to GFIntToWideString. Idempotent.
import sys, re
PAT = re.compile(r'(?<![\w])(IntToWideString|LongToWideString)\b')
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    t = PAT.sub('GFIntToWideString', t)
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'no IntToWideString')
