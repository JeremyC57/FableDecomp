// CCreatureActionBase::AddDependentThing(CThing& thing) -- append a tracked pointer to the thing.
// Leaf: ?AddDependentThing@CCreatureActionBase@@QAEXAAVCThing@@@Z
// Identity: proven; own CActionEventTriggerParticle::Dispatch caller; PDB AddDependentThing(CThing&), actual DependentThings donor164->retail156 due two earlier containers; own vector<CIntelligentPointer<CThing>>::push_back0x4AD520.
// Layout: CreatureActionBase shared data header reused unchanged under alias, typed view over named opaque vector. Other owners have no usable header and use minimal member views at proved offsets. Real tracked-pointer constructor/base destructor calls remain external.
#define CCreatureActionBase CCreatureActionBase_HeaderLayout
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
class CThing {}; class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public: CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer(); CBaseObjectPointer* PData; CBaseObject* GetPItem() const; void SetPItem(CBaseObject*); };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer(T* item):CBaseIntelligentPointer() { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); } T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } };
namespace std { template<class T> class allocator {}; template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; }; template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; void push_back(const T&); }; }
class CCreatureActionBase:public CCreatureActionBase_HeaderLayout { public: void AddDependentThing(CThing& thing); };
void CCreatureActionBase::AddDependentThing(CThing& thing) { reinterpret_cast<std::vector<CIntelligentPointer<CThing> >*>(&DependentThings)->push_back(CIntelligentPointer<CThing>(&thing)); }