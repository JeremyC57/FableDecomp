#ifndef FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGE_H
#define FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGE_H
#include "engine/NCameraModes_CStringAndCageBase.h"
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
private:
 bool AutoGoBehind;
 float AutoGoBehindTime;
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CStringAndCage)==208);
#endif
