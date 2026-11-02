# sfbcat.py <rows> [errfile]: classify CSpawnedFuncBase.h includers (source text) by how they define CCharString / CCountedPointer / CCPPointerInfo
import sys, os, re, collections, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
R = rows(); A = [l.split()[0] for l in open(sys.argv[1]) if l.strip()]
err = set(l.strip() for l in open(sys.argv[2])) if len(sys.argv) > 2 else set()
c = collections.Counter(); per = {}
for a in A:
    t = open(os.path.join(ROOT, 'rebuild/src/compiled', R[a]['Source']), encoding='utf-8', errors='replace').read()
    i = t.find('engine/CSpawnedFuncBase.h'); j = min([x for x in (t.find('engine/CActiveEntityScript'), t.find('engine/CSpawnedFunc.h'), t.find('engine/CSpawnedFunc1.h'), i) if x >= 0] or [len(t)])
    pre = t[:j]
    k = []
    k.append('retail' if re.search(r'#define FABLE_RETAIL_SUSPENDABLE_PROCESS', pre) else ('noinc' if i < 0 and j == len(t) else 'flat'))
    k.append('cpH' if 'engine/CCountedPointer.h' in pre else ('cpL' if re.search(r'(class|struct)\s+CCountedPointer\b[^;]*\{', pre) else ('cpLlate' if re.search(r'(class|struct)\s+CCountedPointer\b[^;]*\{', t) else 'cp-')))
    k.append('piL' if re.search(r'(class|struct)\s+CCPPointerInfo\s*\{', t) else 'pi-')
    k.append('csH' if 'fable_string.h' in pre else ('csL' if re.search(r'(class|struct)\s+CCharString\s*\{', pre) else 'cs-'))
    k.append('ERR' if a in err else 'ok')
    c[tuple(k)] += 1; per[a] = k
for k, v in sorted(c.items(), key=lambda x: -x[1]): print(v, k)
json.dump(per, open(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'runs', 'sfbcat.json'), 'w'))
