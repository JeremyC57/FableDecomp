import re
def structp(s):
    s=s.replace('    float x = a.','    C3DVector p;\n    p.x = a.').replace('    float y = a.','    p.y = a.').replace('    float z = a.','    p.z = a.')
    s=s.replace('if (z < 0.0f)\n        z *= -1.0f;','if (p.z < 0.0f)\n        p.z *= -1.0f;')
    a=s.index('C3DVector result;\n    result.x = b'); b=s.index('return result;',a)
    seg=re.sub(r'\* ([xyz])\b', r'* p.\1', s[a:b])
    return s[:a]+seg+s[b:]
def decl_zyx(s):
    s=s.replace('    float x = a.','    float z, y, x;\n    x = a.').replace('    float y = a.','    y = a.').replace('    float z = a.','    z = a.')
    return s
def decl_yzx(s):
    s=s.replace('    float x = a.','    float y, z, x;\n    x = a.').replace('    float y = a.','    y = a.').replace('    float z = a.','    z = a.')
    return s
VARIANTS=[('structp',structp),('decl_zyx',decl_zyx),('decl_yzx',decl_yzx)]
