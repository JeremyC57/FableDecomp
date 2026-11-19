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
L='''        CMatrix4x4& m = *this;
        for (long c1 = 0; c1 < 4; ++c1)
            for (long c2 = 0; c2 < 4; ++c2)
                m(c1, c2) = %s;'''
T1='a(c1, 0) * b(0, c2) + a(c1, 1) * b(1, c2) + a(c1, 2) * b(2, c2) + a(c1, 3) * b(3, c2)'
T2='b(0, c2) * a(c1, 0) + b(1, c2) * a(c1, 1) + b(2, c2) * a(c1, 2) + b(3, c2) * a(c1, 3)'
T3='(a(c1, 0) * b(0, c2)) + (a(c1, 1) * b(1, c2)) + (a(c1, 2) * b(2, c2)) + (a(c1, 3) * b(3, c2))'
VARIANTS=[('mref_ab',mk(L%T1)),('mref_ba',mk(L%T2)),('mref_paren',mk(L%T3)),
 ('this_ba',mk('''        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                (*this)(r, c) = b(0, c) * a(r, 0) + b(1, c) * a(r, 1) + b(2, c) * a(r, 2) + b(3, c) * a(r, 3);'''))]
