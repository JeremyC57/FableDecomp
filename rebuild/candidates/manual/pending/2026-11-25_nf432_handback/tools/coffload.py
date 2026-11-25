"""coffload.py: per-COMDAT relocations of an object's ??_7 sections (section index exact, not by name)."""
import struct
def vftables(p):
    d = open(p, "rb").read()
    mach, nsec, _, symoff, nsym, opt, _ = struct.unpack_from("<HHIIIHH", d, 0)
    secs = []
    for i in range(nsec):
        o = 20 + opt + 40 * i
        vs, va, rs, rp, rlp, lnp, nr, nl, ch = struct.unpack_from("<IIIIIIHHI", d, o + 8)
        secs.append((rlp, nr))
    strt = symoff + 18 * nsym; syms = []; i = 0
    while i < nsym:
        o = symoff + 18 * i; n = d[o:o+8]
        nm = d[strt + struct.unpack_from("<I", n, 4)[0]:].split(b"\0")[0] if n[:4] == b"\0\0\0\0" else n.rstrip(b"\0")
        val, sec, typ, cls, naux = struct.unpack_from("<IhHBB", d, o + 8)
        syms.append((nm.decode("latin1"), sec, cls, val))
        for _ in range(naux): syms.append(None)
        i += 1 + naux
    out = []
    for s in syms:
        if s and s[0].startswith("??_7") and s[1] > 0 and s[2] == 2:
            rlp, nr = secs[s[1] - 1]
            rel = []
            for k in range(nr):
                va, si, ty = struct.unpack_from("<IIH", d, rlp + 10 * k)
                if va: pass
                if va >= s[3]: rel.append((va - s[3], syms[si][0]))
            out.append((s[0], rel))
    return out
