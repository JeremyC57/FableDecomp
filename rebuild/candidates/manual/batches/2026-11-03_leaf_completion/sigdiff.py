import csv, subprocess, sys, collections, re
L=sys.argv[1]
plan={r["address"]:r for r in csv.DictReader(open(f"{L}/plan1.tsv"),delimiter="\t")}
rep={r["address"]:r for r in csv.DictReader(open(f"{L}/report1.tsv"),delimiter="\t")}
g=[r for r in csv.DictReader(open(f"{L}/grades_all.tsv"),delimiter="\t") if r["result"]=="FAIL"]
syms=set()
for r in g:
    syms.add(plan[r["address"]]["completed"]); syms.update(r["object_symbols_matching_old_prefix"].split())
out=subprocess.run(["llvm-undname"],input="\n".join(sorted(syms))+"\n",capture_output=True,text=True).stdout
dem={}
for b in out.split("\n\n"):
    ls=[x for x in b.split("\n") if x.strip()]
    if len(ls)>=2: dem[ls[0]]=ls[1]
def kind(pdb,obj):
    if not obj: return "no object symbol carries the prefix"
    o=obj[0]
    if "CRetail" in o or "CUnproven" in o: return "reconstruction uses a stand-in container type (CRetail*/CUnproven*)"
    if "_Vector_iterator" in pdb or "_List_iterator" in pdb or "_Tree" in pdb or "_Deque_iterator" in pdb or "_List_const_iterator" in pdb or "_Vector_const_iterator" in pdb: return "PDB symbol is Dinkumware STL (retail is STLport)"
    dp,do=dem.get(pdb,""),dem.get(o,"")
    if not dp or not do: return "undemangled"
    nk=lambda x: re.sub(r"\b(class|struct|union|enum) ","",x)
    nc=lambda x: re.sub(r"__(thiscall|cdecl|stdcall|fastcall) ","",x)
    na=lambda x: re.sub(r"^(public|protected|private): ","",x)
    nv=lambda x: re.sub(r"\b(virtual|static) ","",x)
    if dp==do: return "same demangling (mangling-only difference)"
    if nk(dp)==nk(do): return "class key (class/struct) differs"
    if nc(dp)==nc(do): return "calling convention differs"
    if na(dp)==na(do): return "access differs"
    if nv(na(dp))==nv(na(do)): return "virtual/static differs"
    hp,ho=nk(nc(nv(na(dp)))),nk(nc(nv(na(do))))
    if hp.split("(")[0].split()[-1]!=ho.split("(")[0].split()[-1]: return "qualified name differs"
    if hp.split("(",1)[1:]==ho.split("(",1)[1:]: return "return type differs"
    return "parameter types / const differ"
c=collections.Counter()
with open(f"{L}/SIGNATURE_DIFFS.tsv","w") as f:
    w=csv.writer(f,delimiter="\t",lineterminator="\n")
    w.writerow(["address","catalog_name","kind","pdb_symbol","object_symbol","pdb_demangled","object_demangled","recheck_grade_with_pdb_leaf"])
    for r in g:
        a=r["address"]; pdb=plan[a]["completed"]; obj=r["object_symbols_matching_old_prefix"].split()
        k=kind(pdb,obj); c[k]+=1
        w.writerow([a,rep[a]["catalog_name"],k,pdb," ".join(obj),dem.get(pdb,"")," | ".join(dem.get(o,"") for o in obj),r["grade"]])
for k,v in c.most_common(): print(v,k)
