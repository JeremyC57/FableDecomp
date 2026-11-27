import json,sys,os,shutil
P=json.load(open(sys.argv[1])); root=sys.argv[2]; out=[]
st={"applied":0,"already":0,"missing":0,"new":0,"ambiguous":0}
for i,p in enumerate(P):
    f=os.path.join(root,p["file"]); old=p["old_text"]; new=p["new_text"]
    if not old:
        if os.path.exists(f):
            cur=open(f,encoding="utf-8",newline="").read()
            if cur==new: st["already"]+=1; out.append((i,p["file"],"already(new file)")); continue
            st["missing"]+=1; out.append((i,p["file"],"EXISTS_DIFFERENT(new file)")); continue
        os.makedirs(os.path.dirname(f),exist_ok=True); open(f,"w",encoding="utf-8",newline="").write(new); st["new"]+=1; out.append((i,p["file"],"new")); continue
    if not os.path.exists(f): st["missing"]+=1; out.append((i,p["file"],"NOFILE")); continue
    cur=open(f,encoding="utf-8",newline="").read(); n=cur.count(old)
    if n==1: open(f,"w",encoding="utf-8",newline="").write(cur.replace(old,new)); st["applied"]+=1; out.append((i,p["file"],"applied"))
    elif n==0:
        if new in cur: st["already"]+=1; out.append((i,p["file"],"already"))
        else: st["missing"]+=1; out.append((i,p["file"],"MISSING_OLD"))
    else: st["ambiguous"]+=1; out.append((i,p["file"],"AMBIGUOUS %d"%n))
print(st)
open(sys.argv[3],"w").write("".join("%d\t%s\t%s\n"%o for o in out))
