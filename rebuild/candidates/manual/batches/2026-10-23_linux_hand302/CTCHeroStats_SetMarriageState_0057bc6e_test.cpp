// Behaviour test for CTCHeroStats::SetMarriageState @ 0x0057BC6E
#include <cstdio>

struct Info { unsigned long long Wife; unsigned long long Home; int State; };
struct Node { Node* next; Node* prev; Info data; };
struct Stats { long Weddings, Murdered, Died, Divorces; Node* head; };
static int g_calls; static long g_act[8]; static float g_score[8];
static void Report(long a, float s) { g_act[g_calls] = a; g_score[g_calls] = s; ++g_calls; }

static void SetMarriageState(Stats* st, unsigned long long wife, int state)
{
    Node* last = st->head;
    for (Node* it = last->next; it != last; it = it->next) {
        if (it->data.Wife == wife) {
            it->data.State = state;
            if (state == 5 || state == 6 || state == 7) {
                it->data.Home = 0;
                if (state == 7) { ++st->Murdered; Report(14, (float)(st->Died + st->Murdered)); Report(9, (float)(st->Weddings - st->Died - st->Murdered)); }
                else if (state == 6) { ++st->Died; Report(14, (float)(st->Died + st->Murdered)); Report(9, (float)(st->Weddings - st->Died - st->Murdered)); }
                else if (state == 5) { ++st->Divorces; Report(13, (float)st->Divorces); }
            }
        }
    }
}

int main()
{
    Node head, a, b;
    head.next = &a; a.next = &b; b.next = &head;
    a.data.Wife = 0x100000002ULL; a.data.Home = 77; a.data.State = 1;
    b.data.Wife = 3; b.data.Home = 88; b.data.State = 1;
    Stats st; st.Weddings = 4; st.Murdered = 0; st.Died = 1; st.Divorces = 0; st.head = &head;
    SetMarriageState(&st, 0x100000002ULL, 7);
    if (a.data.State != 7 || a.data.Home != 0 || b.data.Home != 88 || st.Murdered != 1 || g_calls != 2 ||
        g_act[0] != 14 || g_score[0] != 2.0f || g_act[1] != 9 || g_score[1] != 2.0f) { std::printf("0057bc6e_TEST FAIL\n"); return 1; }
    SetMarriageState(&st, 3, 2);
    if (b.data.State != 2 || b.data.Home != 88 || g_calls != 2) { std::printf("0057bc6e_TEST FAIL\n"); return 1; }
    SetMarriageState(&st, 3, 5);
    if (b.data.Home != 0 || st.Divorces != 1 || g_calls != 3 || g_act[2] != 13 || g_score[2] != 1.0f) { std::printf("0057bc6e_TEST FAIL\n"); return 1; }
    std::printf("0057bc6e_TEST PASS\n");
    return 0;
}
