// CTCDegradableBase::OnHitByMelee(const C3DVector& position, const C3DVector& direction, CThing& attacker) -- forward the hit to the virtual OnHit(position, direction)
// Leaf: OnHitByMelee
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC830, length 18 bytes.
// Callees: virtual CTCDegradableBase::OnHit(const C3DVector&, const C3DVector&) (slot 30, `call [eax+78h]`; 0x008ECC80).
// Slot 28 of the CTCDegradableBarrier vtable 0x0126A6A4 (reference 0x0126A714) and of the CTCDegradableBase vtable
// 0x01287F2C (reference 0x01287F9C): the chunk label "CTCDegradableBarrier::vfunc_28" names the inheritor. Name and
// signature from ghidra_out/egor_signatures.tsv (`?OnHitByMelee@CTCDegradableBase@@UAEXABVC3DVector@@0AAVCThing@@@Z`);
// the egor function order (IsAvailableToHit, SetAvailabilityToHitFromFrame, OnHitByShot, OnHitByMelee) matches the
// retail order 0x008EC7E0, 0x008EC800, 0x008EC810, 0x008EC830, so 0x008EC810 is OnHitByShot (slot 29) and
// 0x008EC830 OnHitByMelee (slot 28); the two bodies are byte-identical.
// Shape: `ret 0Ch` (three arguments, the attacker unused); the two references are re-pushed from the
// caller's slots around the vtable load; a plain call (the callee pops 8, the caller 12: no sibling jump).
class C3DVector;
class CThing;

class CTCDegradableBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void OnHitByMelee(const C3DVector& position, const C3DVector& direction, CThing& attacker);   // slot 28
    virtual void OnHitByShot(const C3DVector& position, const C3DVector& direction, CThing& attacker);    // slot 29
    virtual void OnHit(const C3DVector& position, const C3DVector& direction);                            // slot 30
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    long          LastWorldFrameHit;                        // +0x0c
};

void CTCDegradableBase::OnHitByMelee(const C3DVector& position, const C3DVector& direction, CThing& attacker)
{
    OnHit(position, direction);
}