# C++ source helpers: find a class decl by name, matching braces, strip comments for analysis
import re
def strip_comments(s):
    return re.sub(r'//[^\n]*','',re.sub(r'/\*.*?\*/','',s,flags=re.S))
def match_brace(s,i):
    d=0
    for k in range(i,len(s)):
        c=s[k]
        if c=='{': d+=1
        elif c=='}':
            d-=1
            if d==0: return k
    return -1
def find_class(s,name):
    """first definition `struct|class name [: bases] {` (not a forward decl) -> (start, brace_open, brace_close, end_after_semicolon, key, bases)"""
    pats=[r'(?m)^([ \t]*)(?:template\s*<[^>]*>\s*)?(struct|class)\s+(?:__declspec\([^)]*\)\s*)?%s\b\s*(:[^{;]*)?\{'%re.escape(name),
          r'()(?<!\w)(struct|class)\s+(?:__declspec\([^)]*\)\s*)?%s\b\s*(:[^{;()]*)?\{'%re.escape(name)]
    for p in pats:
        for m in re.finditer(p,s):
            ls=s.rfind('\n',0,m.start())+1
            if '//' in s[ls:m.start()]: continue
            if s[max(0,m.start()-2):m.start()].endswith('::'): continue
            bo=m.end()-1; bc=match_brace(s,bo)
            e=s.find(';',bc)
            st=m.start(1) if m.group(1)!='' or p is pats[0] else m.start(2)
            return dict(start=st,indent=m.group(1),bo=bo,bc=bc,end=e+1,key=m.group(2),bases=(m.group(3) or '')[1:].strip())
    return None
