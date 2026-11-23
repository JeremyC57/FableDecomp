# fix.py <file> <method> <old_letters> <new_letters> <lineno>: zero-line-delta in-place respell of ONE in-class declaration of <method>
# on line <lineno>: access label on the same line (old access restored after), `virtual` added, `const` added. Prints the new line.
import sys,re
f,meth,old,new,ln=sys.argv[1:6]; ln=int(ln)
ACC={'Q':('public',0,0),'I':('protected',0,0),'A':('private',0,0),'U':('public',1,0),'M':('protected',1,0),'E':('private',1,0),
     'S':('public',0,1),'K':('protected',0,1),'C':('private',0,1)}
L=open(f,encoding='utf-8',errors='surrogateescape').read().split('\n'); l=L[ln-1]
code,sep,com=l.partition('//')
if not re.search(r'\b'+re.escape(meth)+r'\s*\(',code) or not code.rstrip().endswith(';'): print('NOT A DECL:',l.strip()[:120]); sys.exit(1)
oa,ov,os_=ACC[old[0]]; na,nv,ns=ACC[new[0]]
if ov and not nv: print('NEEDS virtual removal'); sys.exit(1)
if os_!=ns: print('NEEDS static change'); sys.exit(1)
c=code.rstrip(); tail=code[len(c):]; body=c.lstrip(); ind=c[:len(c)-len(body)]
if nv and not ov and not body.startswith('virtual'): body='virtual '+body
if (not os_) and new[1:2]=='B' and old[1:2]=='A':
    b=body[:-1].rstrip(); body=(b[:-3].rstrip()+' const = 0' if b.endswith('= 0') else b+' const')+';'
if na!=oa: body='%s: %s %s:'%(na,body,oa)
L[ln-1]=ind+body+(tail if tail else (' ' if sep else ''))+sep+com
open(f,'w',encoding='utf-8',errors='surrogateescape').write('\n'.join(L)); print('L%d: %s'%(ln,L[ln-1].strip()[:170]))
