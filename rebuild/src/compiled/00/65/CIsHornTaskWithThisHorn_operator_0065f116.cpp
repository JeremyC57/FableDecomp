// CIsHornTaskWithThisHorn::operator()(CCountedPointer<CVillageTaskHorn> task) const -- the functor holds an intelligent pointer to a horn (PHorn): true when the task's horn pointer (CVillageTaskHorn::PHorn, +8 of the task) is the same thing; the by-value counted task is released at the end (~CCountedPointer<CVillageTaskHorn> 0x0065D5A0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F116, length 42 bytes.
// Shape: GetPItem (0x00A01B50) on PHorn then on the task's PHorn, `sete bl`, the parameter destroyed last, `mov al,bl`, `ret 8`.
// Names: FableWin.pdb ??RCIsHornTaskWithThisHorn@@QBE_NV?$CCountedPointer@VCVillageTaskHorn@@@@@Z (const); Ego_r.pdb layout
//   CIsHornTaskWithThisHorn {CIntelligentPointer<CThing> PHorn +0}; CBaseIntelligentPointer::GetPItem() const returns CBaseObject*
//   (Ego_r + FableWin); the parameter's dtor is the CCountedPointer<CVillageTaskHorn> COMDAT 0x0065D5A0 (called out of line, so the
//   template is declared here without the inline bodies of engine/CCountedPointer.h).
// Layout: engine/CVillageTaskHorn.h (flattens the vptr and keeps PHorn as a byte image at +0x08).
#pragma optimize("s", on)
#include "engine/CVillageTaskHorn.h"
class CBaseObject;
class CThing;
class CCPPointerInfo;
class CBaseIntelligentPointer { public:
    virtual void v00();
    CBaseObject* PItem;                            // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
template <class T> class CCountedPointer { public:
    T*              Data;                          // +0x00
    CCPPointerInfo* Info;                          // +0x04
    ~CCountedPointer();                            // 0x0065D5A0 for CVillageTaskHorn
};
class CIsHornTaskWithThisHorn { public:
    CIntelligentPointer<CThing> PHorn;             // +0x00
    bool operator()(CCountedPointer<CVillageTaskHorn> task) const;
};

bool CIsHornTaskWithThisHorn::operator()(CCountedPointer<CVillageTaskHorn> task) const
{
    CBaseObject* horn = PHorn.GetPItem();
    bool same = ((const CBaseIntelligentPointer*)task.Data->PHorn)->GetPItem() == horn;
    return same;
}
