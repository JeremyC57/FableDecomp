// Behaviour test for 0x008378D0 CThingTrackNode::SetAsLinkedTo(CThingTrackNode*): the row's logic over a stub node (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
static void check(bool v, int line) { if (!v) { printf("008378d0_TEST FAIL line %d\n", line); exit(1); } }
class CThingTrackNode { public:
    CThingTrackNode* NextNode; CThingTrackNode* PreviousNode;
    std::string TrackName; bool IsStart, IsEnd; int flips; std::string named;
    CThingTrackNode(const char* name) : NextNode(0), PreviousNode(0), TrackName(name), IsStart(false), IsEnd(false), flips(0) { }
    void FlipTrackDirection() { ++flips; bool s = IsStart; IsStart = IsEnd; IsEnd = s; }
    void SetTrackName(const std::string& name) { named = name; TrackName = name; }
    void SetAsLinkedTo(CThingTrackNode* other);
};
void CThingTrackNode::SetAsLinkedTo(CThingTrackNode* other)
{
    if (other->TrackName == TrackName
        && ((other->IsEnd && IsStart) || (other->IsStart && IsEnd))
        && !(other->IsStart && other->IsEnd && IsStart && IsEnd))
        return;
    if (!NextNode && !PreviousNode) {
        IsStart = true; IsEnd = false; NextNode = other;
        if (!other->NextNode && !other->PreviousNode) {
            other->PreviousNode = this; other->NextNode = 0; other->IsStart = false; other->IsEnd = true;
        } else if (other->IsEnd) {
            IsStart = false; IsEnd = true; other->NextNode = this; other->IsEnd = false; PreviousNode = other; NextNode = 0;
        } else {
            IsStart = true; IsEnd = false; other->IsStart = false; other->PreviousNode = this;
        }
    } else if (IsEnd) {
        IsStart = false; IsEnd = false; NextNode = other;
        if (!other->NextNode && !other->PreviousNode) {
            other->PreviousNode = this; other->NextNode = 0; other->IsStart = false; other->IsEnd = true;
        } else {
            IsStart = false; IsEnd = false; NextNode = other;
            if (other->IsEnd) other->FlipTrackDirection();
            other->PreviousNode = this; other->IsStart = false;
        }
    } else {
        if (IsStart) {
            if (!other->NextNode && !other->PreviousNode) {
                IsStart = false; IsEnd = false; PreviousNode = other; other->IsStart = true;
                other->NextNode = this; other->IsEnd = false;
            } else {
                IsStart = false; IsEnd = false; PreviousNode = other;
                if (other->IsStart) other->FlipTrackDirection();
                other->NextNode = this; other->IsEnd = false;
            }
        }
    }
    other->SetTrackName(TrackName);
}
int main()
{
    // two lone nodes: this becomes start, other becomes end, other takes this track's name
    CThingTrackNode a("A"), b("B");
    a.SetAsLinkedTo(&b);
    check(a.IsStart && !a.IsEnd && a.NextNode == &b && a.PreviousNode == 0, __LINE__);
    check(!b.IsStart && b.IsEnd && b.PreviousNode == &a && b.NextNode == 0 && b.named == "A", __LINE__);
    // already linked on the same track (other end, this start): nothing happens
    b.named = ""; a.SetAsLinkedTo(&b);
    check(b.named == "" && a.NextNode == &b, __LINE__);
    // lone node linked to the end of an existing track: it becomes the new end
    CThingTrackNode c("C"), d("D"), e("E");
    d.NextNode = &e; d.IsEnd = true;
    c.SetAsLinkedTo(&d);
    check(!c.IsStart && c.IsEnd && c.PreviousNode == &d && c.NextNode == 0 && d.NextNode == &c && !d.IsEnd && d.named == "C", __LINE__);
    // lone node linked to a start node: it becomes the new start
    CThingTrackNode f("F"), g("G"), h("H");
    g.PreviousNode = &h; g.IsStart = true;
    f.SetAsLinkedTo(&g);
    check(f.IsStart && !f.IsEnd && f.NextNode == &g && !g.IsStart && g.PreviousNode == &f, __LINE__);
    // end node linked to an end of another track: other flips, both become middles
    CThingTrackNode i("I"), j("J"), k("K");
    i.PreviousNode = &k; i.IsEnd = true; j.PreviousNode = &k; j.IsEnd = true;
    i.SetAsLinkedTo(&j);
    check(j.flips == 1 && !i.IsStart && !i.IsEnd && i.NextNode == &j && j.PreviousNode == &i && !j.IsStart && j.named == "I", __LINE__);
    // end node linked to a lone node: lone node becomes the new end
    CThingTrackNode l("L"), m("M"), n("N");
    l.PreviousNode = &n; l.IsEnd = true;
    l.SetAsLinkedTo(&m);
    check(!l.IsEnd && l.NextNode == &m && m.IsEnd && !m.IsStart && m.PreviousNode == &l, __LINE__);
    // start node linked to a lone node: lone node becomes the new start
    CThingTrackNode o("O"), p("P"), q("Q");
    o.NextNode = &q; o.IsStart = true;
    o.SetAsLinkedTo(&p);
    check(!o.IsStart && o.PreviousNode == &p && p.IsStart && !p.IsEnd && p.NextNode == &o, __LINE__);
    // start node linked to another start: other flips, other's next becomes this
    CThingTrackNode r("R"), s("S"), t("T");
    r.NextNode = &t; r.IsStart = true; s.NextNode = &t; s.IsStart = true;
    r.SetAsLinkedTo(&s);
    check(s.flips == 1 && !r.IsStart && r.PreviousNode == &s && s.NextNode == &r && !s.IsEnd, __LINE__);
    // middle node (neither start nor end, linked): only the name spreads
    CThingTrackNode u("U"), w("W"), x("X");
    u.NextNode = &x; u.PreviousNode = &x;
    u.SetAsLinkedTo(&w);
    check(w.named == "U" && w.NextNode == 0 && w.PreviousNode == 0, __LINE__);
    printf("008378d0_TEST PASS\n");
    return 0;
}