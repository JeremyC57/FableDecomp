# fam_atc8.py <file>...: CThing::AddTC takes `const CTCParamsBase*` (Ego_r.pdb + FableWin.pdb
#   ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z, 0x004C9D60). Respells the third parameter of every
#   `AddTC(const CCharString& n, bool b, CTCParamsBase* p)` declaration / definition; in a test that does so, every other
#   `CTCParamsBase*` (the stand-in's argument records) becomes const too. Idempotent.
import sys, re
PAT = re.compile(r'(\bAddTC\(\s*const\s+CCharString\s*&\s*\w*\s*,\s*bool\s*\w*\s*,\s*)(?<!const )(class\s+)?CTCParamsBase(\s*)\*')
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    t = PAT.sub(lambda m: m.group(1) + 'const ' + (m.group(2) or '') + 'CTCParamsBase' + m.group(3) + '*', t)
    if '/rebuild/tests/' in p.replace('\\', '/') and t != o:   # test stand-ins record the argument: their records go const too
        t = re.sub(r'(?<![\w])(?<!const )(?<!class )CTCParamsBase(\s*)\*', r'const CTCParamsBase\1*', t)
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'no non-const AddTC params')
