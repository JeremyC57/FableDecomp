#ifndef FABLE_ENGINE_NCAMERAMODES_CPCMOUSELOOK_H
#define FABLE_ENGINE_NCAMERAMODES_CPCMOUSELOOK_H

#ifdef FABLE_RETAIL_PCMOUSELOOK_PASS24
#include "engine/NCameraModes_CBase.h"

namespace NCameraModes {
class CPCMouseLook : public CBase {
public:
    CPCMouseLook(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&);
    virtual ~CPCMouseLook();
    virtual void Update();
    virtual EMode GetModeType() const;
    virtual void OnCameraInsideObservedThing();
    virtual bool IsShortenOnOcclusion() const;
protected:
    virtual void OnInit();
    virtual void OnDie();
    void Recentre();
private:
    struct CRandomPacketOfVariablesDefiningTheCameraState {
        float AngleXY;
        float AngleUpDown;
        float DollyLength;
        float DeltaAngleXY;
        long RecentreTimer;
        C3DVector ViewPosOffset;
    };
    std::list<C2DVector> MouseInputs;
    C3DVector AxisX;
    C3DVector AxisY;
    C3DVector AxisZ;
    CRandomPacketOfVariablesDefiningTheCameraState StateCurrent;
    CRandomPacketOfVariablesDefiningTheCameraState StateLast;
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CPCMouseLook) == 0x10c);
#endif
#endif
