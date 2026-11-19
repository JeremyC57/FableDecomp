// Behaviour test for NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo (0x0064D404), a portable model of the row. Every element but the
// mini-game bar (type 6) is cleared; the bar is drawn at the anchor (+ clock ring height + 16 when the mini-map draws, else + 20); its
// extra elements are found by ID and stacked below, each advancing by the previous drawn height (a missing ID keeps the height); for
// the trophy mini-game (6) a box from the anchor, the graphic width wide and at least the minimum height tall, widened by 16 each side.
#include <stdio.h>
#include <vector>
struct Elem { long ID, Type; float Height; bool Cleared; float DrawnX, DrawnY; int Draws; };
struct Box { float TLX, TLY, BRX, BRY; };
struct Result { bool drew, boxed; Box box; };
static Result DrawMiniGameInfo(std::vector<Elem>& elems, long miniGameIndex, bool miniMap, long ringHeight, float ax, float ay,
                               const std::vector<long>& extras, long gameType, long width, float minHeight)
{
    Result r; r.drew = false; r.boxed = false;
    if (miniGameIndex <= 0) return r;
    Elem* bar = 0;
    for (size_t k = 0; k < elems.size(); ++k) {
        if (elems[k].Type == 6) bar = &elems[k]; else elems[k].Cleared = true;
    }
    if (!bar) return r;
    r.drew = true;
    float qx = ax, qy = ay;
    if (miniMap) qy += ringHeight + 16.0f; else qy += 20.0f;
    bar->DrawnX = qx; bar->DrawnY = qy; ++bar->Draws;
    float height = bar->Height;
    float ex = qx - 12.0f, ey = qy;
    for (size_t i = 0; i < extras.size(); ++i) {
        ey += height;
        Elem* e = 0;
        for (size_t k = 0; k < elems.size(); ++k) if (elems[k].ID == extras[i]) { e = &elems[k]; break; }
        if (e) { e->DrawnX = ex; e->DrawnY = ey; ++e->Draws; height = e->Height; }
    }
    if (gameType == 6) {
        ey += height;
        Box b; b.TLX = qx; b.TLY = qy; b.BRX = qx + (float)width; b.BRY = qy + (ey - qy);
        if (minHeight > b.BRY - b.TLY) b.BRY = b.TLY + minHeight;
        b.TLX -= 16.0f; b.BRX += 16.0f;
        r.boxed = true; r.box = b;
    }
    return r;
}
static Elem E(long id, long type, float h) { Elem e = { id, type, h, false, 0, 0, 0 }; return e; }
int main()
{
    int bad = 0;
    std::vector<Elem> v; v.push_back(E(1, 1, 10.0f)); v.push_back(E(2, 6, 30.0f)); v.push_back(E(3, 3, 8.0f));
    std::vector<long> ex; ex.push_back(3); ex.push_back(99); ex.push_back(1);
    Result r = DrawMiniGameInfo(v, 1, true, 40, 500.0f, 10.0f, ex, 6, 120, 100.0f);
    if (!r.drew || !v[0].Cleared || v[1].Cleared || !v[2].Cleared) ++bad;           // the bar alone is not cleared
    if (v[1].DrawnX != 500.0f || v[1].DrawnY != 66.0f) ++bad;                       // 10 + 40 + 16
    if (v[2].DrawnX != 488.0f || v[2].DrawnY != 96.0f) ++bad;                       // + bar height 30
    if (v[0].DrawnY != 112.0f) ++bad;                                               // + 8 for the counter, + 8 again for the missing ID
    if (!r.boxed || r.box.TLX != 484.0f || r.box.BRX != 636.0f || r.box.TLY != 66.0f || r.box.BRY != 166.0f) ++bad;   // min height
    std::vector<Elem> w; w.push_back(E(5, 6, 12.0f));
    Result s = DrawMiniGameInfo(w, 2, false, 40, 0.0f, 0.0f, std::vector<long>(), 1, 50, 5.0f);
    if (!s.drew || s.boxed || w[0].DrawnY != 20.0f) ++bad;                          // no mini-map: + 20; not trophy: no box
    std::vector<Elem> u; u.push_back(E(7, 2, 1.0f));
    Result t = DrawMiniGameInfo(u, 1, false, 0, 0.0f, 0.0f, ex, 6, 50, 5.0f);
    if (t.drew || !u[0].Cleared) ++bad;                                             // no bar: everything cleared, nothing drawn
    std::vector<Elem> z; z.push_back(E(7, 6, 1.0f));
    Result q = DrawMiniGameInfo(z, 0, false, 0, 0.0f, 0.0f, ex, 6, 50, 5.0f);
    if (q.drew || z[0].Cleared) ++bad;                                              // no mini-game index: untouched
    printf(bad ? "0064d404_TEST FAIL\n" : "0064d404_TEST PASS\n");
    return bad ? 1 : 0;
}
