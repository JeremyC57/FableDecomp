X='            transform.AlignedSetMultiplyTransposed('
Y='            SetVSConstants('
def mk(k, at):
    def f(s):
        r=''.join('            long dead%d = 0;\n'%j for j in range(k))
        assert at in s
        return s.replace(at, r+at)
    return f
VARIANTS=[('pre%d'%k, mk(k,X)) for k in range(1,9)]+[('post%d'%k, mk(k,Y)) for k in (1,2,3,4)]
