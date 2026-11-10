// Behaviour test for NPlayerGui::CDrawQuestInfo::Draw (0x00654438): skipped in GUI mode 1 unless forced; elements are drawn
// down from the anchor (pushed below the clock ring when the minimap draws); an element that draws more than 0.0001 high
// advances the pen by its ceiling height, else (or when of type 6) is cleared; when anything drew, the backing box (width from
// the def, at least the def's min height) is drawn; the oracle symbols always draw.
#include <stdio.h>
#include <math.h>

struct Element { int type; float height; bool cleared; float drawnAtY; };
struct ElementList { Element* items[8]; int n; };   // stands in for the std::list of counted element pointers
struct Def { float posX, posY; long width; float minHeight; long ringHeight; };
struct Result { bool boxDrawn; float boxTLX, boxTLY, boxBRX, boxBRY; int oracle; };
static void Draw(int guiMode, bool forceDraw, bool miniMap, const Def& def, ElementList& elements, Result& r)
{
    if (guiMode == 1 && !forceDraw) return;
    float posX = def.posX, posY = def.posY;
    if (miniMap) posY = posY + (float)def.ringHeight + 16.0f;
    float drawY = posY;
    bool drew = false;
    for (int i = 0; i < elements.n; ++i) {
        Element** it = &elements.items[i];
        if (*it) {
            if ((*it)->type != 6) {
                (*it)->drawnAtY = drawY;
                float height = (*it)->height;
                if (height > 0.0001f) { drawY += (float)(long)ceil(height); drew = true; }
                else (*it)->cleared = true;
            } else (*it)->cleared = true;
        }
    }
    if (drew) {
        r.boxDrawn = true; r.boxTLX = posX; r.boxTLY = posY; r.boxBRX = posX + (float)def.width; r.boxBRY = posY + (drawY - posY);
        if (r.boxBRY - posY < def.minHeight) r.boxBRY = posY + def.minHeight;
    }
    ++r.oracle;
}
int main()
{
    int fails = 0;
    Def def = { 500.0f, 20.0f, 120, 64.0f, 30 };
    Element e1 = { 0, 10.2f, false, -1 }, e2 = { 6, 50.0f, false, -1 }, e3 = { 1, 0.0f, false, -1 }, e4 = { 2, 7.0f, false, -1 };
    ElementList l; l.items[0] = &e1; l.items[1] = &e2; l.items[2] = 0; l.items[3] = &e3; l.items[4] = &e4; l.n = 5;
    Result r = { false, 0, 0, 0, 0, 0 };
    Draw(1, false, false, def, l, r);
    if (r.oracle != 0 || e1.drawnAtY != -1) { printf("mode1 FAIL\n"); ++fails; }
    Draw(0, false, true, def, l, r);
    if (e1.drawnAtY != 66.0f || e4.drawnAtY != 77.0f || !e2.cleared || !e3.cleared || e1.cleared || e4.cleared) { printf("layout FAIL\n"); ++fails; }
    if (!r.boxDrawn || r.boxTLY != 66.0f || r.boxBRX != 620.0f || r.boxBRY != 130.0f || r.oracle != 1) { printf("box min FAIL\n"); ++fails; }
    e1.height = 100.5f;
    Draw(1, true, false, def, l, r);
    if (r.boxTLY != 20.0f || r.boxBRY != 128.0f || r.oracle != 2) { printf("box tall FAIL %f\n", r.boxBRY); ++fails; }
    Result r2 = { false, 0, 0, 0, 0, 0 };
    ElementList empty; empty.n = 0;
    Draw(0, false, false, def, empty, r2);
    if (r2.boxDrawn || r2.oracle != 1) { printf("empty FAIL\n"); ++fails; }
    if (fails) { printf("00654438_TEST FAIL\n"); return 1; }
    printf("00654438_TEST PASS\n");
    return 0;
}