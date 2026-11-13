#ifndef FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGE_H
#define FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGE_H
#include "engine/NCameraModes_CStringAndCageBase.h"
#ifdef FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_INIT
#include "engine/NCameraModes_CBaseInit.h"
#endif
namespace NCameraModes {
class CStringAndCage:public CStringAndCageBase {
public:
 CStringAndCage(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,float,float,float,bool,float,float);
 virtual EMode GetModeType()const;
#ifdef FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_VSLOTS
// agentFAM04 (2026-11-11): CBase / CStringAndCageBase overrides of CStringAndCage (vtable 0x0125C404 slots 18 / 28 / 29;
// Ego_r / FableWin ?GetDefaultFOV@CStringAndCage@NCameraModes@@UBEMXZ, ?IsAutoGoBehind@..@MBE_NXZ, ?GetAutoGoBehindTime@..@MBEMXZ). Declarations only.
 virtual float GetDefaultFOV() const;                              // slot 18, 0x0069DEB0
protected:
 virtual bool IsAutoGoBehind() const;                              // slot 28, 0x0069DEC0
 virtual float GetAutoGoBehindTime() const;                        // slot 29, 0x0069DED0
#endif // FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_VSLOTS
#ifdef FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_INIT
 // Retail opt-in (chunk 2033, row 0x0069AE80 CGameCameraManager::CGameCameraManager): the nested factory and the two statics the
 // manager's constructor expands when it registers the mode. Ego_r.pdb / FableWin.pdb
 // ?Alloc@CStringAndCageInit@CStringAndCage@NCameraModes@@UBE.. (vtable 0x0125C2A4 slot 0 = 0x00873160, landed),
 // FableWin.pdb ?StatAlloc@CStringAndCage@NCameraModes@@SIPAVCBaseInit@2@XZ / ?StatGetModeType@CStringAndCage@NCameraModes@@SI?AW4EMode@2@XZ
 // (no Ego_r.pdb copy: inline in the release build; retail 0x0069B0A1..0x0069B0B5 `new` 4 bytes + vptr 0x0125C2A4, mode 3 stored
 // at 0x0069B0ED). EMode's enumerator names are in no PDB name set, string or reference: the value is written as a cast.
 class CStringAndCageInit : public CBaseInit {
 public:
  virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;   // 0x00873160
 };
 static CBaseInit* FABLE_FASTCALL StatAlloc() { return new CStringAndCageInit; }
 static EMode FABLE_FASTCALL StatGetModeType() { return static_cast<EMode>(3); }
#endif
private:
 bool AutoGoBehind;
 float AutoGoBehindTime;
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CStringAndCage)==208);
#endif
