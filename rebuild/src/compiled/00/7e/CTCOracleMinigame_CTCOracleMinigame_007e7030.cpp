// CTCOracleMinigame::~CTCOracleMinigame() -- Release the definition, destroy the owned member, then destroy the component base.
// Leaf: ??1CTCOracleMinigame@@UAE@XZ
// Layout: genuine member chain from PDB; retail PDef+0xe8 and InputsPending+0xe0. Shared headers included as evidence where present but flatten real base/vptr and lifetime-bearing members; Oracle list is4bytes in retail, moving later members by8.
#define CTCOracleMinigame CTCOracleMinigame_HeaderLayout
#include "engine/CTCOracleMinigame.h"
#undef CTCOracleMinigame
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
class COracleMinigameDef;
class CTCOracleMinigame : public CTCBase {public:
    unsigned long CurrentText;long Level;EOracleMinigameState Stage,PostWaitStage;long WaitTime;float TimeBetweenOutputs;long NumOutputsFired;long Output[32];bool Success;long NextOutputFrame;CThing* Symbols[4];long SymbolsFiring[4],FramesFired[4];std::list<EOracleMinigameInput> InputsPending;CThing* SpeechMarker;CDefPointer<const COracleMinigameDef> PDef;
};
void Destroy_CTCOracleMinigame(CTCOracleMinigame* p){p->CTCOracleMinigame::~CTCOracleMinigame();}