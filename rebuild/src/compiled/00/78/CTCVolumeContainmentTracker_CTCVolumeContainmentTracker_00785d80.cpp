// CTCVolumeContainmentTracker::~CTCVolumeContainmentTracker() -- Release the definition, destroy the owned member, then destroy the component base.
// Leaf: ??1CTCVolumeContainmentTracker@@UAE@XZ
// Layout: genuine member chain from PDB; retail PDef+0x1c and PThingInside+0xc. Shared headers included as evidence where present but flatten real base/vptr and lifetime-bearing members; Oracle list is4bytes in retail, moving later members by8.
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CThing;class CBaseObjectPointer;
class CTCBase {public:virtual ~CTCBase();CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;};
// Layout: pointee count+4 and virtual Delete slot1 are confirmed by each assigned destructor. Generated pointee headers flatten the vptr, so actual polymorphism is represented here.
class CIVCountedPointeeBase {public:virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable long IVRefCount;};
template<class T> class CDefPointer {public:T* Object;__forceinline ~CDefPointer(){if(Object){const CIVCountedPointeeBase* base=(const CIVCountedPointeeBase*)Object;if(--base->IVRefCount==0)base->Delete();Object=0;}}};
class CBaseIntelligentPointer {public:virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
class C3DVector {public:float X,Y,Z;};
class CEnginePrimitiveThemeChannelDesc {public:long ID;};
enum EOracleMinigameState {};enum EOracleMinigameInput {};
namespace std {template<class T>class allocator{};template<class T,class A=allocator<T> >class list {public:void* _M_node;~list();};}
class CTCVolumeContainmentTrackerDef;
class CTCVolumeContainmentTracker : public CTCBase {public:
    CIntelligentPointer<CThing> PThingInside;CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;bool PreviouslyUpdated;CDefPointer<const CTCVolumeContainmentTrackerDef> PDef;bool UpdateLightingChannels;
};
void Destroy_CTCVolumeContainmentTracker(CTCVolumeContainmentTracker* p){p->CTCVolumeContainmentTracker::~CTCVolumeContainmentTracker();}