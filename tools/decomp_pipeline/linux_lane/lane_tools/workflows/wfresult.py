"""wfresult.py <task-output-file> <wf-dir>: parse a finished author workflow's result list, save RESULT.json, print a summary."""
import json,sys
t=open(sys.argv[1]).read(); d=None
for i in range(len(t)):
    if t[i]=='[':
        try:
            d=json.JSONDecoder().raw_decode(t[i:])[0]
            if isinstance(d,list) and d and isinstance(d[0],dict) and 'addr' in d[0]: break
        except Exception: pass
        d=None
if not d: print('no parse'); sys.exit(0)
json.dump(d,open(sys.argv[2]+'/RESULT.json','w'),indent=1)
for r in d:
    try:
        last=r.get('last') or {}
        print('==',r.get('addr'),r.get('status'),'att',r.get('attempts'),r.get('module') or last.get('module'),'|',r.get('name') or last.get('name'),'|',[str(f)[:400] for f in (r.get('flags') or last.get('flags') or [])])
        print('  hdr:',[str(h).split('/hdr/')[-1][:120] for h in (r.get('headers_changed') or last.get('headers_changed') or [])])
        for o in r.get('open_issues') or []: print('  OPEN:',str(o)[:1200])
        if r.get('status') not in ('VERIFIED','MATCH_WITH_OPEN_ISSUES'):
            for k in ('grade','residue','next_idea'): print('  %s: %s'%(k,str(last.get(k))[:900]))
    except Exception as e:
        print('== row print failed',e)
