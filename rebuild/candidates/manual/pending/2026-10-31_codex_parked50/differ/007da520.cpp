// 007da520 CTCTavernGameCardBlackjack::GetRequiredScore
// Codex parked-50 best attempt: DIFFER(286v286); residue: 286/286 bytes; frame 0x14 versus retail 0x10 and dependent offsets; input-reading class-return/destructor contexts did not share the required word.
// Flags:
// Leaf: ?GetRequiredScore@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ
#include <stddef.h>
#include "engine/CBaseClassNonCopyable.h"
#define CTCTavernGame CTCTavernGameLayout
#include "engine/CTCTavernGame.h"
#undef CTCTavernGame
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
class CWideStringData;class CCharStringData;
class CWideString{CWideStringData*PStringData;public:~CWideString();};
class CCharString{CCharStringData*PStringData;public:~CCharString();CWideString ToWideString()const;static CCharString __fastcall FormatDouble(long,double);};
CWideString __fastcall operator+(const CWideString&,const CWideString&);
CWideString __fastcall operator+(const CWideString&,const unsigned short*);
extern "C" void __cdecl free(void*);
// Fidelity: whole-object input-reading analysis stand-ins for temporary destructors.
#pragma auto_inline(off)
CWideString::~CWideString(){if(PStringData)free(PStringData);}
CCharString::~CCharString(){if(PStringData)free(PStringData);}
#pragma auto_inline(on)
class CThing;class CTCParamsBase;
class C2DVector;class CRightHandedSet;class CWorld;enum ETCInterfaceType{};
class CPersistContext;class CEnginePrimitivePrepareInfo;namespace NTCMessages{class CMessageBase;}
class CThingManager;
class CTCBase:public CBaseClassNonCopyable{
protected:virtual void OnSerialise(CPersistContext&,bool);CWorld&GetWorld()const;CThingManager&GetThingManager();
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();
    virtual void RenderUpdate(float);virtual bool HasFrameUpdate()const;virtual void FrameUpdate();virtual void BulletTimeFrameUpdate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);virtual void OnDie();virtual void OnKill();virtual void OnLevelUnload();virtual void OnSetInLimbo(bool);
    virtual bool Validate(CCharString&)const;virtual bool IsFreeForDeletion()const;virtual bool IsSerialiseCreatable()const;virtual bool IsSerialisableForHeroState()const;
    virtual ETCInterfaceType GetInterfaceType()const=0;virtual CCharString GetClassTypeName()const=0;
    virtual bool IsComponentSerialisable()const;virtual bool IsDriver()const;virtual bool IsCopyable()const;virtual void GlobalUpdate(bool);
    virtual void HandleMessage(const NTCMessages::CMessageBase&);
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;unsigned char unobservedAlignment[2];
};


class CTavernGameDef:public CSubDefClassBase{
 unsigned char unreadDef[0xbc-sizeof(CSubDefClassBase)];
public:unsigned long RequiredScore;
};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};
}
class CInputEvent;class CProcessedInput;class CInputProcessTavernGame;
class CTCTavernGame:public CTCBase{
 unsigned char unreadGame[sizeof(CTCTavernGameLayout)-sizeof(CTCBase)];
public:virtual void ProcessInput(const CInputEvent&,CProcessedInput&,CInputProcessTavernGame*);
 virtual void EnterGame();virtual void LeaveGame();virtual void NewGame();virtual void NewRound();virtual void EndRound();virtual void QuitMidGame();
 virtual void Greeting();virtual void Options();virtual void Instructions();virtual void Betting();virtual void ReactionAndPayout();virtual bool UpdateValues();virtual void Farewell();
 virtual CWideString GetWinPhrase();virtual CWideString GetBestScore();virtual CWideString GetCurrentScore();virtual CWideString GetRequiredScore();
 void GetDef(CDefPointer<const CTavernGameDef>*);CWideString GetText(unsigned long);
};
class CTCTavernGameCardBase:public CTCTavernGame{unsigned char unreadCards[0x318-sizeof(CTCTavernGame)];};
class CCard;
// Layout: native cards vector318, players324, bets330, current33c; original TPI nested-vector types.
class CTCTavernGameCardBlackjack:public CTCTavernGameCardBase{
 std::vector<CCard*>DealersCards;std::vector<std::vector<CCard*> >PlayersCards;std::vector<long>Bets;
 std::vector<CCard*>*CurrentPlayerCards;
public:long GetHandValue(std::vector<CCard*>*,bool);virtual CWideString GetRequiredScore();
};
CWideString CTCTavernGameCardBlackjack::GetRequiredScore(){
 CDefPointer<const CTavernGameDef>def(NULL_DEF);GetDef(&def);
 if(CurrentPlayerCards)return GetText(def->RequiredScore)+CCharString::FormatDouble(0,GetHandValue(&DealersCards,false)).ToWideString();
 return GetText(def->RequiredScore)+L"0";
}
