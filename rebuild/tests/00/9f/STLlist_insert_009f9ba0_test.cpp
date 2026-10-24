// Behaviour test for 0x009F9BA0 list<CProfileTreeEntry>::insert(position, x): a new node holding a member-wise copy of the
// tree entry (CProfile copy constructor, ID, Children vector copy, Parent) is linked before the position and returned.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gStringCtors = 0, gStringCopies = 0, gStringDtors = 0, gProfileCopies = 0, gVectorCopies = 0, gFrees = 0;

struct CWideString {
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
    CProfileTreeEntry child;
    CProfileTreeEntry e;
    e.Profile.Manager = (CProfileManager*)0x1234;
    e.Profile.TotalTime = 2.5;
    e.Profile.Name.m_id = 77;
    e.Profile.UpdatedCallCounter = 5;
    e.ID = 42;
    CProfileTreeEntry* kids[2] = { &child, &e };
    e.Children._M_start = kids; e.Children._M_finish = kids + 2; e.Children._M_end_of_storage = kids + 2;
    e.Parent = &child;
    list<CProfileTreeEntry>::iterator it = l.insert(l.end(), e);
    _List_node<CProfileTreeEntry>* n = (_List_node<CProfileTreeEntry>*)it._M_node;
    bool ok = n->_M_prev == l._M_node && n->_M_next == l._M_node && l._M_node->_M_next == n && l._M_node->_M_prev == n
           && n->_M_data.Profile.Manager == (CProfileManager*)0x1234 && n->_M_data.Profile.TotalTime == 2.5
           && n->_M_data.Profile.Name.m_id == 77 && n->_M_data.Profile.UpdatedCallCounter == 5 && n->_M_data.ID == 42
           && n->_M_data.Children._M_start != kids && n->_M_data.Children._M_start[1] == &e && n->_M_data.Parent == &child
           && gProfileCopies == 1 && gStringCopies == 1 && gVectorCopies == 1;
    list<CProfileTreeEntry>::iterator it2 = l.insert(it, e);                    // before the first: becomes the new first
    ok = ok && l._M_node->_M_next == it2._M_node && it2._M_node->_M_next == it._M_node && it._M_node->_M_prev == it2._M_node;
    e.Children._M_start = 0;                                                  // the stack array is not ours to free
    if (!ok) { std::printf("009f9ba0_TEST FAIL\n"); return 1; }
    std::printf("009f9ba0_TEST PASS\n");
    return 0;
}