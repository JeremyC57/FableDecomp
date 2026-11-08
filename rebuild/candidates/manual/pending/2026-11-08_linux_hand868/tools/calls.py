import sys, re, subprocess
obj, func = sys.argv[1], sys.argv[2]
out = subprocess.run(['objdump','-dr','-M','intel',obj], capture_output=True, text=True).stdout
lines = out.splitlines()
inf = False; prev = None; res = []
for i, l in enumerate(lines):
    if l.endswith('>:'):
        inf = func in l
        continue
    if not inf: continue
    m = re.match(r'\s*([0-9a-f]+):\s+(.*)', l)
    if not m: continue
    if 'call' in m.group(2):
        nxt = lines[i+1] if i+1 < len(lines) else ''
        r = re.search(r'(DISP32|dir32)\s+(\S+)', nxt)
        if r:
            sym = r.group(2)
            sm = re.match(r'\?+([\$A-Za-z0-9_]+)@([A-Za-z0-9_\$]*)', sym)
            if sym.startswith('??0'): short = 'ctor:'+re.match(r'\?\?0([A-Za-z0-9_\$]+)', sym).group(1)
            elif sym.startswith('??1'): short = 'dtor:'+re.match(r'\?\?1([A-Za-z0-9_\$]+)', sym).group(1)
            elif sym.startswith('??4'): short = 'assign:'+re.match(r'\?\?4([A-Za-z0-9_\$]+)', sym).group(1)
            elif sym.startswith('??3'): short = 'operator delete'
            elif sym.startswith('??$'): short = 'tmpl:'+re.match(r'\?\?\$([A-Za-z0-9_]+)@', sym).group(1)+'<'+sym[len(re.match(r'\?\?\$([A-Za-z0-9_]+)@', sym).group(0)):][:40]+'>'
            elif sm: short = sm.group(1)+'@'+sm.group(2)[:30]
            else: short = sym[:40]
            res.append('%s:%s' % (m.group(1), short))
        else:
            res.append('%s:%s' % (m.group(1), m.group(2).replace('DWORD PTR ','')))
print('\n'.join(res))
