# e1fix.py: entry-1 respell of the six converter binders in rows/ (identifiers in code only; the named comment lines; line count kept)
import re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309/rows/'
def sub_code(txt,mp):
    out=[]
    for l in txt.split('\n'):
        i=l.find('//'); code,com=(l,'') if i<0 else (l[:i],l[i:])
        for a,b in mp: code=re.sub(r'\b%s\b'%a,b,code)
        out.append(code+com)
    return '\n'.join(out)
def fix(a,mp,conv,coms):
    p=A+a+'.cpp'; t=open(p,encoding='utf-8').read(); n0=t.count('\n')
    t=sub_code(t,mp)
    for x,y in conv+coms: assert t.count(x)==1,(a,x); t=t.replace(x,y)
    assert t.count('\n')==n0
    open(p,'w',encoding='utf-8').write(t); print(a,'ok')
V=[('GFAXToVX_009e1e10','GFVXToAX'),('GFVXToAX_009e1e10','GFVXToAX'),('GFAXToVX','GFVXToAX'),('GFAYToVY','GFVYToAY')]
SC=[('float __stdcall GFVXToAX(float x);','float __fastcall GFVXToAX(float x);'),('float __stdcall GFVYToAY(float y);','float __fastcall GFVYToAY(float y);')]
fix('009ddff0',V,SC,[('Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__stdcall float), the','Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (__fastcall float), the')])
fix('009dda50',V,SC,[('Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__stdcall float), the','Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (__fastcall float), the')])
fix('009dd210',V,[],[('Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__fastcall','Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (__fastcall')])
fix('009dca80',V,[],[('Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__fastcall','Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (__fastcall')])
fix('0048f230',V,SC,[('GFVXToAX 0x009E1E10 (__stdcall, virtual to render-target pixels: x * width / 640; some landed files call these GFAXToVX/GFAYToVY)','GFVXToAX 0x009E1E10 (__fastcall, Ego_r / FableWin ?GFVYToAY@@YIMM@Z / ?GFVXToAX@@YIMM@Z; virtual to render-target pixels: x * width / 640)')])
fix('00438aa0',[('GFVXToAX','GFAXToVX')],[('float __stdcall GFAXToVX(float x);','float __fastcall GFAXToVX(float x);')],
 [('GFVXToAX 0x009E1DA0 (__stdcall float; virtual-to-absolute X `K * x / width`, the inverse of the landed GFAXToVX 0x009E1E10, Ego_r.pdb ?GFVXToAX@@YIMM@Z pairs with ?GFVYToAY at 0x009E1DD0)',
   'GFAXToVX 0x009E1DA0 (__fastcall float, the float on the stack; actual-to-virtual X `K * x / width`, Ego_r / FableWin ?GFAXToVX@@YIMM@Z, the first of GFAXToVX / GFAYToVY / GFVXToAX / GFVYToAY = 0x009E1DA0 / 0x009E1DD0 / 0x009E1E10 / 0x009E1E40)')])
