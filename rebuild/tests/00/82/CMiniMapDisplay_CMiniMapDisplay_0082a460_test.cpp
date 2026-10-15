// Behaviour test for 0x0082A460 (CMiniMapDisplay constructor): initial state of the mini-map display.
#include <cstdio>
#include <cstdlib>
struct Node { Node* next; Node* prev; int value[4]; };
struct List { Node* head; List() : head(0) { Node* n = (Node*)std::malloc(sizeof(Node)); n->next = n; n->prev = n; head = n; } ~List() { std::free(head); } };
struct RGB { unsigned char B, G, R, A; RGB() {} RGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct Fade {
    bool Active; float FadeInTime, FadeOutTime; bool Closing, Opening; float OpenTimer, CloseTimer; RGB ToColour;
    Fade() { Active = false; CloseTimer = 0; OpenTimer = 0; Opening = false; Closing = false; ToColour = RGB(0, 0, 0, 0xff); }
};
template <class T> struct Vec { T* f; T* l; T* e; Vec() : f(0), l(0), e(0) {} };
struct HandleVec { int count; HandleVec(unsigned int n) : count((int)n) {} };
struct MiniMapDisplay {
    HandleVec MapPrimitiveHandles; Vec<int> MarkerPrimitiveHandles; int MiniMapGraphic; List Markers; Fade MarkerFlashFade;
    bool f1, f2, f3, f4; Vec<int> MinimapList; void* RegionMinimapTexture; void* PRegion; bool UpdateRequired;
    MiniMapDisplay() : MapPrimitiveHandles(1), MiniMapGraphic(0), f1(false), f2(false), f3(false), f4(false), RegionMinimapTexture(0), PRegion(0), UpdateRequired(false) {}
};
int main()
{
    MiniMapDisplay m;
    int ok = m.MapPrimitiveHandles.count == 1 && !m.MarkerPrimitiveHandles.f && m.MiniMapGraphic == 0;
    ok = ok && m.Markers.head && m.Markers.head->next == m.Markers.head && m.Markers.head->prev == m.Markers.head;
    ok = ok && !m.MarkerFlashFade.Active && !m.MarkerFlashFade.Opening && !m.MarkerFlashFade.Closing && m.MarkerFlashFade.OpenTimer == 0 && m.MarkerFlashFade.CloseTimer == 0;
    ok = ok && m.MarkerFlashFade.ToColour.R == 0 && m.MarkerFlashFade.ToColour.G == 0 && m.MarkerFlashFade.ToColour.B == 0 && m.MarkerFlashFade.ToColour.A == 0xff;
    ok = ok && !m.f1 && !m.f2 && !m.f3 && !m.f4 && !m.MinimapList.f && !m.RegionMinimapTexture && !m.PRegion && !m.UpdateRequired;
    if (!ok) { std::printf("0082a460_TEST FAIL\n"); return 1; }
    std::printf("0082a460_TEST PASS\n");
    return 0;
}