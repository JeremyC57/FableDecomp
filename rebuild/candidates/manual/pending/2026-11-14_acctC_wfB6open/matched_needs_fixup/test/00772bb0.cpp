#include <cstdio>
#include <cmath>
// Behaviour test for CTCShop::AddSaleItem (retail 0x00772BB0): the goods object is created at the sale place; when its graphic has a
// COUNTER_PLACEMENT (or "COUNTER_PLACEMENT ") dummy the object is moved so the dummy sits on the place (object * inverse(dummy) *
// place); then physics is frozen, it is flagged created-by-owner, gets CTCCreatedEntity / CTCOwnedEntity pointing at the shop's
// thing, loses its use action, has its particles re-attached and, if it is a stock item, is put on sale and activated.
struct M { float r[3][3]; float t[3]; };
static M Mul(const M& a, const M& b)
{
    M c;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) c.r[i][j] = a.r[i][0] * b.r[0][j] + a.r[i][1] * b.r[1][j] + a.r[i][2] * b.r[2][j];
    }
    for (int j = 0; j < 3; ++j) c.t[j] = a.t[0] * b.r[0][j] + a.t[1] * b.r[1][j] + a.t[2] * b.r[2][j] + b.t[j];
    return c;
}
static M InverseRT(const M& m)                                           // SetInverseOfUniformScaleRotationTranslation: transpose over the squared scale
{
    M v;
    float scale_squared = m.r[0][0] * m.r[0][0] + m.r[0][1] * m.r[0][1] + m.r[0][2] * m.r[0][2];
    float scale = 1.0f / scale_squared;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) v.r[i][j] = m.r[j][i] * scale;
    for (int j = 0; j < 3; ++j) v.t[j] = -(m.t[0] * v.r[0][j] + m.t[1] * v.r[1][j] + m.t[2] * v.r[2][j]);
    return v;
}
static M Translate(float x, float y, float z) { M m = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } }, { x, y, z } }; return m; }
struct Goods { M world; bool physics, created_by_owner, has_created, has_owned, has_use, particles_reattached, stock, for_sale, activated;
    const void* creator; const void* owner; };
static void AddSaleItem(Goods& g, const M& place, bool has_dummy, const M& dummy, const void* shop_thing)
{
    g.world = place;
    if (has_dummy) g.world = Mul(Mul(g.world, InverseRT(dummy)), place);
    g.physics = false;
    g.created_by_owner = true;
    g.has_created = true; g.creator = shop_thing;
    g.has_owned = true; g.owner = shop_thing;
    if (g.has_use) g.has_use = false;
    g.particles_reattached = true;
    if (g.stock) { g.for_sale = true; g.activated = true; }
}
int main()
{
    int shop = 0; Goods g = Goods(); g.physics = true; g.has_use = true; g.stock = true;
    M place = Translate(10, 20, 0), dummy = Translate(0, 0, 1);
    AddSaleItem(g, place, true, dummy, &shop);
    int ok = std::fabs(g.world.t[0] - 20) < 1e-4 && std::fabs(g.world.t[1] - 40) < 1e-4 && std::fabs(g.world.t[2] + 1) < 1e-4
          && !g.physics && g.created_by_owner && g.creator == &shop && g.owner == &shop && !g.has_use && g.particles_reattached
          && g.for_sale && g.activated;
    Goods h = Goods(); h.physics = true;
    AddSaleItem(h, place, false, dummy, &shop);
    ok = ok && h.world.t[0] == 10 && !h.for_sale && !h.activated && h.owner == &shop;
    Goods k = Goods(); k.physics = true;
    M scaled = { { { 2, 0, 0 }, { 0, 2, 0 }, { 0, 0, 2 } }, { 0, 0, 1 } };   // a dummy with a uniform scale of 2
    AddSaleItem(k, place, true, scaled, &shop);
    ok = ok && std::fabs(k.world.r[0][0] - 0.5f) < 1e-4 && std::fabs(k.world.t[0] - 15) < 1e-4 && std::fabs(k.world.t[1] - 30) < 1e-4
            && std::fabs(k.world.t[2] + 0.5f) < 1e-4;
    if (!ok) { std::printf("00772bb0_TEST FAIL\n"); return 1; }
    std::printf("00772bb0_TEST PASS\n");
    return 0;
}
