// Behaviour test for 0x00531EC0 NUISystem::CComponent::Update(float): the component advances its clock and its own transitions, then
// pushes render zoom / position / colour to its position children (set) and to the children it owns (live and dying):
//  - position independent: parent position = Position (zoom independent) or Position * ParentZoom; relative position = Position or
//    ConvertVector(Position) (relative coords), times RelativeParentZoom when zoom dependent;
//  - otherwise ParentPosition is added to the parent position and RelativeParentPosition to the relative one;
//  - a live child whose GetParent() is 0 is adopted; positions go to children whose PParent is this with no position parent, whose
//    position parent is this, or that are bastards; colour + Update to children whose PParent is this or that are bastards;
//  - deletion requests: ALL_INSTANCES -> RemoveChild(i); THIS_INSTANCE -> deletion reset, RemoveChild(i); PER_PARENT -> this parent
//    removed from the associated list (reset when it empties), RemoveChild(i) only when this parent was listed;
//  - dying children that changed state into state 2 are found, sent back to state 0 when they do not delete, unparented and erased.
#include <stdio.h>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

static bool g_rel = false; static float g_bbw = 1280.0f, g_bbh = 960.0f;
struct V2 { float X, Y; V2() : X(0), Y(0) {} V2(float x, float y) : X(x), Y(y) {} V2 operator+(const V2& o) const { return V2(X + o.X, Y + o.Y); } };
static V2 Convert(V2 v) { V2 r = v; if (g_rel) { r.X = v.X / 640.0f * g_bbw; r.Y = v.Y / 480.0f * g_bbh; } return r; }
enum { DND = 0, THIS_I = 1, ALL_I = 2, PER_P = 3 };
struct Del { int Method; std::list<struct Comp*> Parents; Del() : Method(DND) {} };
struct Comp {
    float Time; V2 Position, ParentPosition, RelativeParentPosition, ParentZoom, RelativeParentZoom, RenderZoom, RelativeRenderZoom;
    int RenderColour; bool PosIndep, ZoomIndep, UseRel, Bastard, Changed; int State; Comp* PParent; Comp* PosParent; Del Deletion;
    std::vector<Comp*> Children, ChildrenToDelete; std::set<Comp*> PositionChildren; std::vector<unsigned long> Removed;
    V2 GotParentPos, GotRelPos, GotZoom, GotRelZoom; int GotColour; float Updated; int Own;
    Comp() : Time(0), RenderColour(0), PosIndep(false), ZoomIndep(true), UseRel(false), Bastard(false), Changed(false), State(0),
             PParent(0), PosParent(0), GotColour(-1), Updated(-1), Own(0) {}
    void Update(float dt);
};
void Comp::Update(float dt)
{
    Time += dt; Own += 3;   // UpdateZoom / UpdatePosition / UpdateColour
    V2 pp, rp;
    if (PosIndep) {
        pp = ZoomIndep ? Position : V2(Position.X * ParentZoom.X, Position.Y * ParentZoom.Y);
        V2 c = UseRel ? Convert(Position) : Position;
        rp = ZoomIndep ? c : V2(c.X * RelativeParentZoom.X, c.Y * RelativeParentZoom.Y);
    } else {
        pp = (ZoomIndep ? Position : V2(Position.X * ParentZoom.X, Position.Y * ParentZoom.Y)) + ParentPosition;
        V2 c = UseRel ? Convert(Position) : Position;
        rp = (ZoomIndep ? c : V2(c.X * RelativeParentZoom.X, c.Y * RelativeParentZoom.Y)) + RelativeParentPosition;
    }
    for (std::set<Comp*>::iterator it = PositionChildren.begin(); it != PositionChildren.end(); ++it) {
        (*it)->GotZoom = RenderZoom; (*it)->GotRelZoom = RelativeRenderZoom; (*it)->GotParentPos = pp; (*it)->GotRelPos = rp;
    }
    for (int pass = 0; pass < 2; ++pass) {
        std::vector<Comp*>& v = pass ? ChildrenToDelete : Children;
        for (unsigned long i = 0; i < v.size(); ++i) {
            Comp* c = v[i];
            if (c->PParent == 0) c->PParent = this;
            if ((c->PParent == this && c->PosParent == 0) || c->PosParent == this || c->Bastard) {
                c->GotZoom = RenderZoom; c->GotRelZoom = RelativeRenderZoom; c->GotParentPos = pp; c->GotRelPos = rp;
            }
            if (c->PParent == this || c->Bastard) {
                c->GotColour = RenderColour; c->Update(0.5f);
                if (!pass) {
                    Del d = c->Deletion;
                    if (d.Method == ALL_I) Removed.push_back(i);
                    else if (d.Method == THIS_I) { c->Deletion = Del(); Removed.push_back(i); }
                    else if (d.Method == PER_P) {
                        std::list<Comp*>::iterator f = std::find(d.Parents.begin(), d.Parents.end(), this);
                        if (f != d.Parents.end()) { d.Parents.erase(f); if (d.Parents.empty()) c->Deletion = Del(); Removed.push_back(i); }
                    }
                } else if (c->Changed && c->State == 2) {
                    std::vector<Comp*>::iterator f = std::find(v.begin(), v.end(), c);
                    if ((*f)->Deletion.Method == DND) (*f)->State = 0;
                    (*f)->PParent = 0; v.erase(f);
                }
            }
        }
    }
}
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { ++fails; printf("FAIL %s\n", what); } }
static bool eq(V2 a, float x, float y) { return a.X > x - 0.001f && a.X < x + 0.001f && a.Y > y - 0.001f && a.Y < y + 0.001f; }
int main()
{
    Comp p; p.Position = V2(64, 48); p.ParentPosition = V2(10, 20); p.RelativeParentPosition = V2(1, 2); p.ParentZoom = V2(2, 3);
    p.RelativeParentZoom = V2(4, 5); p.RenderZoom = V2(7, 8); p.RenderColour = 9;
    Comp a, b, c, d, e, s, k1, k2;
    p.PositionChildren.insert(&s);
    a.Deletion.Method = ALL_I; b.Deletion.Method = THIS_I; c.Deletion.Method = PER_P; c.Deletion.Parents.push_back(&p);
    d.Deletion.Method = PER_P; d.Deletion.Parents.push_back(&a); e.PParent = &a; e.PosParent = &p;
    p.Children.push_back(&a); p.Children.push_back(&b); p.Children.push_back(&c); p.Children.push_back(&d); p.Children.push_back(&e);
    k1.PParent = &p; k1.Changed = true; k1.State = 2; k2.Bastard = true; k2.PParent = &a; k2.Changed = true; k2.State = 1;
    p.ChildrenToDelete.push_back(&k2); p.ChildrenToDelete.push_back(&k1);
    g_rel = true; p.UseRel = true; p.ZoomIndep = false;
    p.Update(2.0f);
    check(p.Time == 2.0f && p.Own == 3, "clock / own transitions");
    check(eq(s.GotParentPos, 64 * 2 + 10, 48 * 3 + 20), "set child parent position (zoomed + ParentPosition)");
    check(eq(s.GotRelPos, 64 / 640.0f * 1280 * 4 + 1, 48 / 480.0f * 960 * 5 + 2), "relative position (converted, zoomed, + relative)");
    check(eq(a.GotZoom, 7, 8) && a.PParent == &p && a.GotColour == 9 && a.Time == 0.5f, "adopted child updated");
    check(eq(e.GotParentPos, 138, 164) && e.GotColour == -1 && e.Time == 0, "position child of another parent: positions only");
    check(p.Removed.size() == 3 && p.Removed[0] == 0 && p.Removed[1] == 1 && p.Removed[2] == 2, "RemoveChild for ALL / THIS / listed PER_PARENT");
    check(b.Deletion.Method == DND && a.Deletion.Method == ALL_I, "THIS_INSTANCE resets, ALL_INSTANCES keeps");
    check(c.Deletion.Method == DND && c.Deletion.Parents.empty(), "PER_PARENT list emptied -> reset");
    check(d.Deletion.Method == PER_P && d.Deletion.Parents.size() == 1, "PER_PARENT without this parent untouched");
    check(p.ChildrenToDelete.size() == 1 && p.ChildrenToDelete[0] == &k2 && k1.PParent == 0 && k1.State == 0, "dying child in state 2 erased");
    check(k2.GotColour == 9 && eq(k2.GotParentPos, 138, 164), "bastard dying child updated");
    // position independent, zoom independent, absolute coordinates
    Comp q, t; q.PosIndep = true; q.Position = V2(3, 4); q.ParentPosition = V2(100, 100); q.Children.push_back(&t); t.PParent = &q; g_rel = false;
    q.Update(1.0f);
    check(eq(t.GotParentPos, 3, 4) && eq(t.GotRelPos, 3, 4) && q.Removed.empty(), "independent position passes Position through");
    if (fails == 0) printf("00531ec0_TEST PASS\n");
    return 0;
}