from pathlib import Path
import csv,json,hashlib,subprocess,re
r=Path('work/codex-lane/w22');o=r/'coordinator_retry188'
R=Path('rebuild/include').resolve();seen=set()
def expand(p):
 p=p.resolve()
 if p in seen:return ''
 seen.add(p);out=[]
 for l in p.read_text().splitlines():
  if l.startswith('#include "'):
   n=l.split('"')[1];q=p.parent/n
   if not q.exists():q=R/n
   out.append(expand(q))
  elif p.name=='rebuild_abi.h' and ('fable_require_ptr32[' in l or l.startswith('#define FABLE_STATIC_ASSERT')):
   if 'fable_require_ptr32['in l:out+=['#ifdef _MSC_VER',l,'#endif']
   else:out+=['#ifndef _MSC_VER','#define FABLE_STATIC_ASSERT(expression)','#else',l]
  elif p.name=='rebuild_abi.h' and 'typedef char FABLE_JOIN(fable_static_assert_' in l:out+=[l.replace('fable_static_assert_','fable_abi_assert_'),'#endif']
  else:out.append(l.replace('fable_static_assert_','fable_abi_assert_'))
 return '\n'.join(out)+'\n'
o.mkdir(exist_ok=True)
authored=[];oracles=[];hashes=[]
assignments=[('00689130','astra240_whole','CInputProcessTargetLockRightStickTargetSelect','ProcessInput',4,'__AFTER_PRODUCTION__'),('00691ae0','astra240_whole','CInputProcessPhotojournalCapture','ProcessInput',4,'__AFTER_PRODUCTION__'),('007a0630','astra240_whole','CTCWillLightning_CLightningBeam','CLightningBeam',2,'__AFTER_PRODUCTION__')]
frozen={x['address']:x for x in json.loads((o/'frozen_hashes.json').read_text())}
for a,d,module,leaf,g,start in assignments:
 w=r/d;source=w/'src'/f'{a}.cpp';s=source.read_text();h=(w/'test'/f'{a}.cpp').read_text()
 sh=hashlib.sha256(s.encode()).hexdigest();th=hashlib.sha256(h.encode()).hexdigest()
 assert sh==frozen[a]['src']['sha256'];assert th==frozen[a]['test']['sha256'];hashes.append((a,sh,th))
 prefix=h[:h.index(s.splitlines()[0])] if a in ['004573fd','00457ca0'] else ''
 tail=s.strip()[-180:]
 h=h[h.index(tail)+len(tail):] if start=='__AFTER_PRODUCTION__' else h[h.index(start):]
 seen.clear();t='#include <stdio.h>\n#include <string.h>\n#include <stdlib.h>\n#include <new>\n#include <cstdio>\n#include <cstring>\n#ifndef _MSC_VER\n#define __forceinline inline\n#define __fastcall\n#define __cdecl\n#define __stdcall\n#define __declspec(x)\n#define __int64 long long\n#endif\n'+prefix+'\n'+expand(source)+'\n'+h
 for segment in re.split(r'^#include "[^"]+"\s*$',s,flags=re.M):assert segment.strip() in t
 (o/f'{a}_test.cpp').write_text(t)
 for flags in [[],['-fno-elide-constructors']]:
  suffix='_noelide'if flags else '';cmd=['g++','-std=c++98','-O2','-fcheck-new']+flags+[str(o/f'{a}_test.cpp'),'-o',str(o/f'{a}_host{suffix}')];p=subprocess.run(cmd,capture_output=True,text=True);(o/f'{a}{suffix}_build.log').write_text(json.dumps(cmd)+'\n'+p.stdout+p.stderr);p.check_returncode()
  p=subprocess.run([str(o/f'{a}_host{suffix}')],capture_output=True,text=True);(o/f'{a}{suffix}_host.log').write_text(p.stdout+p.stderr);print(p.stdout);p.check_returncode()
 for f in Path('work/codex-lane/batch').glob('*_oracle.tsv'):
  row=next((x for x in csv.DictReader(f.open(encoding='utf-8-sig'),delimiter='\t')if x['address']==a),None)
  if row:break
 assert row
 if a=='009ca780':
  (o/'009ca780_original_oracle.json').write_text(json.dumps(row,indent=2))
  row['bytes']=row['bytes'][:94]
 else:
  while a!='00bfd290' and row['bytes'].endswith(('cc','90')):row['bytes']=row['bytes'][:-2]
 row['length']=str(len(bytes.fromhex(row['bytes'])));oracles.append(row)
 authored.append(dict(address=a,module=module,name=leaf,leaf=({'00689130':'?ProcessInput@TargetSelect@','00691ae0':'?ProcessInput@PhotoCapture@','007a0630':'??0LightningBeam@'}[a]),source_cpp=s,test_cpp=t,pass_pattern=a+'_TEST PASS',flags=(['/GX'] if a=='00cab370' else [])))
(o/'payload.json').write_text(json.dumps({'result':{'authored':authored}},indent=2)+'\n')
with(o/'oracle.tsv').open('w')as f:
 z=csv.DictWriter(f,fieldnames=['address','name','length','bytes'],delimiter='\t');z.writeheader();z.writerows(oracles)
(o/'attempts.tsv').write_text('address\tobserved_author_grades\troot_source_grades\n'+''.join(f'{a}\t{g}\t0\n'for a,d,m,l,g,t in assignments))
(o/'author_hashes.tsv').write_text('address\tsource_sha256\tfixture_sha256\n'+''.join('\t'.join(x)+'\n'for x in hashes))
