# wrapnew.py <model.h> <generated.h> <out.h>: NF323 Q1 form for a NEW class header: the gen_class_headers.py flat output is the default
# branch; the polymorphic model (model.h minus guard / rebuild_abi include) goes under #ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL / #else / #endif
# inserted after the generator's `#include "rebuild_abi.h"` line (the landed StartSneaking form, NF323). Asserts are pinned afterwards
# with lane_merge/pin_asserts.py against the generated tree (so stripping the opt-in gives the generator text back).
import sys, re
mod, gen, out = sys.argv[1:4]
M = open(mod, encoding='utf-8').read().split('\n'); G = open(gen, encoding='utf-8').read().split('\n')
cls = re.search(r'^class (\w+)', open(mod).read(), re.M).group(1); mac = 'FABLE_RETAIL_%s_VIRTUAL_MODEL' % cls.upper()
body = [l for l in M if not re.match(r'#(ifndef|define) FABLE_ENGINE_|#endif // FABLE_ENGINE_|#include "rebuild_abi.h"', l)]
while body and body[-1] == '': body.pop()
body = [('// Opt-in form (NF323 Q1 ruling; NF327, 2026-11-24): the generated flat header below is the default branch; a Clone TU defines '
         + mac + '. Needs FABLE_RETAIL_CCREATUREACTIONBASE (the retail base).') if l.startswith('// New header: no class model') else l for l in body]
i = G.index('#include "rebuild_abi.h"'); j = max(k for k, l in enumerate(G) if l.startswith('#endif // FABLE_ENGINE_'))
R = G[:i + 1] + ['#ifdef ' + mac] + body + ['#else'] + G[i + 1:j] + ['#endif // ' + mac] + G[j:]
open(out, 'w', encoding='utf-8').write('\n'.join(R))
print(out, mac)
