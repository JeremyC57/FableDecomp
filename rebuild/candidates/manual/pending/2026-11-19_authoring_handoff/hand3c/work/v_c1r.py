F='const C3DMatrix& b)\n{\n'
Z='    float z = a.Row[2].z'
X='    if (z < 0.0f)'
def mk(k, at, after):
    def f(s):
        assert at in s
        i=s.index(at)+(len(at) if after else 0)
        return s[:i]+''.join('    long dead%d = 0;\n'%j for j in range(k))+s[i:]
    return f
VARIANTS=[('top%d'%k, mk(k,F,True)) for k in range(1,9)]+[('yz%d'%k, mk(k,Z,False)) for k in (1,2,3,4)]+[('ifz%d'%k, mk(k,X,False)) for k in (1,2,3,4)]
