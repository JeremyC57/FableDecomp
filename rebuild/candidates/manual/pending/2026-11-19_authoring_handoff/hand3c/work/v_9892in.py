import os
W=os.environ['W']
H=W+'/hdr/engine/CMatrix4x4AlignedSSE.h'
base=open('/home/user/FableDecomp/rebuild/include/engine/CMatrix4x4AlignedSSE.h').read()
OLD='''        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);'''
def mk(k, where):
    R=''.join('long dead%d = 0; '%j for j in range(k))
    if where=='inner':
        body='''        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c) {
                %s
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);
            }''' % R
    else:
        body='''        %s
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);''' % R
    return lambda s: (s, {H: base.replace(OLD, body)})
VARIANTS=[('in%d'%k, mk(k,'inner')) for k in range(1,9)]+[('arm%d'%k, mk(k,'arm')) for k in (1,2,3,4)]
