#ifndef FABLE_ENGINE_NCAMERAMODES_CPCMOUSELOOK_H
#define FABLE_ENGINE_NCAMERAMODES_CPCMOUSELOOK_H

#ifdef FABLE_RETAIL_PCMOUSELOOK_PASS24
#include "engine/NCameraModes_CBase.h"

namespace NCameraModes {
#ifdef FABLE_RETAIL_PCMOUSELOOK_GETMODE_PASS26
enum EMode { CAMERA_MODE_PC_MOUSE_LOOK = 17 };
#endif
class CPCMouseLook : public CBase {
public:
#ifdef FABLE_RETAIL_PCMOUSELOOK_GETMODE_PASS26
    static EMode FABLE_FASTCALL StatGetModeType() {return CAMERA_MODE_PC_MOUSE_LOOK;}
#endif
    CPCMouseLook(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&);
    virtual ~CPCMouseLook();
    virtual void Update();
    virtual EMode GetModeType() const;
    virtual void OnCameraInsideObservedThing();
    virtual bool IsShortenOnOcclusion() const;
protected:
#ifdef FABLE_RETAIL_PCMOUSELOOK_VIRTUAL_ACCESS_PASS26
public:
#endif
    virtual void OnInit();
#ifdef FABLE_RETAIL_PCMOUSELOOK_VIRTUAL_ACCESS_PASS26
protected:
#endif
    virtual void OnDie();
#ifdef FABLE_RETAIL_PCMOUSELOOK_VIRTUAL_ACCESS_PASS26
public:
#endif
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
FABLE_STATIC_ASSERT_AT(37, sizeof(NCameraModes::CPCMouseLook) == 0x10c);
#endif
#endif
