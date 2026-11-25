"""feat.py: function shape features for retail addresses and SDK lib COMDATs (mnemonic token streams)."""
import sys, bisect, functools, re
sys.path.insert(0, __import__("os").path.dirname(__file__))
from common import *
import capstone
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/libsig")
import coff_lib
MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
@functools.lru_cache(None)
def manifest():
    a = []
    for f in tsv(R + "/rebuild/manifest/functions.tsv"):
        a.append(int(f[0], 16))
    return sorted(set(a))
def extent(va):
    m = manifest(); i = bisect.bisect_right(m, va)
    e = m[i] if i < len(m) else va + 64
    return min(e - va, 4096)
def tok(ins):
    ops = ins.op_str
    ops = re.sub(r"0x[0-9a-f]+", lambda m: "I" if int(m.group(0), 16) > 0x1000 else m.group(0), ops)
    return ins.mnemonic + " " + ops
def toks_bytes(b, va=0):
    out = []
    for ins in MD.disasm(b, va):
        out.append(tok(ins))
    return out
@functools.lru_cache(None)
def retail_toks(va):
    b = rd(va, extent(va))
    t = toks_bytes(b, va)
    # trim trailing int3 / nop padding
    while t and (t[-1].startswith("int3") or t[-1].startswith("nop")): t.pop()
    return tuple(t)
class Lib:
    def __init__(self, path):
        self.f = {}
        for m, o in coff_lib.Archive(path).objects():
            for cs in o.code_sections():
                nm = cs.primary
                if nm not in self.f or len(self.f[nm][0]) < len(cs.data):
                    self.f[nm] = (cs.data, cs.relocs)
    @functools.lru_cache(None)
    def toks(self, nm):
        if nm not in self.f: return None
        b, r = self.f[nm]
        t = toks_bytes(b)
        while t and (t[-1].startswith("int3") or t[-1].startswith("nop")): t.pop()
        return tuple(t)
import difflib
def sim(a, b):
    if a is None or b is None: return None
    if not a and not b: return 1.0
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()
