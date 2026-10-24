#include <cstdio>
#include <cstdlib>
#include <new>

// Model of 0x004127E0 deque<ETutorialCategory>::_M_push_back_aux_v: copy the value, make sure the map has a slot
// past the finish node (_M_reallocate_map 0x00411630, modelled from STLport), malloc the next 0x80-byte node,
// construct the value in the old finish slot, then move finish to the start of the new node.
enum ETutorialCategory { TUTORIAL_NONE = 0, TUTORIAL_FIRST = 1 };
template <class T> struct CRetailDequeIterator {
    T* _M_cur;
    T* _M_first;
    T* _M_last;
    T** _M_node;
    void _M_set_node(T** newNode) { _M_last = (_M_first = *(_M_node = newNode)) + 0x80 / sizeof(T); }
};
template <class T> struct CRetailDeque {
    CRetailDequeIterator<T> _M_start;
    CRetailDequeIterator<T> _M_finish;
    T** _M_map;
    size_t _M_map_size;
    int Reallocations;
    void _M_reallocate_map(size_t nodesToAdd, bool addAtFront)
    {
        ++Reallocations;
        size_t oldNumNodes = _M_finish._M_node - _M_start._M_node + 1;
        size_t newNumNodes = oldNumNodes + nodesToAdd;
        size_t newMapSize = _M_map_size + (_M_map_size > nodesToAdd ? _M_map_size : nodesToAdd) + 2;
        T** newMap = (T**)std::malloc(newMapSize * sizeof(T*));
        T** newStart = newMap + (newMapSize - newNumNodes) / 2 + (addAtFront ? nodesToAdd : 0);
        for (size_t i = 0; i < oldNumNodes; ++i)
            newStart[i] = _M_start._M_node[i];
        std::free(_M_map);
        _M_map = newMap;
        _M_map_size = newMapSize;
        _M_start._M_set_node(newStart);
        _M_finish._M_set_node(newStart + oldNumNodes - 1);
    }
    void _M_reserve_map_at_back(size_t nodesToAdd = 1)
    {
        if (nodesToAdd + 1 > _M_map_size - (_M_finish._M_node - _M_map))
            _M_reallocate_map(nodesToAdd, false);
    }
    T* _M_allocate_node() { return (T*)std::malloc(0x80); }
    void _M_push_back_aux_v(const T& value)
    {
        T copy = value;
        _M_reserve_map_at_back();
        *(_M_finish._M_node + 1) = _M_allocate_node();
        new (_M_finish._M_cur) T(copy);
        _M_finish._M_set_node(_M_finish._M_node + 1);
        _M_finish._M_cur = _M_finish._M_first;
    }
    void push_back(const T& value)                // the inline fast path that hands over to the aux
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1) {
            new (_M_finish._M_cur) T(value);
            ++_M_finish._M_cur;
        } else
            _M_push_back_aux_v(value);
    }
};

static int Fail()
{
    std::printf("004127e0_TEST FAIL\n");
    return 1;
}

int main()
{
    CRetailDeque<ETutorialCategory> d;
    d.Reallocations = 0;
    d._M_map_size = 2;
    d._M_map = (ETutorialCategory**)std::malloc(2 * sizeof(ETutorialCategory*));
    d._M_map[0] = d._M_allocate_node();
    d._M_start._M_set_node(d._M_map);
    d._M_start._M_cur = d._M_start._M_first;
    d._M_finish = d._M_start;
    for (int i = 0; i < 70; ++i)                  // 32 per node: crosses two node boundaries
        d.push_back((ETutorialCategory)(i + 1));
    if (d.Reallocations != 1 || d._M_finish._M_node - d._M_start._M_node != 2 || d._M_finish._M_cur - d._M_finish._M_first != 6)
        return Fail();
    int expected = 1;
    for (ETutorialCategory** node = d._M_start._M_node; node <= d._M_finish._M_node; ++node) {
        ETutorialCategory* last = (node == d._M_finish._M_node) ? d._M_finish._M_cur : *node + 32;
        for (ETutorialCategory* p = *node; p != last; ++p)
            if (*p != (ETutorialCategory)expected++)
                return Fail();
    }
    if (expected != 71)
        return Fail();
    for (ETutorialCategory** node = d._M_start._M_node; node <= d._M_finish._M_node; ++node)
        std::free(*node);
    std::free(d._M_map);
    std::printf("004127e0_TEST PASS\n");
    return 0;
}