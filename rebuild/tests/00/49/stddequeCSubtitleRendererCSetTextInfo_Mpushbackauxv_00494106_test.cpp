#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Behaviour model of the STLport deque<CSetTextInfo> (4 records of 28 bytes per node, malloc allocator).
static int g_copies = 0, g_dtors = 0;

struct CSetTextInfo {
    const char*   Text;
    const char*   Button1Text;
    const char*   Button2Text;
    const char*   Button3Text;
    unsigned long DisplayFlags;
    float         DisplayTime;
    bool          UpdateWhilePaused;
    bool          ChangePlayerMode;
    CSetTextInfo(const char* t, unsigned long f)
        : Text(t), Button1Text(""), Button2Text(""), Button3Text(""), DisplayFlags(f), DisplayTime(1.5f),
          UpdateWhilePaused(true), ChangePlayerMode(false) { }
    CSetTextInfo(const CSetTextInfo& o)
        : Text(o.Text), Button1Text(o.Button1Text), Button2Text(o.Button2Text), Button3Text(o.Button3Text),
          DisplayFlags(o.DisplayFlags), DisplayTime(o.DisplayTime), UpdateWhilePaused(o.UpdateWhilePaused),
          ChangePlayerMode(o.ChangePlayerMode) { ++g_copies; }
    ~CSetTextInfo() { ++g_dtors; }
};

static unsigned int buffer_size() { return sizeof(CSetTextInfo) < 128 ? (unsigned int)(128 / sizeof(CSetTextInfo)) : 1; }

struct Iter {
    CSetTextInfo*  _M_cur;
    CSetTextInfo*  _M_first;
    CSetTextInfo*  _M_last;
    CSetTextInfo** _M_node;
    void _M_set_node(CSetTextInfo** n) { _M_last = (_M_first = *(_M_node = n)) + int(buffer_size()); }
};

struct Deque {
    Iter           _M_start;
    Iter           _M_finish;
    CSetTextInfo** _M_map;
    unsigned int   _M_map_size;
    int            m_reallocs;

    Deque() : _M_map_size(8), m_reallocs(0)
    {
        _M_map = (CSetTextInfo**)std::malloc(_M_map_size * sizeof(CSetTextInfo*));
        std::memset(_M_map, 0, _M_map_size * sizeof(CSetTextInfo*));
        CSetTextInfo** n = _M_map + 3;
        *n = (CSetTextInfo*)std::malloc(buffer_size() * sizeof(CSetTextInfo));
        _M_start._M_set_node(n); _M_start._M_cur = _M_start._M_first;
        _M_finish = _M_start;
    }
    void _M_reallocate_map(unsigned int add)
    {
        ++m_reallocs;
        unsigned int old_nodes = (unsigned int)(_M_finish._M_node - _M_start._M_node) + 1;
        unsigned int new_size = _M_map_size + (_M_map_size > add ? _M_map_size : add) + 2;
        CSetTextInfo** m = (CSetTextInfo**)std::malloc(new_size * sizeof(CSetTextInfo*));
        std::memset(m, 0, new_size * sizeof(CSetTextInfo*));
        CSetTextInfo** ns = m + (new_size - old_nodes - add) / 2;
        std::memcpy(ns, _M_start._M_node, old_nodes * sizeof(CSetTextInfo*));
        std::free(_M_map);
        _M_map = m; _M_map_size = new_size;
        _M_start._M_node = ns;
        _M_finish._M_node = ns + old_nodes - 1;
    }
    void _M_reserve_map_at_back(unsigned int add = 1)
    {
        if (add + 1 > _M_map_size - (unsigned int)(_M_finish._M_node - _M_map)) _M_reallocate_map(add);
    }
    void _M_push_back_aux_v(const CSetTextInfo& __t)
    {
        CSetTextInfo __t_copy = __t;
        _M_reserve_map_at_back();
        unsigned int n = buffer_size();
        *(_M_finish._M_node + 1) = n != 0 ? (CSetTextInfo*)std::malloc(n * sizeof(CSetTextInfo)) : 0;
        new (_M_finish._M_cur) CSetTextInfo(__t_copy);
        _M_finish._M_set_node(_M_finish._M_node + 1);
        _M_finish._M_cur = _M_finish._M_first;
    }
    void push_back(const CSetTextInfo& __t)
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1) {
            new (_M_finish._M_cur) CSetTextInfo(__t);
            ++_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(__t);
    }
    int size() const
    {
        return (int)(buffer_size() * (unsigned int)(_M_finish._M_node - _M_start._M_node - 1)
                     + (_M_finish._M_cur - _M_finish._M_first) + (_M_start._M_last - _M_start._M_cur));
    }
    const CSetTextInfo& at(int i) const
    {
        int off = i + (int)(_M_start._M_cur - _M_start._M_first);
        return *(_M_start._M_node[off / (int)buffer_size()] + off % (int)buffer_size());
    }
};

static const char* g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p",
                                 "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F",
                                 "G", "H", "I", "J", "K", "L", "M", "N" };
int main()
{
    if (buffer_size() * sizeof(CSetTextInfo) != 0x70 && sizeof(void*) == 4) { std::printf("00494106_TEST FAIL\n"); return 1; }
    Deque d;
    // fill the first node up to its last free slot, then take the slow path directly
    for (int i = 0; i < (int)buffer_size() - 1; ++i) { CSetTextInfo v(g_names[i], (unsigned long)i); new (d._M_finish._M_cur) CSetTextInfo(v); ++d._M_finish._M_cur; }
    if (d._M_finish._M_cur != d._M_finish._M_last - 1) { std::printf("00494106_TEST FAIL\n"); return 1; }
    CSetTextInfo** oldNode = d._M_finish._M_node;
    CSetTextInfo* oldCur = d._M_finish._M_cur;
    g_copies = 0; g_dtors = 0;
    CSetTextInfo v("slow", 77);
    d._M_push_back_aux_v(v);
    if (g_copies != 2 || g_dtors != 1) { std::printf("00494106_TEST FAIL\n"); return 1; }   // copy + construct, copy destroyed
    if (oldCur->DisplayFlags != 77 || std::strcmp(oldCur->Text, "slow") != 0) { std::printf("00494106_TEST FAIL\n"); return 1; }
    if (d._M_finish._M_node != oldNode + 1 || d._M_finish._M_cur != d._M_finish._M_first ||
        d._M_finish._M_first != *(oldNode + 1) || d._M_finish._M_last != d._M_finish._M_first + buffer_size()) { std::printf("00494106_TEST FAIL\n"); return 1; }
    if (d.size() != (int)buffer_size()) { std::printf("00494106_TEST FAIL\n"); return 1; }
    // keep taking the slow path through push_back until the map has to grow
    for (int i = 0; i < 36; ++i) { CSetTextInfo w(g_names[i % 40], (unsigned long)(100 + i)); d.push_back(w); }
    if (d.size() != (int)buffer_size() + 36 || d.m_reallocs == 0) { std::printf("00494106_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 36; ++i) if (d.at((int)buffer_size() + i).DisplayFlags != (unsigned long)(100 + i)) { std::printf("00494106_TEST FAIL\n"); return 1; }
    std::printf("00494106_TEST PASS\n");
    return 0;
}