// behaviour test for 0x007ED0A0 CTCCarriedActionUseDrop::vfunc_28
#include <cstdio>
struct Entry { long type; void* component; };
struct CThing {
    unsigned int bits[9]; Entry* first; Entry* last; unsigned char flags6c; int actions;
    unsigned int HasInterface(long t) const { return bits[t >> 5] & (1u << (t & 31)); }
    Entry* LowerBound(const long* t) { Entry* e = first; while (e != last && e->type < *t) ++e; return e; }
    template<class T> bool GetInterface(T** out, long type) {
        if (bits[type >> 5] & (1u << (type & 31))) {
            long k = type; Entry* it = LowerBound(&k);
            if (it == last || type < it->type) it = last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
struct Carrying { CThing* removed; void RemoveThing(CThing* t) { removed = t; } };
struct Drop {
    CThing* ParentThing;
    void vfunc_28(CThing* user)
    {
        Carrying* carrying = 0;
        user->GetInterface(&carrying, 0x46);
        if (ParentThing->HasInterface(0x48) && (user->flags6c & 8)) ++user->actions;
        else carrying->RemoveThing(ParentThing);
    }
};
static CThing mk() { CThing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; t.first = t.last = 0; t.flags6c = 0; t.actions = 0; return t; }
int main()
{
    CThing user = mk(), item = mk();
    Carrying carry; carry.removed = 0;
    Entry e[2]; e[0].type = 0x46; e[0].component = &carry; user.first = e; user.last = e + 1; user.bits[2] = 0x40;
    Drop d; d.ParentThing = &item;
    d.vfunc_28(&user);
    bool ok = carry.removed == &item && user.actions == 0;
    carry.removed = 0; item.bits[2] = 0x100; user.flags6c = 8;
    d.vfunc_28(&user);
    ok = ok && carry.removed == 0 && user.actions == 1;
    user.flags6c = 0;
    d.vfunc_28(&user);
    ok = ok && carry.removed == &item && user.actions == 1;
    if (!ok) { std::printf("007ed0a0_TEST FAIL\n"); return 1; }
    std::printf("007ed0a0_TEST PASS\n");
    return 0;
}