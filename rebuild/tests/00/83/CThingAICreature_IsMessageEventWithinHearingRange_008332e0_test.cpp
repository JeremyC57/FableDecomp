// Behaviour test: CThingAICreature::IsMessageEventWithinHearingRange -- inside min(own sound radius, event hearing radius), and only with a perception interface
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};
template <class T> static const T& Min(const T& a, const T& b) { return b < a ? b : a; }   // STLport min
struct CTCPerceiveThing {
    float SoundRadius, SightRadius;
    float GetSoundRadius() const { return SoundRadius; }
    float GetSightRadius() const { return SightRadius; }
};
struct CMessageEvent {
    C3DVector Pos;
    float SightRadius;
    float HearingRadius;
    CMessageEvent() : Pos(0.0f, 0.0f, 0.0f), SightRadius(0.0f), HearingRadius(0.0f) { }
};
struct CThingAICreature {
    unsigned int      presentBits[9];                   // interface 0x34 = word 1 bit 20
    CTCPerceiveThing* perceive;
    C3DVector         position;
    CThingAICreature() : perceive(0), position(0.0f, 0.0f, 0.0f) { for (int i = 0; i < 9; ++i) presentBits[i] = 0; }
    bool HasTC(long type) const { return (presentBits[type >> 5] & (1u << (type & 31))) != 0; }
    CTCPerceiveThing* GetPerceiveTC() const { return perceive; }
    const C3DVector& GetPosition() const { return position; }
    bool IsMessageEventWithinHearingRange(const CMessageEvent& event) const
    {
        if (HasTC(0x34)) {
            float radius = Min(GetPerceiveTC()->GetSoundRadius(), event.HearingRadius);
            if ((GetPosition() - event.Pos).GetLengthSquared() < radius * radius)
                return true;
        }
        return false;
    }
};

int main()
{
    CTCPerceiveThing tc;
    tc.SoundRadius = 10.0f;
    tc.SightRadius = 10.0f;
    CThingAICreature c;
    c.perceive = &tc;
    c.position = C3DVector(1.0f, 2.0f, 0.0f);
    CMessageEvent e;
    e.Pos = C3DVector(4.0f, 6.0f, 0.0f);                // 5 units away
    e.HearingRadius = 20.0f;
    if (c.IsMessageEventWithinHearingRange(e))                                    // no perception interface yet
        { std::printf("008332e0_TEST FAIL\n"); return 1; }
    c.presentBits[1] = 0x100000;
    if (!c.IsMessageEventWithinHearingRange(e))                                   // min(10, 20) = 10 > 5
        { std::printf("008332e0_TEST FAIL\n"); return 1; }
    e.HearingRadius = 4.0f;                                   // min(10, 4) = 4 < 5
    if (c.IsMessageEventWithinHearingRange(e))
        { std::printf("008332e0_TEST FAIL\n"); return 1; }
    e.HearingRadius = 5.0f;                                   // exactly on the edge: not inside
    if (c.IsMessageEventWithinHearingRange(e))
        { std::printf("008332e0_TEST FAIL\n"); return 1; }
    e.HearingRadius = 50.0f;
    tc.SightRadius = tc.SoundRadius = 3.0f;             // the creature's own radius is the limit now
    if (c.IsMessageEventWithinHearingRange(e))
        { std::printf("008332e0_TEST FAIL\n"); return 1; }
    std::printf("008332e0_TEST PASS\n");
    return 0;
}