import re,os
ROOT='/home/user/FableDecomp'
def block(text,start):
    # text[start] is '{'; return end index of matching '}'
    d=0
    for i in range(start,len(text)):
        if text[i]=='{': d+=1
        elif text[i]=='}':
            d-=1
            if d==0: return i
    return -1
def nested(text,E):
    """find 'class E : public X {' (nested, inline) or 'class O::E : ...{'; return body text or None"""
    for m in re.finditer(r'\bclass\s+(?:\w+::)?%s\s*(:[^{;]*)?\{'%re.escape(E),text):
        e=block(text,m.end()-1)
        return m.group(1) or '', text[m.end():e]
    return None
def hdr_class(O,E):
    for h in ('engine/%s.h'%O,'engine/NScript_%s.h'%O):
        p=ROOT+'/rebuild/include/'+h
        if os.path.exists(p):
            t=open(p).read()
            r=nested(t,E)
            if r: return h,r[0],r[1]
            return h,None,None
    return None,None,None
