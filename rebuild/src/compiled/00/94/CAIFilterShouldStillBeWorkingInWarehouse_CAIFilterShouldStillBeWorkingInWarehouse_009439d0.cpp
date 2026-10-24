// CAIFilter_ShouldStillBeWorkingInWarehouse::~CAIFilter_ShouldStillBeWorkingInWarehouse() -- destroy the creature and task intelligent pointers, then the filter base (its vtable restored)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009439D0, length 27 bytes.
// Leaf: ??1CAIFilter_ShouldStillBeWorkingInWarehouse
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// The body destroys two 8-byte intelligent pointers at +0x10 and +0x08 (~CBaseIntelligentPointer @ 0x00A01C10) and then
// stores vtable 0x0128FD20 = CAIStateGroupBase::CAIFilterBase (rtti-vtables). Ego_r.pdb CAIFilter_ShouldStillBeWorking-
// InWarehouse (24 bytes) is exactly that: vptr, PTCVillage +4, PTask (CIntelligentPointer<CVillageTaskCarryCrate>) +8,
// PCreature (CIntelligentPointer<CThingAICreature>) +0x10; its own vtable is 0x01292A8C. No call or address reference
// to this body was found (the inlined copies serve the callers).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (twice).
// Shape: no derived vtable store -- the filter's destructor stores nothing of its own (`__declspec(novtable)` on the
// derived class, GOTCHAS 24o), the members die in reverse order, then the base's inline destructor stores the
// CAIFilterBase vtable. CIntelligentPointer<T>'s compiler-generated destructor drops its vtable reset.
class CThing;
class CTCVillage;
class CVillageTaskCarryCrate;
class CThingAICreature;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                          // +0x04
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
};
struct CAIStateGroupBase {
    struct CAIFilterBase {                  // vtable 0x0128FD20
        virtual bool operator()(CThing* thing) const;   // 0x00916A50
        ~CAIFilterBase() { }
    };
};
class __declspec(novtable) CAIFilter_ShouldStillBeWorkingInWarehouse : public CAIStateGroupBase::CAIFilterBase {   // vtable 0x01292A8C
public:
    CTCVillage*                                  PTCVillage;   // +0x04
    CIntelligentPointer<CVillageTaskCarryCrate>  PTask;        // +0x08
    CIntelligentPointer<CThingAICreature>        PCreature;    // +0x10
    virtual bool operator()(CThing* thing) const;              // 0x009444E0
    ~CAIFilter_ShouldStillBeWorkingInWarehouse();
};

CAIFilter_ShouldStillBeWorkingInWarehouse::~CAIFilter_ShouldStillBeWorkingInWarehouse()
{
}