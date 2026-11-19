import os
W=os.environ['W']
H=W+'/hdr/engine/CMatrix4x4AlignedSSE.h'
base=open('/home/user/FableDecomp/rebuild/include/engine/CMatrix4x4AlignedSSE.h').read()
OLD='''        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);'''
F='void CShaderRenderManager::UpdateObjectSpaceTextureTransform()\n{\n'
def L(t):
    return '''        CMatrix4x4& m = *this;
        for (%s c1 = 0; c1 < 4; ++c1)
            for (%s c2 = 0; c2 < 4; ++c2)
                m(c1, c2) = a(c1, 0) * b(0, c2) + a(c1, 1) * b(1, c2) + a(c1, 2) * b(2, c2) + a(c1, 3) * b(3, c2);''' % (t, t)
def mk(body, k):
    def f(s):
        s=s.replace(F, F+''.join('    long dead%d = 0;\n'%j for j in range(k)))
        return s, {H: base.replace(OLD, body)}
    return f
VARIANTS=[]
for name, body in (('mlong', L('long')), ('mint', L('int'))):
    for k in range(0,8):
        VARIANTS.append(('%s_k%d'%(name,k), mk(body,k)))
