#!/usr/bin/env python3
"""mut434.py <in payload> <out payload>: Alloc mutant -- swap the data / interface arguments of `new C(data, game)` (the test must FAIL)."""
import json, sys, re
p = json.load(open(sys.argv[1]))
for x in p['result']['authored']:
    s = x['source_cpp']
    s2 = re.sub(r"return new (\w+)\(data, game\);", r"return new \1((CScriptDataBase*)&game, *(CGameScriptInterfaceBase*)data);", s)
    assert s2 != s, x['address']
    x['source_cpp'] = s2
json.dump(p, open(sys.argv[2], 'w'))
