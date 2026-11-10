#ifndef FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGE_H
#define FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGE_H
#include "engine/NCameraModes_CStringAndCageBase.h"
namespace NCameraModes {
class CStringAndCage:public CStringAndCageBase {
public:
 CStringAndCage(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,float,float,float,bool,float,float);
 virtual EMode GetModeType()const;
private:
 bool AutoGoBehind;
 float AutoGoBehindTime;
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CStringAndCage)==208);
#endif
