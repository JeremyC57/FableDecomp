// Behaviour test for 0x00676BE0 CThingBaseDef::Transfer: the source model is copied verbatim and every callee records.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __cdecl
#endif
struct CMemoryDataInputStream;
void __cdecl operator delete[](void* block);           // 0x00BFEB1C (MSVCR71 import thunk)

struct CDataOutputStream {
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};

#pragma pack(push, 1)
struct CThingComponentEntry {                           // 9-byte packed {def name, type, flag}
    long          Name;
    long          Type;
    unsigned char Flag;
};
struct CThingComponentSet {                             // CSmallVector<CThingComponentSet::CEntry, 8>
    CThingComponentEntry* Start;                        // +0
    signed char           Size;                         // +4
    unsigned char         _pad5[3];
    CThingComponentSet() : Start(0), Size(0) { }
    ~CThingComponentSet() { delete[] Start; }
    void TransferBinaryIn(CMemoryDataInputStream* stream);          // 0x008707A0
    void TransferBinaryOut(CDataOutputStream* stream) const;        // 0x008708F0
};
#pragma pack(pop)
struct CDefString {
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
    void CheckCRC(const char* name);                    // 0x00404500
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

static int g_crc = 0, g_writes = 0, g_loads = 0, g_compIn = 0, g_compOut = 0, g_bools = 0, g_deletes = 0;   // g++ may skip delete[] of a null pointer, so deletes are not checked
static float g_written[32];
static const char* g_boolNames[32];
static bool g_boolDefaults[32];
void __cdecl operator delete[](void* block) { ++g_deletes; std::free(block); }
void CPersistContext::CheckCRC(const char* name) { if (name[0] == 0) ++g_crc; }
void CPersistContext::Transfer(const char* name, bool& value, const bool& defaultValue)
{
    g_boolNames[g_bools] = name;
    g_boolDefaults[g_bools] = defaultValue;
    ++g_bools;
    if (m_mode == 2) value = !value;           // a load flips every flag so the bit write-back is visible
}
void CDataOutputStream::WriteLong(long value) { g_written[g_writes++] = (float)value; }
void CDataOutputStream::WriteFloat(float value) { g_written[g_writes++] = value; }
void CThingComponentSet::TransferBinaryIn(CMemoryDataInputStream* stream) { (void)stream; ++g_compIn; }
void CThingComponentSet::TransferBinaryOut(CDataOutputStream* stream) const { (void)stream; ++g_compOut; }
void CDefString::TransferBinaryIn(CMemoryDataInputStream* stream) { (void)stream; m_index = 500 + g_loads++; }
void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream* stream, long* value) { (void)stream; *value = 100 + g_loads++; }
void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream* stream, float* value) { (void)stream; *value = (float)(100 + g_loads++); }

int main()
{
    static CThingBaseDef def;          // zero-initialised: every flag starts false
    static CPersistContext context;
    static CDataOutputStream out;
    bool ok = true;
    def.GroupDef = 7; def.RenderFadeDistance = 1.5f; def.PersistenceFlags = 3; def.Health = 2.5f; def.MinHealth = 0.5f;
    def.MeshHeightOffset = 4.0f; def.MeshHeight = 5.0f; def.MeshRadius = 6.0f; def.ApproxMaxMeshHeight = 8.0f;
    def.DefaultScriptName.m_index = 11; def.DefaultScriptData.m_index = 12;
    def.IsDrawableInEditor = true;
    context.m_mode = 3; context.m_pSaveStream = &out;
    def.Transfer(context);
    const float expected[11] = { 7.0f, 1.5f, 3.0f, 2.5f, 0.5f, 4.0f, 5.0f, 6.0f, 8.0f, 11.0f, 12.0f };
    ok = ok && g_writes == 11 && g_compOut == 1 && g_compIn == 0 && g_crc == 12 && g_bools == 20;
    for (int i = 0; ok && i < 11; ++i) if (g_written[i] != expected[i]) ok = false;
    ok = ok && std::strcmp(g_boolNames[0], "AvailableInEditor") == 0 && g_boolDefaults[0];
    ok = ok && std::strcmp(g_boolNames[8], "CanComeBetweenCameraAndObservedThing") == 0 && g_boolDefaults[8];
    ok = ok && std::strcmp(g_boolNames[19], "GiveHeroStatChangesOnBeingHit") == 0 && !g_boolDefaults[19];
    // load: the scalars and def strings come back in persist order, every transferred flag flips, IsDrawableInEditor stays
    context.m_mode = 2; g_bools = 0; g_writes = 0;
    def.Transfer(context);
    ok = ok && g_compIn == 1 && g_writes == 0 && g_bools == 20;
    ok = ok && def.GroupDef == 100 && def.RenderFadeDistance == 101.0f && def.PersistenceFlags == 102 && def.ApproxMaxMeshHeight == 108.0f;
    ok = ok && def.DefaultScriptName.m_index == 509 && def.DefaultScriptData.m_index == 510;
    ok = ok && def.AvailableInEditor && def.InMapSearches && def.GiveHeroStatChangesOnBeingHit && def.Damageable && def.IsDrawableInEditor;
    if (!ok) { std::printf("00676be0_TEST FAIL\n"); return 1; }
    std::printf("00676be0_TEST PASS\n");
    return 0;
}