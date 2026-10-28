// CAIStateGroup_CreatureUseCoverBase::CAIStateGroup_CreatureUseCoverBase() -- initialize the base, tracked cover and empty cover list.
// Leaf: ??0CAIStateGroup_CreatureUseCoverBase@@QAE@XZ
// Identity: proven; own RTTI vtable0x128D31C, PDB default constructor; EnemyCreatureBase0x8FF720; packed PCoverThing+31 and CoverList+40 (own destructor0x902F71 corroborates).
// Layout: engine/CAIStateGroupBase.h flattens the vptr and Name/priority types. Local real polymorphic CAIStateGroupBase20 is needed by the actual inherited constructor/virtual table; PBrain+4, priority+8, Name+12 and flags+16 agree with header/PDB.
// Layout: engine/CAIStateGroup_EnemyCreatureBase.h uses the wrong struct key and flattens both inheritance and PTargetThing. Actual class-key V, base20 and typed eight-byte tracked member give real base28; this permits the packed CoverBase PCoverThing+31 and CoverList+40 proved by its own destructor.
// Layout: included header uses wrong struct key/flattened vptr and opaque members; actual class base chain plus packed three flags preserves odd PCoverThing+0x1F and explicit pad+0x27 before vector.
#define CAIStateGroup_CreatureUseCoverBase CAIStateGroup_CreatureUseCoverBase_HeaderLayout
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#undef CAIStateGroup_CreatureUseCoverBase
class CThing; class CThingCreatureBase;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseIntelligentPointer(); void* PData; };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer():CBaseIntelligentPointer() {} };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0) {} };
namespace std {
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; vector():_M_start(0),_M_finish(0) { _M_end_of_storage._M_data=0; } };
}
class CAIBrain; class CCharString { public: void* PStringData; }; enum EScriptAIPriority {};
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; EScriptAIPriority ScriptAIPriority; CCharString Name; bool Terminated; unsigned char Priority; bool Enabled; };
class CAIStateGroup_EnemyCreatureBase:public CAIStateGroupBase { public: CAIStateGroup_EnemyCreatureBase(); CIntelligentPointer<CThingCreatureBase> PTargetThing; };
#pragma pack(push,1)
class CAIStateGroup_CreatureUseCoverBase:public CAIStateGroup_EnemyCreatureBase { public: bool HasDiveAnim,HasJumpOutAnims,TimingCritical; CIntelligentPointer<CThing> PCoverThing; unsigned char _pad27; std::vector<CIntelligentPointer<CThing> > CoverList; CAIStateGroup_CreatureUseCoverBase(); };
#pragma pack(pop)
CAIStateGroup_CreatureUseCoverBase::CAIStateGroup_CreatureUseCoverBase():CAIStateGroup_EnemyCreatureBase() {}