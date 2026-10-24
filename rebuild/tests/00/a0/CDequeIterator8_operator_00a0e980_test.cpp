#include <cstdio>

// Model of 0x00A0E980: STLport deque iterator (8-byte element, 0x80-byte nodes = 16 elements) operator+: copy the
// iterator, advance the copy with operator+= (0x00A0E2A0, modelled here from STLport _deque.h) and return it.
struct SDequeElement8 {
    long m_a;
    long m_b;
};
struct CDequeIterator8 {
    SDequeElement8* _M_cur;
    SDequeElement8* _M_first;
    SDequeElement8* _M_last;
    SDequeElement8** _M_node;
    static int _S_buffer_size() { return 0x80 / 8; }
    void _M_set_node(SDequeElement8** node)
    {
        _M_node = node;
        _M_first = *node;
        _M_last = _M_first + _S_buffer_size();
    }
    CDequeIterator8& operator+=(int n)
    {
        int offset = n + (int)(_M_cur - _M_first);
        if (offset >= 0 && offset < _S_buffer_size())
            _M_cur += n;
        else {
            int nodeOffset = offset > 0 ? offset / _S_buffer_size() : -((-offset - 1) / _S_buffer_size()) - 1;
            _M_set_node(_M_node + nodeOffset);
            _M_cur = _M_first + (offset - nodeOffset * _S_buffer_size());
        }
        return *this;
    }
    CDequeIterator8 operator+(int n) const
    {
        CDequeIterator8 tmp = *this;
        tmp += n;
        return tmp;
    }
};

static int Fail()
{
    std::printf("00a0e980_TEST FAIL\n");
    return 1;
}

int main()
{
    static SDequeElement8 nodes[3][16];
    SDequeElement8* map[3] = { nodes[0], nodes[1], nodes[2] };
    for (int n = 0; n < 3; ++n)
        for (int i = 0; i < 16; ++i) {
            nodes[n][i].m_a = n * 16 + i;
            nodes[n][i].m_b = -(n * 16 + i);
        }
    CDequeIterator8 it;
    it._M_set_node(&map[1]);
    it._M_cur = it._M_first + 5;                  // logical index 21
    CDequeIterator8 a = it + 3;                   // same node
    CDequeIterator8 b = it + 20;                  // next node, index 41
    CDequeIterator8 c = it + (-10);               // previous node, index 11
    CDequeIterator8 d = it + (-5);                // first of the node, index 16
    CDequeIterator8 e = it + 11;                  // first of the next node, index 32
    if (it._M_cur != nodes[1] + 5 || it._M_node != &map[1])
        return Fail();
    if (a._M_cur->m_a != 24 || a._M_node != &map[1])
        return Fail();
    if (b._M_cur->m_a != 41 || b._M_node != &map[2] || b._M_first != nodes[2] || b._M_last != nodes[2] + 16)
        return Fail();
    if (c._M_cur->m_a != 11 || c._M_node != &map[0] || c._M_first != nodes[0])
        return Fail();
    if (d._M_cur->m_a != 16 || d._M_node != &map[1] || e._M_cur->m_a != 32 || e._M_node != &map[2])
        return Fail();
    std::printf("00a0e980_TEST PASS\n");
    return 0;
}