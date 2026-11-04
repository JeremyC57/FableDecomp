# decide.py: DECISIONS_V1.tsv for every in-scope target whose landed rows emit more than one spelling today
#   (spellings from compiling every landed definer + caller: logs/scan_base.tsv, logs/defs_base.tsv; NOT the first-binder table)
import sys,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
from spell import load
sys.path.insert(0,ROOT+'/tools/decomp_pipeline')
import reloc_identity as RI
import importlib.util as _iu
_sp=_iu.spec_from_file_location('nmd',ROOT+'/tools/decomp_pipeline/naming/msvc_demangle.py'); M=_iu.module_from_spec(_sp); _sp.loader.exec_module(M)
defs,use=load(); inv=inventory(); cat=catalog()
proven={}
if os.path.exists(W+'/proven.tsv'):
    for l in open(W+'/proven.tsv'):
        q=l.rstrip('\n').split('\t')
        if len(q)>=3 and re.fullmatch(r'[0-9a-f]{8}',q[0]): proven[q[0]]=(q[1],q[2])
JUNK_CLS={'T','S','X','Obj','CObj','Self','Inner','Outer','Member','SubObj','Params','Impl','Base','CBase','Object','Item','Node','RG','A','B','C'}
PDBC=set(open(W+'/logs/pdb_classes.txt').read().split()); PDBM=set(open(W+'/logs/pdb_methods.txt').read().split())
ALLOW={'CHandleBasedMemoryPool_012b4098'}
BCC=set(open(W+'/logs/bc_classes.txt').read().split())|{'IMediaSample','IPin','IUnknown','IMediaSeeking','IMediaPosition','IReferenceClock','IBaseFilter','IFilterGraph','IEnumPins','IEnumMediaTypes','IMemAllocator','IMemInputPin','IQualityControl','AM_MEDIA_TYPE','_AMMediaType','_GUID','GUID','tagVARIANT','ITypeInfo','HWND__','HINSTANCE__','__POSITION','CNode','CNodeCache','CRefTime','IAMovieSetup','IMediaEventSink','IMediaFilter','IPersist','AMOVIESETUP_FILTER','FILTER_INFO','tagQuality'}
DENY=set('Combat Parry Block Creature Hero Village Quest Thing Engine Landscape Steering Navigation Nav Dice Art Appearance Shader Pixel Texture Mesh Inventory Gui Camera Script Weapon Clothing Morph Morphable Particle Graphic Primitive Patch World Faction Experience Alpha Falloff Sharpness Strafe Ability Animate Animation Sprite Instance Instances Idle Target Map Mapwho Diving Teleport Spell Village Crier Chicken Sign Navigator Region Building Door Villager Guard Bandit Balverine Trader Escort Ambush Whisper Potion Window Foreground Exclusive Profile Log Edit Control Centre Reaction Frequency Traits Persist Physics Collision Lod Render Renderer Screen Effect Vertex Buffer Index Mesh Model Bone Skeleton Terrain Water Weather Sky Cutscene Tavern Shop Card Health Will Skill Stat Stats Experience Mana Opinion Fame'.split())
DENY|=set('Action Summoner Lightning Particle Morphs Occlusion Joystick Pixel Lifetimer Reward Hero Experience'.split())
DENY-={'Buffer','Render','Effect','Log','Edit','Control','Instance','Target','Window'}
def engine_name(s,t):
    try:
        d=M.demangle(s)
    except Exception: return None
    if not isinstance(d,dict) or not d.get('ok'): return None
    names=[]
    for comp in d.get('comps',[]): names.append(comp.split('<')[0].lstrip('~'))
    for p in [d.get('ret','')]+list(d.get('params',[])):
        names+=re.findall(r'(?:struct|class) (\w+)',p or '')
    for n in names[:-1] if len(names)>1 else names:
        b=re.sub(r'_?[0-9a-fA-F]{8}$','',n)
        if scope(t)=='strmbase' and b in BCC: continue
        if n not in ALLOW and b in PDBC and b not in ('CString',): return 'engine PDB class name %s on a vendor function (propagated label)'%n
    for n in names:
        if scope(t)=='strmbase' and re.sub(r'_?[0-9a-fA-F]{8}$','',n) in BCC: continue
        toks=set(re.findall(r'[A-Z][a-z]+|[A-Z]+(?![a-z])',n))
        bad=toks & DENY
        if bad: return 'engine/gameplay vocabulary (%s) in %s on an audio-library function (propagated label)'%(','.join(sorted(bad)),n)
    mname=d.get('comps',[''])[-1] if d.get('comps') else ''
    if len(mname)>=14 and mname in PDBM: return 'engine PDB method name %s on a vendor function (propagated label)'%mname
    return None
def cls_of(s):
    k,leaf,c=RI.sym_identity(s); return c
def junk(s):
    if not s: return 'empty'
    if s.startswith('@') : return 'fastcall free stand-in'
    if re.match(r'^\?[A-Za-z_0-9]+@@Y[AGI]',s):
        m=re.match(r'^\?([A-Za-z_0-9]+)@@Y[AGI].(PA[UV]|AA[UV]|PB[UV])',s)
        if m and not s.startswith('??'): return 'free function taking a class pointer first (member stand-in?)'
        return None
    c=cls_of(s)
    m=re.match(r'^\?\?[01_]\w?([A-Za-z_0-9]+)@|^\?[A-Za-z_0-9]+@([A-Za-z_0-9]+)@',s)
    cn=(m.group(1) or m.group(2)) if m else c
    if cn in JUNK_CLS or re.match(r'^Row[0-9a-f]{8}$',cn or ''): return 'generic stand-in class %s'%cn
    return None
def defsym_by_addr():
    d=collections.defaultdict(list)
    for a,(st,s) in defs.items():
        if st in('OK','MERGED') and s: d[s].append(a)
    return d
DBY=defsym_by_addr()
def tmpl_icf(sps):
    ks=set()
    for s in sps:
        m=re.match(r'^\?(\w+)@\?\$(\w+)@',s) or re.match(r'^\?\?([01_]\w?)\?\$(\w+)@',s)
        if not m: return False
        ks.add(m.groups())
    return len(ks)==1
BCM=json.load(open(W+'/logs/bc_methods.json'))
def bc_proven(s,lab):
    try: d=M.demangle(s)
    except Exception: return None
    if not isinstance(d,dict) or not d.get('ok') or len(d.get('comps',[]))!=2: return None
    c,m=d['comps']
    if c=='CBaseObject': return None   # Fable's strmbase spells it CMyBaseObject (RTTI); the name belongs to the engine class
    if c in BCM and m in BCM[c]['m'] and lab in ('%s::%s'%(c,m),):
        return 'BaseClasses %s declares %s::%s and the catalog label agrees'%(BCM[c]['file'],c,m)
    return None
def sdk_sig_bad(s):
    try: d=M.demangle(s)
    except Exception: return 'undemangled'
    if not isinstance(d,dict) or not d.get('ok'): return 'undemangled'
    txt=' '.join([d.get('ret','')]+list(d.get('params',[])))
    for k,n in re.findall(r'(struct|class|union) (\w+)',txt):
        if n in BCC and n.startswith('C') and k!='class': return 'BaseClasses class %s spelled %s'%(n,k)
        if n.startswith('I') and n[1:2].isupper() and k!='struct': return 'COM interface %s spelled %s'%(n,k)
        if n in ('GUID','AM_MEDIA_TYPE','IUnknown_'): return 'SDK type spelled %s (SDK tag: _GUID / _AMMediaType / IUnknown)'%n
    return None
def fcls(s):
    try: d=M.demangle(s)
    except Exception: return None
    if not isinstance(d,dict) or not d.get('ok') or len(d.get('comps',[]))<2: return None
    return d['comps'][-2].split('<')[0]
def tustems(tu): return set(os.path.splitext(f)[0] for f in tu.split('|') if f)
rows=[]
tg=set(use)|set(a for a in defs if scope(a)!='out')
for t in sorted(tg):
    if scope(t)=='out': continue
    st,ds=defs.get(t,('NOTLANDED',''))
    sp={s:sorted(c) for s,c in use.get(t,{}).items()}
    ds_ok = st in ('OK','MERGED') and bool(ds)
    alls=set(sp)|({ds} if ds_ok else set())
    if len(alls)<2 and not (t in proven and proven[t][0] not in alls): continue
    lab=cat.get(t,['','?'])[1]; tu=inv.get(t,{}).get('tu_strings','')
    ev=[]; grade=None; win=None
    if t in proven:
        win,pe=proven[t]; grade='proven'; ev.append('proven: '+pe)
    elif scope(t)=='strmbase' and any(bc_proven(x,lab) for x in sorted(alls)):
        pc=[x for x in sorted(alls) if bc_proven(x,lab)]
        ok_=[x for x in pc if not sdk_sig_bad(x)]
        if ok_:
            win=ok_[0]; grade='proven'; ev.append('proven: '+bc_proven(win,lab)+'; signature keys checked against the SDK conventions')
        else:
            win='-'; grade='deferred'; ev.append('identity proven (%s) but every spelling has a signature-key error (%s): needs the SDK prototype'%(bc_proven(pc[0],lab),'; '.join('%s: %s'%(x,sdk_sig_bad(x)) for x in pc)))
    elif tu and not (ds_ok and fcls(ds) in tustems(tu)) and [x for x in sorted(alls) if fcls(x) in tustems(tu) and not junk(x) and not engine_name(x,t) and len(DBY.get(x,[]))<=1 and x!=ds]:
        c_=[x for x in sorted(alls) if fcls(x) in tustems(tu) and not junk(x) and not engine_name(x,t) and len(DBY.get(x,[]))<=1 and x!=ds]
        c_.sort(key=lambda x:(-len(sp.get(x,[])),len(x)))
        win=c_[0]; grade='placeholder'; ev.append('rule 1b: the only spelling(s) whose class is the __FILE__ class %s (one source for the class; method name unproven); the landed definer emits %s -> respell the definer (QUEUE_V2)'%(fcls(win),ds or 'nothing'))
    elif tmpl_icf(alls) and not ds_ok:
        grade='ICF'; win='-'; ev.append('distinct template-instance spellings of one template member (linker-folded COMDATs): leave')
    else:
        dj=junk(ds) if ds_ok else 'no definer symbol (%s)'%st
        if ds_ok and len(DBY.get(ds,[]))>1: dj='definer spelling also defined at %s'%','.join(x for x in DBY[ds] if x!=t)
        if ds_ok and not dj: dj=engine_name(ds,t)
        if ds_ok and not dj:
            win=ds; grade='placeholder'; ev.append('rule 2: the landed defining row emits it (Leaf %s)'%(leaf_of(open(W+'/cc/'+t+'/x.cpp',errors='replace').read()) or 'none'))
        else:
            cand=[(len(c),s) for s,c in sp.items() if not junk(s) and not engine_name(s,t) and len(DBY.get(s,[]))<=1]
            if not cand:
                grade='deferred'; win='-'; ev.append('every spelling is a stand-in (%s); definer: %s'%('; '.join(sorted(sp)),dj))
            else:
                def key(x):
                    n,s=x; agrees=RI.label_agrees(s,lab.replace('operator_','operator ')) is True
                    tucls=any(cls_of(s)==os.path.splitext(f)[0] for f in tu.split('|') if f)
                    return (-n,-int(agrees),-int(tucls),len(s),s)
                cand.sort(key=key); win=cand[0][1]; grade='placeholder'
                tie=len(cand)>1 and cand[1][0]==cand[0][0]
                ev.append('rule 3: most-bound caller spelling (%d of %d caller rows%s); definer not usable: %s -> respell the definer (QUEUE_V2)'%(cand[0][0],sum(len(c) for c in sp.values()),', tie broken by label/__FILE__/length' if tie else '',dj))
    if scope(t)=='strmbase' and grade in ('placeholder',) and win not in ('-',None) and sdk_sig_bad(win):
        ev.append('winner signature: '+sdk_sig_bad(win)+' -> deferred'); grade='deferred'; win='-'
    if tu: ev.append('__FILE__: '+tu)
    ev.append('catalog: '+lab)
    losers=[(s,c) for s,c in sp.items() if s!=win]
    rows.append((t,win,grade,'; '.join(ev),ds if ds_ok else '',' | '.join('%s[%s]'%(s,','.join(c)) for s,c in sorted(losers))))
BT=collections.defaultdict(set)
for r in bindings():
    if re.fullmatch(r'[0-9a-fA-F]{8}',r['target']): BT[r['symbol']].add(r['target'].lower())
# one retail address, one name: a winner decided for two addresses, or defined today by a landed row at another address, is not usable
wa=collections.defaultdict(list)
for r in rows:
    if r[1] not in ('-',None): wa[r[1]].append(r[0])
for i,r in enumerate(rows):
    t,wn=r[0],r[1]
    if wn in ('-',None): continue
    others=[x for x in wa[wn] if x!=t]+[x for x in DBY.get(wn,[]) if x!=t]+[x for x in BT.get(wn,()) if x!=t]
    if others:
        rows[i]=(t,'-','deferred',r[3]+'; CONFLICT: %s is also decided for / defined at %s (one address, one name: the overloads need their SDK/real signatures)'%(wn,','.join(sorted(set(others)))),r[4],r[5])
with open(W+'/DECISIONS_V1.tsv','w') as o:
    o.write('address\tdecided_symbol\tgrade\tevidence\tdefiner_symbol\tlosing_spellings[callers]\n')
    for r in rows: o.write('\t'.join(r)+'\n')
print(collections.Counter((scope(r[0]),r[2]) for r in rows))
