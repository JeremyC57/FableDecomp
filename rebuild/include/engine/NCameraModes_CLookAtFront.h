#ifndef FABLE_ENGINE_NCAMERAMODES_CLOOKATFRONT_H
#define FABLE_ENGINE_NCAMERAMODES_CLOOKATFRONT_H
#ifdef FABLE_RETAIL_NCAMERAMODES_LOOKATFRONT_PASS27
#include "engine/NCameraModes_CSimplePointBase.h"
#include "engine/NCameraModes_CBaseInit.h"
namespace NCameraModes
{
class CLookAtFront : public CSimplePointBase
{
public:
    CLookAtFront(CGameCameraManager& cam_man, const CDefPointer<const CCameraModeDef>& cam_mode_def,
                 const C3DVector& start_pos, const C3DVector& look_at, bool allow_look_around);
    virtual EMode GetModeType() const;
    static CBaseInit* FABLE_FASTCALL StatAlloc();
    static EMode FABLE_FASTCALL StatGetModeType();

    class CLookAtFrontInit : public CBaseInit
    {
    public:
        virtual CBase* Alloc(CGameCameraManager& cam_man, const CDefPointer<const CCameraModeDef>& cam_mode_def) const;
    };
};
FABLE_STATIC_ASSERT(sizeof(CLookAtFront)==0xcc);
}
#endif
#endif
