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
#ifdef FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS_MORE
// agentFAM04 (2026-11-11): slots 24 / 25 of the CStringAndCageBase vtable 0x0125C37C (Ego_r / FableWin
// ?GetStringBaseOffset@CStringAndCageBase@NCameraModes@@MBE?AVC3DVector@@XZ, ?GetCageBaseOffset@..@MBE?AVC3DVector@@XZ), nested in the
// VSLOTS block so the declaration order stays the slot order. Declarations only.
    virtual C3DVector GetStringBaseOffset() const;  // slot 24, 0x0069DD40
    virtual C3DVector GetCageBaseOffset() const;  // slot 25, 0x0069DD60
protected:
#endif // FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS_MORE
    virtual void SetUpPhysics();  // slot 26, 0x0069DD80
protected:
    virtual bool IsZTargeting() const;  // slot 27, 0x0069DD90
protected:
    virtual bool IsAutoGoBehind() const;  // slot 28, 0x0069DDA0
protected:
#ifdef FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS_MORE2
// agentFAM04 (2026-11-11): slot 29 (Ego_r / FableWin ?GetAutoGoBehindTime@CStringAndCageBase@NCameraModes@@MBEMXZ); slot 30 is not
// declared (no row needs it), so a TU with this block has slot 31 one place early: declarations only, no row calls these. 
    virtual float GetAutoGoBehindTime() const;  // slot 29, 0x0069DDB0
protected:
#endif // FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS_MORE2
    virtual bool IsDriftToMaxStringLength() const;  // slot 31, 0x0069DDE0
public:
#endif
};
}
#endif
