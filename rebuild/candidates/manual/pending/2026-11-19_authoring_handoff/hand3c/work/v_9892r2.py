F='void CShaderRenderManager::UpdateObjectSpaceTextureTransform()\n{\n'
def mk(k):
    def f(s):
        return s.replace(F, F+''.join('    long dead%d = 0;\n'%j for j in range(k)))
    return f
VARIANTS=[('t1k%d'%k, mk(k)) for k in range(1,9)]
