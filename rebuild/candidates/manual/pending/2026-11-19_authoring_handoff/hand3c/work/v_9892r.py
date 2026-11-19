D='#define FABLE_RETAIL_CMATRIX4X4\n'
F='void CShaderRenderManager::UpdateObjectSpaceTextureTransform()\n{\n'
def mk(k, where='top'):
    def f(s):
        s=s.replace(D, D+'#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS\n#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS\n')
        r=''.join('    long dead%d = 0;\n'%j for j in range(k))
        assert F in s
        return s.replace(F, F+r), {}
    return f
VARIANTS=[('k%d'%k, mk(k)) for k in range(0,9)]
