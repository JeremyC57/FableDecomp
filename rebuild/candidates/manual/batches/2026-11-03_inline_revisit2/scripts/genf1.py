import sys,re
P,X=sys.argv[1:]; import os; os.makedirs(X,exist_ok=True); t=open(P).read()
CC=re.search(r"inline CScriptThing::CScriptThing\(const CScriptThing& other\)[^\n]*\n",t).group(0)
INC='#include "engine/CSpawnedFunc1.h"'
CP=re.search(r"__forceinline CCountedPointer\(const CCountedPointer& other\)",t).group(0)
s=t.replace(INC,'#define FABLE_SPAWNEDFUNC_INLINE inline\n'+INC,1)
V={'v1_cc_outofline':t.replace(CC,''),'v3_cc_after':t.replace(CC,'')+'\n'+CC,
   'v8_cp_plain':t.replace(CP,'inline CCountedPointer(const CCountedPointer& other)'),
   'v9_cc_force':t.replace(CC,'__forceinline'+CC[6:]),
   'v10_cc_force_cp_plain':t.replace(CC,'__forceinline'+CC[6:]).replace(CP,'inline CCountedPointer(const CCountedPointer& other)'),
   'v11_cc_auto':t.replace(CC,CC[7:]),
   'sf_s1_inline':s,'sf_s2_inline_ccforce':s.replace(CC,'__forceinline'+CC[6:]),'sf_s3_inline_ccauto':s.replace(CC,CC[7:]),
   'sf_s4_inline_ccout':s.replace(CC,''),'sf_s5_inline_cpplain':s.replace(CP,'inline CCountedPointer(const CCountedPointer& other)')}
for k,v in V.items(): open(f"{X}/{k}.cpp",'w').write(v)
