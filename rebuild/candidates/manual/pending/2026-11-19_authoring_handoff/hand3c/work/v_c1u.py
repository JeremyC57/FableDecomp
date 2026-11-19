R='    result.x = rx; result.y = ry; result.z = rz;\n    return result;'
def late(s, new):
    i=s.rindex('    C3DVector result;\n'); s=s[:i]+s[i+len('    C3DVector result;\n'):]
    assert R in s
    return s.replace(R,new)
VARIANTS=[('late', lambda s: late(s,'    C3DVector result;\n    result.x = rx; result.y = ry; result.z = rz;\n    return result;')),
 ('agg', lambda s: late(s,'    C3DVector result = { rx, ry, rz };\n    return result;')),
 ('late_xzy', lambda s: late(s,'    C3DVector result;\n    result.x = rx; result.z = rz; result.y = ry;\n    return result;'))]
