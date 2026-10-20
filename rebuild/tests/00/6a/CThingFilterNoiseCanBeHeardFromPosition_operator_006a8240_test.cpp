// behaviour test for CThingFilter_NoiseCanBeHeardFromPosition::operator() (0x006A8240): TC gate, min(radius, hearing range), squared compare
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} float GetLengthSquared() const { return X * X + Y * Y + Z * Z; } };
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
template<class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
struct CTCNoiseMaker_37 { float NoiseRadius; };
class CThing { public:
    unsigned long UsingComponents[9]; CInterfaceMap Components; C3DVector m_pos;
    CThing() : m_pos(0, 0, 0) {}
    const C3DVector& GetPosition() const { return m_pos; }
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> T* GetTC(long interfaceType) const
    { long type = interfaceType; const CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
      if (it == map->last || interfaceType < it->type) it = map->last; return (T*)it->component; } };
struct CThingFilter_NoiseCanBeHeardFromPosition {
    C3DVector Position; float ListenersHearingRange;
    CThingFilter_NoiseCanBeHeardFromPosition() : Position(0, 0, 0), ListenersHearingRange(0) {}
    bool operator()(CThing* thing) const
    {
        if (thing->HasTC(0x37)) {
            CTCNoiseMaker_37* noise = thing->GetTC<CTCNoiseMaker_37>(0x37);
            float distanceSquared = (thing->GetPosition() - Position).GetLengthSquared();
            float radius = noise->NoiseRadius;
            float range = Min(radius, ListenersHearingRange);
            return distanceSquared <= range * range;
        }
        return false;
    } };
int main()
{
    CTCNoiseMaker_37 noise; noise.NoiseRadius = 5.0f;
    CInterfaceMap::SEntry e[1] = { { 0x37, (CTCBase*)&noise } };
    CThing t; for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0; t.Components.first = e; t.Components.last = e + 1;
    t.m_pos = C3DVector(3, 4, 0);                                   // distance 5
    CThingFilter_NoiseCanBeHeardFromPosition f; f.ListenersHearingRange = 10.0f;
    int bad = 0;
    if (f(&t)) bad = 1;                                            // no TC
    t.UsingComponents[1] = 1UL << 23;
    if (!f(&t)) bad = 1;                                           // 5 <= min(5, 10)
    noise.NoiseRadius = 4.9f; if (f(&t)) bad = 1;                  // radius too small
    noise.NoiseRadius = 50.0f; f.ListenersHearingRange = 4.0f; if (f(&t)) bad = 1;   // hearing too small
    f.ListenersHearingRange = 6.0f; if (!f(&t)) bad = 1;
    if (bad) { std::printf("006a8240_TEST FAIL\n"); return 1; }
    std::printf("006a8240_TEST PASS\n");
    return 0;
}