"""ocmp.py <obj> [-q]: grade the kept object of 0x00A4EFC0 v retail (status + residue, relocs masked like rd.py)."""
import sys, csv, glob, os, re, difflib
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
addr="00a4efc0"; leaf='?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z'
obj=sys.argv[1]; S=os.environ["S"]; retail=None
for f in ["/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv"] + glob.glob(S + "/batch/*_oracle.tsv"):
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x", "") == addr: retail = bytes.fromhex(r["bytes"]); break
    if retail: break
while retail[-1] in (0xCC, 0x90): retail = retail[:-1]
res = V.obj_text(obj, leaf); built, sec = res[0], res[1]
rel = V.obj_relocs(obj, sec)
st = "MATCH" if built == retail else ("RELOCATION_MATCH" if len(built)==len(retail) and V.mask(retail, rel) == V.mask(built, rel) else "DIFFER(%dv%d)" % (len(built), len(retail)))
md = Cs(CS_ARCH_X86, CS_MODE_32); base = int(addr, 16)
def f(code):
    out=[]
    for i in md.disasm(code, base):
        m=i.mnemonic; ins=m+" "+i.op_str
        if m in("call","jmp") and "0x" in ins and "[" not in ins: ins=m+" REL"
        else:
            ins=re.sub(r"0x[0-9a-f]{6,}","ADDR",ins)
            if m=="mov" and re.search(r", 0$",ins): ins=re.sub(r", 0$",", ADDR",ins)
            if ins=="push 0": ins="push ADDR"
            ins=re.sub(r"\[0\]","[ADDR]",ins)
        out.append(("%x"%i.address,ins))
    return out
R=f(retail);B=f(built)
sm=difflib.SequenceMatcher(None,[x[1] for x in R],[x[1] for x in B],autojunk=False); n=0
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=="equal": continue
    n+=max(i2-i1,j2-j1)
    if "-q" not in sys.argv:
        print("@@ %s"%(R[i1][0] if i1<len(R) else "end"))
        for x in R[i1:i2]: print("-",x[1])
        for x in B[j1:j2]: print("+",x[1])
print(st,"residue",n)
