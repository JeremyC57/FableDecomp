#!/usr/bin/env python3
"""gen436n.py <addr...>: NEW engine/<C>.h master headers (hdr/engine) for level-script classes with no header (gen434 shape + own members):
layout Ego_r.pdb (struct_layouts_egor.tsv, offset - 0x14 = retail), base CLevelScriptBase<T> with T from FableWin.pdb's LF_BCLASS, inline
user ctor, static Alloc, FableWin.pdb overrides (public virtual), compiler-generated destructor, nested enums from FableWin.pdb LF_ENUM,
size assert. Then run gen436a.py on the rows."""
import sys, os, re
W = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436"
sys.path.insert(0, W + "/tools"); import fwcls
R = "/home/user/FableDecomp/"
cls = {l.split("\t")[0]: l.split("\t")[2] for l in open(W + "/debt73.tsv")}
rowsrc = {l.split("\t")[0]: l.rstrip("\n").split("\t")[1] for l in open(W + "/debt_rows.tsv")}
fwvt = {}; cur = None
for l in open(W + "/work/fwvt73.txt"):
    if l.startswith("##"):
        m = re.match(r"## \?\?_7(\w+)@NScript@@6B@ fw \S+ retail (\S*)", l); cur = m.group(1); fwvt[cur] = {"vt": m.group(2), "slots": {}}; continue
    k, nm, ra, _ = l.rstrip("\n").split("\t"); fwvt[cur]["slots"][int(k)] = (nm, ra)
eg = {}
for l in open(R + "ghidra_out/struct_layouts_egor.tsv"):
    f = l.rstrip("\n").split("\t")
    if len(f) == 5 and f[4] != "_padding_": eg.setdefault(f[0], []).append((int(f[2]), f[3], f[4], int(f[1])))
types = open("/tmp/claude-0/egod_pdb/FableWin.types.txt", encoding="utf-8", errors="replace").read()
def enum(C, E):
    m = re.search(r"LF_ENUM \[size = \d+\] `NScript::%s::%s`\n.*\n\s+field list: (0x[0-9A-F]+)" % (C, E), types)
    fl = m.group(1)
    m2 = re.search(r"\n\s+%s \| LF_FIELDLIST [^\n]*\n((?:\s+- LF_ENUMERATE[^\n]*\n)+)" % fl, types)
    return re.findall(r"\[(\w+) = (\d+)\]", m2.group(1))
NAMES = ["RegisterMain", "Main", "Init", "OnPersist"]; SLOT = {"RegisterMain": 1, "Main": 2, "Init": 3, "OnPersist": 4}
SIMPLE = {"bool": "bool", "long": "long", "float": "float", "unsigned long": "unsigned long", "int": "long"}
for a in sys.argv[1:]:
    C = cls[a]; A = a.upper(); fw = fwcls.info(C)
    old = open(R + rowsrc[a]).read(); size = int(re.search(r"\(0x([0-9a-fA-F]+) = sizeof", old).group(1), 16)
    T = re.match(r"CLevelScriptBase<NScript::(\w+)>", fw["bases"][0][0]).group(1)
    ovr = [n for n in NAMES if any(m[0] == n and "virtual" in m[1] for m in fw["methods"])]
    vt = fwvt[C]["vt"]; sa = {nm.split("@")[0][1:]: ra for k, (nm, ra) in fwvt[C]["slots"].items() if k}
    mem = sorted(eg[C]); psize = mem[0][3]
    assert psize - 0x14 == size, (a, psize, size)
    enums = []; md = []
    fwm = {m[0]: m for m in fw["members"]}
    for off, ty, nm, _ in mem:
        assert nm in fwm, (a, nm)
        if ty in SIMPLE: t = SIMPLE[ty]
        else:
            vals = enum(C, ty); t = ty
            enums.append("    enum %s {\n%s\n    };" % (ty, ",\n".join("        %s = %s" % v for v in vals)))
        md.append("    %s %s;%s// +0x%x (Ego_r.pdb +0x%x; FableWin.pdb %s)" % (t, nm, " " * max(1, 44 - len(t) - len(nm)), off - 0x14, off, fwm[nm][1]))
    decl = []
    for n in ovr:
        sig = "CPersistContext&" if n == "OnPersist" else ""
        decl.append("    virtual void %s(%s);%s// slot %d  0x%s" % (n, sig, " " * max(1, 40 - len(n) - len(sig)), SLOT[n], sa[n].upper()))
    G = "FABLE_ENGINE_%s_H" % C.upper()
    hdr = f"""#ifndef {G}
#define {G}
// NScript::{C} (NF436, new header; no includer before NF436). Base CLevelScriptBase<NScript::{T}> (FableWin.pdb LF_BCLASS) with the
// class's own members.
// Layout: Ego_r.pdb {C} (struct_layouts_egor.tsv, {psize} bytes) at retail offsets = PDB - 0x14 (retail CScriptBase is 0x40; no STLport
// container in the class); retail size 0x{size:X} (the landed Alloc 0x{A} news 0x{size:X}). Member names / types: Ego_r.pdb and FableWin.pdb.
// Methods (FableWin.pdb LF_CLASS field list, all public; Ego_r.pdb has no out-of-line ctor copy, so it is inline; the destructor is
// compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the virtual overrides below
// (FableWin.pdb ??_7{C}@NScript@@6B@ slot order; retail vftable 0x{vt.upper()}). The class's other methods are not declared yet.
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
namespace NScript {{
class {T};
class {C} : public CLevelScriptBase<{T}> {{
public:
{chr(10).join(enums + [""]) if enums else ""}    {C}(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<{T}>(data, game) {{ }}
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x{A}
{chr(10).join(decl)}
{chr(10).join(md)}
}};
}}
FABLE_STATIC_ASSERT(sizeof(NScript::{C}) == 0x{size:X});

#endif // FABLE_RETAIL_NSCRIPT
#endif // {G}
"""
    os.makedirs(W + "/hdr/engine", exist_ok=True); open(W + "/hdr/engine/%s.h" % C, "w").write(hdr); print("hdr", a, C, T, "".join(n[0] for n in ovr), len(md), "members")
