# callsto.py <target...> : retail E8 call sites to each target, with up to 3 preceding imm32 pushes shown as narrow / wide strings
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pe
for a in sys.argv[1:]:
    t = int(a, 16)
    for nm, v, vs, ro, rs in pe.SEC:
        if nm != '.text': continue
        blob = pe.B[ro:ro + rs]
        for i in range(len(blob) - 5):
            if blob[i] == 0xE8 and (v + i + 5 + struct.unpack_from('<i', blob, i + 1)[0]) & 0xffffffff == t:
                site = v + i
                pushes = []
                j = i - 1
                for k in range(i - 40, i):
                    if k >= 0 and blob[k] == 0x68:
                        imm = struct.unpack_from('<I', blob, k + 1)[0]
                        b = pe.rd(imm, 64)
                        if b:
                            nar = b.split(b'\0')[0]
                            wid = b[:64].decode('utf-16-le', 'replace').split('\0')[0]
                            pushes.append('%08x n=%r w=%r' % (imm, nar[:30], wid[:30]))
                print('%s <- %08x %s | %s' % (a, site, pe.owner(site), ' ; '.join(pushes[-2:])))
