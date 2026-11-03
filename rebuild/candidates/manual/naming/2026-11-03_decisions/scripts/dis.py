# dis.py <addr> <obj> <leafsubstr>: unified diff of retail (catalog oracle) vs built function disassembly (relocs masked)
import sys, subprocess, re, difflib, os
a, obj, leaf = sys.argv[1], sys.argv[2], sys.argv[3]
cat = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open("rebuild/oracles/auto-re-candidates.tsv")}
b = bytes.fromhex(cat[a][3]); tmp = os.environ["S"] + f"/decisions/tmp/{a}.bin"; open(tmp, "wb").write(b)
def norm(lines):
    out = []
    for l in lines:
        m = re.match(r"\s*[0-9a-f]+:\s+((?:[0-9a-f]{2} )+)\s*(.*)", l)
        if m: out.append(re.sub(r"\s+"," ",re.sub(r"0x[0-9a-f]{3,}(?=$)|[0-9a-f]{3,} <[^>]*>|^(call|j[a-z]+) +0x[0-9a-f]+", "X", m.group(2))).strip())
    return out
r = subprocess.run(["objdump", "-D", "-b", "binary", "-m", "i386", "-M", "intel", tmp], capture_output=True, text=True).stdout
o = subprocess.run(["objdump", "-d", "-M", "intel", "--no-show-raw-insn", obj], capture_output=True, text=True).stdout
blocks = re.split(r"\n(?=[0-9a-f]{8} <)", o); body = [bl for bl in blocks if leaf in bl.split("\n")[0]]; body.sort(key=len, reverse=True)
bl = body[0] if body else ""
bo = [re.sub(r"\s+", " ", l.split(":", 1)[1]).strip() for l in bl.split("\n")[1:] if re.match(r"\s+[0-9a-f]+:", l)]
bo = [re.sub(r"\s+"," ",re.sub(r"0x[0-9a-f]{5,}|[0-9a-f]{3,} <[^>]*>", "X", x)).strip() for x in bo]
for l in difflib.unified_diff(norm(r.split("\n")), bo, "retail", "built", lineterm="", n=2): print(l)
