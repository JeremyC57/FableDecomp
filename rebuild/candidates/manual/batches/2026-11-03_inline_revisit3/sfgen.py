#!/usr/bin/env python3
"""sfgen.py addr : CSpawnedFunc1 family variants (writes rows/<a>/sf_<k>.cpp and prints 'k overlay' lines)"""
import sys,re
a=sys.argv[1]; R=f'rows/{a}'; t=open(R+'/plain.cpp').read()
cc=re.search(r'inline CScriptThing::CScriptThing\(const CScriptThing& other\)[^\n]*\n',t).group(0)
fn=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*\n\{',t,re.M))[-1].start()
V={}
V['plain']=(t,'')
V['sfplain']=(t,'sfplain')
V['sfplain_cp']=(re.sub(r'__forceinline (CCountedPointer\(const CCountedPointer)',r'inline \1',t),'sfplain')
V['implcopy']=(t.replace(cc,'').replace('#define FABLE_RETAIL_NSCRIPT\n','#define FABLE_RETAIL_NSCRIPT\n#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY\n',1),'')
V['ccafter']=(t.replace(cc,'')+'\n'+cc,'')
V['ccinclass']=(t.replace(cc,''),'icc')
V['sfconst']=(t,'sfconst')
V['sfout']=(t,'sfout')
m=re.search(r'ParentClass->SpawnFunction\((new CSpawnedFunc1<[^;]*)\);',t)
if m:
    V['localptr']=(t[:m.start()]+'CSpawnedFuncBase* func = '+m.group(1)+';\n    ParentClass->SpawnFunction(func);'+t[m.end():],'')
V['ob1']=(t.replace('// Leaf:','// Flags: /Ob1\n// Leaf:',1),'')
V['ccforce_none']=(t.replace(cc,cc.replace('inline ','')),'')
for k,(s,ov) in V.items():
    open(f'{R}/sf_{k}.cpp','w').write(s); print(k,ov)
