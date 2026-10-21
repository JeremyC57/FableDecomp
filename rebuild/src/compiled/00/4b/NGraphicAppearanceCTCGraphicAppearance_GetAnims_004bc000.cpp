// NGraphicAppearance::CTCGraphicAppearance::GetAnims(C3DAnimationInterpolator&) const -- fill the interpolator's start/end blend states from the animation TC, if there is one
// Leaf: ?GetAnims@CTCGraphicAppearance@NGraphicAppearance@@IBE_NAAVC3DAnimationInterpolator@@@Z
// Retail: Fable.exe @ 0x004BC000, 38 bytes; Ego_r.pdb same symbol. When PTCAnimation (+0x24) is set, calls its slot 29
// (+0x74) with (&interp.StartState +0x04, &interp.EndState +0x0c) and returns true; else false. Slot 29 is pure in
// CTCAnimationBase (vtable 0x01265024, 0x00BFEA08 = _purecall); Ego_r.pdb's only (C3DAnimationBlendState&,
// C3DAnimationBlendState&) virtual of the family is ?GetMeshAnims@CTCAnimationComplex@@UBEXAAVC3DAnimationBlendState@@0@Z
// (next to GetInterpolatedMeshAnim, slot 28).
// Layout: Ego_r.pdb CTCGraphicAppearance PTCAnimation +0x2c; retail +0x24 (the two maps before it are 12-byte STLport
// trees, 8 bytes less in all). C3DAnimationInterpolator as Ego_r.pdb (StartState +0x04, EndState +0x0c). Local models:
// no engine/ headers for these classes.
class C3DAnimationBlendState { public: long Data[2]; };
class C3DAnimationInterpolator {
public:
    long                   _pad00;                       // +0x00
    C3DAnimationBlendState StartState;                   // +0x04
    C3DAnimationBlendState EndState;                     // +0x0c
};
class CTCAnimationBase {                                 // vtable 0x01265024
public:
    virtual ~CTCAnimationBase();                         // slot 0
    virtual void _slot01();
    virtual void _slot02();
    virtual void _slot03();
    virtual void _slot04();
    virtual void _slot05();
    virtual void _slot06();
    virtual void _slot07();
    virtual void _slot08();
    virtual void _slot09();
    virtual void _slot10();
    virtual void _slot11();
    virtual void _slot12();
    virtual void _slot13();
    virtual void _slot14();
    virtual void _slot15();
    virtual void _slot16();
    virtual void _slot17();
    virtual void _slot18();
    virtual void _slot19();
    virtual void _slot20();
    virtual void _slot21();
    virtual void _slot22();
    virtual void _slot23();
    virtual void _slot24();
    virtual void _slot25();
    virtual void _slot26();
    virtual void _slot27();
    virtual void GetInterpolatedMeshAnim(C3DAnimationBlendState& state, float t) const = 0;            // slot 28
    virtual void GetMeshAnims(C3DAnimationBlendState& start, C3DAnimationBlendState& end) const = 0;  // slot 29
};
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char     _pad00[0x24];                      // +0x00 CTCBase, MeshEffects, TintColours
    CTCAnimationBase* PTCAnimation;                      // +0x24
protected:
    bool GetAnims(C3DAnimationInterpolator& interpolator) const;
};
bool CTCGraphicAppearance::GetAnims(C3DAnimationInterpolator& interpolator) const
{
    if (PTCAnimation) {
        PTCAnimation->GetMeshAnims(interpolator.StartState, interpolator.EndState);
        return true;
    }
    return false;
}
}