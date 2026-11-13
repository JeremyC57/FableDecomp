#ifndef FABLE_ENGINE_C3DGENERATOROBJECT_H
#define FABLE_ENGINE_C3DGENERATOROBJECT_H
// Overlay header (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects).
// C3DGeneratorObject (Ego_r.pdb, 64 bytes): base C3DDummyObject +0x00 (??0C3DGeneratorObject@@QAE@ABVC3DDummyObject@@_NABVCCharString@@K@Z),
// ObjectName (CCharString) +0x34, BankIndex (unsigned long) +0x38, UseLocalOrigin (bool) +0x3C. The accessors are FableWin.pdb
// inlines (?GetBankIndex@C3DGeneratorObject@@QBEKXZ, ?GetUseLocalOrigin@C3DGeneratorObject@@QBE_NXZ,
// ?GetObjectName@C3DGeneratorObject@@QBEABVCCharString@@XZ): AddGeneratedObjects reads +0x38 / +0x3C in place (0x00BF12E2 / 0x00BF1302).
// C3DMeshStats::MeshGenerators is a CArray of them (element stride 0x40, 0x00BF182B `add esi,40h`).
#include "engine/C3DDummyObject.h"
#include "fable_string.h"
class C3DGeneratorObject : public C3DDummyObject {
public:
    unsigned long GetBankIndex() const { return BankIndex; }
    bool GetUseLocalOrigin() const { return UseLocalOrigin; }
    const CCharString& GetObjectName() const { return ObjectName; }
private:
    CCharString ObjectName;                                              // +0x34
    unsigned long BankIndex;                                             // +0x38
    bool UseLocalOrigin;                                                 // +0x3c
};
FABLE_STATIC_ASSERT(sizeof(C3DGeneratorObject) == 0x40);
#endif
