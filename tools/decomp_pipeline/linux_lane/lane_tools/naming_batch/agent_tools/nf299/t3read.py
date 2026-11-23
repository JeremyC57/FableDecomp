# t3read.py <from> <to> [addr=note ...]: append a skipped-port STATUS line for every ENTRIES line in range not yet in STATUS.tsv,
# composed from work/tri3.txt (each stand-in, its witness binders as opened, the local definition found); extra notes per address
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299'
lo,hi=int(sys.argv[1]),int(sys.argv[2]); extra=dict(x.split('=',1) for x in sys.argv[3:])
done=set(l.split('\t')[0].replace('0x','') for l in open(A+'/STATUS.tsv'))
blocks=[];cur=[]
for l in open(A+'/work/tri3.txt'):
    if l.startswith('#'):
        if cur: blocks.append(cur)
        cur=[l.rstrip('\n')]
    else: cur.append(l.rstrip('\n'))
blocks.append(cur)
out=open(A+'/STATUS.tsv','a'); n_out=0
for b in blocks:
    n=int(b[0].split()[0][1:]); t=b[0].split()[1]
    if n<lo or n>hi or t in done: continue
    parts=[]; binders=[]
    for l in b[1:]:
        if l.startswith('   S '): cur_s=l[5:].split('  wit=')[0]; parts.append([cur_s,[]])
        elif 'lines=' in l and parts:
            w=l.split()[0]; binders.append('0x'+w)
            m=re.search(r'localdef=(\S+)',l); ld=m.group(1) if m else '-'
            parts[-1][1].append('%s %s'%(w,'defines '+ld if ld!='-' else ('STALE?' if 'STALE' in l else 'free / out-of-scope stand-in')))
        elif 'no landed source' in l and parts: parts[-1][1].append(l.strip())
    why=[]
    for s,bs in parts:
        kind='free stand-in function' if re.match(r'^(\?[A-Za-z_0-9]+@@Y|@)',s) else 'stand-in class token'
        why.append('%s %s [%s]'%(kind,s[:90],'; '.join(bs)))
    note='entry %d: binder(s) opened (tri3): '%n+' || '.join(why)+'. A respell needs the real class / parameter types (a new or renamed local model, or a port onto the shared header): T3'
    if t in extra: note+='. '+extra[t]
    out.write('0x%s\tskipped-port\t%s\t-\t%s\n'%(t,','.join(sorted(set(binders))) or '-',note)); n_out+=1
print('wrote',n_out)
