#ifndef FABLE_ENGINE_CTCGATHERPOINTBASE_H
#define FABLE_ENGINE_CTCGATHERPOINTBASE_H
// CTCGatherPointBase (new, chunk Q1): RTTI .?AVCTCGatherPointBase@@ : CTCBase (ghidra_out/rtti_bases.tsv), vtable 0x012421E4.
// Ego_r.pdb layout (struct_layouts_egor.tsv, 32 bytes): Users (vector<CIntelligentPointer<const CThing> >) +0x0C, MaxOccupation
// +0x1C; the retail STLport vector is 12 bytes, so MaxOccupation sits at +0x18 (ctor 0x004E0AAB stores 1000 there), sizeof 0x1C.
// Name set (Ego_r.pdb / FableWin.pdb): ??0CTCGatherPointBase@@QAE@AAVCThing@@@Z, ?GetInterfaceType@..UBE?AW4ETCInterfaceType@@XZ,
// ?OnInitialActivate@..UAEXXZ, ?HasFrameUpdate@..UBE_NXZ, ?FrameUpdate@..UAEXXZ, ?OnSerialise@..UAEXAAVCPersistContext@@_N@Z,
// ?IsComponentSerialisable@..UBE_NXZ, ?AddAsUser@..QAE_NABVCThing@@@Z, ?RemoveAsUser@..QAEXABVCThing@@@Z and the protected
// virtuals ?IsPointInside@..MBE_NABVC3DVector@@@Z (derived overrides only: pure here, slot 28), ?Init@..MAEXXZ (slot 29,
// 0x004E0AE4) and ?OnSerialiseLeaf@..MAEXAAVCPersistContext@@@Z (slot 30, 0x004E0AE5). The editor virtuals
// (Get/SetFromPropertiesStruct, Set/GetLeafData...) are absent from retail, as in CTCBase.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing; class C3DVector; class CPersistContext;
class CTCGatherPointBase : public CTCBase {
public:
    CTCGatherPointBase(CThing&);                                         // 0x004E0AAB
    virtual ETCInterfaceType GetInterfaceType() const;                   // slot 21 0x004E0AD5
    virtual void OnInitialActivate();                                    // slot 6  0x004E0ADC
    virtual bool HasFrameUpdate() const;                                 // slot 8  0x004E0AD9
    virtual void FrameUpdate();                                          // slot 9  0x007FFD30
    virtual bool IsComponentSerialisable() const;                        // slot 23 0x004E0AE1
    bool AddAsUser(const CThing&);
    void RemoveAsUser(const CThing&);
protected:
#ifdef FABLE_RETAIL_CTCGATHERPOINTBASE_VSLOTS3
// agentFAM05 (2026-11-11): OnSerialise is public in both PDB name sets (?OnSerialise@CTCGatherPointBase@@UAEXAAVCPersistContext@@_N@Z, Ego_r.pdb /
// FableWin.pdb), not protected: the opt-in restores the PDB access around the declaration (no layout or vtable change).
public:
#endif // FABLE_RETAIL_CTCGATHERPOINTBASE_VSLOTS3
    virtual void OnSerialise(CPersistContext&, bool);                    // slot 1  0x007FFE40
#ifdef FABLE_RETAIL_CTCGATHERPOINTBASE_VSLOTS3
// agentFAM05 (2026-11-11): end of the public OnSerialise access restore above.
protected:
#endif // FABLE_RETAIL_CTCGATHERPOINTBASE_VSLOTS3
    virtual bool IsPointInside(const C3DVector&) const = 0;              // slot 28
    virtual void Init();                                                 // slot 29 0x004E0AE4
    virtual void OnSerialiseLeaf(CPersistContext&);                      // slot 30 0x004E0AE5
    std::vector<CIntelligentPointer<const CThing> > Users;               // +0x0c
    long MaxOccupation;                                                  // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CTCGatherPointBase) == 0x1c);
#endif
