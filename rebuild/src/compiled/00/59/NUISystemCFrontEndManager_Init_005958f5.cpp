// NUISystem::CFrontEndManager::Init(CCountedPointer<CGraphicDataBank>, CCountedPointer<CMeshDataBank>, CInputManager*, CDefinitionManager*) -- boot the front end: clear signatures, hand the UI manager its mesh bank, input and definitions, reset screen transitions and register the existing profiles
// Leaf: ?Init@CFrontEndManager@NUISystem@@QAEXV?$CCountedPointer@VCGraphicDataBank@@@@V?$CCountedPointer@VCMeshDataBank@@@@PAVCInputManager@@PAVCDefinitionManager@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data are the header CCountedPointer's (same offsets).
// Retail: Fable.exe @ 0x005958F5, 182 bytes, `ret 18h`; Ego_r.pdb same symbol.
// Body: Signatures.clear() (map<CWideString,bool> clear 0x00485AEF); CManager::GetInstance() (0x0041E5F2)
// ->SetMeshBank(meshes) (0x0041D198, by-value CCountedPointer built in the argument area by the copy ctor 0x00428905);
// GetInstance()->PInputManager (+0x20) = input; GetInstance()->SetDefinitionManager(definitions) (0x0041CD89, stores +0x64);
// PScreenToActivate = PScreenToDeactivate = 0; CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0)->SetDefaultValuesForNewProfile()
// (0x00409B70); vector<CWideString> names; GetProfileNames(names) (0x005955AB); for each name Get().DeleteTempSavesForProfile
// (name) (0x00406770, an empty body on PC); ~vector 0x00412130; then the by-value parameters die: ~CCountedPointer
// <CGraphicDataBank> 0x00419108 (graphics is otherwise unused) and ~CCountedPointer<CMeshDataBank> 0x0042918C.
// Names: CManager / CFrontEndManager names are Ego_r.pdb symbols; DeleteTempSavesForProfile is chosen among Ego_r.pdb
// CUserProfileManager QAEXABVCWideString members (body: empty). SetDefaultValuesForNewProfile 0x00409B70 (NF104 q4446): Ego_r.pdb +
// FableWin.pdb ?SetDefaultValuesForNewProfile@CUserProfileManager@@QAEXXZ; the body fills the NewProfile option copies (parked
// 0x00409B70 draft), its sibling SetDefaultValues is 0x0040C790; landed 0x0059A238 binds the same spelling.
// Layout: retail CFrontEndManager PScreenToDeactivate +0x98, PScreenToActivate +0x9c, Signatures +0xc4 (Ego_r.pdb +0xd8,
// the same -0x14 shift); CManager input pointer +0x20 (stored inline).
// Opt: size TU (unaligned entry) with a frame pointer (`leave`). Shape: the count and the element pointer are read once
// before the loop (retail `dec edi / add esi,4`); re-reading names.size() or names[i] each pass reloads both (192v182).
// Layout (supersedes any local-model note above): engine/CManager.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CManager CManagerDonor
#include "engine/CManager.h"
#undef CManager
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString { public: void* PStringData; };
// NF50 (round 7): the local std::vector / std::map models are the shared stlport headers: Signatures.clear() is STLport map::clear
// (inline) calling the tree's clear out of line (FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE, declaration only), so the call at 0x00595905
// binds ?clear@?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCWideString@@_N@std@@@3@U?$less@VCWideString@@@3@V?$allocator@U?$pair@$$CBVCWideString@@_N@std@@@3@@std@@QAEXXZ
// (nf41 e117/e217; was the local ?clear@?$map@VCWideString@@_N@std@@QAEXXZ); ~vector<CWideString> 0x00412130 stays out of line
// (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE). CWideString stays the row's pre-existing local model.
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "stlport/fable_map.h"
class CGraphicDataBank;
class CMeshDataBank;
class CInputManager;
class CDefinitionManager;
#include "engine/CCountedPointer.h"
class CUserProfileManager {
public:
    void SetDefaultValuesForNewProfile();                             // 0x00409B70
    void DeleteTempSavesForProfile(const CWideString& name);   // 0x00406770
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace NUISystem {
class CComponent;
class CManager : public CManagerDonor {
public:
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
    void SetMeshBank(CCountedPointer<CMeshDataBank> bank);          // 0x0041D198
    void SetDefinitionManager(CDefinitionManager* definitions);    // 0x0041CD89
};
class CFrontEndManager {
public:
    unsigned char                    _pad00[0x98];
    CComponent*                      PScreenToDeactivate;   // +0x98
    CComponent*                      PScreenToActivate;     // +0x9c
    unsigned char                    _padA0[0xc4 - 0xa0];
    std::map<CWideString, bool>      Signatures;            // +0xc4
protected:
    void GetProfileNames(std::vector<CWideString>& names);   // 0x005955AB (Ego_r.pdb + FableWin.pdb: protected, IAE)
public:
    void Init(CCountedPointer<CGraphicDataBank> graphics, CCountedPointer<CMeshDataBank> meshes, CInputManager* input,
              CDefinitionManager* definitions);
};
void CFrontEndManager::Init(CCountedPointer<CGraphicDataBank> graphics, CCountedPointer<CMeshDataBank> meshes,
                            CInputManager* input, CDefinitionManager* definitions)
{
    Signatures.clear();
    CManager::GetInstance()->SetMeshBank(meshes);
    CManager::GetInstance()->PInputManager = input;
    CManager::GetInstance()->SetDefinitionManager(definitions);
    PScreenToActivate = 0;
    PScreenToDeactivate = 0;
    CUserProfileManager::Get().SetDefaultValuesForNewProfile();
    std::vector<CWideString> names;
    GetProfileNames(names);
    const unsigned int count = names.size();
    CWideString* name = names.begin();
    for (unsigned int i = 0; i < count; ++i)
        CUserProfileManager::Get().DeleteTempSavesForProfile(name[i]);
}
}