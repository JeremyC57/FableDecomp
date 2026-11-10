// CTCCrateStack::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CTCCrateStack expanded (~CBaseIntelligentPointer on SecondCrate +0x14 and FirstCrate +0x0C, then the out-of-line ~CTCBase 0x00686830), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCTCCrateStack@@UAEPAXI@Z
// Retail: Fable.exe @ 0x007B6C90. Identity: slot 0 of vtable 0x0126DA14, RTTI .?AVCTCCrateStack@@ : CTCBase (the catalog name agrees);
//   it follows the class's GetInterfaceType 0x007B6C60 and GetClassTypeName 0x007B6C70 in its TU.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CTCBase::~CTCBase 0x00686830, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CTCCrateStack : CTCBase (retail 0x0C bytes): FirstCrate +0x0C, SecondCrate +0x14 (CIntelligentPointer<CThing>),
//   CrateDefIndex +0x1C. No engine header models CTCCrateStack, so it is declared here over the shared engine/CTCBase.h and
//   engine/CIntelligentPointer.h (retail branches); spellings from Ego_r.pdb (?Alloc@, ?GetInterfaceType@, ?GetClassTypeName@,
//   ?OnInitialActivate@, ...). The class declares no destructor (retail's deleting destructor expands it).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/CTCBase.h"
class C3DVector;
class CTCVillage;
class CTCCrateStack : public CTCBase {
public:
    CTCCrateStack(CThing& thing) : CTCBase(thing) { }
    static CTCBase* FABLE_FASTCALL Alloc(CThing& thing);
    static void FABLE_FASTCALL FillStacks(const CTCVillage& village, long count);
    virtual ETCInterfaceType GetInterfaceType() const;                  // 0x007B6C60
    virtual CCharString GetClassTypeName() const;                       // 0x007B6C70
    virtual void OnInitialActivate();
    void AddCrate(CThing* crate);
    void FillStack(long count, const C3DVector& start, const C3DVector& end);
    CThing* GetNextCrate() const;
    long GetNumCrates() const;
    long GetSpaceLeft() const;
    CThing* RemoveNextCrate();
    CIntelligentPointer<CThing> FirstCrate;                             // +0x0C
    CIntelligentPointer<CThing> SecondCrate;                            // +0x14
    long CrateDefIndex;                                                 // +0x1C
};
FABLE_STATIC_ASSERT(sizeof(CTCCrateStack) == 0x20);

// Emission harness (not a retail function): constructing one emits the vftable and its deleting destructor.
CTCBase* EmitCrateStackDeletingDestructor(CThing& thing)
{
    return new CTCCrateStack(thing);
}
