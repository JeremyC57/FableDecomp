import sys, re
# usage: strings.py <file> <out_ascii> <out_utf16>
data = open(sys.argv[1], 'rb').read()
with open(sys.argv[2], 'w', encoding='latin-1') as f:
    for m in re.finditer(rb'[\x20-\x7e\t]{5,}', data):
        f.write("%08x\t%s\n" % (m.start(), m.group().decode('latin-1')))
with open(sys.argv[3], 'w', encoding='utf-8') as f:
    for m in re.finditer(rb'(?:[\x20-\x7e\t]\x00){5,}', data):
        f.write("%08x\t%s\n" % (m.start(), m.group().decode('utf-16le')))
