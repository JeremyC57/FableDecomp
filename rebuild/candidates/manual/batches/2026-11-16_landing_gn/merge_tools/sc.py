# sc.py <rel> : print the conflict hunks (with 3 lines of context) of the pending seqout conf file for <rel>
import glob,sys,os
L=os.path.dirname(os.path.abspath(__file__))
for f in glob.glob(L+'/seqout/'+sys.argv[1]+'.*.conf'):
    t=open(f).read().split('\n'); print('##',os.path.basename(f))
    for i,l in enumerate(t):
        if l.startswith('<<<<<<<'):
            j=i
            while not t[j].startswith('>>>>>>>'): j+=1
            for k in range(max(0,i-3),min(len(t),j+4)): print('%4d %s'%(k+1,t[k][:int(os.environ.get('W','200'))]))
            print('-----')
