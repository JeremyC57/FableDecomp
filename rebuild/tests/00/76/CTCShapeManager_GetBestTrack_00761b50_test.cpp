// Behaviour test for 0x00761B50 CTCShapeManager::GetBestTrack: returns the FIRST persistent shape whose GetType() is
// the track type (1) through the out pointer; with no track the out pointer is left alone and false is returned.
#include <cstdio>

struct CShape {
    int m_type;
    explicit CShape(int t) : m_type(t) { }
    virtual ~CShape() { }
    virtual int GetType() const { return m_type; }
};
struct CTrack : public CShape { CTrack() : CShape(1) { } };

struct CCountedPointer {                         // {Data, Info}: 8-byte element as in retail
    CShape* Data;
    void* Info;
    CShape* operator->() const { return Data; }
};

struct CIsShapeTrack {
    bool operator()(CCountedPointer shape) const { return shape->GetType() == 1; }
};

template <class It, class Pred> It find_if_model(It first, It last, Pred pred)
{
    for ( ; first != last; ++first)
        if (pred(*first))
            break;
    return first;
}

class CTCShapeManager { public:
    CCountedPointer* first;
    CCountedPointer* last;
    bool GetBestTrack(const CTrack** out) const
    {
        const CCountedPointer* it = find_if_model((const CCountedPointer*)first, (const CCountedPointer*)last, CIsShapeTrack());
        if (it != last) {
            *out = static_cast<const CTrack*>(it->operator->());
            return true;
        }
        return false;
    }
};

int main()
{
    CShape a(0), b(2);
    CTrack t1, t2;
    CCountedPointer v[4] = { { &a, 0 }, { &b, 0 }, { &t1, 0 }, { &t2, 0 } };
    CTCShapeManager m;
    int ok = 1;
    const CTrack* out = 0;
    m.first = v; m.last = v + 4;
    if (!m.GetBestTrack(&out) || out != &t1) ok = 0;          // first track wins
    m.first = v; m.last = v + 2;                              // no track
    const CTrack* sentinel = &t2;
    out = sentinel;
    if (m.GetBestTrack(&out) || out != sentinel) ok = 0;      // out untouched
    m.first = v + 3; m.last = v + 4;
    if (!m.GetBestTrack(&out) || out != &t2) ok = 0;
    m.first = v; m.last = v;                                  // empty
    if (m.GetBestTrack(&out)) ok = 0;
    if (!ok) { std::printf("00761b50_TEST FAIL\n"); return 1; }
    std::printf("00761b50_TEST PASS\n");
    return 0;
}