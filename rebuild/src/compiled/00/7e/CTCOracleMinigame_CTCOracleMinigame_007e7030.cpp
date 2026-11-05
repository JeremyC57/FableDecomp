// CTCOracleMinigame::~CTCOracleMinigame() -- Release the definition, destroy the owned member, then destroy the component base.
// Leaf: ??1CTCOracleMinigame@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CEnginePrimitiveThemeChannelDesc {public:long ID;};
enum EOracleMinigameState {};enum EOracleMinigameInput {};
namespace std {template<class T>class allocator{};template<class T,class A=allocator<T> >class list {public:void* _M_node;~list();};}
class COracleMinigameDef;
class CTCOracleMinigame : public CTCBase {public:
    unsigned long CurrentText;long Level;EOracleMinigameState Stage,PostWaitStage;long WaitTime;float TimeBetweenOutputs;long NumOutputsFired;long Output[32];bool Success;long NextOutputFrame;CThing* Symbols[4];long SymbolsFiring[4],FramesFired[4];std::list<EOracleMinigameInput> InputsPending;CThing* SpeechMarker;CDefPointer<const COracleMinigameDef> PDef;
};
void Destroy_CTCOracleMinigame(CTCOracleMinigame* p){p->CTCOracleMinigame::~CTCOracleMinigame();}