#include <cstdio>
// Behaviour of CTCHeroStats::PeekMarriageInfo (0x0057AC89): copies out the first record for the wife (both UID halves
// must match) and returns true; leaves the output alone and returns false otherwise.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CHeroMarriageInfo { public: u64 WifeUID; u64 MaritalHomeUID; long State; bool GayMarriage; };
struct CHeroMarriageInfoNode { CHeroMarriageInfoNode* Next; CHeroMarriageInfoNode* Prev; CHeroMarriageInfo Info; };
struct CTCHeroStats_Retail {
    CHeroMarriageInfoNode* Marriages_Head;
    bool PeekMarriageInfo(u64 wifeUID, CHeroMarriageInfo* info) const
    {
        CHeroMarriageInfoNode* head = Marriages_Head;
        for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
            if (node->Info.WifeUID == wifeUID) {
                *info = node->Info;
                return true;
            }
        }
        return false;
    }
};
int main()
{
    CHeroMarriageInfoNode head, a, b;
    head.Next = &a; a.Next = &b; b.Next = &head;
    a.Info.WifeUID = 3; a.Info.MaritalHomeUID = 30; a.Info.State = 1; a.Info.GayMarriage = false;
    b.Info.WifeUID = ((u64)1 << 32) | 4; b.Info.MaritalHomeUID = 40; b.Info.State = 2; b.Info.GayMarriage = true;
    CTCHeroStats_Retail s; s.Marriages_Head = &head;
    CHeroMarriageInfo out; out.WifeUID = 0; out.MaritalHomeUID = 0; out.State = -5; out.GayMarriage = false;
    if (s.PeekMarriageInfo(4, &out) || out.State != -5) { std::printf("0057ac89_TEST FAIL\n"); return 1; }
    if (!s.PeekMarriageInfo(((u64)1 << 32) | 4, &out) || out.MaritalHomeUID != 40 || out.State != 2 || !out.GayMarriage) {
        std::printf("0057ac89_TEST FAIL\n"); return 1;
    }
    std::printf("0057ac89_TEST PASS\n");
    return 0;
}