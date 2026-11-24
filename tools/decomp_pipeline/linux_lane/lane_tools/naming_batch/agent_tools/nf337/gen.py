#!/usr/bin/env python3
"""gen.py (NF337): write the moved Copy row + its moved test for the 5-byte `jmp` Copy thunks.
usage: gen.py <out dir> <addr> <class> <opaddr> local
       gen.py <out dir> <addr> <class> <opaddr> header <define,define,...> <include>
The row defines the PDB member ?Copy@<C>@@UAEXPBVCDefClassBase@@@Z as `*this = *(const C*)other;` with C::operator= declared out of line
(retail: a thiscall tail call, `jmp` to the class's operator= emitted right after it). local = a two-declaration local model (classes with no
engine/<C>.h); header = the shared header under the listed switches. The test calls the member qualified through a pointer cast from static
storage (NF335 form) and stubs the callee operator= (the 0x004644FA precedent: the callee stub stays a member of the declaration model)."""
import os, re, sys
R = "/home/user/FableDecomp"
out, a, C, op, route = sys.argv[1:6]
defs = sys.argv[6].split(",") if route == "header" and sys.argv[6] != "-" else []
inc = sys.argv[7] if route == "header" else None
cat = open(R + "/rebuild/build_candidates.ps1", errors="replace").read()
def block(addr):
    m = re.search(r"\[pscustomobject\]@\{([^{}]*?Address = '%s'[^{}]*?)\n\s*\}" % addr, cat, re.S)
    return dict(re.findall(r"(\w+) = '([^']*)'", m.group(1)))
b = block(a)
pp = b["PassPattern"]
test = open(R + "/rebuild/tests/" + b["TestSource"], errors="replace").read()
# the landed test's exact success line (the printf that carries the PassPattern)
m = re.search(r'printf\("([^"]*%s[^"]*)"' % re.escape(pp), test)
okline = m.group(1) if m else pp + "\\n"
pdb = "?Copy@%s@@UAEXPBVCDefClassBase@@@Z" % C
opsym = "??4%s@@QAEAAV0@ABV0@@Z" % C
A = "0x%08X" % int(a, 16); OP = "0x%08X" % int(op, 16)
src = []
src.append("// Leaf: %s" % pdb)
src.append("// %s::Copy(const CDefClassBase* other) -- copy every member of another %s: `*this = *(const %s*)other;`" % (C, C, C))
src.append("// Retail: Fable.exe (TLC, VC7.1) @ %s, 5 bytes: a thiscall tail call, `jmp` to %s::operator= %s (emitted right after it)." % (A, C, OP))
src.append("// Identity: the RTTI vftable placement of %s (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets." % A)
src.append("// Callee: %s::operator=(const %s&) %s, %s (PDB name set + the landed operator= row's own definition at the jmp target)." % (C, C, OP, opsym))
src.append("// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is")
src.append("//   declared out of line here, so the body is the one-instruction tail call retail has.")
if route == "header":
    for d in defs: src.append("#define %s" % d)
src.append('#include "rebuild_abi.h"')
if route == "local":
    src.append("")
    src.append("class CDefClassBase;")
    src.append("class %s {                                // declarations only (no engine/%s.h)" % (C, C))
    src.append("public:")
    src.append("    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, %s" % A)
    src.append("    %s& operator=(const %s& other);   // out of line: %s" % (C, C, OP))
    src.append("};")
else:
    src.append('#include "%s"' % inc)
src.append("")
src.append("void %s::Copy(const CDefClassBase* other)" % C)
src.append("{")
src.append("    *this = *(const %s*)other;" % C)
src.append("}")
tst = []
tst.append("// behaviour test for %s: %s::Copy forwards to %s::operator= with this and the source object" % (A, C, C))
tst.append("// NF337 (NF324 Q2 form): the row now defines the PDB member %s::Copy (%s)." % (C, pdb))
tst.append("// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never")
tst.append("// constructed (no vftable, no RTTI). The callee operator= (%s, out of line in the row) is stubbed as a member here." % opsym)
tst.append("#include <cstdio>")
tst.append("")
tst.append("class CDefClassBase;")
tst.append("class %s { public:" % C)
tst.append("    virtual void Copy(const CDefClassBase* other);")
tst.append("    %s& operator=(const %s& other);" % (C, C))
tst.append("};")
tst.append("")
tst.append("static int g_calls = 0;")
tst.append("static const void* g_this = 0;")
tst.append("static const void* g_other = 0;")
tst.append("%s& %s::operator=(const %s& other)" % (C, C, C))
tst.append("{")
tst.append("    ++g_calls; g_this = this; g_other = &other;")
tst.append("    return *this;")
tst.append("}")
tst.append("")
tst.append("static unsigned long g_dst[4];")
tst.append("static unsigned long g_src[4];")
tst.append("")
tst.append("int main()")
tst.append("{")
tst.append("    ((%s*)(g_dst))->%s::Copy((const CDefClassBase*)(g_src));" % (C, C))
tst.append("    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {")
tst.append('        std::printf("%s");' % okline)
tst.append("        return 0;")
tst.append("    }")
tst.append('    std::printf("FAIL_%s calls=%%d\\n", g_calls);' % a)
tst.append("    return 1;")
tst.append("}")
os.makedirs(out, exist_ok=True)
open(os.path.join(out, a + ".cpp"), "w").write("\n".join(src) + "\n")
open(os.path.join(out, a + "_test.cpp"), "w").write("\n".join(tst) + "\n")
print("GEN", a, C, route, pp, repr(okline))
