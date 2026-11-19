import os
W=os.environ['W']
H=W+'/hdr/engine/CMatrix4x4AlignedSSE.h'
base=open('/home/user/FableDecomp/rebuild/include/engine/CMatrix4x4AlignedSSE.h').read()
OLD='''        const CMatrix4x4* pa = &a;
        const CMatrix4x4* pb = &b;
        CMatrix4x4* pdst = this;
        GF_SSE_MATRIX_MULTIPLY(pdst, pa, pb);'''
assert OLD in base
A='''        const CMatrix4x4* src2;
        float* dest;
        const CMatrix4x4* src1;
        src1 = &a;
        src2 = &b;
        dest = &E11;
        GF_SSE_MATRIX_MULTIPLY(dest, src1, src2);'''
B='''        const CMatrix4x4* src1 = &a;
        const CMatrix4x4* src2 = &b;
        float* dest = &E11;
        GF_SSE_MATRIX_MULTIPLY(dest, src1, src2);'''
VARIANTS=[('sseA', lambda s:(s,{H:base.replace(OLD,A)})),('sseB', lambda s:(s,{H:base.replace(OLD,B)}))]
