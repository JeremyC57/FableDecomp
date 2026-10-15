#include <cstdio>
// CSoundEmitterLH::RemoveInactiveSounds model: erase sounds whose state is clear and progress >= 1, record "any left".
struct CActiveSound { long handle; float progress; long state; };
struct SNode { SNode* next; SNode* prev; CActiveSound* sound; };
struct CSoundList {
    SNode* head;
    SNode* erase(SNode* n) { SNode* nx = n->next; n->prev->next = nx; nx->prev = n->prev; return nx; }
    unsigned long size() const { unsigned long c = 0; for (SNode* n = head->next; n != head; n = n->next) ++c; return c; }
};
struct CSoundEmitterLH {
    bool systemUp; CSoundList sounds; bool hasSounds;
    void RemoveInactiveSounds()
    {
        if (systemUp) {
            SNode* it = sounds.head->next;
            while (it != sounds.head) {
                CActiveSound* s = it->sound;
                long state = s->state;
                float progress = s->progress;
                if (state == 0 && progress >= 1.0f) it = sounds.erase(it);
                else it = it->next;
            }
            hasSounds = sounds.size() != 0;
        }
    }
};
int main()
{
    CActiveSound a = { 1, 1.0f, 0 }, b = { 2, 0.5f, 0 }, c = { 3, 2.0f, 1 };
    SNode h, na, nb, nc;
    h.next = &na; na.prev = &h; na.next = &nb; nb.prev = &na; nb.next = &nc; nc.prev = &nb; nc.next = &h; h.prev = &nc;
    na.sound = &a; nb.sound = &b; nc.sound = &c;
    CSoundEmitterLH e; e.systemUp = true; e.sounds.head = &h; e.hasSounds = false;
    e.RemoveInactiveSounds();
    if (!(e.sounds.size() == 2 && h.next == &nb && e.hasSounds)) { std::printf("00aabca0_TEST FAIL\n"); return 1; }
    std::printf("00aabca0_TEST PASS\n");
    return 0;
}
