#include <cstdio>
struct CThing {
    unsigned char m_removeFlags;
    int drawable;
    virtual ~CThing() { }
    virtual void SetAsDrawable(bool d) { drawable = d ? 1 : 0; }
};
struct CIntelligentPointer { CThing* p; CThing* GetPItem() const { return p; } };
struct SParticleNode { SParticleNode* m_next; SParticleNode* m_prev; CIntelligentPointer m_value; };
struct CParticleList {
    SParticleNode* m_node;
    SParticleNode* begin() const { return m_node->m_next; }
    SParticleNode* end() const { return m_node; }
};
struct CTCParticleAttachmentTracker {
    CParticleList AttachedParticles;
    void SetAsDrawable(bool drawable)
    {
        for (SParticleNode* it = AttachedParticles.begin(); it != AttachedParticles.end(); it = it->m_next) {
            CIntelligentPointer& particle = it->m_value;
            if (particle.GetPItem() && !(particle.GetPItem()->m_removeFlags & 1))
                particle.GetPItem()->SetAsDrawable(drawable);
        }
    }
};
int main()
{
    CThing a, b; a.m_removeFlags = 0; a.drawable = -1; b.m_removeFlags = 1; b.drawable = -1;
    SParticleNode head, n1, n2, n3;
    head.m_next = &n1; n1.m_next = &n2; n2.m_next = &n3; n3.m_next = &head;
    n1.m_value.p = &a; n2.m_value.p = 0; n3.m_value.p = &b;
    CTCParticleAttachmentTracker t; t.AttachedParticles.m_node = &head;
    t.SetAsDrawable(true);
    if (a.drawable != 1 || b.drawable != -1) { std::printf("0071a8a0_TEST FAIL\n"); return 1; }
    t.SetAsDrawable(false);
    if (a.drawable != 0) { std::printf("0071a8a0_TEST FAIL\n"); return 1; }
    SParticleNode empty; empty.m_next = &empty; t.AttachedParticles.m_node = &empty; t.SetAsDrawable(true);
    if (a.drawable != 0) { std::printf("0071a8a0_TEST FAIL\n"); return 1; }
    std::printf("0071a8a0_TEST PASS\n");
    return 0;
}