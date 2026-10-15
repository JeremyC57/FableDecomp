// CUserProfileManager::GetAssignedInputsPassive() -- STLport deque<T>::back() for a 0x30-byte element: copy the finish iterator, step it back one element, return the element
// Retail: Fable.exe @ 0x00913960, 42 bytes (+ int3 padding). No callees, no direct callers (an out-of-line instance).
// Label note: the body is a deque back(), not the profile manager's input-vector getter (propagated manifest label);
// `this` is the deque: _M_start at +0x00, _M_finish (cur, first, last, node) at +0x10.
// Shape: STLport `iterator tmp = _M_finish; --tmp; return *tmp;` with the IMPLICIT (bitwise) iterator copy: the
// 16-byte copy gets a 0x10-byte frame of which only the dead `last` word survives as a store (a user memberwise
// copy constructor drops the frame entirely: 23 v 42); operator-- steps to the previous node when cur == first
// (`first = *(node - 1)`, `last = first + buffer size`, 2 elements = 0x60 bytes), then `cur -= 1` (`sub eax,30h`).
struct SElement30 {
    unsigned char _data[0x30];
};

template <class T>
struct CDequeIterator {
    T* _M_cur;
    T* _M_first;
    T* _M_last;
    T** _M_node;
    static unsigned long buffer_size() { return sizeof(T) < 128 ? 128 / sizeof(T) : 1; }
    void _M_set_node(T** newNode)
    {
        _M_node = newNode;
        _M_first = *newNode;
        _M_last = _M_first + buffer_size();
    }
    CDequeIterator& operator--()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
        return *this;
    }
    T& operator*() const { return *_M_cur; }
};

struct CUserProfileManager {                 // really std::deque<SElement30>
    CDequeIterator<SElement30> _M_start;     // +0x00
    CDequeIterator<SElement30> _M_finish;    // +0x10
    SElement30** _M_map;                     // +0x20
    unsigned long _M_map_size;               // +0x24
    SElement30& GetAssignedInputsPassive();
};

SElement30& CUserProfileManager::GetAssignedInputsPassive()
{
    CDequeIterator<SElement30> tmp = _M_finish;
    --tmp;
    return *tmp;
}