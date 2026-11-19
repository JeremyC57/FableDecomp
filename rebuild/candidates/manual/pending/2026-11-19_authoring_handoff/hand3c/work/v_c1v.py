M='    if (z < 0.0f)\n        z *= -1.0f;\n'
def rep(a,b):
    def f(s):
        assert a in s
        return s.replace(a,b)
    return f
VARIANTS=[('negfirst', rep(M,'    if (z < 0.0f)\n        z = -1.0f * z;\n')),
 ('zmul', rep(M,'    if (z < 0.0f)\n        z = z * -1.0f;\n')),
 ('braces', rep(M,'    if (z < 0.0f) {\n        z *= -1.0f;\n    }\n')),
 ('rdecl', rep('    C3DVector result;\n    float rx','    C3DVector result;\n    float rx, ry, rz;\n    rx').replace if False else (lambda s: s)),
]
def rdecl(s):
    a='    float rx = '; b='    float ry = '; c='    float rz = '
    i=s.index('    if (z < 0.0f)')
    s=s[:i]+'    float rx, ry, rz;\n'+s[i:]
    return s.replace(a,'    rx = ').replace(b,'    ry = ').replace(c,'    rz = ')
VARIANTS[3]=('rdecl', rdecl)
