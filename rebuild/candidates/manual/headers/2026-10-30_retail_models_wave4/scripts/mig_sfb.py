#!/usr/bin/env python3
"""Wave 1 family SFB: a row-local opaque CSpawnedFuncBase stand-in (`virtual ~CSpawnedFuncBase(); RunSuspendableProcess;
SuspendSuspendableProcess(bool); Restart; SuspendableProcess() = 0; unsigned char _body[0x30]; ctor`) becomes the shared
FABLE_RETAIL_SUSPENDABLE_PROCESS header chain; a row-local trivial CBaseClass becomes engine/CBaseClass.h (the chain includes
it). Idempotent: rows without the local block are skipped. usage: mig_sfb.py <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
OK_SFB = re.compile(r'^(?:class|struct) CSpawnedFuncBase \{ public: (?:'
    r'virtual ~CSpawnedFuncBase\(\); |virtual void RunSuspendableProcess\(\); |virtual void SuspendSuspendableProcess\(bool \w*\); |'
    r'virtual void Restart\(\); |virtual void SuspendableProcess\(\) = 0; |virtual void v00\(\); |unsigned char _body\[0x30\]; |'
    r'CSpawnedFuncBase\(const CCharString& \w+, unsigned long \w+\); )+\};$')
OK_BC = re.compile(r"^(?:class|struct) CBaseClass ?\{ ?public: ?(?:CBaseClass\(\); ?|CBaseClass\(\) ?\{ ?\} ?|virtual ?~CBaseClass\(\); ?|virtual void v00\(\); ?)+\};$")
SFB_REPL = ('// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers\n'
            '//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :\n'
            '//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).\n'
            '#define FABLE_RETAIL_SUSPENDABLE_PROCESS\n#include "engine/CSpawnedFuncBase.h"\n')
BC_REPL = '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0\n'
def fix(t):
    b = find_block(t, "CSpawnedFuncBase")
    if not b: return None, "no local CSpawnedFuncBase"
    blk = t[b[0]:b[1]]
    if not OK_SFB.match(norm(blk)): return None, "unexpected CSpawnedFuncBase shape: " + norm(blk)[:200]
    bc = find_block(t, "CBaseClass")
    if bc and not OK_BC.match(norm(t[bc[0]:bc[1]])): return None, "unexpected CBaseClass shape: " + norm(t[bc[0]:bc[1]])[:200]
    if bc and bc[0] > b[0]:   # CBaseClass defined after the chain include: drop it (the include defines it)
        t = t[:bc[0]] + t[bc[1]:]
        t = t[:b[0]] + SFB_REPL + t[b[1]:]
    else:
        t = t[:b[0]] + SFB_REPL + t[b[1]:]
        if bc: t = t[:bc[0]] + BC_REPL + t[bc[1]:]
    return t, "edit"
if __name__ == "__main__":
    for f in sys.argv[1:]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        open(f, "w", encoding="utf-8").write(r); print("edit", f)
