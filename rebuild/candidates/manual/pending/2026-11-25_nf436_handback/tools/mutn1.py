import json, sys
p = json.load(open(sys.argv[1]))
for x in p['result']['authored']:
    s = x['source_cpp']
    if x['address'] == '00644627':
        s2 = s.replace("    if (!Drawing)\n", "    if (Drawing)\n")
    elif x['address'] == '00442760':
        s2 = s.replace("void CDrawBase::ClearPrimitives()\n{\n}", "void CDrawBase::ClearPrimitives()\n{\n    Drawing = true;\n}")
    else:
        s2 = s.replace(": CDrawBase(playerGui, illustrators)", ": CDrawBase(playerGui, *(std::vector<CDrawBase*>*)&playerGui)")
    assert s2 != s, x['address']; x['source_cpp'] = s2
json.dump(p, open(sys.argv[2], 'w'))
