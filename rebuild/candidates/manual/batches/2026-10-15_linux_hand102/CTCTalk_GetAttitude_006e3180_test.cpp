// Behaviour test for CTCTalk::GetAttitude @ 0x006E3180: 10 when interface 49 is present and its mode type is 0x56, else Attitude.
#include <cstdio>
struct CMode { long type; long GetModeType() { return type; } };
struct CModeComponent { CMode* mode; CMode* GetCurrentMode() { return mode; } };
struct SEntry { long type; void* component; };
struct CThing {
    unsigned long bits[9]; SEntry entries[3]; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* t) { SEntry* p = first; while (p != last && p->type < *t) ++p; return p; }
    template<class T> bool GetInterface(T** out, long type)
    {
        if (bits[type >> 5] & (1UL << (type & 31))) {
            long key = type; SEntry* it = LowerBound(&key);
            if (it == last || type < it->type) it = last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
struct CTCTalk {
    CThing* ParentThing; long Attitude;
    long GetAttitude()
    {
        CModeComponent* modes;
        if (ParentThing->GetInterface(&modes, 0x31)) { if (modes->GetCurrentMode()->GetModeType() == 0x56) return 10; }
        return Attitude;
    }
};
int main()
{
    CThing th; for (int i = 0; i < 9; ++i) th.bits[i] = 0;
    CMode m; m.type = 0x56; CModeComponent mc; mc.mode = &m;
    th.entries[0].type = 4; th.entries[0].component = 0; th.entries[1].type = 0x31; th.entries[1].component = &mc;
    th.first = th.entries; th.last = th.entries + 2;
    CTCTalk t; t.ParentThing = &th; t.Attitude = 3;
    if (t.GetAttitude() != 3) { std::printf("006e3180_TEST FAIL\n"); return 1; }
    th.bits[1] = 0x20000;
    if (t.GetAttitude() != 10) { std::printf("006e3180_TEST FAIL\n"); return 1; }
    m.type = 0x55;
    if (t.GetAttitude() != 3) { std::printf("006e3180_TEST FAIL\n"); return 1; }
    std::printf("006e3180_TEST PASS\n");
    return 0;
}
