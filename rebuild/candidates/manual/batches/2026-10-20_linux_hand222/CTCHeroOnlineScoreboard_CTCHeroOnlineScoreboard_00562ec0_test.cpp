// Behaviour test for 0x00562EC0 CTCHeroOnlineScoreboard::CTCHeroOnlineScoreboard (model copy: empty containers, initial states).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct SNode { bool color; SNode *parent, *left, *right; };
struct CMap { SNode* header; unsigned long count;
    CMap() : header((SNode*)std::malloc(32)), count(0) { header->color = false; header->parent = 0; header->left = header; header->right = header; } };
struct LNode { LNode *next, *prev; };
struct CList { LNode* node; CList() { LNode* n = (LNode*)std::malloc(28); n->next = n; n->prev = n; node = n; } };
static const char* g_server = 0; static const char* g_page = 0; static bool g_init = false; static int g_registered = 0, g_activities = 0;
struct CBoard {
    void* PHttp; CMap PictureCache, LocalScoreboard; void* first; void* last; void* end; CList MyClans, ClanMembers; CMap Downloaded;
    long ScoreboardState, TaskState; void *a, *b, *c, *d;
    CBoard() : PHttp(0), first(0), last(0), end(0), ScoreboardState(0), TaskState(100), a(0), b(0), c(0), d(0)
    {
        g_server = "fableclangw.lionhead.com"; g_page = "/index.php";
        if (!g_init) g_init = true;
        ++g_registered; ++g_activities;
    }
};

int main()
{
    CBoard b;
    if (b.PHttp || b.PictureCache.count || b.PictureCache.header->left != b.PictureCache.header || b.MyClans.node->next != b.MyClans.node ||
        b.ClanMembers.node->prev != b.ClanMembers.node || b.Downloaded.header->right != b.Downloaded.header || b.TaskState != 100 ||
        b.ScoreboardState != 0 || b.d || !g_init || g_registered != 1 || g_activities != 1 || std::strcmp(g_page, "/index.php") != 0)
    { std::printf("00562ec0_TEST FAIL\n"); return 1; }
    std::printf("00562ec0_TEST PASS\n");
    return 0;
}
