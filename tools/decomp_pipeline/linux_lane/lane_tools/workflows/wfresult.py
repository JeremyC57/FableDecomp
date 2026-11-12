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
    print('==',r['addr'],r['status'],'att',r['attempts'],r['module'],'|',r['name'],'|',[f[:400] for f in r['flags']])
    print('  hdr:',[h.split('/hdr/')[-1][:120] for h in r['headers_changed']])
    for o in r['open_issues']: print('  OPEN:',o[:1200])
