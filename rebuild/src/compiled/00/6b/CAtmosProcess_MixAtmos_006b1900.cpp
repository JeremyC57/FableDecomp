// CAtmosProcess::MixAtmos(const CCharString& from, const CCharString& to, float mix, float time) -- when the sound manager exists, fetch the first atmos def by name and hand the mix to the sound manager
// Leaf: ?MixAtmos@CAtmosProcess@@QAEXABVCCharString@@0MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1900, 94 bytes, thiscall (this unused), `ret 10h`.
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
class CSoundManagerLH { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void MixAtmos(const CCharString& from, const CCharString& to, float mix, float time);   // slot 40 (+0xA0)
};
extern CSoundManagerLH* g_pSoundSystem;            // 0x013B8394

class CAtmosProcess {
public:
    long DayNightState;                            // +0x00
    void MixAtmos(const CCharString& from, const CCharString& to, float mix, float time);
};

void CAtmosProcess::MixAtmos(const CCharString& from, const CCharString& to, float mix, float time)
{
    if (g_pSoundSystem) {
        CDefPointer def;
        CGameDefinitionManager::Get().GetDef_006b2660(from, def);
        g_pSoundSystem->MixAtmos(from, to, mix, time);
    }
}