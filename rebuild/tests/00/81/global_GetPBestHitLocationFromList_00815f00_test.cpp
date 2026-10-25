// Behaviour test for GetPBestHitLocationFromList (0x00815F00): an empty list gives null; otherwise only the elements
// sharing the LAST element's key are considered (equal_range over the key-sorted list), the closest to the origin wins
// (strict <, so the earliest of equal distances is kept), and its intersection point and outward normal are copied out.
#include <cstdio>

struct Vec { float X, Y, Z; };
struct HitLocation { int id; };
struct Element { long key; HitLocation* hit_location; Vec intersection_pos; Vec outward_normal; };

static const Element* LowerBound(const Element* f, const Element* l, long k)
{
    while (f != l) { const Element* m = f + (l - f) / 2; if (m->key < k) f = m + 1; else l = m; }
    return f;
}
static const Element* UpperBound(const Element* f, const Element* l, long k)
{
    while (f != l) { const Element* m = f + (l - f) / 2; if (k < m->key) l = m; else f = m + 1; }
    return f;
}

static HitLocation* GetPBestHitLocationFromList(const Element* elements, unsigned int count, const Vec& origin,
                                                Vec& outPos, Vec& outNormal)
{
    if (count == 0)
        return 0;
    const Element* last = elements + count;
    long key = (last - 1)->key;
    const Element* first = LowerBound(elements, last, key);
    const Element* second = UpperBound(elements, last, key);
    const Element* best = first;
    float dx = best->intersection_pos.X - origin.X, dy = best->intersection_pos.Y - origin.Y,
          dz = best->intersection_pos.Z - origin.Z;
    float bestDistance = dx * dx;
    bestDistance += dy * dy;
    bestDistance += dz * dz;
    for (const Element* it = first + 1; it != second; ++it) {
        float ex = it->intersection_pos.X - origin.X, ey = it->intersection_pos.Y - origin.Y,
              ez = it->intersection_pos.Z - origin.Z;
        float distance = ez * ez;
        distance += ey * ey;
        distance += ex * ex;
        if (distance < bestDistance) { bestDistance = distance; best = it; }
    }
    outPos = best->intersection_pos;
    outNormal = best->outward_normal;
    return best->hit_location;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Same(const Vec& a, float x, float y, float z) { return Near(a.X, x) && Near(a.Y, y) && Near(a.Z, z); }

int main()
{
    int fails = 0;
    HitLocation h[6] = { {0}, {1}, {2}, {3}, {4}, {5} };
    Vec origin = { 0.0f, 0.0f, 0.0f };
    Vec pos = { 9.0f, 9.0f, 9.0f }, nrm = { 9.0f, 9.0f, 9.0f };
    Element list[6] = {
        { 1, &h[0], { 0.1f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },   // closest overall, but key 1 is not the last key
        { 3, &h[1], { 5.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
        { 3, &h[2], { 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
        { 3, &h[3], { 0.0f, 0.0f, -2.0f }, { -1.0f, 0.0f, 0.0f } },  // ties with h[2]: the earlier one stays
        { 3, &h[4], { 4.0f, 4.0f, 4.0f }, { 0.0f, -1.0f, 0.0f } },
        { 3, &h[5], { 3.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
    };
    if (GetPBestHitLocationFromList(list, 0, origin, pos, nrm) != 0 || !Same(pos, 9, 9, 9)) ++fails;
    if (GetPBestHitLocationFromList(list, 6, origin, pos, nrm) != &h[2] || !Same(pos, 0, 2, 0) || !Same(nrm, 0, 0, 1))
        ++fails;
    if (GetPBestHitLocationFromList(list, 1, origin, pos, nrm) != &h[0] || !Same(pos, 0.1f, 0, 0) || !Same(nrm, 1, 0, 0))
        ++fails;
    Vec far = { 4.0f, 4.0f, 5.0f };
    if (GetPBestHitLocationFromList(list, 6, far, pos, nrm) != &h[4] || !Same(pos, 4, 4, 4) || !Same(nrm, 0, -1, 0))
        ++fails;
    Vec right = { 6.0f, 0.0f, 0.0f };
    if (GetPBestHitLocationFromList(list, 6, right, pos, nrm) != &h[1] || !Same(nrm, 0, 1, 0)) ++fails;
    if (fails) { std::printf("00815f00_TEST FAIL\n"); return 1; }
    std::printf("00815f00_TEST PASS\n");
    return 0;
}