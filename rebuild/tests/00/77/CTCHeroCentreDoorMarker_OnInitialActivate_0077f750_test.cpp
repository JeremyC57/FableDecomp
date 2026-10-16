#include <cstdio>
// Behaviour model of CTCHeroCentreDoorMarker::OnInitialActivate (0x0077F750).
struct Link { void* centre; };
struct Marker { void* heroCentre;
    void Activate(Link* link) { if (link) heroCentre = link->centre; } };
int main()
{
    int c; Link l; l.centre = &c;
    Marker m; m.heroCentre = 0; m.Activate(0);
    if (m.heroCentre) { std::printf("0077f750_TEST FAIL\n"); return 1; }
    m.Activate(&l);
    if (m.heroCentre != &c) { std::printf("0077f750_TEST FAIL\n"); return 1; }
    std::printf("0077f750_TEST PASS\n");
    return 0;
}