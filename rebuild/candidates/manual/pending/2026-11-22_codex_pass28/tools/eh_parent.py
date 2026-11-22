#!/usr/bin/env python3
"""eh_parent.py <Fable.exe> <funclet VA>...: find the EH-framed parent of each unwind funclet.
funclet -> the unwind-map entry {toState, action} that points at it -> the FuncInfo (magic 0x1993052x) owning that map -> the
`mov eax, FuncInfo ; jmp __CxxFrameHandler` thunk in .text -> the function whose prologue pushes that thunk (`push -1 ; push thunk`).
Prints: funclet, state index, FuncInfo, thunk, parent start. No bytes are printed."""
import sys, struct
exe = open(sys.argv[1], 'rb').read(); pe = struct.unpack_from('<I', exe, 0x3c)[0]
n = struct.unpack_from('<H', exe, pe + 6)[0]; opt = struct.unpack_from('<H', exe, pe + 20)[0]
secs = []
for i in range(n):
    o = pe + 24 + opt + 40 * i; nm = exe[o:o + 8].rstrip(b'\0').decode(); vs, rva, rs, raw = struct.unpack_from('<IIII', exe, o + 8)
    secs.append((nm, 0x400000 + rva, exe[raw:raw + rs]))
def rd(va):
    for nm, b, blob in secs:
        if b <= va < b + len(blob) - 3: return struct.unpack_from('<I', blob, va - b)[0]
    return None
text = [s for s in secs if s[0] == '.text'][0]
funcinfos = []
for nm, b, blob in secs:
    if nm not in ('.rdata', '.data'): continue
    for magic in (0x19930520, 0x19930521, 0x19930522):
        pat = struct.pack('<I', magic); j = blob.find(pat)
        while j >= 0:
            if j % 4 == 0:
                fi = b + j; maxs, umap = struct.unpack_from('<II', blob, j + 4)
                if 0 < maxs < 4096: funcinfos.append((fi, maxs, umap))
            j = blob.find(pat, j + 1)
def thunk_of(fi):
    pat = b'\xb8' + struct.pack('<I', fi); j = text[2].find(pat)
    return text[1] + j if j >= 0 else None
def parent_of(th):
    pat = b'\x6a\xff\x68' + struct.pack('<I', th); j = text[2].find(pat)
    if j < 0: return None
    if j >= 3 and text[2][j - 3:j] == b'\x55\x8b\xec': j -= 3
    return text[1] + j
for a in sys.argv[2:]:
    va = int(a, 16); hit = None
    for fi, maxs, umap in funcinfos:
        for s in range(maxs):
            if rd(umap + 8 * s + 4) == va: hit = (fi, s); break
        if hit: break
    if not hit: print(f'{va:08x}\t-\t-\t-\t-'); continue
    th = thunk_of(hit[0]); par = parent_of(th) if th else None
    print(f"{va:08x}\t{hit[1]}\t{hit[0]:08x}\t{(th or 0):08x}\t{(par or 0):08x}")
