// Behaviour test for 00c43780: CAudioObject::SetLoopPoints -- forwards (&m_pLoopPoints, &m_loopPointsStorage, value) to the set-or-clear helper
#include <cstdio>

struct SLoopPoints {
    long  m_start;
    float m_length;
    bool operator==(const SLoopPoints& o) const { return m_start == o.m_start && m_length == o.m_length; }
};

class CAudioObject {
public:
    CAudioObject() : m_dirtyFlags(0), m_pLoopPoints(0) { m_loopPointsStorage.m_start = 0; m_loopPointsStorage.m_length = 0.0f; }
    virtual ~CAudioObject() {}
    virtual void SetLoopPoints(const SLoopPoints* value) { UpdateLoopPoints(&m_pLoopPoints, &m_loopPointsStorage, value); }
    // the landed helper 0x00C43420
    void UpdateLoopPoints(SLoopPoints** current, SLoopPoints* storage, const SLoopPoints* value)
    {
        if (!value) { if (*current) { *current = 0; m_dirtyFlags |= 0x80; } return; }
        if (!*current) { *current = storage; *storage = *value; m_dirtyFlags |= 0x80; return; }
        if (!(*storage == *value)) { *storage = *value; m_dirtyFlags |= 0x80; }
    }
    unsigned int  m_dirtyFlags;
    SLoopPoints*  m_pLoopPoints;
    SLoopPoints   m_loopPointsStorage;
};

int main()
{
    CAudioObject o;
    SLoopPoints v; v.m_start = 7; v.m_length = 2.5f;
    o.SetLoopPoints(&v);
    if (o.m_pLoopPoints != &o.m_loopPointsStorage || o.m_loopPointsStorage.m_start != 7 || o.m_dirtyFlags != 0x80) {
        std::printf("00c43780_TEST FAIL\n"); return 1;
    }
    o.m_dirtyFlags = 0;
    o.SetLoopPoints(&v);                       // unchanged: no dirty bit
    if (o.m_dirtyFlags != 0) { std::printf("00c43780_TEST FAIL\n"); return 1; }
    o.SetLoopPoints(0);                        // clear
    if (o.m_pLoopPoints != 0 || o.m_dirtyFlags != 0x80) { std::printf("00c43780_TEST FAIL\n"); return 1; }
    std::printf("00c43780_TEST PASS\n");
    return 0;
}