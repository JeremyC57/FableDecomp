// Behaviour test for 00c3b7a0: CBankCriteria::Serialise -- name, then the element count (read + resize when reading, the current count when writing), then every element; false at the first failed transfer
#include <cstdio>

struct CString { const char* m_pData; };

struct CParser {
    bool m_reading;
    unsigned long m_stream[16];   // reading: values handed out; writing: values recorded
    int m_pos;
    int m_failAt;                 // transfer index that fails (-1: never)
    int m_count;
    bool IsReading() { return m_reading; }
    bool Serialise(unsigned long& v)
    {
        if (m_count++ == m_failAt) return false;
        if (m_reading) v = m_stream[m_pos++]; else m_stream[m_pos++] = v;
        return true;
    }
    bool Serialise(CString&) { return m_count++ != m_failAt; }
};

struct CAutoDeleteArray {
    unsigned long m_data[8];
    unsigned long m_size;
    void Resize(unsigned long size) { m_size = size; }
    unsigned long& operator[](unsigned long index) { return m_data[index]; }
};

struct CBankCriteria {
    CString m_name;
    CAutoDeleteArray m_values;
    bool Serialise(CParser& parser)
    {
        if (!parser.Serialise(m_name))
            return false;
        unsigned long count;
        if (parser.IsReading()) {
            if (!parser.Serialise(count))
                return false;
            m_values.Resize(count);
        } else {
            count = m_values.m_size;
            if (!parser.Serialise(count))
                return false;
        }
        for (unsigned long i = 0; i < m_values.m_size; ++i) {
            if (!parser.Serialise(m_values[i]))
                return false;
        }
        return true;
    }
};

static int fail() { std::printf("00c3b7a0_TEST FAIL\n"); return 1; }

int main()
{
    CBankCriteria c; c.m_name.m_pData = "x";
    CParser r; r.m_reading = true; r.m_pos = 0; r.m_failAt = -1; r.m_count = 0;
    r.m_stream[0] = 3; r.m_stream[1] = 10; r.m_stream[2] = 20; r.m_stream[3] = 30;
    if (!c.Serialise(r) || c.m_values.m_size != 3 || c.m_values.m_data[2] != 30) return fail();
    CParser w; w.m_reading = false; w.m_pos = 0; w.m_failAt = -1; w.m_count = 0;
    if (!c.Serialise(w) || w.m_pos != 4 || w.m_stream[0] != 3 || w.m_stream[3] != 30) return fail();
    CParser f; f.m_reading = false; f.m_pos = 0; f.m_failAt = 3; f.m_count = 0;      // second element fails
    if (c.Serialise(f) || f.m_count != 4) return fail();
    CParser g; g.m_reading = true; g.m_pos = 0; g.m_failAt = 0; g.m_count = 0;      // name fails
    if (c.Serialise(g) || g.m_count != 1) return fail();
    std::printf("00c3b7a0_TEST PASS\n");
    return 0;
}