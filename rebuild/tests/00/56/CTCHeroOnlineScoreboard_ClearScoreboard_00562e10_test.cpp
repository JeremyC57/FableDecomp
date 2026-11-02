// Behaviour test for CTCHeroOnlineScoreboard::ClearScoreboard (0x00562E10): the downloaded scoreboard map (nodes erased, header
// reset to the empty self-linked state, count 0), the clan list, the selected clan (set to the empty string) and the clan
// member list are cleared, in that order; an empty map is left untouched.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
struct Node { Node* parent; Node* left; Node* right; };
struct Tree {
    Node* header; unsigned count; int erased;
    void Erase(Node* root) { if (root) ++erased; g_log[g_n++] = 1; }
    void clear()
    {
        if (count != 0) {
            Erase(header->parent);
            header->left = header;
            header->parent = 0;
            header->right = header;
            count = 0;
        }
    }
};
struct List { int size; int id; void clear() { size = 0; g_log[g_n++] = id; } };
struct WideString {
    const char* text;
    static WideString NullString;
    WideString& operator=(const WideString& o) { text = o.text; g_log[g_n++] = 3; return *this; }
};
WideString WideString::NullString = { "" };
struct Scoreboard {
    List MyClans; WideString SelectedClan; List ClanMembers; Tree DownloadedClanScoreboard;
    void ClearScoreboard()
    {
        DownloadedClanScoreboard.clear();
        MyClans.clear();
        SelectedClan = WideString::NullString;
        ClanMembers.clear();
    }
};
int main()
{
    Node header, root; header.parent = &root; header.left = &root; header.right = &root;
    Scoreboard s;
    s.MyClans.size = 3; s.MyClans.id = 2; s.ClanMembers.size = 5; s.ClanMembers.id = 4;
    s.SelectedClan.text = "Hero Guild";
    s.DownloadedClanScoreboard.header = &header; s.DownloadedClanScoreboard.count = 4; s.DownloadedClanScoreboard.erased = 0;
    s.ClearScoreboard();
    if (g_n != 4 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_log[3] != 4 || s.DownloadedClanScoreboard.erased != 1 ||
        header.parent != 0 || header.left != &header || header.right != &header || s.DownloadedClanScoreboard.count != 0 ||
        s.MyClans.size != 0 || s.ClanMembers.size != 0 || s.SelectedClan.text[0] != 0) {
        std::printf("00562e10_TEST FAIL\n"); return 1;
    }
    g_n = 0;
    s.ClearScoreboard();                                  // empty map: no erase call
    if (g_n != 3 || g_log[0] != 2) { std::printf("00562e10_TEST FAIL\n"); return 1; }
    std::printf("00562e10_TEST PASS\n");
    return 0;
}