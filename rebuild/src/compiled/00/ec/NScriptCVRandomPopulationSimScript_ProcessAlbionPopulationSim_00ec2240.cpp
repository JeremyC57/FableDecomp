// CV_RandomPopulationSimScript::ProcessAlbionPopulationSim() -- each time a region loads (outside the Greatwood/Fisherman regions while Q_BountyHunt runs) look up its REGION_<NAME> def, set the current population region and copy the def's three population maxima; the high-detail sim is active unless all three are zero
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC2240, 0x2DF bytes, plain `ret`.
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 / ToUpper 0x0099EE20 /
//   ~ 0x0099EAE0, operator+ 0x0099F570 (__fastcall, hidden result in ecx), Compare 0x00411570 and
//   CDefinitionManager::GetDef<T> 0x00CDE280 (both DEFINED here under auto_inline(off): with their bodies in the TU VC7.1
//   knows neither leaks or writes the locals, so it keeps the region's PData and the def pointer in esi across the
//   following calls as retail does; declared extern they are reloaded after every call), GFGetPDefinitionManager
//   0x00CD3EF0, GetPDefFromInstantiationName 0x009ADA40, SetCurrentPopulationRegionFromRegionName 0x00EC2520 (the
//   manifest's CScriptBase_ProcessAlbionPopulationSim_Filter), IsActiveThreadTerminating 0x00CB7940; Game slots 7
//   NewScriptFrame, 22 MsgOnRegionLoaded, 284 IsQuestActive. The def's type name is not recovered (MaxPopulation[3] +0x3c).
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, HighDetailSimulationActive +0x58, PopulationSim[3] +0x78.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    __forceinline bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString ToUpper();                                  // 0x0099EE20
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CRefCounted {                                        // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CRegionDef : public CDefBase { public:                              // the REGION_<name> def (type name not recovered)
    unsigned char _pad08[0x3c - 0x08];
    long MaxPopulation[3];                                  // +0x3c bandits / villagers / guards
};
struct CDefPointer {
    CRegionDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
    CRegionDef* operator->() const { return m_p; }
};
class CDefinitionManager { public:
    CDefPointer GetPDefFromInstantiationName(const CCharString& name);   // 0x009ADA40 (hidden result pointer)
    bool GetDef_00cde280(const CCharString& name, CDefPointer& out);   // 0x00CDE280 (body below: the template's header definition)
};
#pragma auto_inline(off)
bool CDefinitionManager::GetDef_00cde280(const CCharString& name, CDefPointer& out)
{
    CDefPointer def = GetPDefFromInstantiationName(name);
    if (def.m_p) {
        out = def;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
CDefinitionManager* GFGetPDefinitionManager();              // 0x00CD3EF0
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CPopulationSim {
    long MaxPopulation;
};
namespace NScript {
class CV_RandomPopulationSimScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 PMasterData;                      // +0x44
    float CurrentPopulationScale[4];                        // +0x48
    bool  HighDetailSimulationActive;                       // +0x58
    long  CurrentPopulationRegion;                          // +0x5c
    long  CreaturesAlive[3];                                // +0x60
    unsigned long BanditCreatureGeneratorIndex;             // +0x6c
    unsigned long VillagerCreatureGeneratorIndex;           // +0x70
    unsigned long GuardCreatureGeneratorIndex;              // +0x74
    CPopulationSim PopulationSim[3];                        // +0x78
    void SetCurrentPopulationRegionFromRegionName(CCharString& region);   // 0x00EC2520
    void ProcessAlbionPopulationSim();
};
} // namespace NScript
using NScript::CV_RandomPopulationSimScript;

void NScript::CV_RandomPopulationSimScript::ProcessAlbionPopulationSim()
{
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        if (Game->MsgOnRegionLoaded(region)) {
            if (IsActiveThreadTerminating())
                return;
            bool bountyHuntRegion = false;
            if (Game->IsQuestActive("Q_BountyHunt")) {
                if (IsActiveThreadTerminating())
                    return;
                if (region == "GreatwoodEntrance" || region == "GreatwoodLake" || region == "Fisherman") {
                    if (IsActiveThreadTerminating())
                        return;
                    bountyHuntRegion = true;
                }
            }
            HighDetailSimulationActive = false;
            CDefPointer def;
            CCharString defName = CCharString("REGION_") + CCharString(region).ToUpper();
            if (!bountyHuntRegion && GFGetPDefinitionManager()->GetDef_00cde280(defName, def)) {
                if (IsActiveThreadTerminating())
                    return;
                SetCurrentPopulationRegionFromRegionName(region);
                PopulationSim[0].MaxPopulation = def->MaxPopulation[0];
                PopulationSim[1].MaxPopulation = def->MaxPopulation[1];
                PopulationSim[2].MaxPopulation = def->MaxPopulation[2];
                if (PopulationSim[0].MaxPopulation == 0 && PopulationSim[1].MaxPopulation == 0 && PopulationSim[2].MaxPopulation == 0) {
                    if (IsActiveThreadTerminating())
                        return;
                    HighDetailSimulationActive = false;
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    HighDetailSimulationActive = true;
                }
            }
        }
        Game->NewScriptFrame();
    }
}