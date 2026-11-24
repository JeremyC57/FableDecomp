# disfn.py <obj> <symbol-substring>: disassemble one function COMDAT of an object (capstone), relocation symbols inline (NF355)
import sys, capstone
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF355/tools'); import nf
o, want = sys.argv[1], sys.argv[2]
secs, sl = nf.coff(o)
for i, n, v, sec, t, scl in sl:
    if scl == 2 and sec > 0 and want in n and secs[sec-1]['name'].startswith('.text'):
        s = secs[sec-1]; rel = {off: sym for off, sym, typ in s['rel']}
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        for ins in md.disasm(s['data'], 0):
            r = [rel[k] for k in range(ins.address, ins.address + ins.size) if k in rel]
            print('%04x %-8s %-30s %s' % (ins.address, ins.mnemonic, ins.op_str, (r[0][:70] if r else '')))
        break
