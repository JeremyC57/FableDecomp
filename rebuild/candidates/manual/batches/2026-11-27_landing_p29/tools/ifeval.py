import sys,re
def ev(expr,D):
    e=re.sub(r"//.*","",expr); e=re.sub(r"/\*.*?\*/","",e)
    e=re.sub(r"defined\s*\(\s*(\w+)\s*\)",lambda m:"1" if m.group(1) in D else "0",e)
    e=re.sub(r"defined\s+(\w+)",lambda m:"1" if m.group(1) in D else "0",e)
    e=e.replace("&&"," and ").replace("||"," or ").replace("!"," not ")
    e=re.sub(r"\b[A-Za-z_]\w*\b",lambda m:m.group(0) if m.group(0) in("and","or","not") else ("1" if m.group(0) in D else "0"),e)
    return bool(eval(e))
def run(path,D):
    out=[];st=[]  # (active, taken)
    for l in open(path).read().replace("\\\n"," ").split("\n"):
        s=l.strip(); act=all(a for a,_ in st)
        m=re.match(r"#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)",s)
        if m:
            k,r=m.group(1),m.group(2)
            if k in("ifdef","ifndef","if"):
                v=(r.split()[0] in D) if k=="ifdef" else (r.split()[0] not in D) if k=="ifndef" else ev(r,D)
                st.append((v,v))
            elif k=="elif":
                a,t=st.pop(); v=(not t) and ev(r,D); st.append((v,t or v))
            elif k=="else":
                a,t=st.pop(); st.append((not t,True))
            else: st.pop()
            continue
        if act:
            c=re.sub(r"//.*","",l).strip()
            if c and not c.startswith("#include") and not c.startswith("#define FABLE_ENGINE"): out.append(c)
    assert not st,"unbalanced"
    return out
