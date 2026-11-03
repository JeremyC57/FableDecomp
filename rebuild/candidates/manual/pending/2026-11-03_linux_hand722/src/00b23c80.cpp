// CEngine::SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags) -- attach a primitive to a parent primitive through the attachment manager (a CIEngine interface forwarder)
// Leaf: ?SetAttachedPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23C80, length 11 bytes: `mov ecx,[0x01436E70] ; jmp 0x00B3A180`.
// Identity: slot 32 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetAttachedPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveAttachmentManager::SetAttachedPrimitive @ 0x00B3A180, Ego_r + FableWin ?SetAttachedPrimitive@CEnginePrimitiveAttachmentManager@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z;
//   its `ret 18h` pops the same 6 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E70 (CEnginePrimitiveAttachmentManager*) under its landed spelling g_pPrimitiveAttachmentManager_01436e70; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CEnginePrimitive;
class CEnginePrimitiveHandle;
class CEnginePrimitiveAttachmentInfo;

class CEnginePrimitiveAttachmentManager { public:
    void SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags);   // 0x00B3A180
};
extern CEnginePrimitiveAttachmentManager* g_pPrimitiveAttachmentManager_01436e70;   // 0x01436E70

class CEngine { public:                      // vtable 0x012A0F3C, slot 32
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags);
};

void CEngine::SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags)
{
    g_pPrimitiveAttachmentManager_01436e70->SetAttachedPrimitive(handle, primitive, attachmentInfo, parentHandle, attachIndex, flags);
}
