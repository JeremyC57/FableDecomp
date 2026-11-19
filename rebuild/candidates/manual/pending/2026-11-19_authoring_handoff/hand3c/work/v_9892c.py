import os
W=os.environ['W']
H=W+'/hdr/engine/CMatrix4x4AlignedSSE.h'
base=open('/home/user/FableDecomp/rebuild/include/engine/CMatrix4x4AlignedSSE.h').read()
OLD='''        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);'''
assert OLD in base
D='#define FABLE_RETAIL_CMATRIX4X4\n'
def mk(body):
    def f(s):
        return (s.replace(D, D+'#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS\n#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS\n'), {H: base.replace(OLD, body)})
    return f
def L(expr, t='long'):
    return '''        CMatrix4x4& m = *this;
        for (%s c1 = 0; c1 < 4; ++c1)
            for (%s c2 = 0; c2 < 4; ++c2)
                m(c1, c2) = %s;''' % (t, t, expr)
P=lambda k: 'a(c1, %d) * b(%d, c2)' % (k, k)
VARIANTS=[
 ('rev', mk(L(' + '.join(P(k) for k in (3,2,1,0))))),
 ('rnest', mk(L('%s + (%s + (%s + %s))' % tuple(P(k) for k in range(4))))),
 ('pairs', mk(L('(%s + %s) + (%s + %s)' % tuple(P(k) for k in range(4))))),
 ('int', mk(L(' + '.join(P(k) for k in range(4)), 'int'))),
 ('kloop0', mk('''        CMatrix4x4& m = *this;
        for (long c1 = 0; c1 < 4; ++c1)
            for (long c2 = 0; c2 < 4; ++c2) {
                float sum = 0.0f;
                for (long k = 0; k < 4; ++k)
                    sum += a(c1, k) * b(k, c2);
                m(c1, c2) = sum;
            }''')),
 ('kloop1', mk('''        CMatrix4x4& m = *this;
        for (long c1 = 0; c1 < 4; ++c1)
            for (long c2 = 0; c2 < 4; ++c2) {
                float sum = a(c1, 0) * b(0, c2);
                for (long k = 1; k < 4; ++k)
                    sum += a(c1, k) * b(k, c2);
                m(c1, c2) = sum;
            }''')),
]
