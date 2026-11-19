F='bool transpose) const\n{\n'
G='    } else {\n'
H='    CMatrix4x4& m = *this;\n    m(0, 0) *= scale.X;'
def mk(k, at, ind='    '):
    def f(s):
        assert at in s
        i=s.index(at)+len(at) if at!=H else s.index(at)
        return s[:i]+''.join(ind+'long dead%d = 0;\n'%j for j in range(k))+s[i:]
    return f
VARIANTS=[('top%d'%k, mk(k,F)) for k in range(1,9)]+[('else%d'%k, mk(k,G,'        ')) for k in range(1,9)]+[('arm%d'%k, mk(k,H)) for k in range(1,9)]
