"""egod_locals.py <Ego_d.pdb> [--tsv] <qualified name>...

Lists the debug symbols Ego_d.pdb still holds for a function: the carved S_GPROC32 / S_LPROC32 record (debug length), then its
named parameters and locals (S_BPREL32 frame offset or S_REGREL32 register-relative, CodeView type index, name) and the scope
blocks (S_BLOCK32) between them, up to the matching S_END. Ego_d's old module streams are carved from free pages (see
docs/journal/2026-11/EGOD_FINDINGS.md), so a function can occur more than once (several incremental links) or be cut short; the
first complete copy is printed. Resolve type indices with `llvm-pdbutil dump -types -type-index=0x... Ego_d.pdb`.
Ego_d is a VC10 /Od build of the TLC source tree: names, types and scopes are source evidence, frame offsets are not retail's.
"""
import sys, struct, mmap
args = sys.argv[1:]
tsv = '--tsv' in args
args = [a for a in args if a != '--tsv']
f = open(args[0], 'rb'); m = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
PROC = (0x1110, 0x110F)

def cstr(k):
    e = m.find(b'\0', k, k + 512)
    return m[k:e].decode('latin1') if e > 0 else None

def walk(j):
    ln = struct.unpack_from('<H', m, j)[0]
    out = []; k = j + 2 + ln; depth = 0
    for _ in range(2000):
        if k + 4 > len(m): return None
        ln2, kd = struct.unpack_from('<HH', m, k)
        if ln2 < 2 or ln2 > 1024: return None
        if kd == 0x0006:
            if depth == 0: return out
            depth -= 1; out.append(('end', '', '', ''))
        elif kd == 0x1103:
            depth += 1; out.append(('block', '', '', ''))
        elif kd == 0x110B:
            off, ti = struct.unpack_from('<iI', m, k + 4); out.append(('bprel', '%+d' % off, '0x%x' % ti, cstr(k + 12)))
        elif kd == 0x110C:
            ti, off, sg = struct.unpack_from('<IIH', m, k + 4); out.append(('static', 'S_LDATA32', '0x%x' % ti, cstr(k + 14)))
        elif kd == 0x1111:
            off, ti, rg = struct.unpack_from('<iIH', m, k + 4); out.append(('regrel', 'r%d%+d' % (rg, off), '0x%x' % ti, cstr(k + 14)))
        elif kd in PROC:
            return None
        k += 2 + ln2
    return None

for name in args[1:]:
    key = name.encode() + b'\0'; pos = 0; done = False
    while not done:
        i = m.find(key, pos)
        if i < 0: break
        pos = i + 1
        j = i - 39
        if j < 0: continue
        ln, kd = struct.unpack_from('<HH', m, j)
        if kd not in PROC or not (i + len(key) <= j + 2 + ln <= i + len(key) + 4): continue
        body = walk(j)
        if body is None: continue
        plen = struct.unpack_from('<I', m, j + 16)[0]
        if tsv:
            print('%s\t%d\t%d\t%s' % (name, plen, sum(1 for b in body if b[0] in ('bprel', 'regrel')),
                                       ', '.join(b[3] for b in body if b[0] in ('bprel', 'regrel'))))
        else:
            print('== %s (debug length %d)' % (name, plen)); d = 1
            for kind, off, ti, nm in body:
                if kind == 'end': d -= 1; continue
                print('  ' * d + ('{' if kind == 'block' else '%s %s type %s %s' % (kind, off, ti, nm)))
                if kind == 'block': d += 1
        done = True
    if not done and tsv: print('%s\t\t0\t' % name)
    elif not done: print('== %s: no complete record' % name)
