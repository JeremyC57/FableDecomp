# annotate info.txt: virtual call slots -> CGameScriptInterface names
import re, sys
slots = {}
for l in open('/home/user/FableDecomp/rebuild/include/engine/CGameScriptInterface.h'):
    m = re.search(r'virtual\s+(.*?)\s*;\s*//\s*(\d+)\s+0x', l)
    if m:
        nm = re.search(r'(\w+)\s*\(', m.group(1))
        slots[int(m.group(2))] = nm.group(1) if nm else m.group(1)
for l in open(sys.argv[1]):
    l = l.rstrip('\n')
    m = re.search(r'call dword ptr \[e\w\w \+ (0x[0-9a-f]+)\]', l)
    if m:
        s = int(m.group(1), 16) // 4
        l += '   ; slot %d %s' % (s, slots.get(s, '?'))
    m = re.search(r'call dword ptr \[e\w\w\]$', l)
    if m: l += '   ; slot 0'
    print(l)
