class C3DAnimationBlendState;
// Leaf: AddCombinationBlend

struct BlendMgr {
    void Add_0051df70(C3DAnimationBlendState* s);
};

extern "C" void __fastcall helper_6a5230_006a5230(C3DAnimationBlendState* self);

extern BlendMgr* g_theMgr;

void __fastcall AddCombinationBlend(C3DAnimationBlendState* self)
{
    helper_6a5230_006a5230(self);
    g_theMgr->Add_0051df70(self);
}