// CEngine::AllocateTextureFromEngineResourcePools(unsigned long usage, const C2DExtentsI& extents, long mipLevels, const CPixelFormat& format) -- allocate a texture for outside use from the resource manager's pools (a CIEngine interface forwarder)
// Leaf: ?AllocateTextureFromEngineResourcePools@CEngine@@UAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23F40, length 11 bytes: `mov ecx,[0x01436E98] ; jmp 0x00B3B1F0`.
// Identity: slot 69 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?AllocateTextureFromEngineResourcePools@CEngine@@UAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineResourceManager::AllocateExternalTextureFromResourcePools @ 0x00B3B1F0, Ego_r + FableWin ?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z;
//   its `ret 10h` pops the same 4 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E98 (CEngineResourceManager*) under its landed spelling g_pResourceManager; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CTexture;
class C2DExtentsI;
class CPixelFormat;

class CEngineResourceManager { public:
    CTexture* AllocateExternalTextureFromResourcePools(unsigned long usage, const C2DExtentsI& extents, long mipLevels, const CPixelFormat& format);   // 0x00B3B1F0
};
extern CEngineResourceManager* g_pResourceManager;   // 0x01436E98

class CEngine { public:                      // vtable 0x012A0F3C, slot 69
    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long usage, const C2DExtentsI& extents, long mipLevels, const CPixelFormat& format);
};

CTexture* CEngine::AllocateTextureFromEngineResourcePools(unsigned long usage, const C2DExtentsI& extents, long mipLevels, const CPixelFormat& format)
{
    return g_pResourceManager->AllocateExternalTextureFromResourcePools(usage, extents, mipLevels, format);
}
