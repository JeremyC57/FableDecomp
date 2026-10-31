// 0070bf50 IsAnimationStateBaseValid
// Codex parked-50 best attempt: DIFFER(104v104); residue: 104/104 bytes; corrected original action model still differs in merged animation-state branch layout and registers.
// Leaf: ?IsAnimationStateBaseValid@?A0x
// Flags:
#define CMeshDataBank CMeshDataBankLayout
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank
#define C3DAnimationInfo C3DAnimationInfoLayout
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
#define C3DAnimationState C3DAnimationStateLayout
#include "engine/C3DAnimationState.h"
#undef C3DAnimationState
#define C3DAnimationTransitionState C3DAnimationTransitionStateLayout
#include "engine/C3DAnimationTransitionState.h"
#undef C3DAnimationTransitionState
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
#define CBankFile CBankFileLayout
#include "engine/CBankFile.h"
#undef CBankFile
#define CMeshDataBankEntry CMeshDataBankEntryLayout
#include "engine/CMeshDataBankEntry.h"
#undef CMeshDataBankEntry
// Layout evidence: RTTI rtti_bases.tsv lines38/1395/1396 proves both base chains.
// CBankFile.h proves retail base sizes0x110 and0x164; unobserved bytes retain
// the imported complete sizes. Only references to these objects are used here.
class CBaseClass{public:virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual ~CBaseClassNonCopyable();};
class CBankFile:public CBaseClassNonCopyable{public:virtual ~CBankFile();unsigned char unobservedBankFile[sizeof(CBankFileLayout)-sizeof(CBaseClassNonCopyable)];};
class CBankFileAsync:public CBankFile{public:virtual ~CBankFileAsync();unsigned char unobservedAsync[0x164-sizeof(CBankFile)];};
class C3DAnimationInfo;
class CMeshDataBank:public CBankFileAsync{public:unsigned char unobservedMeshBank[sizeof(CMeshDataBankLayout)-sizeof(CBankFileAsync)];const C3DAnimationInfo*PeekAnimation(unsigned long)const;};
class CMeshDataBankEntry{public:virtual ~CMeshDataBankEntry();CMeshDataBank*Bank;unsigned long Index;};
class C3DAnimationInfo:public CMeshDataBankEntry{public:unsigned char unobservedInfo[sizeof(C3DAnimationInfoLayout)-sizeof(CMeshDataBankEntry)];bool IsLoaded()const;};
class C3DAnimationStateBase:public CIVCountedPointeeBase{public:enum EType{};virtual EType GetType()const=0;};
class C3DAnimationState:public C3DAnimationStateBase{public:long AnimationIndex;float AnimationTime;virtual EType GetType()const;};
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase*Object;T&operator*()const{return *static_cast<T*>(Object);}};
class C3DAnimationTransitionState:public C3DAnimationStateBase{public:CIVCountedPointer<C3DAnimationStateBase>StartState,EndState;float Progress;virtual EType GetType()const;};
namespace{
bool __fastcall IsAnimationStateBaseValid(const CMeshDataBank&bank,const C3DAnimationStateBase&state){
    if(state.GetType()==0 || state.GetType()==1){
        const bool loaded=bank.PeekAnimation(static_cast<const C3DAnimationState&>(state).AnimationIndex)->IsLoaded();
        if(!loaded)return false;
    }else if(state.GetType()==2){
        const C3DAnimationTransitionState&transition=static_cast<const C3DAnimationTransitionState&>(state);
        if(!IsAnimationStateBaseValid(bank,*transition.StartState))return false;
        if(!IsAnimationStateBaseValid(bank,*transition.EndState))return false;
    }
    return true;
}
}
bool(__fastcall*implementation_entry_0070bf50)(const CMeshDataBank&,const C3DAnimationStateBase&)=IsAnimationStateBaseValid;
