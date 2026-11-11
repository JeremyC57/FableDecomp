#ifndef FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGEBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGEBASE_H
#include "engine/NCameraModes_CBase.h"
// Layout: Ego_r class0xec, base0xc8; native base0xa4 and class0xc8.
// Exact private members and access independently present in both original PDBs.
namespace NCameraModes {
class CStringAndCageBase : public CBase {
#ifdef FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_CONSTRUCT
public:
    virtual void Update();
    CStringAndCageBase(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,float,float,float);
#endif
private:
    float DesiredHeightOffset,DesiredStringLength,DesiredCageRadius;
    float CurrentHeightOffset,CurrentStringLength,CurrentCageRadius;
    long NextAutoGoBehindWF,TimerForExpansion;
    bool DriftToMaxStringLength;
    void UpdateXYVars();
#ifdef FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsShortenOnOcclusion() const;  // slot 6, 0x0069DD10
public:
    virtual bool IsAvoidLandscape() const;  // slot 9, 0x0069DD30
public:
    virtual bool IsStringAndCage() const;  // slot 14, 0x0069DD20
public:
    virtual void OnDie();  // slot 22, 0x0069DD00
protected:
    virtual void SetUpPhysics();  // slot 26, 0x0069DD80
protected:
    virtual bool IsZTargeting() const;  // slot 27, 0x0069DD90
protected:
    virtual bool IsAutoGoBehind() const;  // slot 28, 0x0069DDA0
protected:
    virtual bool IsDriftToMaxStringLength() const;  // slot 31, 0x0069DDE0
public:
#endif
};
}
#endif
