#!/usr/bin/env python3
"""gen436a.py <batch> <addr...>: NF436 generator for NScript level-script Alloc rows whose class already has an engine header.
Per row: FableWin.pdb class (base T, public virtual overrides of RegisterMain / Main / Init / OnPersist), the header's own declarations
(tools/hdrscan.py); missing declarations (ctor, static Alloc, overrides) go into an add-only opt-in FABLE_RETAIL_<C>_ALLOC block at the
end of the outer class (public). Writes batches/<b>/rows/<a>.cpp, tests/<a>_test.cpp, hdr/engine/<C>.h, orig/hdr_<C>.h (agent root),
and gen/<b>.tsv (spec for slot0chk / meta). Env EXTRA_DEFS_<addr>="M1 M2" adds row/test macros."""
import sys, os, re
W = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436"
sys.path.insert(0, W + "/tools")
import fwcls, hdrscan
R = "/home/user/FableDecomp/"
b = sys.argv[1]; addrs = sys.argv[2:]; B = W + "/batches/" + b
for d in ("rows", "tests", "hdr/engine"): os.makedirs(B + "/" + d, exist_ok=True)
os.makedirs(W + "/orig", exist_ok=True)
rowsrc = {}; cls = {}
for l in open(W + "/debt_rows.tsv"):
    f = l.rstrip("\n").split("\t"); rowsrc[f[0]] = f[1]
for l in open(W + "/debt73.tsv"):
    f = l.rstrip("\n").split("\t"); cls[f[0]] = f[2]
fwvt = {}; cur = None
for l in open(W + "/work/fwvt73.txt"):
    if l.startswith("##"):
        m = re.match(r"## \?\?_7(\w+)@NScript@@6B@ fw \S+ retail (\S*)", l); cur = m.group(1); fwvt[cur] = {"vt": m.group(2), "slots": {}}
        continue
    k, nm, ra, _ = l.rstrip("\n").split("\t"); fwvt[cur]["slots"][int(k)] = (nm, ra)
NAMES = ["RegisterMain", "Main", "Init", "OnPersist"]; SLOT = {"RegisterMain": 1, "Main": 2, "Init": 3, "OnPersist": 4}
spec = open(W + "/gen/%s.tsv" % b, "w")
def one(a):
    C = cls[a]; A = a.upper(); CU = C.upper()
    old = open(R + rowsrc[a]).read()
    leaf = re.search(r"^// Leaf:\s*(\S+)", old, re.M).group(1)
    size = int(re.search(r"\(0x([0-9a-fA-F]+) = sizeof", old).group(1), 16)
    fw = fwcls.info(C)
    T = re.match(r"CLevelScriptBase<NScript::(\w+)>", fw["bases"][0][0]).group(1)
    ovr = [n for n in NAMES if any(m[0] == n and "virtual" in m[1] for m in fw["methods"])]
    assert all("public" in m[1] for m in fw["methods"] if m[0] in NAMES + ["Alloc", C]), (a, "non-public FableWin method")
    newh = not os.path.exists(R + "rebuild/include/engine/%s.h" % C)
    hs = hdrscan.scan(C, W + "/hdr/engine/%s.h" % C if newh else None)
    if newh:
        import shutil; shutil.copy(W + "/hdr/engine/%s.h" % C, B + "/hdr/engine/%s.h" % C)
    assert hs["T"] == T, (a, "header T", hs["T"], T)
    assert hs["size"] and int(hs["size"], 16) == size, (a, "size", hs["size"], hex(size))
    dec = {k: v for k, v in hs["decl"].items() if k != "other_virtual" and not v[1]}
    for k, v in dec.items(): assert v[0] == "public", (a, k, "declared non-public")
    dwrap = dec.pop("dtor", None)
    assert not dwrap or any(m[0] == "~" + C and "compiler-generated" in m[1] for m in fw["methods"]), (a, "user dtor in FableWin")
    slots = fwvt[C]["slots"]; vt = fwvt[C]["vt"]
    sa = {nm.split("@")[0][1:]: ra for k, (nm, ra) in slots.items() if k}
    miss = []
    if "ctor" not in dec: miss.append("    %s(CScriptDataBase* data, CGameScriptInterfaceBase& game);%s// FableWin.pdb ??0%s@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z" % (C, " ", C))
    if "Alloc" not in dec: miss.append("    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x%s" % A)
    for n in ovr:
        if n not in dec:
            sig = "CPersistContext&" if n == "OnPersist" else ""
            miss.append("    virtual void %s(%s);%s// slot %d 0x%s" % (n, sig, " " * max(1, 40 - len(n) - len(sig)), SLOT[n], sa[n].upper()))
    def wrap(k, line):
        v = hs["decl"].get(k)
        if v and v[1]: return "#ifndef %s\n%s\n#endif" % (v[1][-1], line)
        return line
    keys = (["ctor"] if "ctor" not in dec else []) + (["Alloc"] if "Alloc" not in dec else []) + [n for n in ovr if n not in dec]
    miss = [wrap(k, x) for k, x in zip(keys, miss)]
    macro = "FABLE_RETAIL_%s_ALLOC" % CU
    hl = hs["lines"][:]
    whats = [re.sub(r"\(.*", "", x.split("\n")[-2 if "\n" in x else 0].strip().replace("static CScriptBase* FABLE_FASTCALL ", "").replace("virtual void ", "")) for x in miss]
    if newh: assert not miss, (a, "new header lacks", miss)
    if dwrap and not miss: raise SystemExit("dtor wrap needs an opt-in block")
    if miss:
        blk = ["#ifdef %s" % macro,
               "    // Retail opt-in (NF436, row 0x%s Alloc): FableWin.pdb ??_7%s@NScript@@6B@ / LF_CLASS (all public: the user ctor, static" % (A, C),
               "    //   Alloc, the virtual overrides) for what this header does not declare yet: %s. Declarations only, no layout" % ", ".join(whats),
               "    //   change; existing includers do not define the macro.",
               "public:"] + sum([x.split("\n") for x in miss], []) + ["#endif"]
        tail = []
        for j in range(hs["end"], len(hl)):
            l = hl[j]
            if "__LINE__" in l: raise SystemExit("HOLD %s __LINE__ after insertion" % a)
            l = re.sub(r"^(\s*)FABLE_STATIC_ASSERT\(", lambda m: "%sFABLE_STATIC_ASSERT_AT(%d, " % (m.group(1), j + 1), l)
            tail.append(l)
        hl = hl[:hs["end"]] + blk + tail
        if dwrap:
            i = dwrap[2]; assert "~" + C in hl[i]
            hl[i:i + 1] = ["#ifndef %s   // NF436: FableWin.pdb marks the destructor compiler-generated (the ??_G calls ~CScriptBase directly)" % macro, hl[i], "#endif"]
        open(W + "/orig/hdr_%s.h" % C, "w").write(open(R + "rebuild/include/engine/%s.h" % C).read())
        open(B + "/hdr/engine/%s.h" % C, "w").write("\n".join(hl)); os.makedirs(W + "/hdr/engine", exist_ok=True)
        open(W + "/hdr/engine/%s.h" % C, "w").write("\n".join(hl))
    extra = os.environ.get("EXTRA_DEFS_" + a, "").split()
    defs = ["FABLE_RETAIL_NSCRIPT", "FABLE_RETAIL_CLEVELSCRIPTBASE"] + extra + ([macro] if miss else [])
    dl = "\n".join("#define " + d for d in defs) + "\n#include \"rebuild_abi.h\"" + "".join("\n#include \"%s\"" % x for x in os.environ.get("EXTRA_INC_" + a, "").split())
    # header comment of the old row, minus its Model paragraph
    hdr = []; skip = False
    for l in old.split("\n"):
        if not l.startswith("//"): break
        if l.startswith("// Model:"): skip = True; continue
        if skip and l.startswith("//   "): continue
        skip = False; hdr.append(l)
    ctor = "" if "ctor" in dec and ("inline" in hs["lines"][dec["ctor"][2]] or "{" in hs["lines"][dec["ctor"][2]]) else \
        "inline NScript::%s::%s(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<%s>(data, game)\n{\n}\n" % (C, C, T)
    model = ("// Model (NF436): the shared engine/%s.h%s%s, replacing a local class that declared none of the class's own virtuals\n"
             "//   and an out-of-line destructor (FableWin.pdb: compiler-generated). %s The vftable 0x%s now carries the FableWin.pdb overrides (%s) and\n"
             "//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls\n"
             "//   ~CScriptBase 0x00CBD510 directly." % (C, " (NEW header: Ego_r.pdb layout, FableWin.pdb methods)" if newh else "", (" (+ add-only opt-in %s: %s)" % (macro, ", ".join(whats))) if miss else "",
                                                          "The user constructor is inline in the header." if newh else "The user constructor is defined `inline` here (FableWin.pdb\n//   declares it; Alloc expands it, 0x00D80C00 pattern).", vt.upper(), " ".join(ovr)))
    row = "\n".join(hdr) + "\n" + model + "\n" + dl + "\n#include \"engine/%s.h\"\n" % C + ctor + \
        "CScriptBase* FABLE_FASTCALL NScript::%s::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)\n{\n    return new %s(data, game);\n}\n" % (C, C)
    open(B + "/rows/%s.cpp" % a, "w").write(row)
    calls = "\n".join("    p->%s(%s); ok = ok && g_last == %d;" % (n, "*(CPersistContext*)g_ctx" if n == "OnPersist" else "", SLOT[n]) for n in ovr)
    odefs = "\n".join("void Owner::%s(%s) { g_last = %d; }" % (n, "CPersistContext&" if n == "OnPersist" else "", SLOT[n]) for n in ovr)
    bdefs = "\n".join("void CScriptBase::%s(%s) { g_last = %d; }" % (n, "CPersistContext&" if n == "OnPersist" else "", 10 + SLOT[n]) for n in NAMES)
    test = f"""// Behaviour test for 0x{A} NScript::{C}::Alloc (NF436): links the row object and calls Alloc. The out-of-line callees the row object
// references are defined here and record their calls: CScriptBase's constructor / destructor as __fastcall stand-ins bound by
// /alternatename (the 0x008C01B0 test pattern), operator new / delete, and the virtuals of both vftables the object emits. Checks: one
// 0x{size:X}-byte allocation, CScriptBase() on it, Game +0x40 / MasterScriptData +0x44 stored, the class's own overrides reached through the
// vptr (not CScriptBase's), and `delete` reaching ~CScriptBase on this and freeing it (the implicit ??_G).
{dl}
#include "engine/{C}.h"
#include <stdio.h>
#include <stdlib.h>
typedef NScript::{C} Owner;
static int g_last = 0; static int g_ctor = 0; static void* g_ctorThis = 0; static void* g_dtorThis = 0;
static unsigned g_size = 0; static void* g_new = 0; static void* g_freed = 0; static long g_ctx[4];
{odefs}
{bdefs}
extern "C" void* __fastcall nf436_base_ctor(void* self) {{ ++g_ctor; g_ctorThis = self; return self; }}
extern "C" void* __fastcall nf436_base_dtor(void* self) {{ g_dtorThis = self; return self; }}
#pragma comment(linker, "/alternatename:??0CScriptBase@@QAE@XZ=@nf436_base_ctor@4")
#pragma comment(linker, "/alternatename:??1CScriptBase@@UAE@XZ=@nf436_base_dtor@4")
void* operator new(size_t n) {{ g_size = (unsigned)n; g_new = malloc(n); return g_new; }}
void operator delete(void* p) {{ g_freed = p; free(p); }}
int main(void)
{{
    static long data[4]; static long game[4];
    CScriptBase* b = Owner::Alloc((CScriptDataBase*)data, *(CGameScriptInterfaceBase*)game);
    Owner* p = (Owner*)b;
    int ok = b != 0 && (void*)b == g_new && g_size == 0x{size:X} && g_ctor == 1 && g_ctorThis == (void*)b;
    ok = ok && (void*)p->Game == (void*)game && (void*)p->MasterScriptData == (void*)data;
{calls}
    delete p;
    ok = ok && g_dtorThis == (void*)b && g_freed == (void*)b;
    if (ok) {{ printf("{a}_TEST PASS\\n"); return 0; }}
    printf("{a}_TEST FAIL\\n");
    return 1;
}}
"""
    open(B + "/tests/%s_test.cpp" % a, "w").write(test)
    ovl = "".join({"RegisterMain": "R", "Main": "M", "Init": "I", "OnPersist": "O"}[n] for n in ovr)
    spec.write("\t".join([a, C, T, ovl, ",".join(sa[n] for n in ovr), "%x" % size, vt, leaf]) + "\n")
    print("gen", a, C, T, ovl, "missing:", whats)
hold = open(W + "/gen/%s.hold" % b, "w")
for a in addrs:
    try: one(a)
    except (AssertionError, SystemExit, Exception) as e: print("HOLD", a, cls.get(a), e); hold.write("%s\t%s\t%s\n" % (a, cls.get(a), e))
