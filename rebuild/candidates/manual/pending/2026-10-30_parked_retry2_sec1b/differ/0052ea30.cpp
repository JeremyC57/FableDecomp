// Grade: DIFFER(181v181) sec1b: residue unchanged (by-value argument copy loads Y after sub esp,8; result copy stores X before the push). Model fix: ConvertX 0x0052E530 is a __fastcall function returning C2DVector by value (non-POD -> hidden result in ecx), `d = ConvertX(d);` replaces the member-on-result LTCG stand-in, same bytes. A same-TU static body of ConvertX makes it worse (private convention, result in eax). Earlier: DIFFER(181v181) -- scheduling only: retail loads both delta dwords before `sub esp,8` and stores the converted X after the UI-scale push; ours loads Y after and stores X before | tried: function-scope converted local (frame 18h), block-scoped converted local (fixed frame), body-form copy ctor | next: lane gap: instruction scheduling
// NUISystem::CComponent::ChangePositionDelta(const C2DVector& delta, float time, bool immediate) -- move the component by a delta (converted to screen units and scaled by the UI scale when it uses screen coordinates) from its current position
// Leaf: ChangePositionDelta
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EA30, 181 bytes (192 with padding), `?ChangePositionDelta@CComponent@NUISystem@@UAEXABVC2DVector@@M_N@Z`.
// Callees: the component's virtual slots 117 (+0x1d4, bool) and 31 (+0x7c, move to position/time/flag), the LTCG converter
//   0x0052E530 (result address in ecx, the vector by value on the stack; manifest "ConvertX" is a propagated label),
//   CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall) and CManager::GetUIScale 0x0041CF47 (C2DVector by hidden pointer;
//   landed caller 0x0055C600).
// Layout: Position +0x34 (X) / +0x38 (Y) (retail; the sibling ChangeTargetPositionDelta 0x0052EAF0 uses +0x3c/+0x40).
// Shape: the converter is a __thiscall member on its result object (GOTCHAS: LTCG struct returns), its result a
//   block-scoped local so the UI-scale temporaries reuse its slot (16-byte frame); each axis is
//   `scale * delta`; the new position is written back into the delta local, whose address is passed to MoveTo.
struct C2DVector {
    float X;
    float Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
C2DVector __fastcall ConvertX(C2DVector source);   // 0x0052E530 (manifest ConvertX: hidden result in ecx, by-value vector, ret 8)


struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    C2DVector GetUIScale();                              // 0x0041CF47
};

namespace NUISystem {

struct CComponent {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void MoveTo(const C2DVector& position, float time, bool immediate);   // slot 31 (+0x7c)
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual bool UsesScreenCoordinates();      // slot 117 (+0x1d4)
    unsigned char _pad04[0x34 - 4];
    C2DVector Position;                                  // +0x34
    C2DVector TargetPosition;                            // +0x3c
    void ChangePositionDelta(const C2DVector& delta, float time, bool immediate);
};

void CComponent::ChangePositionDelta(const C2DVector& delta, float time, bool immediate)
{
    C2DVector d = delta;
    if (UsesScreenCoordinates()) {
        d = ConvertX(d);
        d.X = CFrontEndManager::GetInstance()->GetUIScale().X * d.X;
        d.Y = CFrontEndManager::GetInstance()->GetUIScale().Y * d.Y;
    }
    d.X = d.X + Position.X;
    d.Y = d.Y + Position.Y;
    MoveTo(d, time, immediate);
}

} // namespace NUISystem
