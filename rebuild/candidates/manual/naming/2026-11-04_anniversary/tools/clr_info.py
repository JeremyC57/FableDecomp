import sys; sys.path.append("/root/.local/lib/python3.13/site-packages")
import dnfile, collections
pe = dnfile.dnPE(sys.argv[1])
md = pe.net.mdtables
print("CLR flags", hex(pe.net.struct.Flags), "runtime", pe.net.struct.MajorRuntimeVersion, pe.net.struct.MinorRuntimeVersion)
print("metadata version", pe.net.metadata.struct.Version)
if md.Assembly:
    for a in md.Assembly: print("Assembly", a.Name, a.MajorVersion, a.MinorVersion, a.BuildNumber, a.RevisionNumber)
if md.Module:
    for m in md.Module: print("Module", m.Name)
if md.AssemblyRef:
    for r in md.AssemblyRef: print("AsmRef", r.Name, r.MajorVersion, r.MinorVersion)
ns = collections.Counter()
types = []
if md.TypeDef:
    for t in md.TypeDef:
        n_=str(t.TypeNamespace or ""); ns[n_] += 1; types.append((n_, str(t.TypeName)))
print("TypeDefs", len(types)); print(ns.most_common(30))
if md.MethodDef: print("MethodDefs", len(md.MethodDef))
with open(sys.argv[2], "w") as f:
    for n, t in types: f.write("%s\t%s\n" % (n, t))
