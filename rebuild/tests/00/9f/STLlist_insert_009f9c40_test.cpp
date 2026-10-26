// Behaviour test for 0x009F9C40 list<CProfileTreeEntry>::insert(position): inserts a default-constructed tree entry (Manager 0,
// default name, empty Children) before the position and destroys the temporary (name destructor, no Children buffer to free).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gStringCtors = 0, gStringCopies = 0, gStringDtors = 0, gProfileCopies = 0, gVectorCopies = 0, gFrees = 0;

class CWideString { public:
    int m_id;
    CWideString() : m_id(0) { ++gStringCtors; }
    CWideString(const CWideString& other) : m_id(other.m_id) { ++gStringCopies; }
    ~CWideString() { ++gStringDtors; }
};

class CProfileManager;

class CProfile {
public:
    CProfile() : Manager(0) {}
    CProfile(const CProfile& other)
        : Manager(other.Manager), StartTime(other.StartTime), TotalTime(other.TotalTime), LastTime(other.LastTime),
          Running(other.Running), Percentage(other.Percentage), Name(other.Name), TreeDepth(other.TreeDepth),
          NewNode(other.NewNode), Visible(other.Visible), CallCounter(other.CallCounter),
          UpdatedCallCounter(other.UpdatedCallCounter)
    {
        ++gProfileCopies;
    }
    CProfileManager* Manager;
    double StartTime;
    double TotalTime;
    double LastTime;
    bool Running;
    double Percentage;
    CWideString Name;
    long TreeDepth;
    bool NewNode;
    long Visible;
    long CallCounter;
    long UpdatedCallCounter;
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    vector(const vector& x)
    {
        ++gVectorCopies;
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0;
        if (n)
            std::memcpy(_M_start, x._M_start, n * sizeof(T));
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    ~vector() { if (_M_start) { ++gFrees; std::free(_M_start); } }
};

class CProfileManager {
public:
    struct CProfileTreeEntry {
        CProfile Profile;
        long ID;
        vector<CProfileTreeEntry*> Children;
        CProfileTreeEntry* Parent;
    };
};
typedef CProfileManager::CProfileTreeEntry CProfileTreeEntry;

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };

template <class T> struct list {
    typedef _List_node<T> _Node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* n) : _M_node(n) {}
        iterator(const iterator& x) : _M_node(x._M_node) {}
    };
    _Node* _M_node;
    list() { _M_node = (_Node*)std::malloc(sizeof(_Node)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = (_Node*)std::malloc(sizeof(_Node));
        new (&tmp->_M_data) T(x);
        _List_node_base* n = position._M_node;
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    iterator insert(iterator position) { return insert(position, T()); }
};

int main()
{
    list<CProfileTreeEntry> l;
    gStringCtors = gStringDtors = gFrees = 0;
    list<CProfileTreeEntry>::iterator it = l.insert(l.end());
    _List_node<CProfileTreeEntry>* n = (_List_node<CProfileTreeEntry>*)it._M_node;
    bool ok = l._M_node->_M_next == n && l._M_node->_M_prev == n && n->_M_data.Profile.Manager == 0
           && n->_M_data.Children._M_start == 0 && n->_M_data.Children._M_finish == 0
           && gStringCtors == 1 && gStringCopies == 1 && gStringDtors == 1 && gProfileCopies == 1 && gFrees == 0;
    list<CProfileTreeEntry>::iterator it2 = l.insert(l.begin());
    ok = ok && l._M_node->_M_next == it2._M_node && it2._M_node->_M_next == it._M_node && gStringDtors == 2;
    if (!ok) { std::printf("009f9c40_TEST FAIL\n"); return 1; }
    std::printf("009f9c40_TEST PASS\n");
    return 0;
}