// Grade: DIFFER(321v329) -- retail keeps the empty-vector return's null-tested free separate and drops the test on the shared end block (failed thing checks + main path); ours cross-jumps the failed checks into the empty-path free and keeps the test at the end
// CTCTavernGame::CameraUseCameraPoint(const CCharString& name) -- find the world thing with this script name; if it is a live scripted camera point, hand the camera manager a tavern-game script module running from the point's start position to its end position
// Leaf: ?CameraUseCameraPoint@CTCTavernGame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EBFA0, 327 bytes (`ret 4`).
// Callees: CCharString(const CCharString&) 0x0099EC30 (into the dead name argument slot); CTCBase::GetWorld() const
//          0x00686A80 and CTCBase::GetWorld() 0x00686A70 (identical bodies, const and non-const); the world thing
//          collection's script-name search 0x005ED860 (manifest "GetAllThings<...>" is propagated, landed spelling from
//          0x008A7D60); ~CCharString 0x0099EAE0; free 0x00BFEA14 (the vector storage); CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound 0x0040F020 (interface 0x3F = word 1 bit 31, the `jns` test); the camera point's slot 28
//          (+0x70, its type; 6 = scripted); the world's +0x34 camera source slot 9 (camera-manager out getter);
//          operator new 0x00BFEA1A; CTCCameraPointScripted 0x0075CCB0 (a position getter taking one pointer, name unproven;
//          manifest label is propagated) and ::GetEndPos(C3DVector*, CRightHandedSet*, ...) 0x0075CD30;
//          NCameraModes::CScriptModuleTavernGame(const C3DVector&, const C3DVector&) 0x005ED240 (vtable 0x01255FC4);
//          CCountedPointer<NCameraModes::CScriptModuleBase>(T*) 0x005ED270; CGameCameraManager::SetScriptedCameraMode(
//          CCountedPointer<CScriptModuleBase>) 0x00699E60.
// Layout: CWorld thing collection +0x20, camera source +0x34; the thing's interface bitset +0x20, map +0x44, dead flag
//          bit 0 of +0x91.
// Shape: as the landed CGameScriptInterface::GetThingWithScriptName: the 12-byte vector local is zeroed in the frame, the
//        name is copied into its own argument slot by a `__forceinline` const helper taking the CCharString BY VALUE (so
//        the const GetWorld is called), and the storage is released by hand-inlined `free` (the null test drops on the
//        path that dereferenced begin()); the uninitialised scripted pointer, the lookup key and the camera manager all
//        reuse the dead argument slot; the counted pointer is built in place as the by-value argument.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
extern "C" void free(void* p);                  // 0x00BFEA14
// operator new (0x00BFEA1A) is the global one from <new>; not redeclared here

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
template <class T> struct vector {              // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    T* begin() { return _M_start; }
};

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct CThingCollection {                       // the world's thing collection at +0x20
    void FindThingsWithScriptName(const CCharString& name, vector<CThing*>& things);   // 0x005ED860
};

namespace NCameraModes {
struct CScriptModuleBase {
    virtual void v00();
};
struct CScriptModuleTavernGame : CScriptModuleBase {   // 0x64 bytes, vtable 0x01255FC4
    unsigned char _body[0x60];
    CScriptModuleTavernGame(const C3DVector& position, const C3DVector& lookAt);   // 0x005ED240
};
}
template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                 // 0x005ED270
    CCountedPointer(const CCountedPointer& other);
};
struct CGameCameraManager {
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module);   // 0x00699E60
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);     // slot 9 (+0x24)
};
struct CWorld {
    unsigned char     _pad[0x20];
    CThingCollection* Things;                   // +0x20
    unsigned char     _pad_0x24[0x10];
    CCameraManagerSource* m_pCameraSource;      // +0x34
    CCameraManagerSource* GetCameraSource() { return m_pCameraSource; }
};

struct CTCCameraPoint {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType();          // slot 28 (+0x70)
};
struct CTCCameraPointScripted : CTCCameraPoint {
    C3DVector GetStartPos(const CRightHandedSet* set);                          // 0x0075CCB0 (name unproven)
    C3DVector GetEndPos(const CRightHandedSet* set, const CRightHandedSet* view); // 0x0075CD30
};

struct CTCTavernGame {
    virtual void v00();
    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    __forceinline void FindThingsWithScriptName(CCharString name, vector<CThing*>& things) const
    {
        CWorld* world = GetWorld();
        world->Things->FindThingsWithScriptName(name, things);
    }
    __forceinline CGameCameraManager* GetCameraManager()
    {
        CGameCameraManager* camera;
        GetWorld()->GetCameraSource()->GetCameraManager(&camera);
        return camera;
    }
    void CameraUseCameraPoint(const CCharString& name);
};

void CTCTavernGame::CameraUseCameraPoint(const CCharString& name)
{
    vector<CThing*> things;
    FindThingsWithScriptName(name, things);
    if (things.empty())
        return;
    CThing* thing = *things.begin();
    if (thing && !thing->IsDead() && thing->HasTC(0x3f)) {
        CTCCameraPoint* point = (CTCCameraPoint*)thing->GetTC(0x3f);
        CTCCameraPointScripted* scripted;
        if (point->GetCameraPointType() == 6)
            scripted = (CTCCameraPointScripted*)point;
        CGameCameraManager* camera = GetCameraManager();
        camera->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
            new NCameraModes::CScriptModuleTavernGame(scripted->GetEndPos(0, 0), scripted->GetStartPos(0))));
        return;
    }
}
