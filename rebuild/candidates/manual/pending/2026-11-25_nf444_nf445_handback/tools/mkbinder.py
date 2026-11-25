#!/usr/bin/env python3
"""mkbinder.py: NF444 batch-A binder rows. From the HEAD copies in orig/, replace the local base model CVtable_012beda4 with the
shared engine/CSalRender.h (constructor rows: the FABLE_RETAIL_CSALRENDER_MEMBERS layout; destructor rows: the default path)."""
import re,glob,os
HDR_ON='''#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4, 0x28 bytes; constructor 0x00C6E6A0, destructor 0x00C6E630)
'''
HDR_OFF='''#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4; destructor 0x00C6E630)
'''
NOTE='''// NF444 binder: the base is the shared engine/CSalRender.h (GameCODA SDK class; was the local model CVtable_012beda4). This row keeps
//   its own symbol; only the base %s it calls is respelled (??%sCVtable_012beda4@@%s -> ??%sCSalRender@@%s, unit 0x00C6E630).
'''
for f in sorted(glob.glob('orig/CVtable012be*_00c*.cpp')):
    if f.endswith('_test.cpp'): continue
    a=f[-12:-4]; lines=open(f).read().split('\n')
    k=0
    while k<len(lines) and lines[k].startswith('//'): k+=1
    leaf=[l for l in lines[:k] if l.startswith('// Leaf:')][0]
    isdtor='??1' in leaf
    body='\n'.join(lines[k:])
    body=re.sub(r'(struct|class) CVtable_012beda4\b[^{;]*\{.*?\n\};\n','',body,count=1,flags=re.S)
    body=re.sub(r'#include "engine/CCodaObjectBase.h"[^\n]*\n','',body)
    if isdtor:
        body=body.replace('public CVtable_012beda4','public CSalRender'); inc=HDR_OFF; note=NOTE%('destructor','1','UAE@XZ','1','UAE@XZ')
    else:
        if 'CVtable_012beda4, SAlignedScratch<64>' in body:
            body=body.replace('CVtable_012beda4, SAlignedScratch<64>','CSalRender, SHrtfHeader, SAlignedScratch<64>')
            body=body.replace('template <int N> struct SAlignedFloats','struct SHrtfHeader {                               // +0x28: 0x80 bytes the constructor does not touch\n    unsigned char m_bytes[0x80];\n};\ntemplate <int N> struct SAlignedFloats',1)
        else:
            body=re.sub(r': CVtable_012beda4 \{',': CSalRender {',body)
        body=re.sub(r'\n    virtual void v00\(\);','',body); inc=HDR_ON; note=NOTE%('constructor','0','QAE@XZ','0','QAE@XZ')
    assert 'CVtable_012beda4' not in body,(a,body[:300])
    open(f'rows/{a}_b1.cpp','w').write('\n'.join(lines[:k])+'\n'+note+inc+body.lstrip('\n'))
    print(a,'dtor' if isdtor else 'ctor')
