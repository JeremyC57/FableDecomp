// behaviour test for 0x0048F939 CSubtitleRenderer::InitPlayerMode (model: swap the added player mode, default button texts)
#include <cstdio>
#include <cstring>
struct Player { int Added[8]; int NAdded; int Removed; Player() : NAdded(0), Removed(-1) { } void AddMode(int m, bool) { Added[NAdded++] = m; } void RemoveMode(int m) { Removed = m; } };
struct Renderer {
    const char* Button1; const char* Button2; unsigned long DisplayFlags; int PlayerModeAdded;
    void Fill(const char*& text, const char* tag) { text = tag; }
    void InitPlayerMode(Player& player)
    {
        if (PlayerModeAdded != 0) { player.RemoveMode(PlayerModeAdded); PlayerModeAdded = 0; }
        if (DisplayFlags & 0x80) { player.AddMode(0x1A, false); PlayerModeAdded = 0x1A; }
        else if (DisplayFlags & 0x1000) { player.AddMode(0x2A, false); PlayerModeAdded = 0x2A; }
        else if (DisplayFlags & 0x400) { player.AddMode(0x27, false); PlayerModeAdded = 0x27; }
        else if (DisplayFlags & 0x2) {
            player.AddMode(0x0E, false); PlayerModeAdded = 0x0E;
            if (std::strcmp(Button1, "") == 0) Fill(Button1, "TEXT_GUI_NEXT");
        } else if (DisplayFlags & 0x4) {
            player.AddMode(0x0F, false); PlayerModeAdded = 0x0F;
            if (std::strcmp(Button1, "") == 0) Fill(Button1, "TEXT_GUI_CONFIRM");
            if (std::strcmp(Button2, "") == 0) Fill(Button2, "TEXT_GUI_CANCEL");
        }
    }
};
int main()
{
    Player p; Renderer r = { "", "Keep", 0x4, 0x1A };
    r.InitPlayerMode(p);
    bool ok = p.Removed == 0x1A && p.NAdded == 1 && p.Added[0] == 0x0F && r.PlayerModeAdded == 0x0F
              && std::strcmp(r.Button1, "TEXT_GUI_CONFIRM") == 0 && std::strcmp(r.Button2, "Keep") == 0;
    Renderer s = { "", "", 0x2 | 0x80, 0 };
    Player q; s.InitPlayerMode(q);                     // bit 7 wins, no button text
    ok = ok && q.Removed == -1 && q.Added[0] == 0x1A && std::strcmp(s.Button1, "") == 0;
    Renderer t = { "", "", 0x2, 0 };
    Player u; t.InitPlayerMode(u);
    ok = ok && u.Added[0] == 0x0E && std::strcmp(t.Button1, "TEXT_GUI_NEXT") == 0 && std::strcmp(t.Button2, "") == 0;
    Renderer v = { "", "", 0, 0x27 };
    Player w; v.InitPlayerMode(w);                     // no flag: only the removal
    ok = ok && w.Removed == 0x27 && w.NAdded == 0 && v.PlayerModeAdded == 0;
    std::printf(ok ? "0048f939_TEST PASS\n" : "0048f939_TEST FAIL\n");
    return ok ? 0 : 1;
}