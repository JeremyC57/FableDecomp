import sys,json,re,importlib.util
spec=importlib.util.spec_from_file_location('h',sys.argv[1]+'/tools/hunks.py')
W=sys.argv[1]+'/w/'; OUT=sys.argv[1]+'/res/'
import os; os.makedirs(OUT,exist_ok=True)
src=open(sys.argv[1]+'/tools/hunks.py').read().split("mode=sys.argv[1]")[0]
ns={}; exec(src,ns); parse,auto=ns['parse'],ns['auto']
def elif_join(o,t):  # o ends with '#else' or '#endif'; t starts with #ifdef X
    assert o[-1].strip() in ('#else','#endif'),o[-1]
    m=re.match(r'#ifdef (\w+)$',t[0].strip()); assert m,t[0]
    return o[:-1]+['#elif defined(%s)'%m.group(1)]+t[1:]
def cond_or(o,t):
    a=re.match(r'#ifdef (\w+)$',o[0].strip()).group(1); b=re.match(r'#ifdef (\w+)$',t[0].strip()).group(1)
    oc=[l for l in o[1:] if l.strip().startswith('//')]; tc=[l for l in t[1:] if l.strip().startswith('//')]
    return ['#if defined(%s) || defined(%s)'%(a,b)]+oc+tc+[l for l in o[1:] if not l.strip().startswith('//')]
R={
'C2DBoxI':{0:lambda o,b,t:o[:-1]+t},
'C3DPrimitive2':{0:'ot'},'CEngineCamera':{0:'ot'},'CTCInventory':{0:'ot'},'CEnginePrimitiveRenderUtil':{0:'ot'},
'CEngineSubPrimitiveBase':{0:'ot'},'CEngineSubPrimitiveRepeatedStaticMesh':{0:'ot'},
'C3DVector':{0:'to'},
'C4DVector':{0:lambda o,b,t:cond_or(o,t)},
'CEnginePrimitive2DViewportManager':{0:lambda o,b,t:cond_or(o,t)},
'CEnginePrimitiveManagerParticleLineGroup':{0:'ot',1:'to'},
'CEnginePrimitiveRenderer':{0:lambda o,b,t:o[:-1]+['#elif defined(%s)'%re.match(r'#ifdef (\w+)$',t[0]).group(1)]+t[1:]},
'CGraphicInfo':{0:lambda o,b,t:o[:6]+['#if defined(FABLE_RETAIL_CGRAPHICINFO_PEEKGRAPHICHEADER) || defined(FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE)']+o[7:],1:'theirs'},
'CMatrix3x4':{0:lambda o,b,t:elif_join(o,t)},
'CRGBFloatColour':{0:lambda o,b,t:elif_join(o,t),1:'theirs',2:lambda o,b,t:elif_join(o,t)},
'CWorld':{0:'ours'},
'CDateAndTime':{0:lambda o,b,t:o[:5]+['#ifdef FABLE_RETAIL_CDATEANDTIME_VALID',t[2].replace('// original','// Codex pass 22: original'),'#endif',o[5],o[6],
   '#elif defined(FABLE_RETAIL_CDATEANDTIME_VALID)',t[0],t[2],t[4],'#else',o[8],'#endif']},
'fable_containers':{0:'ot'},
}
for f in sorted(os.listdir(W)):
    if not f.startswith('conf_'): continue
    key=f[5:-2].split('_',1)[1]
    segs=parse(W+f); out=[]; n=0; rr=R.get(key,{})
    for k,v in segs:
        if k=='t': out+=v; continue
        o,b,t=v; r=rr.get(n)
        if r is None: a=auto(o,b,t); assert a is not None,(f,n); out+=a
        elif r=='ours': out+=o
        elif r=='theirs': out+=t
        elif r=='ot': out+=o+t
        elif r=='to': out+=t+o
        else: out+=r(o,b,t)
        n+=1
    s='\n'.join(out); assert '<<<<<<<' not in s and '>>>>>>>' not in s
    open(OUT+f[5:],'w').write(s); print(f,n)
