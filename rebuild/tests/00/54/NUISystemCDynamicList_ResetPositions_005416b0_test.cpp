// Behaviour test for 0x005416B0 NUISystem::CDynamicList::ResetPositions()
#include <cstdio>
#include <map>
#include <vector>

class C2DVector { public: float X, Y; };
class CRGBColour { public: unsigned char B, G, R, A; };
struct CUIState { C2DVector Position; CRGBColour Colour; };
static const long DynamicListResetStates[6] = { 0, 1, 4, 3, 5, 6 };

struct CComponent {
    std::map<unsigned long, CUIState> States;
    C2DVector Position;
    CRGBColour Colour;
    std::map<unsigned long, CUIState>& GetStates() { return States; }
    void SetPosition(const C2DVector& p) { Position = p; }
    CRGBColour GetColour() const { return Colour; }
    void SetColour(const CRGBColour& c) { Colour = c; }
};
struct CDynamicList {
    C2DVector PositionOffset;
    long TopIndex, SelectionIndex, NumVisible;
    std::vector<CComponent*> ScrollingChildren;
    void ResetPositions()
    {
        C2DVector position = { (float)-TopIndex * PositionOffset.X, (float)-TopIndex * PositionOffset.Y };
        for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
            CComponent* child = ScrollingChildren[i];
            for (unsigned long s = 0; s < 6; ++s) {
                long state = DynamicListResetStates[s];
                child->GetStates()[state].Position = position;
                if (i >= (unsigned long)TopIndex && i < (unsigned long)(TopIndex + NumVisible))
                    child->GetStates()[state].Colour.A = 0xff;
                else
                    child->GetStates()[state].Colour.A = 0;
            }
            child->SetPosition(position);
            CRGBColour colour = child->GetColour();
            if (i >= (unsigned long)TopIndex && i < (unsigned long)(TopIndex + NumVisible))
                colour.A = 0xff;
            else
                colour.A = 0;
            child->SetColour(colour);
            position.X += PositionOffset.X;
            position.Y += PositionOffset.Y;
        }
    }
};

int main()
{
    CComponent c[5];
    CDynamicList list;
    list.PositionOffset.X = 2.0f; list.PositionOffset.Y = 10.0f;
    list.TopIndex = 1; list.SelectionIndex = 0; list.NumVisible = 2;
    for (int i = 0; i < 5; ++i) { c[i].Colour.R = 7; c[i].Colour.A = 0x40; list.ScrollingChildren.push_back(&c[i]); }
    list.ResetPositions();
    for (int i = 0; i < 5; ++i) {
        float x = (i - 1) * 2.0f, y = (i - 1) * 10.0f;
        bool visible = i >= 1 && i < 3;
        if (c[i].Position.X != x || c[i].Position.Y != y) { std::printf("005416b0_TEST FAIL\n"); return 1; }
        if (c[i].Colour.A != (visible ? 0xff : 0) || c[i].Colour.R != 7) { std::printf("005416b0_TEST FAIL\n"); return 1; }
        if (c[i].States.size() != 6 || c[i].States.count(2) != 0) { std::printf("005416b0_TEST FAIL\n"); return 1; }
        for (std::map<unsigned long, CUIState>::iterator it = c[i].States.begin(); it != c[i].States.end(); ++it)
            if (it->second.Position.X != x || it->second.Position.Y != y || it->second.Colour.A != (visible ? 0xff : 0)) { std::printf("005416b0_TEST FAIL\n"); return 1; }
    }
    std::printf("005416b0_TEST PASS\n");
    return 0;
}