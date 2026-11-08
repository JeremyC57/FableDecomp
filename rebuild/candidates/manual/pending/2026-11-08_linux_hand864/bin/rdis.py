# dis.py <addr> [from_hex] [to_hex]  -- disassemble the retail oracle row (or with --obj <addr>_<tag> our build)
import sys,os,csv
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ['S']; a=sys.argv[1]
for r in csv.DictReader(open(S+'/batch/chunk864_oracle.tsv',encoding='utf-8-sig'),delimiter='\t'):
    if r['address'].lower()==a: b=bytes.fromhex(r['bytes'])
base=int(a,16); lo=int(sys.argv[2],16) if len(sys.argv)>2 else base; hi=int(sys.argv[3],16) if len(sys.argv)>3 else base+len(b)
for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,base):
    if lo<=i.address<hi: print(f"{i.address:08x} {i.bytes.hex():20} {i.mnemonic} {i.op_str}")
