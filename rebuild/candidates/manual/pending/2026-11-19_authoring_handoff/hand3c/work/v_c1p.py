O='''    C3DVector result;
    result.x = b.Row[1].x * y + b.Row[2].x * z + b.Row[0].x * x + b.Translation.x;
    result.y = b.Row[1].y * y + b.Row[0].y * x + b.Row[2].y * z + b.Translation.y;
    result.z = b.Row[1].z * y + b.Row[0].z * x + b.Row[2].z * z + b.Translation.z;'''
T='''    C3DVector result;
    result.x = b.Row[2].x * p.z + b.Row[1].x * p.y + b.Row[0].x * p.x + b.Translation.x;
    result.y = b.Row[2].y * p.z + b.Row[0].y * p.x + b.Row[1].y * p.y + b.Translation.y;
    result.z = b.Row[2].z * p.z + b.Row[0].z * p.x + b.Row[1].z * p.y + b.Translation.z;'''
def r(pre):
    def f(s):
        assert O in s
        return s.replace(O, pre+T)
    return f
VARIANTS=[('p_zyx', r('    C3DVector p;\n    p.z = z; p.y = y; p.x = x;\n')),
          ('p_xzy', r('    C3DVector p;\n    p.x = x; p.z = z; p.y = y;\n')),
          ('p_yzx', r('    C3DVector p;\n    p.y = y; p.z = z; p.x = x;\n'))]
