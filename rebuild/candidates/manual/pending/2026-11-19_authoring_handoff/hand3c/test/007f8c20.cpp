// Behaviour test for CTCAnimatingObject::AddQueuedAnimation (0x007F8C20), a portable model of the row. When the def is present and has an
// animation of that name, it appends one CAnimInit (type 6, type flags 0, the first entry's bank index, update mode 1 (from speed),
// looping as given, flags 0, priority 100) to the back of the queue. Otherwise the queue is unchanged.
#include <stdio.h>
#include <string.h>
#include <list>
struct CAnimInit { unsigned char Type; unsigned long TypeFlags; long Anim; int Mode; bool Looping; unsigned long Flags; long Priority; };
struct Def { const char* names[2]; long banks[2]; };
struct Obj {
    const Def* def; std::list<CAnimInit> QueuedAnims;
    void AddQueuedAnimation(const char* anim_name, bool looping)
    {
        if (!def) return;
        for (int i = 0; i < 2; ++i)
            if (def->names[i] && !strcmp(def->names[i], anim_name))
            {
                CAnimInit anim_init = { 6, 0, def->banks[i], 1, looping, 0, 100 };
                QueuedAnims.push_back(anim_init);
                return;
            }
    }
};
int main()
{
    int bad = 0;
    Def d = { { "IDLE", "OPEN" }, { 7, 9 } };
    Obj none = { 0 };
    none.AddQueuedAnimation("IDLE", true);
    if (!none.QueuedAnims.empty()) ++bad;                                  // no def: nothing queued
    Obj o = { &d };
    o.AddQueuedAnimation("MISSING", true);
    if (!o.QueuedAnims.empty()) ++bad;                                     // unknown name: nothing queued
    o.AddQueuedAnimation("OPEN", false);
    o.AddQueuedAnimation("IDLE", true);
    if (o.QueuedAnims.size() != 2) ++bad;
    const CAnimInit& a = o.QueuedAnims.front(); const CAnimInit& b = o.QueuedAnims.back();
    if (a.Anim != 9 || a.Looping || a.Type != 6 || a.Mode != 1 || a.Priority != 100) ++bad;   // pushed at the back, in order
    if (b.Anim != 7 || !b.Looping) ++bad;
    printf(bad ? "007f8c20_TEST FAIL\n" : "007f8c20_TEST PASS\n");
    return bad ? 1 : 0;
}
