F='void CShaderRenderManager::UpdateObjectSpaceTextureTransform()\n{\n'
def mk(k):
    def f(s):
        return s.replace(F, F+''.join('    long dead%d = 0;\n'%j for j in range(k)))
    return f
VARIANTS=[('big%d'%k, mk(k)) for k in (12,16,20,24,28,32,40,48)]
