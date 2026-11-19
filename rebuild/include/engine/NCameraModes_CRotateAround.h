#ifndef FABLE_ENGINE_NCAMERAMODES_CROTATEAROUND_H
#define FABLE_ENGINE_NCAMERAMODES_CROTATEAROUND_H
// NCameraModes::CRotateAround (NEW, hand4a, row 0x0087BD70): RTTI : NCameraModes::CBase, vtable 0x0125C674. Members from Ego_r.pdb
// struct_layouts_egor.tsv "CRotateAround" (StringLengthXY 200, HeightOffset 204, XAxis 208, YAxis 220, Theta 232; sizeof 236), 0x24 lower
// in retail (STLport CBase 0xA4 against the donor's 0xC8), as the landed OnInit 0x0087BF40 reads them; sizeof 0xC8. Virtual
// declarations: Ego_r / FableWin name set (?Update@CRotateAround@NCameraModes@@UAEXXZ, ?OnInit@..MAEXXZ, ...). Needs
// FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER.
#include "engine/NCameraModes_CBase.h"
#include "engine/C3DVector.h"
namespace NCameraModes {
class CRotateAround : public CBase {
public:
    virtual void Update();
    virtual EMode GetModeType() const;
    virtual bool IsShortenOnOcclusion() const;
    virtual bool IsFOVChange() const;
    virtual bool IsAvoidLandscape() const;
    virtual float GetDefaultFOV() const;
    virtual float GetDefaultRollAngle() const;
    virtual float GetDefaultObservedPositionZOffset() const;
protected:
    virtual void OnInit();
    virtual void OnDie();
private:
    float StringLengthXY;                                  // +0xa4
    float HeightOffset;                                    // +0xa8
    C3DVector XAxis;                                       // +0xac
    C3DVector YAxis;                                       // +0xb8
    float Theta;                                           // +0xc4
};
FABLE_STATIC_ASSERT(sizeof(CRotateAround) == 0xc8);
}
#endif
