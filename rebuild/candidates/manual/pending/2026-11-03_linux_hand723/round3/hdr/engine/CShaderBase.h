#ifndef FABLE_ENGINE_CSHADERBASE_H
#define FABLE_ENGINE_CSHADERBASE_H
// Hand-written (chunk 723): the polymorphic root of CPixelShader / CVertexShader. RTTI .?AVCShaderBase@@ (no bases); vtable
// 0x0129A030: slot 0 scalar deleting dtor 0x00987D10, slot 1 GetDebugName 0x00987CF0 (Ego_r.pdb ?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ,
// protected; the derived overrides share the body), slot 2 _purecall (CountReferences, private in both derived classes:
// ?CountReferences@CPixelShader@@EAEKXZ / ?CountReferences@CVertexShader@@EAEKXZ). The destructor is inline and empty: every
// expanded derived destructor ends on the 0x0129A030 store (0x0098C3D0, 0x00B36EC0, 0x00B71880). No data members (4 bytes).
#include "rebuild_abi.h"
class CCharString;
class CShaderBase {
public:
    virtual ~CShaderBase() { }                                           // slot 0  ??_G 0x00987D10
protected:
    virtual CCharString GetDebugName() const;                            // slot 1  0x00987CF0
private:
    virtual unsigned long CountReferences() = 0;                         // slot 2  (_purecall)
};
FABLE_STATIC_ASSERT(sizeof(CShaderBase) == 0x4);

#endif // FABLE_ENGINE_CSHADERBASE_H
