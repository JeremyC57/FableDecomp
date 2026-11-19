// Grade: DIFFER(185v192) -- structure/registers: retail keeps the target position in the frame (eax/ecx copy) and tests !parent and IsPositionAbsolute before re-testing it, ours holds the position in ebx/ebp and orders the returns differently; x87 result build differs | tried: inline ToParentSpace helper (not inlined), __forceinline helper, explicit three-return body | next: lane gap: x87 temporaries / return-path layout
// NUISystem::CComponent::GetTargetRenderPosition() -- the target position, scaled by the zoom and offset by the parent's render position when the component has a parent and is not absolutely positioned
// Leaf: GetTargetRenderPosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EE00, 192 bytes, `ret 4` (C2DVector by hidden pointer).
// Callees: the component's virtual slot 102 (+0x198, twice: the outer test and the inlined helper's), the parent's virtual
//   slot 112 (+0x1c0, its render position by hidden pointer).
// Layout: TargetPosition +0x3c, Zoom +0x74/+0x78, the parent pointers +0x118 (preferred) and +0xc8 (retail).
// Shape: an inline ToParentSpace(pos) re-tests IsPositionAbsolute (the second virtual call); three return paths.
struct C2DVector {
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
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
    virtual void v031();
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
    virtual bool IsPositionAbsolute();         // slot 102 (+0x198)
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual C2DVector GetRenderPosition();     // slot 112 (+0x1c0)
    unsigned char _pad04[0x3c - 4];
    C2DVector TargetPosition;                            // +0x3c
    unsigned char _pad44[0x74 - 0x44];
    C2DVector Zoom;                                      // +0x74
    unsigned char _pad7c[0xc8 - 0x7c];
    CComponent* PContainer;                              // +0xc8
    unsigned char _padcc[0x118 - 0xcc];
    CComponent* PParent;                                 // +0x118
    CComponent* GetParent() const { CComponent* p = PParent; if (!p) p = PContainer; return p; }
    __forceinline C2DVector ToParentSpace(C2DVector pos, CComponent* parent)
    {
        if (IsPositionAbsolute())
            return pos;
        pos.X *= Zoom.X;
        pos.Y *= Zoom.Y;
        C2DVector offset = parent->GetRenderPosition();
        return C2DVector(pos.X + offset.X, pos.Y + offset.Y);
    }
    C2DVector GetTargetRenderPosition();
};

C2DVector CComponent::GetTargetRenderPosition()
{
    C2DVector pos = TargetPosition;
    CComponent* parent = GetParent();
    if (parent == 0 || IsPositionAbsolute())
        return pos;
    if (IsPositionAbsolute())
        return C2DVector(pos.X, pos.Y);
    pos.X *= Zoom.X;
    pos.Y *= Zoom.Y;
    C2DVector offset = parent->GetRenderPosition();
    return C2DVector(pos.X + offset.X, pos.Y + offset.Y);
}

} // namespace NUISystem
