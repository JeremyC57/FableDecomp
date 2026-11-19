import os
W=os.environ['W']
H=W+'/hdr/engine/CMatrix4x4AlignedSSE.h'
base=open('/home/user/FableDecomp/rebuild/include/engine/CMatrix4x4AlignedSSE.h').read()
OLD='''        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);'''
NEW='''        if (false) {} else for (int r = 0; r < 4; ++r)
            if (false) {} else for (int c = 0; c < 4; ++c)
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);'''
NEWM='''        CMatrix4x4& m = *this;
        if (false) {} else for (long c1 = 0; c1 < 4; ++c1)
            if (false) {} else for (long c2 = 0; c2 < 4; ++c2)
                m(c1, c2) = a(c1, 0) * b(0, c2) + a(c1, 1) * b(1, c2) + a(c1, 2) * b(2, c2) + a(c1, 3) * b(3, c2);'''
L='    for (unsigned long c_transform = 0;'
def caller(s): 
    assert L in s
    return s.replace(L, '    if (false) {} else for (unsigned long c_transform = 0;')
VARIANTS=[('caller', lambda s: caller(s)),
          ('caller_hdr', lambda s: (caller(s), {H: base.replace(OLD, NEW)})),
          ('caller_hdrm', lambda s: (caller(s), {H: base.replace(OLD, NEWM)})),
          ('hdr', lambda s: (s, {H: base.replace(OLD, NEW)}))]
