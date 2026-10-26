class C3DAnimationStateAlpha;

class CTCPerceiveThing { public:
    void helperB(C3DAnimationStateAlpha* a); // @0x686910 __fastcall(this, a on stack)
    void AddCombinationBlend(C3DAnimationStateAlpha* a);
public:
    void ResetPerceptionVariables();   // 0x00744DB0
};

void CTCPerceiveThing::AddCombinationBlend(C3DAnimationStateAlpha* a)
{
    ResetPerceptionVariables();
    this->helperB(a);
}