# queue_mark.py QUEUE_STATUS.tsv [QUEUE] [OUT] : write a copy of NAME_FIXES_QUEUE.tsv with ` [2026-11-04: index-verified; done]`
# appended to the note column (col 4) of every line QUEUE_STATUS.tsv calls `satisfied`.  A line is matched by its sha1 (first 12
# hex of the exact line text), so a queue that changed since the index run only marks the lines that are still identical; lines
# whose hash moved are reported, never guessed.  Defaults: QUEUE = repo rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv,
# OUT = $P/NAME_FIXES_QUEUE.new.tsv.  The coordinator copies OUT over the repo file after reading the diff.
import sys,hashlib,csv
csv.field_size_limit(1<<30)
P='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/plan/p1'
st=sys.argv[1]; Q=sys.argv[2] if len(sys.argv)>2 else '/home/user/FableDecomp/rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv'
OUT=sys.argv[3] if len(sys.argv)>3 else P+'/NAME_FIXES_QUEUE.new.tsv'
TAG=' [2026-11-04: index-verified; done]'
want={}
for r in csv.DictReader(open(st),delimiter='\t',quoting=csv.QUOTE_NONE):
    if r['status']=='satisfied': want[r['line_sha1_12']]=r
raw=open(Q,'rb').read().decode('utf-8'); nl='\r\n' if '\r\n' in raw else '\n'
L=raw.split(nl); n=0; seen=set()
for i,l in enumerate(L):
    h=hashlib.sha1(l.encode('utf-8','replace')).hexdigest()[:12]
    if h in want and ' done]' not in l:
        q=l.split('\t'); q[3]=q[3]+TAG; L[i]='\t'.join(q); n+=1; seen.add(h)
open(OUT,'wb').write(nl.join(L).encode('utf-8'))
miss=[r for h,r in want.items() if h not in seen]
print('marked',n,'of',len(want),'satisfied lines; not found (queue changed):',len(miss))
for r in miss[:20]: print('  line',r['line'],r['address'],r['new_symbol'][:80])
