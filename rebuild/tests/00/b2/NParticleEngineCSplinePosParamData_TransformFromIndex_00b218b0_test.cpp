// Behaviour test for 0x00B218B0: flag off -> nothing; bound key found -> lookup only; else closest points + transform.
#include <cstdio>
struct V { float X, Y, Z; };
struct Params { int lookups; bool found; bool Get(V* out, const unsigned int&) { ++lookups; if (found) out->X = 1; return found; } };
struct Data { bool keep; unsigned int keys[4]; int closest, transforms; bool closestOk;
  bool Closest(long, V*, V*, V*, V*) { ++closest; return closestOk; }
  void Transform(V* p) { ++transforms; p->X = 2; }
  void TransformFromIndex(Params* pr, V* point, long index) {
    if (keep) { unsigned int key = keys[index]; if (key && pr->Get(point, key)) return; V a, b, c, d; if (Closest(index, &a, &b, &c, &d)) Transform(point); } } };
int main()
{
    Params pr = {0, true}; V pt = {0, 0, 0};
    Data d = {false, {0, 5, 0, 0}, 0, 0, true}; d.TransformFromIndex(&pr, &pt, 1); bool ok = pr.lookups == 0 && d.closest == 0;
    d.keep = true; d.TransformFromIndex(&pr, &pt, 1); ok = ok && pr.lookups == 1 && d.closest == 0 && pt.X == 1;
    d.TransformFromIndex(&pr, &pt, 0); ok = ok && pr.lookups == 1 && d.closest == 1 && d.transforms == 1 && pt.X == 2;
    pr.found = false; d.closestOk = false; d.TransformFromIndex(&pr, &pt, 1); ok = ok && pr.lookups == 2 && d.closest == 2 && d.transforms == 1;
    if (!ok) { std::printf("00b218b0_TEST FAIL\n"); return 1; }
    std::printf("00b218b0_TEST PASS\n");
    return 0;
}