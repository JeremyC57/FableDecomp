// Behaviour test for 0x00794180 CTCUnholyPower::StopSound (see src/00794180.cpp): when the parent thing's component bitset has
// interface 16 the sorted component map is searched (lower bound, end() when absent) and the sound player's
// StopAttachedSound gets the sound handle; without the bit nothing is looked up or stopped.
#include <cstdio>
struct Player { unsigned long Stopped; int Calls; void StopAttachedSound(unsigned long s) { Stopped = s; ++Calls; } };
struct Entry { long first; Player* second; };
struct Thing {
    unsigned long Using[9]; Entry* Begin; Entry* End; int Lookups;
    Entry* LowerBound(long k) { ++Lookups; Entry* e = Begin; while (e != End && e->first < k) ++e; return e; }
    bool GetInterface(Player** out, long type)
    {
        if (Using[type >> 5] & (1UL << (type & 31))) {
            Entry* it = LowerBound(type);
            *out = ((it == End || type < it->first) ? End : it)->second;
            return true;
        }
        return false;
    }
};
static void StopSound(Thing* parent, unsigned long sound)
{
    Player* player;
    if (parent->GetInterface(&player, 16)) player->StopAttachedSound(sound);
}
int main()
{
    Player p = { 0, 0 };
    Entry map[3] = { { 3, 0 }, { 16, &p }, { 40, 0 } };
    Thing t = { { 0x10000, 0, 0, 0, 0, 0, 0, 0, 0 }, map, map + 3, 0 };
    StopSound(&t, 77);
    bool ok = p.Calls == 1 && p.Stopped == 77 && t.Lookups == 1;
    Thing u = { { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, map, map + 3, 0 };
    StopSound(&u, 5);
    ok = ok && p.Calls == 1 && u.Lookups == 0;
    std::printf(ok ? "00794180_TEST PASS\n" : "00794180_TEST FAIL\n");
    return ok ? 0 : 1;
}