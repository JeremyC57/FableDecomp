// NUISystem::CScrollingComponent::Initialise() -- initialise the changing-state base and set state3 vertical offset beyond the lowest child
// Leaf: Initialise
// Retail00547160,217 bytes ending ret00547238;7 trailing int3 bytes excluded.
// Shared header is donor-sized flattened and lacks inherited children/states; method view uses
// established component States+24 and8-byte child records+b0. CUIState Position.Y+c agrees PDB.
// Base Initialise0052C730, state-map operator[]0042D5B1, virtual GetCurrentState slot50/GetSize slot23.
#include "engine/CScrollingComponent.h"
// NF44 (round 6): the local C2DVector / CUIState / state-map stand-ins replaced by the shared engine/C2DVector.h,
// engine/NUISystem_CUIState.h and stlport/fable_map.h (FABLE_RETAIL_STLP_MAP + _INDEX_DECL_ONLY: operator[] out of line), so the
// subscript binds STLport map<unsigned long, NUISystem::CUIState>::operator[] 0x0042D5B1 (the landed row spelling).
#include "engine/C2DVector.h"
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#include "stlport/fable_map.h"
namespace NUISystem {
typedef std::map<unsigned long, CUIState> CStateMap;   // STLport, 12 bytes (0x0042D5B1 operator[])
struct CComponentCalls {
    virtual ~CComponentCalls() {}
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual C2DVector GetSize() const;
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual CUIState* GetCurrentState();
};
struct CCountedComponent { CComponentCalls* Data; void* Info; };
struct CChangingStateComponentCalls {
    void* VTable;
    unsigned char LeadingState[0x24-4];
    CStateMap States;
    unsigned char BeforeChildren[0xb0-0x30];
    CCountedComponent* ChildrenStart; CCountedComponent* ChildrenFinish; CCountedComponent* ChildrenEnd;
    unsigned char RemainingState[0x15c-0xbc];
    unsigned long size() const {return ChildrenFinish-ChildrenStart;}
    void Initialise(); //0052C730
};
struct CScrollingComponentInitialiseView : CChangingStateComponentCalls { void Initialise(); };
void CScrollingComponentInitialiseView::Initialise() {
    CChangingStateComponentCalls::Initialise();
    float maximum=0.0f;
    unsigned long lowest=0;
    for(unsigned long i=0;i<size();++i) {
        if(ChildrenStart[i].Data->GetCurrentState()->Position.Y>maximum) {
            maximum=ChildrenStart[i].Data->GetCurrentState()->Position.Y;
            lowest=i;
        }
    }
    States[3].Position.Y=lowest<size() ? -maximum-ChildrenStart[lowest].Data->GetSize().Y : -maximum;
}
}