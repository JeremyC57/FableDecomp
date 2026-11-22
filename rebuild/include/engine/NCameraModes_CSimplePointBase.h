#ifndef FABLE_ENGINE_NCAMERAMODES_CSIMPLEPOINTBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CSIMPLEPOINTBASE_H
#ifdef FABLE_RETAIL_NCAMERAMODES_LOOKATFRONT_PASS27
// Ego_r CSimplePointBase data and FableWin virtuals and constructor in cm_first_person.obj.
// The retail constructor at 0x00873A10 copies Position and LookAtPos, then stores AllowLookAround. StartPos has its default constructor.
#include "engine/NCameraModes_CBase.h"
namespace NCameraModes
{
class CSimplePointBase : public CBase
{
public:
    CSimplePointBase(CGameCameraManager& cam_man, const CDefPointer<const CCameraModeDef>& cam_mode_def,
                     const C3DVector& start_pos, const C3DVector& look_at, bool allow_look_around);
    virtual void OnInit();
    virtual void Update();
    virtual void OnDie();
    virtual bool IsUseRightStickLookAround() const;

private:
    C3DVector StartPos;
    C3DVector Position;
    C3DVector LookAtPos;
    bool AllowLookAround;
};
FABLE_STATIC_ASSERT(sizeof(CSimplePointBase)==0xcc);
}
#endif
#endif
