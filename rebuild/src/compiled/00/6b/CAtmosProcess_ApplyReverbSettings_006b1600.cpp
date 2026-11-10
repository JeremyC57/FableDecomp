// CAtmosProcess::ApplyReverbSettings() -- hand the sound system the MAIN_SOUND_SETUP reverb parameters of the current reverb environment, then its level
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the entry is indexed twice in the copy call (`ReverbEnvironmentEntries[ReverbType].Params` for first and last) instead of through a const reference local, which gives retail's unfolded `mov eax,[esi+6Ch] ; add eax,edx` entry address
// Leaf: ?ApplyReverbSettings@CAtmosProcess@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B1600, length 165 bytes (+ int3 padding), thiscall, plain `ret`.
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CSoundSystem model is gone and its slots carry the header names (SetReverbParameters slot 29, SetReverbLevel slot 30).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CDefinitionManager::Get @ 0x0044C6B0,
//   CDefinitionManager::GetDef<CSoundDef>(const CCharString&, CDefPointer<const CSoundDef>*) const @ 0x004196B2, memmove @ 0x00BFEAE6
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
    // Ego_r.pdb: ??$GetDef@VCSoundDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCSoundDef@@@@@Z (const, out by pointer)
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out) const;   // 0x004196B2 for T = CSoundDef
};
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0

#include "engine/CASoundManager.h"

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
    if (PSoundManager) {
        CDefPointer<const CSoundDef> soundDef;
        {
            CCharString name("MAIN_SOUND_SETUP", -1);
            CGameDefinitionManager::Get().GetDef(name, &soundDef);
        }
        CSoundDef* def = const_cast<CSoundDef*>(soundDef.Object);
        CReverbParameters params;
        std::copy(def->ReverbEnvironmentEntries[ReverbType].Params, def->ReverbEnvironmentEntries[ReverbType].Params + 21,
                  params.Parameters);
        PSoundManager->SetReverbParameters(params);
        PSoundManager->SetReverbLevel(ReverbLevel, 0.0f);
        if (--def->RefCount == 0)
            def->Destroy();
    }
}