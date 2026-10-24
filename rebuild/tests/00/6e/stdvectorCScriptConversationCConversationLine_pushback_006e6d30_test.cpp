// Behaviour test for 0x006E6D30 (vector<CConversationLine>::push_back: in-place copy when there is room, else the overflow path).
#include <cstdio>
#include <string>
#include <new>

struct Line { long PersonSpeaking; long PersonSpokenTo; std::string LineToSay; bool ShowSubtitle; };

struct LineVector {
    Line* _M_start; Line* _M_finish; Line* _M_end_of_storage;
    int overflowCalls; unsigned int lastFill; bool lastAtEnd;
    void _M_insert_overflow(Line* pos, const Line& x, unsigned int fill, bool atend)
    {
        ++overflowCalls; lastFill = fill; lastAtEnd = atend;
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        Line* mem = (Line*)operator new(sizeof(Line) * (n * 2 + 1));
        Line* out = mem;
        for (Line* p = _M_start; p != pos; ++p) new (out++) Line(*p);
        new (out++) Line(x);
        for (Line* p = _M_start; p != _M_finish; ++p) p->~Line();
        operator delete(_M_start);
        _M_start = mem; _M_finish = out; _M_end_of_storage = mem + n * 2 + 1;
    }
    void push_back(const Line& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) Line(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};
#define FAIL { std::printf("006e6d30_TEST FAIL\n"); return 1; }
int main()
{
    LineVector v; v._M_start = v._M_finish = v._M_end_of_storage = 0; v.overflowCalls = 0;
    Line a; a.PersonSpeaking = 2; a.PersonSpokenTo = -1; a.LineToSay = "hi"; a.ShowSubtitle = true;
    v.push_back(a);
    if (v.overflowCalls != 1 || v.lastFill != 1 || !v.lastAtEnd || v._M_finish - v._M_start != 1) FAIL;
    Line b = a; b.LineToSay = "there"; b.ShowSubtitle = false;
    v.push_back(b);
    if (v.overflowCalls != 2 || v._M_finish - v._M_start != 2) FAIL;
    v.push_back(a);
    if (v.overflowCalls != 2 || v._M_finish - v._M_start != 3) FAIL;
    if (v._M_start[1].LineToSay != "there" || v._M_start[1].ShowSubtitle || v._M_start[2].PersonSpeaking != 2) FAIL;
    for (Line* p = v._M_start; p != v._M_finish; ++p) p->~Line();
    operator delete(v._M_start);
    std::printf("006e6d30_TEST PASS\n");
    return 0;
}