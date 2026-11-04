// Behaviour test for CSoundEmitterLH::GetActiveSoundInfo (0x00AAC5E0): a model of the row -- with the sound system up, every
// active sound whose +0x0c / +0x18 words equal the two keys appends an info record {+0x18, +0x0c, +0x10}; without it nothing.
#include <stdio.h>
#include <list>
#include <vector>
struct Sound { long Type, Handle, Id; };            // +0x0c, +0x10, +0x18
struct Info { long Id, Type, Handle; };
static void Collect(bool systemUp, const std::list<Sound*>& sounds, long key0c, long key18, std::vector<Info>* out)
{
    if (!systemUp) return;
    for (std::list<Sound*>::const_iterator it = sounds.begin(); it != sounds.end(); ++it) {
        Sound* s = *it;
        if (s->Type == key0c && s->Id == key18) { Info i = { s->Id, s->Type, s->Handle }; out->push_back(i); }
    }
}
int main()
{
    Sound a = { 1, 100, 7 }, b = { 2, 200, 7 }, c = { 1, 300, 7 }, d = { 1, 400, 8 };
    std::list<Sound*> l; l.push_back(&a); l.push_back(&b); l.push_back(&c); l.push_back(&d);
    std::vector<Info> v; Collect(true, l, 1, 7, &v);
    std::vector<Info> w; Collect(false, l, 1, 7, &w);
    int ok = v.size() == 2 && v[0].Handle == 100 && v[1].Handle == 300 && v[1].Id == 7 && v[1].Type == 1 && w.empty();
    printf(ok ? "00aac5e0_TEST PASS\n" : "00aac5e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
