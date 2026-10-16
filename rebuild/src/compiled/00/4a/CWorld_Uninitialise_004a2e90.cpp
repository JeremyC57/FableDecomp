// CWorld::Uninitialise() -- tear down the world: world map prepared for reload, thing manager made current then deleted, camera manager deleted, skeletal-morph resources released
// Leaf: ?Uninitialise@CWorld@@UAEXXZ
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
struct CWorldMap { void PrepareForLoad(); };          // 0x004FF560
struct CQuestManager { void OnWorldDestruction(); };  // 0x004B49E0
extern CQuestManager* g_pQuestManager;                // 0x013B89FC

struct CThingManager { virtual ~CThingManager(); };
void __fastcall SetCurrentThingManager(CThingManager* manager);   // 0x004C74F0

struct CAIGameCameraBase {
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

struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;                             // +0
    CCPPointerInfo* Info;                             // +4
    void Reset(T* object);                            // 0x004A9A80
};

class CSkeletalMorphResourceManager {                 // 16 bytes, vtable 0x01238F84
public:
    CSkeletalMorphResourceManager();                  // 0x004AE2A0
    virtual ~CSkeletalMorphResourceManager();
    void ReleaseResources();                          // 0x006C35E0
    static CSkeletalMorphResourceManager& GetInstance()
    {
        if (Instance.Data == 0)
            Instance.Reset(new CSkeletalMorphResourceManager);
        return *Instance.Data;
    }
    static CCountedPointer<CSkeletalMorphResourceManager> Instance;   // 0x013BAB10
private:
    unsigned char _body[0xc];
};

struct CWorld {
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
    g_pQuestManager->OnWorldDestruction();
    PThingManager.reset(0);
    PGameCameraManager.reset(0);
    CSkeletalMorphResourceManager::GetInstance().ReleaseResources();
}