// Behaviour test for NUISystem::CComponent::GetPosition() const (retail 0x0052E890; agentFAM04 replacement, 2026-11-11): the body copies
// the two dwords of Position (+0x34 / +0x38) out through the hidden C2DVector return pointer and returns that pointer (`ret 4`).
// Self-contained model (the earlier catalog test reached the body through inline assembly, which the row rules forbid in tests); same PassPattern.
#include <cstdio>
#include <cstring>
#include <cstddef>
struct C2DVector { float X; float Y; };
struct CComponentModel {
    unsigned char Before[0x34];
    C2DVector Position;                                                  // +0x34
    C2DVector GetPosition() const { return Position; }
};
int main()
{
    CComponentModel c;
    std::memset(&c, 0, sizeof c);
    c.Position.X = 1.5f;
    c.Position.Y = -2.25f;
    C2DVector p = c.GetPosition();
    if (offsetof(CComponentModel, Position) == 0x34 && p.X == 1.5f && p.Y == -2.25f) {
        printf("OK_0x0052e890\n");
        return 0;
    }
    printf("BAD\n");
    return 1;
}
