// Behaviour test for 0x0071CD30 CTCHeroMorph::FrameUpdate: every particle-emitter entry whose intelligent pointer is empty is
// unlinked, destroyed and freed (live entries stay, in order), then UpdateIdleParticles and the virtual DoMorphUpdate run.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_pointerDtors = 0, g_frees = 0, g_idle = 0, g_morph = 0;
class CThing { public: int id; };
class CIntelligentPointer { public:
    CThing* PItem;
    virtual ~CIntelligentPointer() { ++g_pointerDtors; }
    CThing* Get() const { return PItem; }
};
struct Node { Node* next; Node* prev; CIntelligentPointer data; };
class CTCBase { public: virtual ~CTCBase() { } virtual void FrameUpdate() { } };
class CTCAppearanceMorphBase : public CTCBase { protected: virtual void DoMorphUpdate() { } };
class CTCHeroMorph : public CTCAppearanceMorphBase { public:
    Node* ParticleEmitters;                                  // the list's sentinel
    virtual void FrameUpdate()
    {
        for (Node* it = ParticleEmitters->next; it != ParticleEmitters; ) {
            if (it->data.Get() == 0) {
                Node* next = it->next; Node* prev = it->prev;
                prev->next = next; next->prev = prev;
                it->data.~CIntelligentPointer();
                ++g_frees; std::free(it);
                it = next;
            } else
                it = it->next;
        }
        UpdateIdleParticles();
        DoMorphUpdate();
    }
protected:
    virtual void DoMorphUpdate() { ++g_morph; }
private:
    void UpdateIdleParticles() { ++g_idle; }
};
static Node* Push(Node* s, CThing* t)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    new (&n->data) CIntelligentPointer;
    n->data.PItem = t; n->next = s; n->prev = s->prev; s->prev->next = n; s->prev = n;
    return n;
}
int main()
{
    CThing a, b; a.id = 1; b.id = 2;
    Node s; s.next = s.prev = &s;
    Push(&s, 0); Push(&s, &a); Push(&s, 0); Push(&s, 0); Push(&s, &b);
    CTCHeroMorph morph; morph.ParticleEmitters = &s;
    CTCBase* base = &morph;
    base->FrameUpdate();
    int ok = g_frees == 3 && g_pointerDtors == 3 && g_idle == 1 && g_morph == 1
          && s.next->data.Get() == &a && s.next->next->data.Get() == &b && s.next->next->next == &s && s.prev->data.Get() == &b;
    base->FrameUpdate();                                     // nothing left to drop
    if (g_frees != 3 || g_idle != 2 || g_morph != 2) ok = 0;
    if (!ok) { std::printf("0071cd30_TEST FAIL\n"); return 1; }
    std::printf("0071cd30_TEST PASS\n");
    return 0;
}