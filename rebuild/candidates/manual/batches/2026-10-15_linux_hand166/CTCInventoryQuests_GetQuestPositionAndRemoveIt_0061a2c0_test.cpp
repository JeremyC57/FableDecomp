// Behaviour test for 0x0061A2C0 CTCInventoryQuests::GetQuestPositionAndRemoveIt
#include <cstdio>
struct CThing { unsigned short DefIndex; unsigned char Dead; };
struct CPtr { CThing* p; CThing* GetPItem() const { return p; } };
struct CVec {
    CPtr data[8]; CPtr* _M_start; CPtr* _M_finish;
    void erase(CPtr* pos) { for (CPtr* q = pos + 1; q != _M_finish; ++q) *(q - 1) = *q; --_M_finish; }
};
static long GetQuestPositionAndRemoveIt(CVec& v, long def)
{
    for (CPtr* it = v._M_start; it != v._M_finish; ++it) {
        CThing* quest = it->GetPItem();
        if (quest && !quest->Dead && quest->DefIndex == def) {
            long position = (long)(it - v._M_start);
            v.erase(it);
            return position;
        }
    }
    return -1;
}
int main()
{
    CThing a = { 5, 0 }, b = { 7, 1 }, c = { 7, 0 };
    CVec v; v.data[0].p = &a; v.data[1].p = 0; v.data[2].p = &b; v.data[3].p = &c;
    v._M_start = v.data; v._M_finish = v.data + 4;
    bool ok = GetQuestPositionAndRemoveIt(v, 7) == 3 && v._M_finish - v._M_start == 3
           && GetQuestPositionAndRemoveIt(v, 9) == -1 && GetQuestPositionAndRemoveIt(v, 5) == 0
           && v.data[0].p == 0;
    if (!ok) { std::printf("0061a2c0_TEST FAIL\n"); return 1; }
    std::printf("0061a2c0_TEST PASS\n");
    return 0;
}
