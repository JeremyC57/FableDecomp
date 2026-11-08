#ifndef FABLE_ENGINE_C3DGENERATOROBJECT_H
#define FABLE_ENGINE_C3DGENERATOROBJECT_H
// C3DGeneratorObject (chunk 868, row 0x00A2F740): Ego_r.pdb struct_layouts_egor.tsv -- a C3DDummyObject (0x34 bytes, the layout dump's
// 13 unnamed words) then CCharString ObjectName +0x34, unsigned long BankIndex +0x38, bool UseLocalOrigin +0x3C; sizeof 0x40. RTTI: no
// .?AVC3DGeneratorObject@@ entry (not polymorphic). FableWin.pdb: ??0C3DGeneratorObject@@QAE@ABVC3DDummyObject@@_NABVCCharString@@K@Z
// (inline in retail: 0x00A2FAF8..0x00A2FB39 copies the dummy with `rep movsd`, copy-constructs the name 0x0099EC30, stores the bank
// index and the flag), ??0C3DGeneratorObject@@QAE@XZ, ??0 copy / ??4 assignment / ??1 (the implicit ones), ?GetObjectName@..@@QBEABVCCharString@@XZ,
// ?GetBankIndex@..@@QBEKXZ, ?GetUseLocalOrigin@..@@QBE_NXZ (inline), ?LoadBinary@..@@QAEXAAVCMemoryDataInputStream@@@Z,
// ?SaveBinary@..@@QAEXAAVCDataOutputStream@@@Z (out of line).
#include "rebuild_abi.h"
#include "engine/C3DDummyObject.h"
#include "fable_string.h"
class CMemoryDataInputStream;
class CDataOutputStream;
class C3DGeneratorObject : public C3DDummyObject {
public:
    C3DGeneratorObject() : BankIndex(0), UseLocalOrigin(false) { }
    C3DGeneratorObject(const C3DDummyObject& dummy, bool useLocalOrigin, const CCharString& objectName, unsigned long bankIndex)
        : C3DDummyObject(dummy), ObjectName(objectName), BankIndex(bankIndex), UseLocalOrigin(useLocalOrigin) { }
    const CCharString& GetObjectName() const { return ObjectName; }
    unsigned long GetBankIndex() const { return BankIndex; }
    bool GetUseLocalOrigin() const { return UseLocalOrigin; }
    void LoadBinary(CMemoryDataInputStream&);
    void SaveBinary(CDataOutputStream&);
private:
    CCharString ObjectName;                                              // +0x34
    unsigned long BankIndex;                                             // +0x38
    bool UseLocalOrigin;                                                 // +0x3C
};
FABLE_STATIC_ASSERT(sizeof(C3DGeneratorObject) == 0x40);
#endif // FABLE_ENGINE_C3DGENERATOROBJECT_H
