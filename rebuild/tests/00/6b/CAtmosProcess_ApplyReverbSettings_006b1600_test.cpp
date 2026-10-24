// Behaviour test for 0x006B1600 CAtmosProcess::ApplyReverbSettings: with a sound system, the MAIN_SOUND_SETUP def's
// reverb entry for the current ReverbType is copied (all 21 floats) to SetReverbParameters, the level goes to
// SetReverbLevel with a zero time, and the def reference is released (Destroy at zero). No sound system: nothing.
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std {
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
          + ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
}

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CDefPointeeBase {
public:
    virtual void Destroy0();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
};

class CReverbEnvironmentEntry {
public:
    float Params[21];
};
class CReverbParameters {
public:
    float Parameters[21];
};

template <class T> class CRetailVector {                    // STLport vector body
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};

class CSoundDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x6c - 0x08];
    CRetailVector<CReverbEnvironmentEntry> ReverbEnvironmentEntries;   // +0x6c
};

class CDefinitionManager {
public:
    static CDefinitionManager& __fastcall Get();                                    // 0x0044C6B0
    template <class T> bool GetDef(const CCharString& name, CDefPointer<T>& out);   // 0x004196B2 for T = const CSoundDef
};

class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void SetReverbParameters(const CReverbParameters& params);   // slot 29 (+0x74, descriptive)
    virtual void SetReverbLevel(float level, float time);               // slot 30 (+0x78, descriptive)
};
extern CSoundSystem* g_pSoundSystem;                        // 0x013B8394

class CAtmosProcess {
public:
    unsigned char _pad00[0x18];
    long  ReverbType;                                       // +0x18 EReverbEnvironmentType
    float ReverbLevel;                                      // +0x1c
public:
    void ApplyReverbSettings();
};

void CAtmosProcess::ApplyReverbSettings()
{
    if (g_pSoundSystem) {
        CDefPointer<const CSoundDef> soundDef;
        {
            CCharString name("MAIN_SOUND_SETUP", -1);
            CDefinitionManager::Get().GetDef(name, soundDef);
        }
        CSoundDef* def = const_cast<CSoundDef*>(soundDef.Object);
        CReverbParameters params;
        std::copy(def->ReverbEnvironmentEntries[ReverbType].Params, def->ReverbEnvironmentEntries[ReverbType].Params + 21,
                  params.Parameters);
        g_pSoundSystem->SetReverbParameters(params);
        g_pSoundSystem->SetReverbLevel(ReverbLevel, 0.0f);
        if (--def->RefCount == 0)
            def->Destroy();
    }
}


static int g_destroyed = 0;
static int g_getDefCalls = 0;
static int g_names = 0;
static CReverbParameters g_lastParams;
static float g_lastLevel = -1.0f, g_lastTime = -1.0f;
static int g_paramCalls = 0, g_levelCalls = 0;
static CSoundDef* g_def = 0;
CSoundSystem* g_pSoundSystem = 0;

CCharString::CCharString(const char* text, long) { PStringData = (void*)text; if (strcmp(text, "MAIN_SOUND_SETUP") == 0) ++g_names; }
CCharString::~CCharString() { }
void CDefPointeeBase::Destroy0() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static CDefinitionManager g_manager;
CDefinitionManager& __fastcall CDefinitionManager::Get() { return g_manager; }
template <class T> bool CDefinitionManager::GetDef(const CCharString&, CDefPointer<T>& out)
{ ++g_getDefCalls; out.Object = g_def; ++g_def->RefCount; return true; }
#define STUB(n) void CSoundSystem::v##n() { }
STUB(00) STUB(01) STUB(02) STUB(03) STUB(04) STUB(05) STUB(06) STUB(07) STUB(08) STUB(09)
STUB(10) STUB(11) STUB(12) STUB(13) STUB(14) STUB(15) STUB(16) STUB(17) STUB(18) STUB(19)
STUB(20) STUB(21) STUB(22) STUB(23) STUB(24) STUB(25) STUB(26) STUB(27) STUB(28)
void CSoundSystem::SetReverbParameters(const CReverbParameters& params) { g_lastParams = params; ++g_paramCalls; }
void CSoundSystem::SetReverbLevel(float level, float time) { g_lastLevel = level; g_lastTime = time; ++g_levelCalls; }

int main()
{
    static CReverbEnvironmentEntry entries[3];
    for (unsigned int e = 0; e < 3; ++e)
        for (unsigned int k = 0; k < 21; ++k)
            entries[e].Params[k] = (float)(e * 100 + k) + 0.5f;
    static CSoundDef def;
    def.RefCount = 1;                                   // held by the manager
    def.ReverbEnvironmentEntries._M_start = entries;
    def.ReverbEnvironmentEntries._M_finish = entries + 3;
    def.ReverbEnvironmentEntries._M_end_of_storage = entries + 3;
    g_def = &def;

    CAtmosProcess atmos;
    memset(&atmos, 0, sizeof(atmos));
    atmos.ReverbType = 2;
    atmos.ReverbLevel = 0.75f;

    int ok = 1;
    atmos.ApplyReverbSettings();                        // no sound system: nothing at all
    if (g_getDefCalls != 0 || g_paramCalls != 0 || g_levelCalls != 0) ok = 0;

    static CSoundSystem sound;
    g_pSoundSystem = &sound;
    atmos.ApplyReverbSettings();
    if (g_names != 1 || g_getDefCalls != 1 || g_paramCalls != 1 || g_levelCalls != 1) ok = 0;
    for (unsigned int k = 0; k < 21; ++k)
        if (fabs(g_lastParams.Parameters[k] - entries[2].Params[k]) > 1e-6f) ok = 0;
    if (fabs(g_lastLevel - 0.75f) > 1e-6f || fabs(g_lastTime) > 1e-6f) ok = 0;
    if (def.RefCount != 1 || g_destroyed != 0) ok = 0;   // released back to the manager's count

    atmos.ReverbType = 0;
    atmos.ReverbLevel = 0.25f;
    def.RefCount = 0;                                   // only this call holds it: released to zero -> Destroy
    atmos.ApplyReverbSettings();
    for (unsigned int k = 0; k < 21; ++k)
        if (fabs(g_lastParams.Parameters[k] - entries[0].Params[k]) > 1e-6f) ok = 0;
    if (fabs(g_lastLevel - 0.25f) > 1e-6f || g_destroyed != 1 || def.RefCount != 0) ok = 0;

    printf(ok ? "006b1600_TEST PASS\n" : "006b1600_TEST FAIL\n");
    return ok ? 0 : 1;
}