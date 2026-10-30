#!/usr/bin/env python3
"""cpm_ptr.py [--dry] <file>...: wave 9 CPM pointer stand-ins. A row-local CPlayerManager that declares invented
pointer-returning accessors (`CPlayer* GetMainPlayer() [const]`, `CPlayer* GetPlayer(long) [const]`) next to header
members: the header's accessors return references (Ego_r.pdb / FableWin.pdb ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ
/ QBEABV.., ?GetPlayer@..@@QAEAAVCPlayer@@J@Z / QBEABV..J@Z). The block is replaced by engine/CPlayerManager.h (mig_cpm) and
every call is respelled: `..GetMainPlayer()->m` -> `..GetMainPlayer().m`, a pointer use `p = ..GetMainPlayer()` ->
`p = &..GetMainPlayer()`. mcheck decides (a const-ness change that moves the call to the other overload is a
RELOC_RENAMED conflict -> revert)."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mig_cpm
PTR = {"CPlayer*GetMainPlayer();": "GetMainPlayer", "CPlayer*GetMainPlayer()const;": "GetMainPlayer",
       "CPlayer*GetPlayer(long#);": "GetPlayer", "CPlayer*GetPlayer(long#)const;": "GetPlayer"}
def respell(t, name):
    code_idx = [m.start() for m in re.finditer(r"\b%s\s*\(" % name, t)]
    out = t; n = 0
    for s in reversed(code_idx):
        line_start = out.rfind("\n", 0, s) + 1
        if "//" in out[line_start:s]: continue
        # skip declarations (`CPlayer* GetMainPlayer(` / `& GetMainPlayer(`)
        before = out[line_start:s].rstrip()
        if not re.search(r"(->|\.)\s*$", before): continue
        # matching paren
        i = out.index("(", s); d = 0; j = i
        while True:
            if out[j] == "(": d += 1
            elif out[j] == ")":
                d -= 1
                if d == 0: break
            j += 1
        e = j + 1
        if re.match(r"\s*->", out[e:]):
            k = out.index("->", e); out = out[:k] + "." + out[k + 2:]; n += 1; continue
        # pointer use: find the start of the postfix chain
        b = s
        while True:
            m = re.search(r"(->|\.)\s*$", out[:b])
            if not m: break
            b = m.start()
            # the operand before -> / .
            q = b
            while q > 0 and out[q - 1] in " \t": q -= 1
            if out[q - 1] == ")":
                d = 0; q -= 1
                while True:
                    if out[q] == ")": d += 1
                    elif out[q] == "(":
                        d -= 1
                        if d == 0: break
                    q -= 1
                mm = re.search(r"[A-Za-z_]\w*(::[A-Za-z_]\w*)*\s*$", out[:q]); q = mm.start() if mm else q
            else:
                mm = re.search(r"[A-Za-z_]\w*(::[A-Za-z_]\w*)*\s*$", out[:q]); q = mm.start() if mm else q
            b = q
        out = out[:b] + "&" + out[b:]; n += 1
    return out, n
def fix(t):
    m = re.search(r"(?ms)^[ \t]*class\s+CPlayerManager\s*\{(.*?)\};", t)
    if not m: return None, "no block"
    names = set()
    body = re.sub(r"//[^\n]*", "", m.group(1)); body = re.sub(r"\bpublic\s*:", "", body)
    newb = []
    for d in [x.strip() for x in body.split(";") if x.strip()]:
        n = re.sub(r"\s+", "", d) + ";"; n = re.sub(r"\(long\w*\)", "(long#)", n)
        if n in PTR: names.add(PTR[n]); continue
        if n not in mig_cpm.OK: return None, "member " + d[:50]
    if not names: return None, "no pointer stand-in"
    # make the block header-shaped so mig_cpm takes it, then respell the calls
    blk = m.group(0)
    t2 = t[:m.start()] + re.sub(r"(?m)^.*CPlayer\s*\*\s*(GetMainPlayer|GetPlayer)\s*\([^;]*;.*\n?", "", blk) + t[m.end():]
    t2 = t2.replace(re.sub(r"(?m)^.*CPlayer\s*\*\s*(GetMainPlayer|GetPlayer)\s*\([^;]*;.*\n?", "", blk), blk.split("{")[0] + "{ public: };", 1) if False else t2
    r, why = mig_cpm.fix(t2)
    if r is None: return None, "cpm " + why
    tot = 0
    for nm in names:
        r, n = respell(r, nm); tot += n
    return r, "ok %d" % tot
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
