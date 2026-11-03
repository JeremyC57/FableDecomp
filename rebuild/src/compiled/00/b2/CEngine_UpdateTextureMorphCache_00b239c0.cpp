// CEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs, bool flag, float value, const std::set<long>* indices) -- change the graphic data bank's scene, then forward the arguments to the surface-composition manager's disk-cache update (sibling call)
// Leaf: ?UpdateTextureMorphCache@CEngine@@UAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B239C0, length 27 bytes.
// Callees: CGraphicDataBank::ChangeScene @ 0x009FD0C0 (on the bank at +0x954 of the engine data record @ 0x01436E98), the surface-composition manager's
// UpdateDiskCache @ 0x00B2D9F0 (on the manager @ 0x01436EAC; a `jmp`: the four stack arguments are passed through unchanged and the
// callee's `ret 10h` pops them). Identity (naming wave 34): Ego_r.pdb / FableWin.pdb ?UpdateTextureMorphCache@CEngine@@UAEXABV?$CArray
// @VCEngineTextureMorph@@@@_NMPBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z (virtual, the CIEngine interface method) and
// ?UpdateDiskCache@CEngineSurfaceCompositionManager@@QAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBV?$set@... (same parameters);
// `this` is not read (the two objects come from the engine's static singletons). Was landed as a static no-argument member.
template <class T> class CArray;
class CEngineTextureMorph;
namespace std {
template <class T> struct less;
template <class T> class allocator;
template <class K, class C, class A> class set;
}
typedef std::set<long, std::less<long>, std::allocator<long> > CLongSet;

class CGraphicDataBank { public:
    void ChangeScene();                            // 0x009FD0C0
};

struct CEngineData_01436e98 {
    unsigned char      _pad[0x954];
    CGraphicDataBank*  m_pGraphicBank;             // +0x954
};

class CEngineSurfaceCompositionManager { public:
    void UpdateDiskCache(const CArray<CEngineTextureMorph>& morphs, bool flag, float value, const CLongSet* indices);   // 0x00B2D9F0
};

extern CEngineData_01436e98*               g_pEngineData_01436e98;           // 0x01436E98
extern CEngineSurfaceCompositionManager*   g_pSurfaceComposition_01436eac;   // 0x01436EAC

class CEngine { public:
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs, bool flag, float value, const CLongSet* indices);
};

void CEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs, bool flag, float value, const CLongSet* indices)
{
    g_pEngineData_01436e98->m_pGraphicBank->ChangeScene();
    g_pSurfaceComposition_01436eac->UpdateDiskCache(morphs, flag, value, indices);
}
