// CThingBaseDef::Transfer(CPersistContext& context) -- persist the base thing definition: the component set, the twenty named flag bits with their defaults, the group/fade/persistence words, the health and mesh-size floats and the default script name/data
// Leaf: ?Transfer@CThingBaseDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00676BE0, 1770 bytes (+ int3 padding), `ret 4`; virtual Transfer of CThingBaseDef
//   (vtable 0x0125B6AC; derived def Transfers call it directly).
// Callees: CPersistContext::CheckCRC 0x00404500 (pooled empty key 0x0122D70E), CThingComponentSet::TransferBinaryIn 0x008707A0
//   / TransferBinaryOut 0x008708F0, operator delete[] 0x00BFEB1C (the default-constructed CThingComponentSet temporary's
//   destructor, folded to delete[] 0), Transfer<bool>(name, value, default) 0x004045C0, CPersistTraits<long>::TransferBinaryIn
//   0x0040FE60 / CDataOutputStream::WriteLong 0x00993EB0, CPersistTraits<float>::TransferBinaryIn 0x0040EFB0 / WriteFloat
//   0x00993EE0, CDefString::TransferBinaryIn 0x0044FC00.
// Layout: Ego_r.pdb CThingBaseDef (0x74) over the retail 0x39-byte CParentDefClassBase (all members 4 bytes below the PDB):
//   Components +0x39 (unaligned, pack(1)), the flag bits in +0x41..+0x43, GroupDef +0x44 .. DefaultScriptData +0x6c (retail 0x70).
// Shape: the landed CShoveHaPennyRegionDef::Transfer (0x007D0070) inline CPersistContext::Transfer<T> for the binary members
//   (traits temporary in the dead context slot, the final save/load arms with their own epilogues) and the landed
//   CEngineLocalDetailObjectDef::Transfer (0x0047769E) flag-bit form with each flag a block-scoped bool (a single
//   function-scope flag local pushes the traits temporaries onto the default temporary's frame byte); the component set
//   passes a default-constructed set, whose destructor survives as `delete[] 0`.
// No engine header for CThingBaseDef; local model.
class CMemoryDataInputStream;
void __cdecl operator delete[](void* block);           // 0x00BFEB1C (MSVCR71 import thunk)

class CDataOutputStream { public:
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};

#pragma pack(push, 1)
struct CThingComponentEntry {                           // 9-byte packed {def name, type, flag}
    long          Name;
    long          Type;
    unsigned char Flag;
};
class CThingComponentSet { public:                             // CSmallVector<CThingComponentSet::CEntry, 8>
    CThingComponentEntry* Start;                        // +0
    signed char           Size;                         // +4
    unsigned char         _pad5[3];
    CThingComponentSet() : Start(0), Size(0) { }
    ~CThingComponentSet() { delete[] Start; }
    void TransferBinaryIn(CMemoryDataInputStream* stream);          // 0x008707A0
    void TransferBinaryOut(CDataOutputStream* stream) const;        // 0x008708F0
};
#pragma pack(pop)
class CDefString { public:
    long m_index;
    void TransferBinaryIn(CMemoryDataInputStream* stream);          // 0x0044FC00
};

template<class T> struct CPersistTraits {              // a value with its own binary transfer
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value) { value->TransferBinaryIn(stream); }
    static void TransferBinaryOut(CDataOutputStream* stream, const T& value) { value.TransferBinaryOut(stream); }
};
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);    // 0x0040FE60
    static void TransferBinaryOut(CDataOutputStream* stream, const long& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);   // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};
template<> struct CPersistTraits<CDefString> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CDefString* value) { value->TransferBinaryIn(stream); }
    static void TransferBinaryOut(CDataOutputStream* stream, const CDefString& value) { stream->WriteLong(value.m_index); }
};

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    protected: void CheckCRC(const char* name); public:                    // 0x00404500
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    template<class T> void Transfer(const char* name, T& value, const T& defaultValue)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};

#pragma pack(push, 1)
class CThingBaseDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x35];                        // CParentDefClassBase body (retail 0x39 bytes)
    CThingComponentSet Components;                      // +0x39
    bool AvailableInEditor : 1;                         // +0x41 bit 0
    bool AlwaysFrameUpdate : 1;                         // +0x41 bit 1
    bool StartKillLocked : 1;                           // +0x41 bit 2
    bool LevelLess : 1;                                 // +0x41 bit 3
    bool IsDrawable : 1;                                // +0x41 bit 4
    bool IsDrawableInEditor : 1;                        // +0x41 bit 5
    bool ForceNoSerialise : 1;                          // +0x41 bit 6
    bool InMapSearches : 1;                             // +0x41 bit 7
    bool IsLoadableGlobal : 1;                          // +0x42 bit 0
    bool GameStatePersisted : 1;                        // +0x42 bit 1
    bool CanComeBetweenCameraAndObservedThing : 1;      // +0x42 bit 2
    bool DrawWeaponTrails : 1;                          // +0x42 bit 3
    bool DrawProjectileWeaponTrails : 1;                // +0x42 bit 4
    bool IsVulnerableToMelee : 1;                       // +0x42 bit 5
    bool Damageable : 1;                                // +0x42 bit 6
    bool AlwaysIncludeInObjectStrikeScans : 1;          // +0x42 bit 7
    bool DisableOcclusionTesting : 1;                   // +0x43 bit 0
    bool DrawAfterWater : 1;                            // +0x43 bit 1
    bool ForceRenderedLastFrameOverride : 1;            // +0x43 bit 2
    bool AddToComboMultiplierOnHit : 1;                 // +0x43 bit 3
    bool GiveHeroStatChangesOnBeingHit : 1;             // +0x43 bit 4
    long       GroupDef;                                // +0x44
    float      RenderFadeDistance;                      // +0x48
    long       PersistenceFlags;                        // +0x4c
    float      Health;                                  // +0x50
    float      MinHealth;                               // +0x54
    float      MeshHeightOffset;                        // +0x58
    float      MeshHeight;                              // +0x5c
    float      MeshRadius;                              // +0x60
    float      ApproxMaxMeshHeight;                     // +0x64
    CDefString DefaultScriptName;                       // +0x68
    CDefString DefaultScriptData;                       // +0x6c
};
#pragma pack(pop)

void CThingBaseDef::Transfer(CPersistContext& context)
{
    context.Transfer("", Components, CThingComponentSet());
    {
        bool flag = AvailableInEditor;
        context.Transfer("AvailableInEditor", flag, true);
        AvailableInEditor = flag;
    }
    {
        bool flag = GameStatePersisted;
        context.Transfer("GameStatePersisted", flag, false);
        GameStatePersisted = flag;
    }
    {
        bool flag = LevelLess;
        context.Transfer("LevelLess", flag, false);
        LevelLess = flag;
    }
    {
        bool flag = IsDrawable;
        context.Transfer("IsDrawable", flag, false);
        IsDrawable = flag;
    }
    {
        bool flag = AlwaysFrameUpdate;
        context.Transfer("AlwaysFrameUpdate", flag, true);
        AlwaysFrameUpdate = flag;
    }
    {
        bool flag = InMapSearches;
        context.Transfer("InMapSearches", flag, true);
        InMapSearches = flag;
    }
    {
        bool flag = StartKillLocked;
        context.Transfer("StartKillLocked", flag, false);
        StartKillLocked = flag;
    }
    {
        bool flag = IsLoadableGlobal;
        context.Transfer("IsLoadableGlobal", flag, false);
        IsLoadableGlobal = flag;
    }
    {
        bool flag = CanComeBetweenCameraAndObservedThing;
        context.Transfer("CanComeBetweenCameraAndObservedThing", flag, true);
        CanComeBetweenCameraAndObservedThing = flag;
    }
    {
        bool flag = IsVulnerableToMelee;
        context.Transfer("IsVulnerableToMelee", flag, false);
        IsVulnerableToMelee = flag;
    }
    {
        bool flag = Damageable;
        context.Transfer("Damageable", flag, false);
        Damageable = flag;
    }
    {
        bool flag = ForceNoSerialise;
        context.Transfer("ForceNoSerialise", flag, false);
        ForceNoSerialise = flag;
    }
    {
        bool flag = DrawWeaponTrails;
        context.Transfer("DrawWeaponTrails", flag, true);
        DrawWeaponTrails = flag;
    }
    {
        bool flag = DrawProjectileWeaponTrails;
        context.Transfer("DrawProjectileWeaponTrails", flag, true);
        DrawProjectileWeaponTrails = flag;
    }
    {
        bool flag = AlwaysIncludeInObjectStrikeScans;
        context.Transfer("AlwaysIncludeInObjectStrikeScans", flag, false);
        AlwaysIncludeInObjectStrikeScans = flag;
    }
    {
        bool flag = DisableOcclusionTesting;
        context.Transfer("DisableOcclusionTesting", flag, false);
        DisableOcclusionTesting = flag;
    }
    {
        bool flag = DrawAfterWater;
        context.Transfer("DrawAfterWater", flag, false);
        DrawAfterWater = flag;
    }
    {
        bool flag = ForceRenderedLastFrameOverride;
        context.Transfer("ForceRenderedLastFrameOverride", flag, false);
        ForceRenderedLastFrameOverride = flag;
    }
    {
        bool flag = AddToComboMultiplierOnHit;
        context.Transfer("AddToComboMultiplierOnHit", flag, false);
        AddToComboMultiplierOnHit = flag;
    }
    {
        bool flag = GiveHeroStatChangesOnBeingHit;
        context.Transfer("GiveHeroStatChangesOnBeingHit", flag, false);
        GiveHeroStatChangesOnBeingHit = flag;
    }
    context.Transfer("", GroupDef, 0L);
    context.Transfer("", RenderFadeDistance, 0.0f);
    context.Transfer("", PersistenceFlags, 0L);
    context.Transfer("", Health, 0.0f);
    context.Transfer("", MinHealth, 0.0f);
    context.Transfer("", MeshHeightOffset, 0.0f);
    context.Transfer("", MeshHeight, 0.0f);
    context.Transfer("", MeshRadius, 0.0f);
    context.Transfer("", ApproxMaxMeshHeight, 0.0f);
    context.Transfer("", DefaultScriptName, CDefString());
    context.Transfer("", DefaultScriptData, CDefString());
}