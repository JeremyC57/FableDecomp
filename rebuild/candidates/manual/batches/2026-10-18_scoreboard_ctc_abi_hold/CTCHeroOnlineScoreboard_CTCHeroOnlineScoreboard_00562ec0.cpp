// CTCHeroOnlineScoreboard constructor (manifest clock sprite wrong), VT124DB34.
// No class header; PDB native map12/list4/vector12 vs donor16/12/16 gives68vs88.
#define CTCBase CTCBaseDonor
#include "engine/CTCBase.h"
#undef CTCBase
#include "fable_string.h"
#include <stdlib.h>
// Native RTTI124DB34: scoreboard -> CTCBase -> CBaseClassNonCopyable -> CBaseClass.
struct CBaseClass {CBaseClass();virtual ~CBaseClass();};
struct CBaseClassNonCopyable:CBaseClass {protected:CBaseClassNonCopyable();};
struct CPersistContext;struct CTCParamsBase;struct CEnginePrimitivePrepareInfo;namespace NTCMessages{struct CMessageBase;}
typedef long ETCInterfaceTypeABI;
struct __declspec(novtable) CTCBase:CBaseClassNonCopyable {
 CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;CTCBase(CThing&);virtual ~CTCBase();
 virtual void OnSerialise(CPersistContext&,bool); //1
 virtual void OnRemove();virtual void ConstructFromParams(const CTCParamsBase&);virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();virtual void RenderUpdate(float);virtual bool HasFrameUpdate()const;
 virtual void FrameUpdate();virtual void BulletTimeFrameUpdate();virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);virtual void EditFrameUpdate();virtual void EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);virtual void Copy(const CTCBase&);virtual void OnDie();virtual void OnKill();virtual void OnLevelUnload();virtual void OnSetInLimbo(bool);virtual bool Validate(CCharString&)const;virtual bool IsFreeForDeletion()const;
 virtual ETCInterfaceTypeABI GetInterfaceType()const=0;virtual CCharString GetClassTypeName()const=0;virtual bool IsComponentSerialisable()const;virtual bool IsDriver()const;virtual bool IsCopyable()const;virtual void GlobalUpdate(bool);virtual void HandleMessage(const NTCMessages::CMessageBase&);
 void AddToGlobalUpdateTCs(); //686ED0
};
struct CScoreScopedHttp {void* Data;CScoreScopedHttp():Data(0){}};
struct CScoreTexturePointer {void* Data;void* Info;};
// Public STLport _vector.h:66 and _alloc.h:472-481: capacity is an allocator proxy member.
struct CScoreAllocator {};
template<class T>struct CScoreAllocProxy:CScoreAllocator {T* Data;CScoreAllocProxy(const CScoreAllocator& a,T* p):CScoreAllocator(a),Data(p){}};
template<class T>struct CScoreArrayBase {T* Begin;T* End;CScoreAllocProxy<T> Capacity;CScoreArrayBase(const CScoreAllocator& a):Begin(0),End(0),Capacity(a,0){}};
template<class T>struct CScoreArray:CScoreArrayBase<T> {CScoreArray():CScoreArrayBase<T>(CScoreAllocator()){}};
template<class V>struct CScoreMap {
 struct Node {unsigned char Colour;unsigned char Pad[3];Node* Parent;Node* Left;Node* Right;int Key;V Value;};
 Node* Head;unsigned Size;unsigned char Allocator[4];
 CScoreMap():Head(0){Head=static_cast<Node*>(malloc(sizeof(Node)));Size=0;Head->Colour=0;Head->Parent=0;Head->Left=Head;Head->Right=Head;}
};
template<class T>struct CScoreList {struct Node {Node* Next;Node* Prev;T Value;};Node* Head;CScoreList():Head(0){Node* node=static_cast<Node*>(malloc(sizeof(Node)));node->Next=node;node->Prev=node;Head=node;}};
struct CScoreActivity {CCharString Name,Description,Unit;unsigned char Decimals;bool LowerIsBetter;};
struct CScoreClanInfo {CWideString Name;int ClanID;CWideString Description;CCharString URL;int NumMembers;};
struct CScoreClanMember {CWideString Name;int MemberID;CWideString Description;};
struct CScoreValue {float Score;CWideString Name;};
extern CCharString gScoreHost,gScorePath; //13B8AF0/AF4
extern bool gScoreConfigured; //13B8AE0
struct CTCHeroOnlineScoreboard:CTCBase {
 CScoreScopedHttp PHttp;CScoreMap<CScoreTexturePointer> PictureCache;CScoreMap<float> LocalScoreboard;CScoreArray<CScoreActivity> ActivityInfo;
 CCharString SessionID;CScoreList<CScoreClanInfo> MyClans;CWideString SelectedClan;CScoreList<CScoreClanMember> ClanMembers;CScoreMap<CScoreArray<CScoreValue> > DownloadedClanScoreboard;
 int ScoreboardState,TaskState;void* Screen;void* ClanMenuEntry;void* ConnectingMessage;void* ConnectingMessage1;
 CTCHeroOnlineScoreboard(CThing&);virtual ~CTCHeroOnlineScoreboard();
 virtual void OnSerialise(CPersistContext&,bool);virtual bool HasFrameUpdate()const;
 virtual ETCInterfaceTypeABI GetInterfaceType()const;virtual CCharString GetClassTypeName()const;virtual bool IsComponentSerialisable()const;virtual void GlobalUpdate(bool);void InitialiseActivityInfo(); //562710
};
#ifdef _MSC_VER
typedef char Score68[sizeof(CTCHeroOnlineScoreboard)==0x68?1:-1];
typedef char Session34[offsetof(CTCHeroOnlineScoreboard,SessionID)==0x34?1:-1];
typedef char Scoreboard50[offsetof(CTCHeroOnlineScoreboard,ScoreboardState)==0x50?1:-1];
#endif
CTCHeroOnlineScoreboard::CTCHeroOnlineScoreboard(CThing& parent):CTCBase(parent),ScoreboardState(0),TaskState(100),Screen(0),ClanMenuEntry(0),ConnectingMessage(0),ConnectingMessage1(0){
 gScoreHost="fableclangw.lionhead.com";gScorePath="/index.php";if(!gScoreConfigured)gScoreConfigured=true;AddToGlobalUpdateTCs();InitialiseActivityInfo();
}