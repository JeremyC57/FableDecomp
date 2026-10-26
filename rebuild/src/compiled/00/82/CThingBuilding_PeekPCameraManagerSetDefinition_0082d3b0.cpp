// CThingBuilding::PeekPCameraManagerSetDefinition() const -- camera set
// Leaf: PeekPCameraManagerSetDefinition
class CDefClassBase { public:
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
#pragma pack(push, 1)
class CThingBuildingDef : public CDefClassBase { public:
    unsigned char _pad08[0x88 - 0x08];
    unsigned long WallMaterial;                    // +0x88
    unsigned char _pad8c[0x9c - 0x8c];
    long CameraManagerSet;                         // +0x9c
};
#pragma pack(pop)
class CCameraManagerSetDef : public CDefClassBase { public: };
class CRegionDef : public CDefClassBase { public:
    unsigned char _pad08[0x4c - 0x08];
    long CameraManagerSet;                         // +0x4c
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object)
    {
        if (object)
            ++static_cast<CDefClassBase*>(object)->RefCount;
    }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        CDefClassBase* old = Object;                  // released through the unpacked base (the def classes pack to 1)
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingBuildingDef>* out);   // 0x004AB840
    bool GetDef(long index, CDefPointer<CCameraManagerSetDef>* out);   // 0x004AB920
    bool GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* out);   // 0x004AB990
};
void* FindDef_CDefinitionManager_GetDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingBuildingDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CCameraManagerSetDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CRegion { public:
    unsigned char _pad00[0x24];
    CRegionDef* PDef;                              // +0x24 CDefPointer<const CRegionDef>
};
class CWorldMap { public:
    long GetCurrentRegionIndex() const;            // 0x004FB150
    CRegion& GetRegion(long index);                // 0x004FC180
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};
void* FindDef_CDefinitionManager_GetDef_2();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThingBuilding { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;                   // +0x8c
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    CWorld* GetWorld() const;                       // 0x004C7AB0
    __forceinline const CThingBuildingDef* PeekPDef() const
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Object;
    }
    CDefPointer<CCameraManagerSetDef> PeekPCameraManagerSetDefinition() const;
};
CDefPointer<CCameraManagerSetDef> CThingBuilding::PeekPCameraManagerSetDefinition() const
{
    CDefPointer<CCameraManagerSetDef> def;
    if (PeekPDef()->CameraManagerSet > 0) {
        GetDefinitionManager()->GetDef(PeekPDef()->CameraManagerSet, &def);
        return def;
    }
    CDefPointer<CRegionDef> regionDef(GetWorld()->GetWorldMap()->GetRegion(GetWorld()->GetWorldMap()->GetCurrentRegionIndex()).PDef);
    const CRegionDef* rdef = regionDef.Object;
    if (rdef && rdef->CameraManagerSet > 0) {
        GetDefinitionManager()->GetDef(rdef->CameraManagerSet, &def);
        return def;
    }
    GetDefinitionManager()->GetDef("CAMERA_MANAGER_SET_BUILDING", &def);
    return def;
}