// C3DAnimationBlendState::AddCombinationBlend  (retail 0x00a3c310)
// agentNF20 (2026-11-11): the first call is the private Ego_r.pdb / FableWin.pdb ?MakeOwnCopyOfData@C3DAnimationBlendState@@AAEXXZ
//   0x00A3C1A0 (copy-on-write of the shared data, as every setter here; was the placeholder Foo).
class C3DAnimationStateAlpha;

// Subobject living at offset +0xc inside the member-4 object.
struct BlendSub {
    void Bar(C3DAnimationStateAlpha* a);   // declared only -> real call rel32
};

struct MemberFour {
    char pad[0xc];
    BlendSub sub;   // at +0xc
};

class C3DAnimationBlendState { public:
    void* m_pad0;         // at +0
    MemberFour* m_four;   // at +4
private:
    void MakeOwnCopyOfData();   // 0x00A3C1A0
public:
    void AddCombinationBlend(C3DAnimationStateAlpha* alpha);
};

void C3DAnimationBlendState::AddCombinationBlend(C3DAnimationStateAlpha* alpha)
{
    this->MakeOwnCopyOfData();
    this->m_four->sub.Bar(alpha);
}