#ifndef FABLE_ENGINE_CANIMCOMPONENTBASE_H
#define FABLE_ENGINE_CANIMCOMPONENTBASE_H
// Hand-written (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): the root of the animation components, RTTI
// CAnimComponentBase : CIVCountedPointeeBase (vptr +0x00, IVRefCount +0x04; sizeof 8, Ego_r.pdb). FableWin.pdb ??0CAnimComponentBase@@QAE@XZ
// is inline in retail. Slots (CAnimComponentFlags vtable 0x01255850): 0 scalar deleting destructor, 1 Delete (CIVCountedPointeeBase),
// 2 GetType (0x005DFE80, returns the EAnimComponent: called for the RegisteredComponentsByType key, `call [edx+8]` 0x005DD760),
// 3 Initialise (0x005DC3C0), 4 Alloc (0x005DFE90; Clone 0x005DB1D0 calls slot 4 then copies Flags), 5 Clone, 6 TransferBinaryIn
// (0x005DB8D0), 7 TransferBinaryOut (0x005DB1E0, writes +0x08 through 0x00993EB0), 8 SetDefaultValue (0x005DB1C0 `mov [ecx+8],0`;
// `call [eax+20h]` 0x005DD6A4). Names: both PDBs (?GetType@..UBE?AW4EAnimComponent@@XZ, ?Initialise@..UAE_NAAVCPersistContext@@ABVCCharString@@@Z,
// ?Alloc@..UBEPAVCAnimComponentBase@@XZ, ?Clone@.., ?TransferBinaryIn@..UAEXAAVCMemoryDataInputStream@@@Z,
// ?TransferBinaryOut@..UBEXAAVCDataOutputStream@@@Z, ?SetDefaultValue@..UAEXXZ); slots 3, 4 and 6 are placed by the FableWin.pdb
// declaration order and the Clone body. Needs FABLE_RETAIL_DEF_CLASS_BASE (the polymorphic CIVCountedPointeeBase).
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/EAnimComponent.h"
class CCharString;
class CDataOutputStream;
class CMemoryDataInputStream;
class CPersistContext;
class CAnimComponentBase : public CIVCountedPointeeBase {
public:
    virtual EAnimComponent GetType() const = 0;                                          // slot 2
    virtual bool Initialise(CPersistContext& context, const CCharString& token) = 0;     // slot 3
    virtual CAnimComponentBase* Alloc() const = 0;                                       // slot 4
    virtual CAnimComponentBase* Clone() const = 0;                                       // slot 5
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;                   // slot 6
    virtual void TransferBinaryOut(CDataOutputStream& stream) const = 0;                 // slot 7
    virtual void SetDefaultValue() = 0;                                                  // slot 8
};
FABLE_STATIC_ASSERT(sizeof(CAnimComponentBase) == 0x8);
#endif // FABLE_ENGINE_CANIMCOMPONENTBASE_H
