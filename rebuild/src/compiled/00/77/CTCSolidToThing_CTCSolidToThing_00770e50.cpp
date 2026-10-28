// CTCSolidToThing::CTCSolidToThing(CThing& thing) -- initialize the component and its empty vector.
// Leaf: ??0CTCSolidToThing@@QAE@AAVCThing@@@Z
// Identity: proven; own RTTI vtable0x0126A62C, factory allocation caller and PDB CThing& constructor; NotSolidToThings vector+12, retail12bytes.
// Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
// Layout: no target header; real polymorphic CTCBase12 retains ParentThing+4, flags+8 and vector+12.
class CThing {}; template<class T> class CIntelligentPointer;
class CThing;
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CTCBase(CThing&); CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace std {
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; vector():_M_start(0),_M_finish(0) { _M_end_of_storage._M_data=0; } };
}
class CTCSolidToThing:public CTCBase { public: std::vector<CIntelligentPointer<const CThing> > NotSolidToThings; CTCSolidToThing(CThing&); };
CTCSolidToThing::CTCSolidToThing(CThing& thing):CTCBase(thing) {}