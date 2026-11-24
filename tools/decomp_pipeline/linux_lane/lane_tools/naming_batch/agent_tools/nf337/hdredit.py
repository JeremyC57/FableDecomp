#!/usr/bin/env python3
"""hdredit.py (NF337) <plan.tsv>: add-only `FABLE_RETAIL_<CLS>_COPY_DECLS` blocks.
plan.tsv columns: class, copy addr, operator= addr, operator= symbol, declare_copy (1/0), probe defines (comma list).
For each class: work/tree (a full copy of rebuild/include, made once) gets the block inserted just before the class's closing `};`
(the first `};` line after the `class <C>` head); declarations only, under a new macro no landed row defines; moved asserts pinned
(lane_merge/pin_asserts.py). The copy goes to hdr/engine/<C>.h, the original to orig/hdr_<C>.h. Probes (clang, i686 MS mode):
  override  against the ORIGINAL header, `struct P : C { void Copy(const CDefClassBase*) override; }` must compile (Copy is the base's
            slot-19 virtual: the declaration takes no new slot) and the made-up-name control `CopyNF337(...) override` must fail;
  decl      against the new tree with the macro: the class compiles and both declarations are visible (a definition-free use of each).
Writes work/HDR_PLAN.tsv."""
import os, re, sys, shutil, subprocess
R = "/home/user/FableDecomp"; S = os.environ["S"]; A = f"{S}/w22/agentNF337"; W = f"{A}/work"; T = f"{W}/tree"
VCINC = f"{S}/wine/vc71rtm/include"
if not os.path.exists(T): shutil.copytree(f"{R}/rebuild/include", T, symlinks=True)
os.makedirs(f"{A}/hdr/engine", exist_ok=True); os.makedirs(f"{A}/orig", exist_ok=True); os.makedirs(f"{W}/probe", exist_ok=True)
def clang(src, inc):
    p = f"{W}/probe/p.cpp"; open(p, "w").write(src)
    r = subprocess.run(["timeout", "60", "clang++", "-target", "i686-pc-windows-msvc", "-fms-extensions", "-fms-compatibility",
                        "-fms-compatibility-version=13.10", "-Xclang", "-fno-wchar", "-fsyntax-only", "-w", f"-I{inc}", "-isystem", VCINC, p],
                       capture_output=True, text=True, errors="replace")
    return r.returncode == 0, [l for l in r.stderr.splitlines() if "error" in l][:2]
out = []
for l in open(sys.argv[1]).read().splitlines():
    if not l.strip() or l.startswith("#"): continue
    C, ca, oa, opsym, dc, defs = l.split("\t")
    defs = [d for d in defs.split(",") if d]
    M = f"FABLE_RETAIL_{C.upper()}_COPY_DECLS"
    h = f"engine/{C}.h"; orig = open(f"{R}/rebuild/include/{h}").read()
    if M in orig: out.append([C, "SKIP", M, "macro already in the header"]); continue
    L = orig.split("\n")
    ci = next(i for i, x in enumerate(L) if re.match(r'^\s*class\s+(__declspec\(\w+\)\s+)?%s\b' % C, x) and not x.rstrip().endswith(";"))
    ei = next(i for i in range(ci, len(L)) if L[i].strip() == "};")
    ret = "const " if "ABV0@ABV0" in opsym else ""
    if opsym == "-":    # body-form Copy (retail calls CDefClassBase::operator= 0x00431F10 itself): the Copy declaration only
        blk = [f"#ifdef {M}",
           f"    // Opt-in (NF337, 2026-11-24): declaration only, no layout change, for {C}::Copy 0x{ca.upper()} (retail: `call 0x00431F10`",
           f"    // CDefClassBase::operator=, then the member copy). ?Copy@{C}@@UAEXPBVCDefClassBase@@@Z (Ego_r.pdb + FableWin.pdb) is the member the",
           f"    // retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names at 0x{ca.upper()} (slot 19; it overrides CDefClassBase::Copy:",
           f"    // clang override probe, made-up-name control fails).",
           "public:"]
    else:
      blk = [f"#ifdef {M}",
           f"    // Opt-in (NF337, 2026-11-24): declarations only, no layout change, for {C}::Copy 0x{ca.upper()}, which tail-calls the out-of-line",
           f"    // operator= (retail `jmp` 0x{oa.upper()}). ?Copy@{C}@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable placement",
           f"    // (ghidra_out/labels_rtti_port.tsv) names at 0x{ca.upper()} (slot 19; it overrides CDefClassBase::Copy: clang override probe, made-up-name",
           f"    // control fails); {opsym} (Ego_r.pdb + FableWin.pdb) is the operator= at 0x{oa.upper()}.",
           "public:"]
    if dc == "1": blk.append("    virtual void Copy(const CDefClassBase*);")
    blk += ([f"    {ret}{C}& operator=(const {C}&);"] if opsym != "-" else []) + ["#endif"]
    N = L[:ei] + blk + L[ei:]
    open(f"{T}/{h}", "w").write("\n".join(N))
    r = subprocess.run(["python3", f"{R}/tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py", f"{R}/rebuild/include", T, h], capture_output=True, text=True)
    pins = re.search(r'pinned (\d+)', r.stdout); warn = "!!" in r.stdout
    shutil.copy(f"{T}/{h}", f"{A}/hdr/engine/{C}.h"); open(f"{A}/orig/hdr_{C}.h", "w").write(orig)
    pre = "".join(f"#define {d}\n" for d in defs)
    ok1, e1 = clang(pre + f'#include "{h}"\nstruct NF337Probe : {C} {{ void Copy(const CDefClassBase*) override; }};\n', f"{R}/rebuild/include")
    ok2, e2 = clang(pre + f'#include "{h}"\nstruct NF337Probe : {C} {{ void CopyNF337(const CDefClassBase*) override; }};\n', f"{R}/rebuild/include")
    use = (f"void NF337Use({C}& a, const {C}& b, const CDefClassBase* p) {{ a = b; a.{C}::Copy(p); }}\n" if opsym != "-" else f"void NF337Use({C}& a, const CDefClassBase* p) {{ a.{C}::Copy(p); }}\n")
    ok3, e3 = clang(pre + f"#define {M}\n" + f'#include "{h}"\n' + use, T)
    ok4, e4 = clang(pre + f'#include "{h}"\n' + use, T)   # control: without the macro the declarations are absent (Copy undeclared or operator= implicit)
    v = "OK" if ok1 and not ok2 and ok3 and not warn else "FAIL"
    out.append([C, v, M, f"insert at line {ei + 1}; pinned {pins.group(1) if pins else '?'}{' WARN' if warn else ''}; override {ok1} {' '.join(e1)[:120]}; "
                          f"control(made-up name) {'compiles!' if ok2 else 'fails'}; decl-use {ok3} {' '.join(e3)[:120]}; without macro {'compiles' if ok4 else 'fails'}"])
    print("\t".join(out[-1]), flush=True)
open(f"{W}/HDR_PLAN.tsv", "a").write("class\tverdict\tmacro\tnotes\n" + "".join("\t".join(r) + "\n" for r in out))
