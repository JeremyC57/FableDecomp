class C3DAnimationStateAlpha;

class CTCPerceiveThing { public:
    void helperA();                          // @0x744db0 __fastcall(this)
    void helperB(C3DAnimationStateAlpha* a); // @0x686910 __fastcall(this, a on stack)
    void AddCombinationBlend(C3DAnimationStateAlpha* a);
};

void CTCPerceiveThing::AddCombinationBlend(C3DAnimationStateAlpha* a)
{
    this->helperA();
    this->helperB(a);
}