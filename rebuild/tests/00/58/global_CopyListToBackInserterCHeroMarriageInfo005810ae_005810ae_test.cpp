#include <cstdio>

struct CHeroMarriageInfo { long m_words[6]; };

struct CHeroMarriageInfoVector {                 // fixed-capacity stand-in for the STLport vector
    CHeroMarriageInfo m_items[8];
    int m_count;
    CHeroMarriageInfoVector() : m_count(0) { }
    void push_back(const CHeroMarriageInfo& x) { m_items[m_count++] = x; }
};
struct _List_node {
    _List_node*       _M_next;
    _List_node*       _M_prev;
    CHeroMarriageInfo _M_data;
};
struct _List_iterator {
    _List_node* _M_node;
    explicit _List_iterator(_List_node* n) : _M_node(n) { }
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) { }
    CHeroMarriageInfo& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
struct back_insert_iterator {
    CHeroMarriageInfoVector* container;
    explicit back_insert_iterator(CHeroMarriageInfoVector& x) : container(&x) { }
    back_insert_iterator& operator=(const CHeroMarriageInfo& value) { container->push_back(value); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};
struct input_iterator_tag { input_iterator_tag() { } };

static back_insert_iterator Copy(_List_iterator first, _List_iterator last, back_insert_iterator result, const input_iterator_tag&, int*)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}

int main()
{
    // circular list with a head sentinel and three nodes
    _List_node head, n[3];
    head._M_next = &n[0]; n[0]._M_next = &n[1]; n[1]._M_next = &n[2]; n[2]._M_next = &head;
    head._M_prev = &n[2]; n[0]._M_prev = &head; n[1]._M_prev = &n[0]; n[2]._M_prev = &n[1];
    for (int i = 0; i < 3; ++i) for (int w = 0; w < 6; ++w) n[i]._M_data.m_words[w] = i * 10 + w;
    CHeroMarriageInfoVector v;
    input_iterator_tag tag;
    back_insert_iterator r = Copy(_List_iterator(head._M_next), _List_iterator(&head), back_insert_iterator(v), tag, 0);
    bool ok = r.container == &v && v.m_count == 3;
    for (int i = 0; i < 3; ++i) for (int w = 0; w < 6; ++w) if (v.m_items[i].m_words[w] != i * 10 + w) ok = false;
    Copy(_List_iterator(&head), _List_iterator(&head), back_insert_iterator(v), tag, 0);     // empty range
    if (v.m_count != 3) ok = false;
    if (!ok) { std::printf("005810ae_TEST FAIL\n"); return 1; }
    std::printf("005810ae_TEST PASS\n");
    return 0;
}