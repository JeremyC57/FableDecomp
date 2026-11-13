#ifndef FABLE_ENGINE_CCAMERAMANAGERSETDEF_H
#define FABLE_ENGINE_CCAMERAMANAGERSETDEF_H
// Retail model (chunk 2033, row 0x0069AE80 CGameCameraManager::CGameCameraManager; replaces the quarantined generated header
// engine/_quarantine/CCameraManagerSetDef.h, whose offsets it keeps): RTTI .?AVCCameraManagerSetDef@@ : CParentDefClassBase :
// CDefClassBase (ghidra_out/rtti_bases.tsv), vtable 0x01233C4C (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x00456818,
// 19 Copy 0x0045D578, 20 GetSizeofClass 0x00456814).
// Members: Ego_r.pdb CCameraManagerSetDef (80 bytes: CameraManagerMain +0x40, CameraManagerCombat +0x44, CameraManagerPCMain +0x48,
//   CameraManagerPCCombat +0x4C); retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase, sizeof 0x4C.
//   Retail CGameCameraManager::SetCameraManagerSetDef 0x0069AC90 reads +0x44 (`mov eax,[edx+44h]`) = CameraManagerPCMain.
// Virtual overrides: Ego_r.pdb ?Transfer@CCameraManagerSetDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CCameraManagerSetDef@@UAEXPBVCDefClassBase@@@Z
//   and FableWin.pdb ?GetSizeofClass@CCameraManagerSetDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE.
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CCameraManagerSetDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    long          CameraManagerMain;                         // +0x3C
    long          CameraManagerCombat;                       // +0x40
    long          CameraManagerPCMain;                       // +0x44
    long          CameraManagerPCCombat;                     // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CCameraManagerSetDef) == 0x4c);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CCAMERAMANAGERSETDEF_H
