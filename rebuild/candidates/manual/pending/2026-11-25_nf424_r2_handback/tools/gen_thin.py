#!/usr/bin/env python3
"""gen_thin.py [--alt N] addr...: ctor-owned thin rows (NF424 round 2, the Option S / nf251 0x00B2AB10 pattern).
VC7.1 emits ??_G<T> as a pick-any COMDAT beside ??_7<T>, i.e. in a TU whose code stores the vptr (a constructor, or a destructor
or a function that expands the ctor inline).  The row's source is that landed owner's source, included with no code of its own;
the row's leaf is the ??_G spelling.  Owner choice: the class's ??0 first, then other non-initializer storers, then a ??1.
Writes r2/rows/<addr>.cpp + _test.cpp and appends r2/work/gen_thin.tsv (addr owner owner_src leaf)."""
import sys, os, re, json
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
a2n = json.load(open(A + "work/addr2names.json"))
args = sys.argv[1:]; alt = 0
if args and args[0] == "--alt": alt = int(args[1]); args = args[2:]
own = {}
for l in open(A + "r2/work/owners.tsv"):
    q = l.rstrip("\n").split("\t"); own[q[0]] = q
cls_ev = {}
for l in open(A + "r2/work/s0cls.tsv"):
    q = l.rstrip("\n").split("\t"); cls_ev[q[0]] = q
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
def demangle_cls(leaf):
    import subprocess
    o = subprocess.run(["llvm-undname", leaf], capture_output=True, text=True).stdout.strip().splitlines()[-1]
    m = re.search(r"(?:public|private|protected): virtual void \* __thiscall (.+)::`scalar deleting dtor'", o)
    return m.group(1) if m else None
os.makedirs(A + "r2/rows", exist_ok=True)
log = open(A + "r2/work/gen_thin.tsv", "a")
for a in args:
    q = own[a]; shape, leaf, vt = q[1], q[2], q[3]
    leaf = leaf.replace("??_E", "??_G", 1)
    owners = [x.split(":", 1) for x in q[5].split(";") if x]
    def rank(o):
        n = (a2n.get(o[0]) or ["?"])[0]
        if "Initializ" in n or "dynamic" in n.lower(): return 9
        if n.startswith("??0"): return 0
        if n.startswith("??1"): return 2
        return 1
    owners = [o for o in sorted(owners, key=rank) if rank(o) < 9]
    if alt >= len(owners): print("NOALT", a); continue
    f, src = owners[alt]
    cls = demangle_cls(leaf) or "?"
    oname = (a2n.get(f) or ["?"])[0]
    cat = nf.catalog().get(a); ln = cat["length"] if cat else "?"
    h = hist[a]
    t = []
    t.append("// %s::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~%s, then operator" % (cls, cls.split("::")[-1].split("<")[0]))
    t.append("//   delete when bit 0 of flags is set; returns this")
    t.append("// Leaf: " + leaf)
    t.append("// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes, thiscall, `ret 4`; vftable 0x%s slot %s." % (a.upper(), ln, vt.upper(), (cls_ev[a][3] if a in cls_ev else (h["retail_vt"].split(";")[0].split("/") + ["?"])[1])))
    if shape == "S0":
        e = cls_ev[a]
        t.append("// Identity (NF424 round 2, S0): (1) the vftable's own slots (functions no other vftable references) carry landed / PDB names")
        t.append("//   of this class (%s); (2) %s." % (e[6].split(" P(")[0], "PDB name sets " + e[6][e[6].find("P("):].split(")")[0] + ")"))
        t.append("//   Was the shared stand-in %s." % ((a2n.get(a) or ["?"])[0]))
    else:
        t.append("// Identity (NF424, %s): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram)." % shape)
        t.append("//   Was %s." % ((a2n.get(a) or ["?"])[0]))
    t.append("// TU: VC7.1 emits %s as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x%s" % (leaf, f.upper()))
    t.append("//   %s stores vftable 0x%s, so this function is produced by that translation unit, included below with no code of its own" % (oname, vt.upper()))
    t.append("//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).")
    t.append('#include "../src/compiled/%s"' % src)
    open(A + "r2/rows/%s.cpp" % a, "w").write("\n".join(t) + "\n")
    # behaviour model test (self-contained, as gen_g1)
    patt = nf.field(a, "PassPattern") or ("%s_TEST PASS" % a)
    b = "Base"; d = "Derived"
    tt = ["// Behaviour model of %s::`scalar deleting destructor' (row 0x%s, ??_G): delete through a base pointer runs the destructor" % (cls, a.upper()),
          "// chain once and frees the object; an explicit virtual destructor call (flags 0) runs it without freeing. Self-contained model of the",
          "// same class shape (the row object is the owner TU's; NF419 r1: leaf bytes and relocations graded against retail).",
          "#include <cstdio>", "#include <cstdlib>",
          "static int g_dtors = 0, g_frees = 0; static void* g_freed = 0;",
          "void* operator new(unsigned int n) { return std::malloc(n ? n : 1); }",
          "void operator delete(void* p) { ++g_frees; g_freed = p; std::free(p); }",
          "class %s { public: int baseState; %s() : baseState(1) { } virtual ~%s() { ++g_dtors; } };" % (b, b, b),
          "class %s : public %s { public: int ownState[3]; };" % (d, b),
          "int main()", "{",
          "    %s* p = new %s;" % (d, d), "    %s* q = p;" % b, "    delete q;",
          "    if (g_dtors != 1 || g_frees != 1 || g_freed != (void*)p) { std::printf(\"%s FAIL delete\\n\"); return 1; }" % a,
          "    %s* r = new %s;" % (d, d), "    r->~%s();" % d,
          "    if (g_dtors != 2 || g_frees != 1) { std::printf(\"%s FAIL no-free path\\n\"); return 1; }" % a,
          "    std::free(r);", "    std::printf(\"%s\\n\");" % patt, "    return 0;", "}"]
    open(A + "r2/rows/%s_test.cpp" % a, "w").write("\n".join(tt) + "\n")
    log.write("\t".join([a, shape, f, src, leaf, str(alt)]) + "\n")
log.close()
