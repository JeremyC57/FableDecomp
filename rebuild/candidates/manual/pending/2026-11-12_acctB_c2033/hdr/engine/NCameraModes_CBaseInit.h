#ifndef FABLE_ENGINE_NCAMERAMODES_CBASEINIT_H
#define FABLE_ENGINE_NCAMERAMODES_CBASEINIT_H
// New header (chunk 2033, row 0x0069AE80 CGameCameraManager::CGameCameraManager). NCameraModes::CBaseInit, the factory every
// camera mode registers with the manager (CGameCameraManager::RegisteredModes, map<EMode, CCountedPointer<CBaseInit> >).
// Evidence: Ego_r.pdb / FableWin.pdb ?AllocMain@CBaseInit@NCameraModes@@QBEPAVCBase@2@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z
// (retail 0x0084D270) and the per-mode overrides ?Alloc@<Mode>Init@<Mode>@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z
// (e.g. CStringAndCageInit 0x00873160). The derived init vtables (0x0125C264 .. 0x0125C2A4) hold one slot, Alloc, and no
// destructor: CCountedPointer<CBaseInit>::DeleteData 0x0069BBA0 is a bare operator delete. CBaseInit has no RTTI vtable in
// retail (abstract; its vptr store is dead in every inline derived constructor). sizeof 4 (the vptr). FableWin.pdb has the
// implicit ??0CBaseInit@NCameraModes@@QAE@XZ (debug build).
class CGameCameraManager;
class CCameraModeDef;
template <class T> class CDefPointer;
namespace NCameraModes {
class CBase;
class CBaseInit {
public:
    virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const = 0;   // slot 0
    CBase* AllocMain(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;          // 0x0084D270
};
}
#endif
