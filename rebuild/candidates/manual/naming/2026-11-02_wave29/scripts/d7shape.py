# d7shape.py: per D7 row, the retail body's member-destructor sequence (offset:callee) from the disassembly
import subprocess, os, re, sys
EXE = os.environ['FABLE_EXE']
for l in open(sys.argv[1]):
    f = l.rstrip('\n').split('\t'); a = int(f[1], 16); n = int(f[3])
    d = subprocess.run(['objdump', '-d', '-M', 'intel', '--no-show-raw-insn', '--start-address=%d' % a, '--stop-address=%d' % (a + n), EXE], capture_output=True, text=True).stdout
    seq = []; off = None
    for ln in d.splitlines():
        m = re.search(r'lea\s+ecx,\[esi\+0x([0-9a-f]+)\]', ln)
        if m: off = int(m.group(1), 16)
        m = re.search(r'mov\s+ecx,esi', ln)
        if m: off = 0
        m = re.search(r'call\s+0x([0-9a-f]+)', ln)
        if m: seq.append('%s:%s' % ('-' if off is None else hex(off), m.group(1))); off = None
    print(f[0], f[1], n, ' '.join(seq), f[4][:120])
