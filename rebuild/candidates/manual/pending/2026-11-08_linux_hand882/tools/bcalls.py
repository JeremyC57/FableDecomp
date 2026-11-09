# list the call targets of the built function (objdump -dr on the q.py obj) next to the retail call sequence (c.txt)
import sys, re, subprocess, os, difflib
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S=os.environ.get("S","/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad"); A=S+"/w22/agent882"
obj=sys.argv[1]
out=subprocess.run(["objdump","-dr","--no-show-raw-insn",obj],capture_output=True,text=True).stdout
# find the target function block
blk=None; cur=[]; name=None
for l in out.splitlines():
    m=re.match(r"^[0-9a-f]+ <(.*)>:$",l)
    if m and m.group(1).startswith("$"): continue
    if m:
        if name and "ConstructQuestListPC" in name: blk=cur
        name=m.group(1); cur=[]; continue
    cur.append(l)
if name and "ConstructQuestListPC" in name: blk=cur
seq=[]
for i,l in enumerate(blk):
    m=re.search(r"[0-9a-f]+: DISP32\s+(\S+)",l)
    if m: seq.append(m.group(1))
def short(n):
    n=re.sub(r"std::|NUISystem::|\(.*","",n)
    n=re.sub(r"<[^<>]*(<[^<>]*(<[^<>]*>[^<>]*)*>[^<>]*)*>","<>",n)
    return n
def mshort(n):
    m=re.match(r"\?\?(\d|_[A-Z])\?\$([A-Za-z_0-9]+)@",n)
    if m: return {"0":"ctor","1":"dtor"}.get(m.group(1),"op"+m.group(1))+":"+m.group(2)
    m=re.match(r"\?\?(\d|_[A-Z]|[A-Z])([A-Za-z_0-9]+)@",n)
    if m: return {"0":"ctor","1":"dtor"}.get(m.group(1),"op"+m.group(1))+":"+m.group(2)
    m=re.match(r"\?\?\$([A-Za-z_0-9]+)@",n)
    if m: return m.group(1)
    m=re.match(r"\?([A-Za-z_0-9]+)@(\?\$)?([A-Za-z_0-9]+)@",n)
    if m: return m.group(3)+"::"+m.group(1)
    m=re.match(r"_([A-Za-z_0-9]+)",n)
    if m: return m.group(1)
    return n
B=[mshort(x) for x in seq]
fixb={"??2@YAPAXI@Z":"new","??3@YAXPAX@Z":"delete","??H@YI?AVCWideString@@PBGABV0@@Z":"op+"}
B=[("op+" if x.startswith("??H") else {"??2@YAPAXI@Z":"new","??3@YAXPAX@Z":"delete"}.get(x,x)) for x in B]
B=[x.replace("??$","") for x in B]
B=[{"op4:CWideString":"CWideString::operator=","opY:CWideString":"CWideString::operator+=","GetDef":"CDefinitionManager::GetDef"}.get(x,x) for x in B]
def rshort(addr,n):
    fixed={"bfea1a":"new","bfe9bc":"delete","bfea0e":"malloc","bfea14":"free","99bff0":"op+","99be70":"op+","99bf30":"op+","99f690":"op+",
           "428494":"CManager::CreateMenuEntryComponent","429c15":"ctor:CCountedPointer","42aa29":"ctor:CCountedPointer",
           "42be50":"ctor:CAction","4365b0":"CVectorMap::LowerBound","40f020":"CVectorMap::LowerBound","436c80":"vector::push_back",
           "436ac0":"vector::_M_insert_overflow","51cf10":"vector::_M_insert_overflow","4ac940":"vector::_M_insert_overflow",
           "42b312":"CDefinitionManager::GetDef","5ff190":"CTCInventoryMap::CreateMapMarkers","603000":"CTCInventoryMap::CreateMiniMapMarkers",
           "99ba70":"CWideString::Sprintf","99ee20":"CCharString::GetAsUpperCase","624620":"_Rb_tree::insert_unique","512da0":"_Rb_tree::insert_unique",
           "42d2c6":"_Rb_tree::insert_unique","4a9cb0":"_Rb_tree::_M_erase","52f440":"ctor:CUIState","630350":"ctor:CUIStateDef","6250b0":"ctor:CUIStateDef",
           "9fc550":"dtor:CResource","51a900":"ctor:CRegion","51a110":"dtor:CRegion","a01b10":"ctor:CBaseIntelligentPointer","a01c10":"dtor:CBaseIntelligentPointer"}
    if addr in fixed: return fixed[addr]
    parts=n.split("::")
    if len(parts)>=2 and parts[-1]==parts[-2]: return "ctor:"+parts[-1]
    if parts[-1].startswith("~"): return "dtor:"+parts[-1][1:]
    return "::".join(parts[-2:])
R=[]
for l in open(A+"/c.txt"):
    m=re.search(r"call 0x([0-9a-f]+) ->\s*(\S*)",l)
    if m: R.append(rshort(m.group(1),m.group(2)))
if len(sys.argv)>2 and sys.argv[2]=="b":
    print("\n".join(B)); sys.exit()
for l in difflib.unified_diff(R,B,lineterm="",n=2): print(l)
