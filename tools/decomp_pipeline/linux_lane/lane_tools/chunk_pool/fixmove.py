# fixmove.py <payload.json>: move bootstrap fixture copies under src/compiled|tests for the payload's addresses to integration/bootstrap_asm
import json,re,sys,subprocess,os
bs_path="rebuild/build_bootstrap.ps1"; bs=open(bs_path,encoding="utf-8",newline="").read(); B="rebuild/integration/bootstrap_asm"
addrs={c["address"].lower() for c in json.load(open(sys.argv[1]))["result"]["authored"]}
for m in set(re.findall(r"'((src\\compiled|tests)\\[0-9a-f]{2}\\[0-9a-f]{2}\\[^']*_([0-9a-fA-F]{8})(_test)?\.cpp)'",bs)):
    win,kind,a,_=m
    if a.lower() not in addrs: continue
    p="rebuild/"+win.replace("\\","/"); k="src" if kind.startswith("src") else "tests"; q=f"{B}/{k}/{os.path.basename(p)}"
    subprocess.run(["git","mv",p,q],check=True); bs=bs.replace(f"'{win}'",f"'integration\\bootstrap_asm\\{k}\\{os.path.basename(p)}'"); print("fixture moved:",q)
    with open(f"{B}/README.md","a") as f: f.write(f"- {os.path.basename(p)} moved here 2026-10-24 when the address landed as genuine C++.\n")
open(bs_path,"w",encoding="utf-8",newline="").write(bs)
