# aproxy.py <addr> <suffix> [base]: turn the vector's plain end-of-storage pointer member into an STLport
# _STLP_alloc_proxy-style member object with a constructor (init-list NAME(0) kept; other uses become NAME.m_data)
import re,sys,os
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]
src=open(sys.argv[3] if len(sys.argv)>3 else f"{W}/src/{a}.cpp",errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
m=re.search(r"\b(_M_end_of_storage|m_end|end_of_storage|_M_end|m_pEnd|m_endOfStorage|EndOfStorage)\s*\(0\)",src)
if not m: print("NONE"); sys.exit(1)
name=m.group(1)
d=re.search(r"^([ \t]*)([\w:<>]+(?:\s*<[^;>]*>)?)\s*\*\s*"+name+r"\s*;([^\n]*)$",src,re.M)
if not d: print("NODECL"); sys.exit(1)
ind,typ,cmt=d.group(1),d.group(2).strip(),d.group(3)
decl=f"""{ind}struct CEndOfStorage_s7 {{                     // STLport _STLP_alloc_proxy<{typ}*, ...>: a member object with a constructor
{ind}    {typ}* m_data;
{ind}    CEndOfStorage_s7({typ}* data) : m_data(data) {{ }}
{ind}}} {name};{cmt}"""
s=src[:d.start()]+decl+src[d.end():]
# rename other uses (not the declaration, not NAME(0) init, not inside the inserted struct)
start=d.start(); end=start+len(decl)
def fix(t):
    t=re.sub(r"\b"+name+r"\b(?!\s*\(0\))(?!\.m_data)",name+".m_data",t)
    return t
s=fix(s[:start])+s[start:end]+fix(s[end:])
open(f"{W}/var/{a}_{suf}.cpp","w").write(s); print(f"var/{a}_{suf}.cpp {name} {typ}")
