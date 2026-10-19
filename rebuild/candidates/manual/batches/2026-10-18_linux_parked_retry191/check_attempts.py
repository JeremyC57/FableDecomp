from pathlib import Path
import json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry191';out=[]
def record(a,src,sha,command,flags,log,kind='source_grade'):
 p=Path(src);assert hashlib.sha256(p.read_bytes()).hexdigest()==sha,(a,p)
 out.append(dict(address=a,source=str(p),sha256=sha,command=command,effective_flags=flags,log=str(log),status=Path(log).read_text().splitlines()[0],kind=kind))
for line in (r/'astra241_whole/ATTEMPTS.jsonl').read_text().splitlines():
 q=json.loads(line)
 if q['address']=='00a65590' and q['kind']=='grade':
  p=Path(q['snapshot']);record(q['address'],p/'source.cpp',q['sha256'],q['command'],q['base_flags']+q['extra_flags'],p/'stdout.txt')
for p in sorted((r/'astra239_whole/snapshots').glob('009fe8e0_0[123]')):
 q=json.loads((p/'grade.json').read_text());record('009fe8e0',p/'source.cpp',q['sha256'],q['command'],q['effective_flags'],p/'grade.log')
for p in sorted((r/'luna236_whole/attempts/00b90270').glob('grade0[124]')):
 sha=(p/'input.sha256').read_text().split()[0];record('00b90270',p/'source.cpp',sha,(p/'command.txt').read_text(),['/O2','/Oy','/W3'],p/'stdout.txt')
q=json.loads((o/'root_amendments/grade.json').read_text());record('009fe8e0',q['source'],q['sha256'],q['command'],q['effective_flags'],o/'root_amendments/grade.log','root_source_grade')
assert len(out)==9
(o/'attempt_audit.json').write_text(json.dumps({'counted_source_grades':9,'per_address':{'00a65590':2,'009fe8e0':4,'00b90270':3},'excluded_infrastructure':'luna236_whole/attempts/00b90270/grade03: Wine socket PermissionError before compiler, preserved; directory04 is counted sourcegrade3.','records':out},indent=2)+'\n')
print('PASS nine source-grade hashes, commands and preserved results; precompiler infrastructure request excluded')
