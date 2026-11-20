"""dtorfix.py <addr> <Class> <dtor_addr> <src> <out>: in-place respelling of a ??_G stand-in row's destructor call onto ??1<Class>@@UAE@XZ."""
import sys,re
a,C,da,src,out=sys.argv[1:6]
t=open(src).read()
hdr=("// agentNF182 (2026-11-20), in-place respelling of this row's existing local model: the destructor it calls is ??1%s@@UAE@XZ %s\n"
     "//   (Ego_r.pdb + FableWin.pdb; its landed row defines that leaf), declared virtual as in retail and called by qualified name so the\n"
     "//   call stays direct. The real ??_G is emitted only by a TU that compiles a %s constructor or destructor (retail TU-mate: the\n"
     "//   out-of-line constructor, a separate landed row), so the self leaf stays a stand-in (NF182 NOTES).\n") % (C,da,C)
if 'void Dtor();' in t:
    t=t.replace('void Dtor();','virtual ~%s();' % C,1)
    t=t.replace('this->Dtor();','this->%s::~%s();' % (C,C),1)
else:
    m=re.search(r'void __fastcall (\w+)\(%s\* (\w+)\);\n' % C,t); fn,arg=m.group(1),m.group(2)
    t=t.replace(m.group(0),'',1)
    t=t.replace('class %s { public:\n    void* vftable;\n};' % C,'class %s { public:\n    virtual ~%s();      // %s\n};' % (C,C,da),1)
    t=re.sub(r'\b%s\((\w+)\);' % fn, r'\1->%s::~%s();' % (C,C), t, count=1)
    assert fn not in t, fn
t=hdr+t
open(out,'w').write(t)
