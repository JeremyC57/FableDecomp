// CAtmosProcess::ApplyReverbSettings() -- hand the sound system the MAIN_SOUND_SETUP reverb parameters of the current reverb environment, then its level
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the entry is indexed twice in the copy call (`ReverbEnvironmentEntries[ReverbType].Params` for first and last) instead of through a const reference local, which gives retail's unfolded `mov eax,[esi+6Ch] ; add eax,edx` entry address
// Leaf: ?ApplyReverbSettings@CAtmosProcess@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B1600, length 165 bytes (+ int3 padding), thiscall, plain `ret`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CDefinitionManager::Get @ 0x0044C6B0,
//   CDefinitionManager::GetDef<const CSoundDef>(const CCharString&, CDefPointer&) @ 0x004196B2, memmove @ 0x00BFEAE6
//   (MSVCR71 import), the sound system's virtual slots 29 (+0x74, the reverb parameters) and 30 (+0x78, the reverb
//   level and a zero; both names descriptive), the def's virtual Destroy (slot 1).
// Globals: g_pSoundSystem @ 0x013B8394 (the landed CAtmosProcess ctor 0x006B1960 / CMusicManager ctor names);
//   literal "MAIN_SOUND_SETUP" @ 0x0122EF98.
// Identity: Ego_r.pdb `?ApplyReverbSettings@CAtmosProcess@@AAEXXZ` (name set only). Callers: the landed CAtmosProcess
//   ctor 0x006B1960 (its "LoadMainTheme_006b1600") and CAtmosProcess::UpdateInsideBuildings 0x006B1F30, which
//   changes ReverbType / ReverbLevel. The catalog's "Sound_LoadMainThemeSetup" is a descriptive auto label.
// Layout: CAtmosProcess as the landed ctor (ReverbType +0x18, ReverbLevel +0x1c). Retail CSoundDef
//   ReverbEnvironmentEntries (STLport vector of 0x54-byte CReverbEnvironmentEntry {float Params[21]}) at +0x6c (PDB
//   +0x80; the ctor's AtmosListenerDampingMin +0x78 follows it). CReverbParameters {float Parameters[21]} (Ego_r).
// Shape: the ctor's block-scoped name + chained `Get().GetDef(name, def)`; the entry's 21 floats go into a frame
//   CReverbParameters through STLport copy -> __copy_trivial (`last == first ? result : memmove(...)`); the def is
//   released by hand without a null test (`dec [esi+4] ; jne`). The entry is indexed twice (no reference local).
// Tried: `*(_M_start + n)` (this), `*(begin() + n)`, a CReverbParameters(const CReverbEnvironmentEntry&) ctor doing
//   the copy, and a named `entries = _M_start` base pointer: all four give the same 163-byte body.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int count);   // 0x00BFEAE6

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
    template <class T> bool GetDef(const CCharString& name, CDefPointer<T>& out);   // 0x004196B2 for T = const CSoundDef
};
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0

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
private:
    void ApplyReverbSettings();
};

void CAtmosProcess::ApplyReverbSettings()
{
    if (g_pSoundSystem) {
        CDefPointer<const CSoundDef> soundDef;
        {
            CCharString name("MAIN_SOUND_SETUP", -1);
            CGameDefinitionManager::Get().GetDef(name, soundDef);
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