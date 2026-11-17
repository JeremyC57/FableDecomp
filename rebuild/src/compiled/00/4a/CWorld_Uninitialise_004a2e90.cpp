// CWorld::Uninitialise() -- tear down the world: world map prepared for reload, thing manager made current then deleted, camera manager deleted, skeletal-morph resources released
// Leaf: ?Uninitialise@CWorld@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x004A2E90, 128 bytes (ends in a tail jump). PDB ?Uninitialise@CWorld@@UAEXXZ (virtual slot 10).
// Callees: CWorldMap::PrepareForLoad 0x004FF560, __fastcall SetCurrentThingManager 0x004C74F0 (stores 0x013B8A1C),
// CQuestManager::OnWorldDestruction 0x004B49E0 on the singleton 0x013B89FC, the thing manager's deleting destructor
// (slot 0, flag 1), the camera manager's deleting destructor (slot 24, +0x60), operator new 0x00BFEA1A (0x10 bytes),
// CSkeletalMorphResourceManager::CSkeletalMorphResourceManager 0x004AE2A0 (vtable 0x01238F84; manifest label
// "LTextTree_InitSentinel"), CCountedPointer<CSkeletalMorphResourceManager>::Reset(T*) 0x004A9A80 on the static
// pointer 0x013BAB10 (object) / 0x013BAB14 (count block), and the manager method 0x006C35E0 (label
// "CTCCarryable::OnKill" is a byte-twin; it releases the manager's resources).
// CWorld (engine/CWorld.h offsets agree): PWorldMap +0x14, PGameCameraManager +0x30, PThingManager +0x50 (scoped_ptrs).
// Shape: scoped_ptr::reset(0) (old read, zero stored, `delete old`); the singleton access is an inline static
// GetInstance() that creates the manager on first use, its method call becomes the tail jump.
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
class CWorldMap { public: void PrepareForLoad(); };          // 0x004FF560
class CQuestManager { public: void OnWorldDestruction(); };  // 0x004B49E0
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

class CThingManager { public: virtual ~CThingManager(); };
void __fastcall SetCurrentThingManager(CThingManager* manager);   // 0x004C74F0

class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual ~CAIGameCameraBase();                     // slot 24 (+0x60)
};

template <class T> struct scoped_ptr {
    T* ptr;
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
    T* get() const { return ptr; }
    T* operator->() const { return ptr; }
};

// NF52 (round 7): the skeletal-morph manager is engine/CSkeletalMorphResourceManager.h's CTBaseSingleton<CSkeletalMorphResourceManager>;
// its private static PClass (?PClass@?$CTBaseSingleton@VCSkeletalMorphResourceManager@@@@0V?$CCountedPointer@VCSkeletalMorphResourceManager@@@@A,
// 0x013BAB10 / count block 0x013BAB14) is reached through the public inline Get() (create on first use, Reset 0x004A9A80), and the
// method 0x006C35E0 is the header's UnloadBoneConfigs (Ego_r / FableWin ?UnloadBoneConfigs@CSkeletalMorphResourceManager@@QAEXXZ). Was a
// local CSkeletalMorphResourceManager model with the made-up static Instance / GetInstance / ReleaseResources.
#include "engine/CSkeletalMorphResourceManager.h"

class CWorld { public:
    virtual void v00();
    unsigned char             _pad04[0x10];
    scoped_ptr<CWorldMap>     PWorldMap;              // +0x14
    unsigned char             _pad18[0x18];
    scoped_ptr<CAIGameCameraBase> PGameCameraManager; // +0x30
    unsigned char             _pad34[0x1c];
    scoped_ptr<CThingManager> PThingManager;          // +0x50
    virtual void Uninitialise();
};

void CWorld::Uninitialise()
{
    PWorldMap->PrepareForLoad();
    SetCurrentThingManager(PThingManager.get());
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().OnWorldDestruction();
    PThingManager.reset(0);
    PGameCameraManager.reset(0);
    CSkeletalMorphResourceManager::Get().UnloadBoneConfigs();
}