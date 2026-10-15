#include <cstdio>
// Behaviour of CTCHeroStats::GetHeroSexuality (0x0057ACC4): scripted sex and current marriages (states 1..4) decide
// 0 none, 1 straight, 2 bisexual, 3 gay; marriages in other states are ignored.
struct CHeroMarriageInfoNode { CHeroMarriageInfoNode* Next; long State; bool GayMarriage; };
struct CTCHeroStats_Retail {
    bool HadScriptedSex;
    bool HadScriptedGaySex;
    CHeroMarriageInfoNode* Marriages_Head;
    long GetHeroSexuality() const
    {
        bool straight = HadScriptedSex;
        bool gay = HadScriptedGaySex;
        CHeroMarriageInfoNode* head = Marriages_Head;
        for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
            long state = node->State;
            if (state == 1 || state == 3 || state == 2 || state == 4) {
                if (node->GayMarriage)
                    gay = true;
                else
                    straight = true;
            }
        }
        if (!gay)
            return straight ? 1 : 0;
        return straight ? 2 : 3;
    }
};
int main()
{
    CHeroMarriageInfoNode head, a, b;
    head.Next = &head;
    CTCHeroStats_Retail s; s.HadScriptedSex = false; s.HadScriptedGaySex = false; s.Marriages_Head = &head;
    if (s.GetHeroSexuality() != 0) { std::printf("0057acc4_TEST FAIL\n"); return 1; }
    s.HadScriptedSex = true;
    if (s.GetHeroSexuality() != 1) { std::printf("0057acc4_TEST FAIL\n"); return 1; }
    head.Next = &a; a.Next = &head; a.State = 3; a.GayMarriage = true;
    if (s.GetHeroSexuality() != 2) { std::printf("0057acc4_TEST FAIL\n"); return 1; }
    s.HadScriptedSex = false;
    if (s.GetHeroSexuality() != 3) { std::printf("0057acc4_TEST FAIL\n"); return 1; }
    a.State = 5; a.Next = &b; b.Next = &head; b.State = 2; b.GayMarriage = false;   // divorced gay marriage ignored
    if (s.GetHeroSexuality() != 1) { std::printf("0057acc4_TEST FAIL\n"); return 1; }
    std::printf("0057acc4_TEST PASS\n");
    return 0;
}