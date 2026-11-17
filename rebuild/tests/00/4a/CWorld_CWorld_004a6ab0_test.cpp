#include <stdio.h>
#include <stdlib.h>
#include <new>
// BEGIN exact source snapshot SHA256 5dcd52f653824c832fb3657a8f9d889e9f635ef8f112e0e9e557cd66d8abe48a
// Global CWorld::~CWorld, retail 0x004A6AB0 (886 bytes).
// Original identity: egor 3759 / debug 0x004B0430. The manifest's
// HeroInformationScreens::CBase label is disproved by the two CWorld RTTI
// stores and constructor evidence. Import the shared PDB donor under an alias;
// this local class uses the evidenced smaller retail container layout.
#define CWorld CWorldPdb
#include "engine/CWorld.h"
#undef CWorld
#include <stddef.h>

// Shared donors describe complete debug objects; only CWorld is the reconstructed
// owner here. Other local types below are method views for external cleanup.
// Retail SearchTools is24B (three pointers +12Bvector), debug28B.
// Camera remains4B with one vfptr and destructor atslot24. UI bank offsets
// differ(debug20/28 versusretail10/18); this caller uses only external methods.
#define CWorldMap CWorldMapPdbReview
#define CEnvironment CEnvironmentPdbReview
#define CThingSearchTools CThingSearchToolsPdbReview
#define CAIGameCameraBase CAIGameCameraBasePdbReview
#define CGameScriptInterface CGameScriptInterfacePdbReview
#define CMeshDataBank CMeshDataBankPdbReview
#define CNavigatorManager CNavigatorManagerPdbReview
#define CBulletTimeManager CBulletTimeManagerPdbReview
#define CManager CManagerPdbReview
#define CMeshBankManager CMeshBankManagerPdbReview
#include "engine/CWorldMap.h"
#include "engine/CEnvironment.h"
#include "engine/CThingSearchTools.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CMeshDataBank.h"
#include "engine/CNavigatorManager.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CManager.h"
#include "engine/CMeshBankManager.h"
#undef CWorldMap
#undef CEnvironment
#undef CThingSearchTools
#undef CAIGameCameraBase
#undef CGameScriptInterface
#undef CMeshDataBank
#undef CNavigatorManager
#undef CBulletTimeManager
#undef CManager
#undef CMeshBankManager


extern "C" void free(void*);
void operator delete(void*);
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall *DeleteFunc)(void*);
    void* Data;
};
class CBaseClass { public: CBaseClass(); virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();
};
class CProceduralTexture; class CIDrawEnvironment; class CIDrawWorldMap;
class CIDrawWorld { public:
    virtual const CProceduralTexture* DrawGetProceduralTexture() const;
    virtual const CIDrawEnvironment* DrawGetEnvironment() const;
    virtual const CIDrawWorldMap* DrawGetWorldMap() const;
    virtual ~CIDrawWorld() { }
};
class CCharStringData;
class CWideStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();
    ~CCharString(); // 0x0099EAE0
};
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();
    ~CWideString(); // 0x0099B510
};
struct CAllocator { };

template<class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) {
        if(Info) ++Info->RefCount;
    }
    ~CCountedPointer() {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data=0; Info=0;
    }
    void Reset() {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Info=0; Data=0;
    }
};
template<class T> struct CScopedPtr {
    T* Ptr;
    CScopedPtr() : Ptr(0) { }
    ~CScopedPtr() { if (Ptr) delete Ptr; }
    void reset(T* p) { T* old=Ptr; Ptr=p; if(old) delete old; }
};
template<class T> struct CVector {
    T* Start; T* Finish; T* Limit;
    CVector() : Start(0), Finish(0), Limit(0) { }
    ~CVector() {
        T* const last=Finish;
        T* p=Start;
        while(p!=last) { p->~T(); ++p; }
        if(Start) free(Start);
    }
};
struct CCharStringArray {
    CCharString* Start; CCharString* Finish; CCharString* Limit;
    CCharStringArray() : Start(0), Finish(0), Limit(0) { }
    ~CCharStringArray() {
        CCharString* const last=Finish; CCharString* p=Start;
        while(p!=last) { p->~CCharString(); ++p; }
        if(Start) free(Start);
    }
};
class CTestQuest { public:
    CCharString QuestName;
    CCharString HSPName;
    long ColourIndex;
    CCharString DisplayName;
    CCharString IniFile;
    CCharString OutiFile;
    CCharString QuestCardDefName;
};
void __fastcall DestroyCTestQuestRange(CTestQuest*,CTestQuest*,CAllocator&);
struct CTestQuestVector {
    CTestQuest* Start; CTestQuest* Finish; CTestQuest* Limit;
    CTestQuestVector() : Start(0), Finish(0), Limit(0) { }
    ~CTestQuestVector() {
        CAllocator allocator;
        DestroyCTestQuestRange(Start,Finish,allocator); // std::_Destroy_range at 0x004ABD90
        if(Start) free(Start);
    }
};
class CAtmosCopyInfo { public:
    CWideString DVDFileName, HDFileName;
    long EntryID;
};
struct CAtmosCopyNode { CAtmosCopyNode* Next; CAtmosCopyNode* Prev; CAtmosCopyInfo Info; };
struct CAtmosCopyList {
    CAtmosCopyNode* Head;
    CAtmosCopyList() : Head(0) { }
    void clear(); // std::list<CAtmosCopyInfo>::clear at 0x004AC270
    ~CAtmosCopyList() {
        clear();
        if(Head) free(Head);
    }
};
class CThing;
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer(); void* PData; };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: virtual ~CIntelligentPointer() { } };
class CEnginePrimitiveHandle { public: virtual ~CEnginePrimitiveHandle(); void* PData; };

class CWorldMap : public CBaseClass { public: virtual ~CWorldMap(); };
class CEnvironment : public CBaseClass { public: virtual ~CEnvironment(); };
class CGameTimeManager { public: };
class CThingSearchTools { public:
    void* WorldMap;
    void* ThingMapManager;
    void* ThingManager;
    CVector<void*> AreaMaps;                  // +0c
    ~CThingSearchTools() { }
};
class CAtmosProcess : public CBaseClass { public: virtual ~CAtmosProcess(); };
class CAIGameCameraBase { public:
    virtual void CameraSlot01(); virtual void CameraSlot02(); virtual void CameraSlot03();
    virtual void CameraSlot04(); virtual void CameraSlot05(); virtual void CameraSlot06();
    virtual void CameraSlot07(); virtual void CameraSlot08(); virtual void CameraSlot09();
    virtual void CameraSlot10(); virtual void CameraSlot11(); virtual void CameraSlot12();
    virtual void CameraSlot13(); virtual void CameraSlot14(); virtual void CameraSlot15();
    virtual void CameraSlot16(); virtual void CameraSlot17(); virtual void CameraSlot18();
    virtual void CameraSlot19(); virtual void CameraSlot20(); virtual void CameraSlot21();
    virtual void CameraSlot22(); virtual void CameraSlot23(); virtual void CameraSlot24();
    virtual ~CAIGameCameraBase();
};
class CGameScriptInterface : public CBaseClass { public: virtual ~CGameScriptInterface(); };
class CMeshDataBank : public CBaseClass { public: virtual ~CMeshDataBank(); };
class C3DAnimationManager;
class CNavigatorManager : public CBaseClass { public: virtual ~CNavigatorManager(); };
class CCombatManager : public CBaseClass { public: virtual ~CCombatManager(); };
class CThingManager : public CBaseClass { public: virtual ~CThingManager(); };
class CFaction;
struct CCountedData { CFaction* Data; CCPPointerInfo* Info; };
struct CFactionPair { unsigned long Key; CCountedData Value; };
struct CFactionVectorMap {
    CFactionPair* Start; CFactionPair* Finish; CFactionPair* Limit;
    unsigned char Compare; unsigned char Dirty; unsigned char _pad0e[2];
};
class CFactionManager { public: CFactionVectorMap Factions; };
class CScriptInfoManager : public CBaseClass { public: virtual ~CScriptInfoManager(); };
class CMessageEventManager : public CBaseClass { public: virtual ~CMessageEventManager(); };
class CBulletTimeManager : public CBaseClass { public: virtual ~CBulletTimeManager(); };
class CMusicManager : public CBaseClass { public: virtual ~CMusicManager(); };
class COpinionReactionManager : public CBaseClass { public: virtual ~COpinionReactionManager(); };
class CScriptConversationManager : public CBaseClass { public: virtual ~CScriptConversationManager(); };
class CPlayerManager; class CGameDefinitionManager; class CMainGameComponent;
class CGraphicDataBank;

namespace NUISystem {
class CManager { public:
    static CManager* __fastcall GetInstance(); // 0x0041E5F2
    void SetMeshBank(CCountedPointer<CMeshDataBank>); // 0x0041D198, by value
};
}
class CMeshBankManager { public: void FlushUnusedBanks(); }; // 0x00A09E70
class CSystemManager { public: unsigned char _pad00[0x74]; CCountedPointer<CMeshBankManager> PMeshBankManager; };
CSystemManager* __fastcall GFGetSystemManager(); // 0x009A4EC0
class C3DAnimationManager { public: virtual ~C3DAnimationManager(); };

void __fastcall DestroyFactionRange(CFactionPair*,CFactionPair*,CAllocator&);
void __fastcall CheckedDeleteGameTimeManager(CGameTimeManager*); // 0x004ADD30
void __fastcall CheckedDeleteBulletTimeManager(CBulletTimeManager*); // 0x004AE080
void __fastcall CheckedDeleteScriptConversationManager(CScriptConversationManager*); // 0x004AE810

template<> struct CScopedPtr<CGameTimeManager> {
    CGameTimeManager* Ptr;
    CScopedPtr() : Ptr(0) { }
    ~CScopedPtr() { CheckedDeleteGameTimeManager(Ptr); }
};
template<> struct CScopedPtr<CBulletTimeManager> {
    CBulletTimeManager* Ptr;
    CScopedPtr() : Ptr(0) { }
    ~CScopedPtr() { CheckedDeleteBulletTimeManager(Ptr); }
};
template<> struct CScopedPtr<CScriptConversationManager> {
    CScriptConversationManager* Ptr;
    CScopedPtr() : Ptr(0) { }
    ~CScopedPtr() { CheckedDeleteScriptConversationManager(Ptr); }
};
template<> struct CScopedPtr<CFactionManager> {
    CFactionManager* Ptr;
    CScopedPtr() : Ptr(0) { }
    ~CScopedPtr() {
        if (Ptr) {
            CFactionManager* object=Ptr;
            CAllocator allocator;
            DestroyFactionRange(object->Factions.Start,object->Factions.Finish,allocator);
            if (object->Factions.Start) {
                free(object->Factions.Start);
            }
            operator delete(object);
        }
    }
};
template<> struct CScopedPtr<CThingSearchTools> {
    CThingSearchTools* Ptr;
    CScopedPtr() : Ptr(0) { }
    ~CScopedPtr() {
        if (Ptr) {
            CThingSearchTools* object=Ptr;
            if (object->AreaMaps.Start) free(object->AreaMaps.Start);
            operator delete(object);
        }
    }
};

#pragma pack(push,1)
class CWorld : public CBaseClassNonCopyable, public CIDrawWorld { public:
    CMainGameComponent* Component; CPlayerManager* PlayerManager; CGameDefinitionManager* DefinitionManager;
    CScopedPtr<CWorldMap> PWorldMap;
    CScopedPtr<CEnvironment> PEnvironment;
    CScopedPtr<CGameTimeManager> PGameTimeManager;
    CScopedPtr<CThingSearchTools> PThingSearchTools;
    CCountedPointer<CAtmosProcess> PAtmosProcessor;
    CScopedPtr<CAIGameCameraBase> PGameCamera;
    CScopedPtr<CAIGameCameraBase> PGameCameraManager;
    CAIGameCameraBase* PCurrentGameCamera;
    CScopedPtr<CGameScriptInterface> PGameScriptInterface;
    CCountedPointer<CMeshDataBank> PMainMeshBank;
    C3DAnimationManager* PAnimationManager;
    CScopedPtr<CNavigatorManager> PNavigationManager;
    CScopedPtr<CCombatManager> PThingCombatManager;
    CScopedPtr<CThingManager> PThingManager;
    CScopedPtr<CFactionManager> PFactionManager;
    CCountedPointer<CScriptInfoManager> PScriptInfoManager;
    CCountedPointer<CMessageEventManager> PMessageEventManager;
    CScopedPtr<CBulletTimeManager> PBulletTimeManager;
    CCountedPointer<CMusicManager> PMusicManager;
    CCountedPointer<COpinionReactionManager> POpinionReactionManager;
    CScopedPtr<CScriptConversationManager> PScriptConversationManager;
    bool JustLoaded; unsigned char _pad81[3]; CCharString CurrentWorldName;
    long ConsolePauseAtFrameNumber, FrameStarted3DRendering; double LastUpdateTimeLength, LastUpdateTime;
    long CountdownTimer; bool Paused,SlowMotion,ShowDebugText,ShowFPSText,ShowProfileText; unsigned char _padA9[3];
    CCharStringArray InitialActiveQuests;
    CCharStringArray RegisteredQuests;
    CTestQuestVector ActiveTestQuests;
    unsigned char _padD0[0x2c];
    CWideString SaveGamePathName;
    unsigned char _pad100[0x18];
    CVector<CIntelligentPointer<CThing> > RegionLoadFollowers;
    unsigned char _pad124[0x18];
    CWideString MostRecentManualSaveName;
    unsigned char _pad140[0x28];
    CVector<CEnginePrimitiveHandle> WeatherMaskingPrimitiveHandles;
    CAtmosCopyList AtmosBanksWaitingToCopy;
    unsigned char _pad178[0x20];
    bool HasInitialisedStartTime; unsigned char _tail[7];
    virtual ~CWorld();
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(offsetof(CWorld,PWorldMap)==0x14);
FABLE_STATIC_ASSERT(offsetof(CWorld,CurrentWorldName)==0x84);
FABLE_STATIC_ASSERT(offsetof(CWorld,InitialActiveQuests)==0xac);
FABLE_STATIC_ASSERT(offsetof(CWorld,RegisteredQuests)==0xb8);
FABLE_STATIC_ASSERT(offsetof(CWorld,ActiveTestQuests)==0xc4);
FABLE_STATIC_ASSERT(offsetof(CWorld,SaveGamePathName)==0xfc);
FABLE_STATIC_ASSERT(offsetof(CWorld,RegionLoadFollowers)==0x118);
FABLE_STATIC_ASSERT(offsetof(CWorld,MostRecentManualSaveName)==0x13c);
FABLE_STATIC_ASSERT(offsetof(CWorld,WeatherMaskingPrimitiveHandles)==0x168);
FABLE_STATIC_ASSERT(offsetof(CWorld,AtmosBanksWaitingToCopy)==0x174);

CWorld::~CWorld()
{
    PWorldMap.reset(0);
    PCurrentGameCamera=0;
    PAnimationManager=0;
    NUISystem::CManager::GetInstance()->SetMeshBank(CCountedPointer<CMeshDataBank>());
    PMainMeshBank.Reset();
    GFGetSystemManager()->PMeshBankManager.Data->FlushUnusedBanks();
}

// END exact source snapshot
static int g_charDtors,g_wideDtors,g_handleDtors,g_followerDtors;
static int g_questCount,g_factionCount,g_timeCalls,g_bulletCalls,g_conversationCalls;
static int g_order;
static int g_resetAt;
static int g_bankAt;
static int g_flushAt;
static int g_emptyBank;
static int g_worldMapDtorAt;
static int g_meshDtorCount;
static int g_meshDeleteCount;
static int g_meshDeleteAt[2];
static int g_operatorDeleteCount;
static int g_unownedPointersWereCleared;
static CWorld* g_world;
static CSystemManager g_system;
static CMeshBankManager g_meshBanks;
static NUISystem::CManager g_ui;

void* operator new(unsigned int n) { return malloc(n); }
void operator delete(void* p) { if (p) { ++g_operatorDeleteCount; free(p); } }
void* operator new[](unsigned int n) { return malloc(n); }
void operator delete[](void* p) { if (p) free(p); }

CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
const CProceduralTexture* CIDrawWorld::DrawGetProceduralTexture() const { return 0; }
const CIDrawEnvironment* CIDrawWorld::DrawGetEnvironment() const { return 0; }
const CIDrawWorldMap* CIDrawWorld::DrawGetWorldMap() const { return 0; }
void CAIGameCameraBase::CameraSlot01() { }
void CAIGameCameraBase::CameraSlot02() { }
void CAIGameCameraBase::CameraSlot03() { }
void CAIGameCameraBase::CameraSlot04() { }
void CAIGameCameraBase::CameraSlot05() { }
void CAIGameCameraBase::CameraSlot06() { }
void CAIGameCameraBase::CameraSlot07() { }
void CAIGameCameraBase::CameraSlot08() { }
void CAIGameCameraBase::CameraSlot09() { }
void CAIGameCameraBase::CameraSlot10() { }
void CAIGameCameraBase::CameraSlot11() { }
void CAIGameCameraBase::CameraSlot12() { }
void CAIGameCameraBase::CameraSlot13() { }
void CAIGameCameraBase::CameraSlot14() { }
void CAIGameCameraBase::CameraSlot15() { }
void CAIGameCameraBase::CameraSlot16() { }
void CAIGameCameraBase::CameraSlot17() { }
void CAIGameCameraBase::CameraSlot18() { }
void CAIGameCameraBase::CameraSlot19() { }
void CAIGameCameraBase::CameraSlot20() { }
void CAIGameCameraBase::CameraSlot21() { }
void CAIGameCameraBase::CameraSlot22() { }
void CAIGameCameraBase::CameraSlot23() { }
void CAIGameCameraBase::CameraSlot24() { }
CAIGameCameraBase::~CAIGameCameraBase() { }
CWorldMap::~CWorldMap() { }
CEnvironment::~CEnvironment() { }
CAtmosProcess::~CAtmosProcess() { }
CGameScriptInterface::~CGameScriptInterface() { }
CMeshDataBank::~CMeshDataBank() { ++g_meshDtorCount; }
CNavigatorManager::~CNavigatorManager() { }
CCombatManager::~CCombatManager() { }
CThingManager::~CThingManager() { }
CScriptInfoManager::~CScriptInfoManager() { }
CMessageEventManager::~CMessageEventManager() { }
CBulletTimeManager::~CBulletTimeManager() { }
CMusicManager::~CMusicManager() { }
COpinionReactionManager::~COpinionReactionManager() { }
CScriptConversationManager::~CScriptConversationManager() { }
C3DAnimationManager::~C3DAnimationManager() { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_followerDtors; }
CEnginePrimitiveHandle::~CEnginePrimitiveHandle() { ++g_handleDtors; }
CCharString::CCharString() : PStringData(0) { }
CCharString::~CCharString() { ++g_charDtors; }
CWideString::CWideString() : PStringData(0) { }
CWideString::~CWideString() { ++g_wideDtors; }
void __fastcall DestroyCTestQuestRange(CTestQuest* first,CTestQuest* last,CAllocator&)
{ while(first!=last) { ++g_questCount; first->~CTestQuest(); ++first; } }
void CAtmosCopyList::clear()
{
    CAtmosCopyNode* head=Head;
    if(!head) return;
    CAtmosCopyNode* node=head->Next;
    while(node!=head) {
        CAtmosCopyNode* next=node->Next;
        node->Info.HDFileName.~CWideString();
        node->Info.DVDFileName.~CWideString();
        free(node);
        node=next;
    }
    head->Next=head;
    head->Prev=head;
}
void __fastcall DestroyFactionRange(CFactionPair* first,CFactionPair* last,CAllocator&) { g_factionCount+=(int)(last-first); }
void __fastcall CheckedDeleteGameTimeManager(CGameTimeManager* p) { ++g_timeCalls; delete p; }
void __fastcall CheckedDeleteBulletTimeManager(CBulletTimeManager* p) { ++g_bulletCalls; delete p; }
void __fastcall CheckedDeleteScriptConversationManager(CScriptConversationManager* p) { ++g_conversationCalls; delete p; }

struct TestWorldMap : CWorldMap {
    virtual ~TestWorldMap()
    {
        g_worldMapDtorAt = ++g_order;
        if (g_world && g_world->PWorldMap.Ptr != 0) g_worldMapDtorAt = -1;
    }
};

static void __fastcall destroyMeshBank(void* object)
{
    ++g_meshDeleteCount;
    g_meshDeleteAt[g_meshDeleteCount - 1] = ++g_order;
    delete (CMeshDataBank*)object;
}
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_ui; }
void NUISystem::CManager::SetMeshBank(CCountedPointer<CMeshDataBank> p)
{
    if (p.Data || p.Info) return;
    g_emptyBank++;
    g_bankAt = ++g_order;
    if (g_world) {
        g_unownedPointersWereCleared =
            g_world->PCurrentGameCamera == 0 && g_world->PAnimationManager == 0;
        g_world->PCurrentGameCamera = (CAIGameCameraBase*)1;
        g_world->PAnimationManager = (C3DAnimationManager*)1;
    }
}
void CMeshBankManager::FlushUnusedBanks()
{
    g_flushAt = ++g_order;
    // Exercise the automatic member-dtor null check after this callback.
    CMeshDataBank* bank = new CMeshDataBank;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 1;
    info->DeleteFunc = destroyMeshBank;
    info->Data = bank;
    g_world->PMainMeshBank.Data = bank;
    g_world->PMainMeshBank.Info = info;
}
CSystemManager* __fastcall GFGetSystemManager()
{ g_resetAt = ++g_order; return &g_system; }

int main()
{
    {
        CWorld world;
        g_world = &world;
        g_system.PMeshBankManager.Data = &g_meshBanks;
        world.PCurrentGameCamera = (CAIGameCameraBase*)1;
        world.PAnimationManager = (C3DAnimationManager*)1;
        world.PWorldMap.Ptr = new TestWorldMap;
        // Nonempty ranges verify the actual assigned destructor's element calls.
        // Dependency destructors above are spies; they do not claim full engine lifetimes.
        world.InitialActiveQuests.Start=(CCharString*)malloc(2*sizeof(CCharString));
        new(world.InitialActiveQuests.Start)CCharString;
        new(world.InitialActiveQuests.Start+1)CCharString;
        world.InitialActiveQuests.Finish=world.InitialActiveQuests.Limit=world.InitialActiveQuests.Start+2;
        world.RegisteredQuests.Start=(CCharString*)malloc(sizeof(CCharString));
        new(world.RegisteredQuests.Start)CCharString;
        world.RegisteredQuests.Finish=world.RegisteredQuests.Limit=world.RegisteredQuests.Start+1;
        world.ActiveTestQuests.Start=(CTestQuest*)malloc(sizeof(CTestQuest));
        new(world.ActiveTestQuests.Start)CTestQuest;
        world.ActiveTestQuests.Finish=world.ActiveTestQuests.Limit=world.ActiveTestQuests.Start+1;
        world.WeatherMaskingPrimitiveHandles.Start=(CEnginePrimitiveHandle*)malloc(2*sizeof(CEnginePrimitiveHandle));
        new(world.WeatherMaskingPrimitiveHandles.Start)CEnginePrimitiveHandle;
        new(world.WeatherMaskingPrimitiveHandles.Start+1)CEnginePrimitiveHandle;
        world.WeatherMaskingPrimitiveHandles.Finish=world.WeatherMaskingPrimitiveHandles.Limit=world.WeatherMaskingPrimitiveHandles.Start+2;
        typedef CIntelligentPointer<CThing> Follower;
        world.RegionLoadFollowers.Start=(Follower*)malloc(2*sizeof(Follower));
        new(world.RegionLoadFollowers.Start)Follower;
        new(world.RegionLoadFollowers.Start+1)Follower;
        world.RegionLoadFollowers.Finish=world.RegionLoadFollowers.Limit=world.RegionLoadFollowers.Start+2;
        CAtmosCopyNode* head=(CAtmosCopyNode*)malloc(sizeof(CAtmosCopyNode));
        CAtmosCopyNode* node=(CAtmosCopyNode*)malloc(sizeof(CAtmosCopyNode));
        new(&node->Info.DVDFileName)CWideString;
        new(&node->Info.HDFileName)CWideString;
        head->Next=head->Prev=node;node->Next=node->Prev=head;
        world.AtmosBanksWaitingToCopy.Head=head; // Sentinel's payload is intentionally unconstructed.
        world.PFactionManager.Ptr=new CFactionManager;
        CFactionVectorMap& factions=world.PFactionManager.Ptr->Factions;
        factions.Start=(CFactionPair*)malloc(2*sizeof(CFactionPair));
        factions.Finish=factions.Limit=factions.Start+2;
        factions.Start[0].Value.Info=factions.Start[1].Value.Info=0;

        CMeshDataBank* initialBank = new CMeshDataBank;
        CCPPointerInfo* initialInfo = new CCPPointerInfo;
        initialInfo->RefCount = 1;
        initialInfo->DeleteFunc = destroyMeshBank;
        initialInfo->Data = initialBank;
        world.PMainMeshBank.Data = initialBank;
        world.PMainMeshBank.Info = initialInfo;
    }
    if(g_charDtors!=10 || g_wideDtors!=4 || g_handleDtors!=2 || g_followerDtors!=2 ||
       g_questCount!=1 || g_factionCount!=2 || g_timeCalls!=1 || g_bulletCalls!=1 || g_conversationCalls!=1) return 11;
    if (g_emptyBank != 1 || !g_resetAt || !g_bankAt || !g_flushAt) return 1;
    if (g_bankAt >= g_meshDeleteAt[0] || g_meshDeleteAt[0] >= g_resetAt ||
        g_resetAt >= g_flushAt) return 2;
    if (!g_worldMapDtorAt || g_worldMapDtorAt >= g_bankAt) return 3;
    if (g_meshDeleteCount != 2 || g_meshDtorCount != 2) return 4;
    if (!(g_meshDeleteAt[0] < g_flushAt && g_flushAt < g_meshDeleteAt[1])) return 5;
    if (!g_operatorDeleteCount) return 6;
    if (!g_unownedPointersWereCleared) return 7;
    g_meshDeleteCount = 0;
    g_meshDtorCount = 0;
    g_operatorDeleteCount = 0;
    {
        CCountedPointer<CMeshDataBank> owner;
        owner.Data = new CMeshDataBank;
        owner.Info = new CCPPointerInfo;
        owner.Info->RefCount = 1;
        owner.Info->DeleteFunc = destroyMeshBank;
        owner.Info->Data = owner.Data;
        {
            CCountedPointer<CMeshDataBank> shared(owner);
            if (owner.Info->RefCount != 2 || shared.Data != owner.Data) return 8;
        }
        if (owner.Info->RefCount != 1) return 9;
        owner.Reset();
    }
    if (g_meshDeleteCount != 1 || g_meshDtorCount != 1 || g_operatorDeleteCount != 2) return 10;
    puts("004a6ab0_TEST PASS");
    return 0;
}