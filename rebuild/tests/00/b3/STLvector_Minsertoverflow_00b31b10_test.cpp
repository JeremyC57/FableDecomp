#include <cstdio>
#include <cstdlib>
#include <new>

// Model of STLport vector<CDebugScriptElement>::_M_insert_overflow: grow to old + max(old, n), copy head, n copies, tail.
struct CDebugScriptElement { float v[8]; };
struct Vec {
    CDebugScriptElement* start;
    CDebugScriptElement* finish;
    CDebugScriptElement* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void InsertOverflow(CDebugScriptElement* pos, const CDebugScriptElement& x, unsigned int fillLen, bool atEnd)
    {
        unsigned int oldSize = size();
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        CDebugScriptElement* ns = len ? (CDebugScriptElement*)std::malloc(len * sizeof(CDebugScriptElement)) : 0;
        CDebugScriptElement* nf = ns;
        for (CDebugScriptElement* p = start; p != pos; ++p, ++nf) new (nf) CDebugScriptElement(*p);
        for (unsigned int i = 0; i < fillLen; ++i, ++nf) new (nf) CDebugScriptElement(x);
        if (!atEnd)
            for (CDebugScriptElement* p = pos; p != finish; ++p, ++nf) new (nf) CDebugScriptElement(*p);
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    CDebugScriptElement e; for (int i = 0; i < 8; ++i) e.v[i] = (float)i;
    v.InsertOverflow(v.finish, e, 1, true);
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].v[7] != 7.0f) { std::printf("00b31b10_TEST FAIL\n"); return 1; }
    CDebugScriptElement f; for (int i = 0; i < 8; ++i) f.v[i] = 10.0f + i;
    v.InsertOverflow(v.start, f, 3, false);
    if (v.size() != 4 || v.eos - v.start != 4 || v.start[0].v[0] != 10.0f || v.start[2].v[1] != 11.0f || v.start[3].v[7] != 7.0f) { std::printf("00b31b10_TEST FAIL\n"); return 1; }
    std::free(v.start);
    std::printf("00b31b10_TEST PASS\n");
    return 0;
}