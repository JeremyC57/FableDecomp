#include <cstdio>
#include <cstring>
// CSoundEmitterLH::StopSoundsContaining model: stop live sounds whose name contains the text, mark them -1, prune.
struct CActiveSound { long handle; const char* name; };
struct SNode { SNode* next; SNode* prev; CActiveSound* sound; };
static int g_stops = 0;
struct CSoundEmitterLH {
    bool systemUp; SNode* head; int prunes;
    void RemoveInactiveSounds() { ++prunes; }
    void StopSoundsContaining(const char* text)
    {
        if (systemUp) {
            for (SNode* n = head->next; n != head; n = n->next) {
                CActiveSound* s = n->sound;
                if (s->handle != -1 && std::strstr(s->name ? s->name : "", text ? text : "")) {
                    ++g_stops;
                    s->handle = -1;
                }
            }
            RemoveInactiveSounds();
        }
    }
};
int main()
{
    CActiveSound a = { 5, "DRAGON_ROAR" }, b = { 6, "WIND" }, c = { -1, "DRAGON_WING" };
    SNode h, na, nb, nc;
    h.next = &na; na.next = &nb; nb.next = &nc; nc.next = &h;
    na.sound = &a; nb.sound = &b; nc.sound = &c;
    CSoundEmitterLH e; e.systemUp = true; e.head = &h; e.prunes = 0;
    e.StopSoundsContaining("DRAGON");
    if (!(a.handle == -1 && b.handle == 6 && g_stops == 1 && e.prunes == 1)) { std::printf("00aac460_TEST FAIL\n"); return 1; }
    std::printf("00aac460_TEST PASS\n");
    return 0;
}
