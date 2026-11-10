// CAtmosProcess::MixAtmos(const CCharString& from, const CCharString& to, float mix, float time) -- when the sound manager exists, fetch the first atmos def by name and hand the mix to the sound manager
// Leaf: ?MixAtmos@CAtmosProcess@@QAEXABVCCharString@@0MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1900, 94 bytes, thiscall (this unused), `ret 10h`.
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CSoundManagerLH model is gone and its slots carry the header names (the local MixAtmos slot -> CASoundManager::SetChannelGain slot 40; the row keeps its own Ego_r name CAtmosProcess::MixAtmos).
// Identity (probable): Ego_r.pdb ?MixAtmos@CAtmosProcess@@QAEXABVCCharString@@0MM@Z is the only (const CCharString&,
//   const CCharString&, float, float) member in either PDB name set; the callers (the atmosphere code at 0x006B1A20 /
//   0x006B2260 and CTCSoundAtmosphereVillage 0x007F53B0) push two string references, 1.0f and 2000.0f. The catalog
//   label "CGameScriptInterface::SetQuestInfoName" is propagated (that PDB function takes one const char*).
// Callees: CGameDefinitionManager::Get() 0x0044C6B0 (Ego_r.pdb static __fastcall, returns CGameDefinitionManager&),
//   the definition manager's by-name def lookup 0x006B2660 (landed under the invented spelling
//   CDefinitionManager::GetDef_006b2660(const CCharString&, CDefPointer&); its def type is unproven -- reused as landed,
//   see NOTES "Review at landing"), the sound manager's virtual slot 40 (+0xA0, CSoundManagerLH vtable 0x0129D2C4 slot
//   40 = 0x00A3EED0, descriptive name), the def's virtual slot 1 (+4) when its count drops to zero.
// Globals: the sound manager pointer 0x013B8394 (landed spellings vary; this one is the CSoundManagerLH form).
// Shape: a zeroed def-pointer local released behind a null test after the virtual call.
class CCharString { public: void* PStringData; };
class CDefPointeeBase { public:
    virtual void Destroy();                        // slot 0
    virtual void Delete();                         // slot 1 (+0x04)
    long m_refCount;                               // +0x04
};
struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
};
class CDefinitionManager { public:
    bool GetDef_006b2660(const CCharString& name, CDefPointer& out);   // 0x006B2660
};
void* FindDef_CDefinitionManager_GetDef_0();       // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail, landed 0x006B2660): it writes the out pointer only inline, so
// VC7.1 knows the def local's address does not escape and keeps the def in ecx for the release (`dec [ecx+4] ; jne`).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_006b2660(const CCharString& name, CDefPointer& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    out.Object = (CDefPointeeBase*)def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
};
#include "engine/CASoundManager.h"

class CAtmosProcess {
public:
    long DayNightState;                            // +0x00
    void MixAtmos(const CCharString& from, const CCharString& to, float mix, float time);
};

void CAtmosProcess::MixAtmos(const CCharString& from, const CCharString& to, float mix, float time)
{
    if (PSoundManager) {
        CDefPointer def;
        CGameDefinitionManager::Get().GetDef_006b2660(from, def);
        PSoundManager->SetChannelGain(from, to, mix, time);
    }
}