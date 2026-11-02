# h27_pxs.py: naming wave 27, family pxs (DECISIONS_W26_RESOLVED D4 + D5) -- Ego_r.pdb / FableWin.pdb spell the CPixelShader
#  ctor 0x00A5ECB0 and CShaderRenderManager::CreatePixelShader 0x0098A9A0 private (AAE): ??0CPixelShader@@AAE@PAUIDirect3D
#  PixelShader9@@ABV?$CArray@J@@ABVCCharString@@@Z / ?CreatePixelShader@CShaderRenderManager@@AAE?AVCPixelShader@@ABV?$CArray@K@@
#  ABV?$CArray@VCCharString@@@@ABVCCharString@@@Z. Retail calls the private ctor from CShaderRenderManager and the private
#  CreatePixelShader from CShaderResource::GetPixelShader (0x00A5D620), so retail's classes declared those friends.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = ('// Naming wave 27 (family pxs, DECISIONS_W26 D4): %s')
def note(a, txt, leaf=None):
    p, rel, t = he._rw(a, 'src')
    if 'family pxs' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else: L.insert(1, '// Leaf: ' + leaf); i = [1]
    L.insert((i[0] + 1) if i else 1, NOTE % txt); he._w(p, '\n'.join(L))
# own row 0x00A5ECB0
a = '00a5ecb0'
he.edit(a, 'src', '''class CPixelShader : public CShaderBase { public:                // vtable 0x0129A054
    CPixelShaderData* Data;                        // +0x04
    virtual void v00();
    CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name);
};''', '''class CShaderRenderManager;
class CPixelShader : public CShaderBase { public:                // vtable 0x0129A054
    CPixelShaderData* Data;                        // +0x04
    virtual void v00();
private:
    CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name);   // private (Ego_r / FableWin AAE)
    friend class CShaderRenderManager;             // retail's CreatePixelShader 0x0098A9A0 builds it
};''')
note(a, 'the ctor is private (Ego_r.pdb / FableWin.pdb AAE) with\n//   CShaderRenderManager a friend (its CreatePixelShader 0x0098A9A0 constructs the shader). Was public (QAE).',
     '??0CPixelShader@@AAE@PAUIDirect3DPixelShader9@@ABV?$CArray@J@@ABVCCharString@@@Z')
# 0x0098A9A0 CreatePixelShader
a = '0098a9a0'
he.edit(a, 'src', '''    CPixelShaderData* Data;
    CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name);   // 0x00A5ECB0
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }''', '''    CPixelShaderData* Data;
private:
    CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name);   // 0x00A5ECB0 (private, AAE)
    friend class CShaderRenderManager;
public:
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }''')
he.edit(a, 'src', '''    CNameIndexMap     PSConstIndexMap;          // +0x3c0 (PSConstNames +0x3b4, its map at +0xc)
    CPixelShader CreatePixelShader(''', '''    CNameIndexMap     PSConstIndexMap;          // +0x3c0 (PSConstNames +0x3b4, its map at +0xc)
private:                                        // private (Ego_r.pdb / FableWin.pdb AAE)
    CPixelShader CreatePixelShader(''')
note(a, 'CreatePixelShader and the CPixelShader ctor 0x00A5ECB0 are\n//   private (Ego_r.pdb / FableWin.pdb AAE); CPixelShader declares CShaderRenderManager a friend. Was public (QAE).',
     '?CreatePixelShader@CShaderRenderManager@@AAE?AVCPixelShader@@ABV?$CArray@K@@ABV?$CArray@VCCharString@@@@ABVCCharString@@@Z')
# caller 0x00A5D620 CShaderResource::GetPixelShader
a = '00a5d620'
he.edit(a, 'src', '''class CShaderRenderManager { public:
    CPixelShader CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constants,
                                   const CCharString& name);   // 0x0098A9A0
};''', '''class CShaderRenderManager {
    CPixelShader CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constants,
                                   const CCharString& name);   // 0x0098A9A0 (private: Ego_r.pdb / FableWin.pdb AAE)
    friend class CShaderResource;                     // retail's GetPixelShader calls the private CreatePixelShader
};''')
note(a, 'CShaderRenderManager::CreatePixelShader 0x0098A9A0 is private (Ego_r.pdb /\n//   FableWin.pdb AAE); CShaderResource, which calls it, is its friend. Was public (QAE).')
