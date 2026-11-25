#!/usr/bin/env python3
"""cfilt438.py: narrow work/census.tsv to rows whose text holds EVERY identifier of the demangled wrong spelling (scope path,
name, parameter / return type names; C++ keywords and std/allocator template plumbing excluded) -> work/census_f.tsv."""
import sys, os, re, subprocess, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
L=[l.rstrip('\n').split('\t') for l in open('work/census.tsv')]
syms=sorted({x[2] for x in L})
out=subprocess.run(['llvm-undname'],input='\n'.join(syms),capture_output=True,text=True).stdout
dem={}
for b in out.strip().split('\n\n'):
    l=b.split('\n')
    if len(l)>=2: dem[l[0]]=l[1]
KW=set('public private protected virtual static void __thiscall __fastcall __cdecl __stdcall class struct union enum const unsigned int long char short bool float double signed operator std allocator less pair char_traits basic_string _Container_base wchar_t __int64 __ptr64 volatile'.split())
txt={}
def text(a):
    if a not in txt:
        p=nf.src_of(a); txt[a]=set(re.findall(r'\w+',open(p,encoding='utf-8',errors='replace').read())) if p else set()
    return txt[a]
keep=[]
for x in L:
    d=dem.get(x[2])
    ids={w for w in re.findall(r'[A-Za-z_]\w*',d) if w not in KW} if d else {x[4]}
    if ids <= text(x[0]): keep.append(x)
open('work/census_f.tsv','w').write(''.join('\t'.join(x)+'\n' for x in keep))
print('lines',len(keep),'rows',len({x[0] for x in keep}),'targets',len({x[1] for x in keep}))
print(collections.Counter(x[4] for x in keep).most_common(10))
