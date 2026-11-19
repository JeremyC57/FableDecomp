O='''    result.x = b.Row[1].x * y + b.Row[2].x * z + b.Row[0].x * x + b.Translation.x;
    result.y = b.Row[1].y * y + b.Row[0].y * x + b.Row[2].y * z + b.Translation.y;
    result.z = b.Row[1].z * y + b.Row[0].z * x + b.Row[2].z * z + b.Translation.z;'''
def g(fmt):
    rows={'x':('y','z','x'),'y':('y','x','z'),'z':('y','x','z')}
    R={'x':0,'y':1,'z':2}
    out=[]
    for c in 'xyz':
        t=['b.Row[%d].%s * %s'%(R[v],c,v) for v in rows[c]]
        out.append('    result.%s = %s;'%(c, fmt.format(t[0],t[1],t[2],'b.Translation.'+c)))
    return '\n'.join(out)
def r(fmt):
    def f(s):
        assert O in s
        return s.replace(O, g(fmt))
    return f
VARIANTS=[('p1', r('{0} + ({1} + {2}) + {3}')),('p2', r('({0} + {1}) + ({2} + {3})')),('p3', r('{0} + ({1} + ({2} + {3}))')),('tfirst', r('{3} + {0} + {1} + {2}'))]
