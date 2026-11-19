R='    result.x = rx; result.y = ry; result.z = rz;\n    return result;'
def late(new):
    def f(s):
        i=s.rindex('    C3DVector result;\n'); s=s[:i]+s[i+len('    C3DVector result;\n'):]
        assert R in s
        return s.replace(R,new)
    return f
def mk(s):
    s=late('    return MakeVector(rx, ry, rz);')(s)
    i=s.index('static __declspec(noinline) C3DVector TransformPointThroughMirroredFrame')
    return s[:i]+'inline C3DVector MakeVector(float x, float y, float z) { C3DVector v; v.x = x; v.y = y; v.z = z; return v; }\n'+s[i:]
VARIANTS=[('copy2', late('    C3DVector out;\n    out.x = rx; out.y = ry; out.z = rz;\n    C3DVector result = out;\n    return result;')),
          ('make', mk)]
