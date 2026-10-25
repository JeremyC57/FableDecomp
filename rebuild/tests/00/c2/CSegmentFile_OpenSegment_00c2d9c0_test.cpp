// Behaviour test for CSegmentFile::OpenSegment @ 0x00C2D9C0 (look the segment up, select its range; 0 / -1).
#include <cstdio>

struct SSegment { unsigned int m_first; unsigned int m_count; };

struct CSegmentTable {
    unsigned int ids[3];
    SSegment     segments[3];
    SSegment* Find(unsigned int id)
    {
        for (int i = 0; i < 3; ++i)
            if (ids[i] == id)
                return &segments[i];
        return 0;
    }
};

struct CSegmentFile {
    CSegmentTable m_segments;
    unsigned int  m_size, m_rangeFirst, m_rangeCount;
    long SetRange(unsigned int first, unsigned int count)      // the landed 0x00C3AA50 behaviour
    {
        if (first + count > m_size) { m_rangeFirst = 0; m_rangeCount = 0; return -1; }
        m_rangeFirst = first; m_rangeCount = count;
        return 0;
    }
    long OpenSegment(unsigned int id)
    {
        SSegment* segment = m_segments.Find(id);
        if (!segment)
            return -1;
        return SetRange(segment->m_first, segment->m_count) >= 0 ? 0 : -1;
    }
};

int main()
{
    CSegmentFile f;
    f.m_size = 1000; f.m_rangeFirst = 7; f.m_rangeCount = 7;
    f.m_segments.ids[0] = 10; f.m_segments.segments[0].m_first = 100; f.m_segments.segments[0].m_count = 200;
    f.m_segments.ids[1] = 11; f.m_segments.segments[1].m_first = 900; f.m_segments.segments[1].m_count = 200;
    f.m_segments.ids[2] = 12; f.m_segments.segments[2].m_first = 0;   f.m_segments.segments[2].m_count = 1000;
    bool ok = f.OpenSegment(10) == 0 && f.m_rangeFirst == 100 && f.m_rangeCount == 200;
    ok = ok && f.OpenSegment(99) == -1 && f.m_rangeFirst == 100;     // unknown: the range is left alone
    ok = ok && f.OpenSegment(11) == -1 && f.m_rangeCount == 0;       // does not fit
    ok = ok && f.OpenSegment(12) == 0 && f.m_rangeCount == 1000;
    if (!ok) { std::printf("00c2d9c0_TEST FAIL\n"); return 1; }
    std::printf("00c2d9c0_TEST PASS\n");
    return 0;
}