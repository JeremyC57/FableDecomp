// C3DAnimationBlendState::AddCombinationBlend @ 0099ef90

class C3DAnimationStateAlpha;

struct Inner {
    void addAlpha(C3DAnimationStateAlpha* a);   // extern __fastcall @ 0x9a01c0
};

class C3DAnimationBlendState { public:
    Inner* pInner;      // offset 0: mov ecx,[esi]
    void helperA();     // extern __fastcall @ 0x99eb40
};

void __fastcall AddCombinationBlend(C3DAnimationBlendState* self, int /*edx*/, C3DAnimationStateAlpha* a)
{
    self->helperA();
    self->pInner->addAlpha(a);
}