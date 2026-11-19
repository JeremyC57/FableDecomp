D='#define FABLE_RETAIL_CMATRIX4X4\n'
F='void CShaderRenderManager::UpdateObjectSpaceTextureTransform()\n{\n'
def mk(defs, k=0):
    def f(s):
        s=s.replace(D, D+''.join('#define %s\n'%d for d in defs))
        return s.replace(F, F+''.join('    long dead%d = 0;\n'%j for j in range(k)))
    return f
VARIANTS=[('ctor3x4', mk(['FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4'])),('ctorshadow', mk(['FABLE_RETAIL_CMATRIX4X4_SHADOW_PERSPECTIVE_PASS22']))]
