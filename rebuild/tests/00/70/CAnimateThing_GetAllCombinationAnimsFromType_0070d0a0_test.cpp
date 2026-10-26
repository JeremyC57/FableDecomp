// Behaviour test for 0x0070D0A0 CAnimateThing::GetAllCombinationAnimsFromType.
#include <cstdio>
#include <cstdlib>
#include <new>

class CAnimSet { public: unsigned char Type; int id; };
struct SNode { SNode* next; SNode* prev; CAnimSet data; };
struct CVector {
    CAnimSet** _M_start; CAnimSet** _M_finish; CAnimSet** _M_end_of_storage;
    void push_back(CAnimSet* const& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CAnimSet*(x); ++_M_finish; return; }
        unsigned int size = (unsigned int)(_M_finish - _M_start), len = size + (size < 1 ? 1 : size);
        CAnimSet** p = (CAnimSet**)std::malloc(len * sizeof(CAnimSet*));
        for (unsigned int i = 0; i < size; ++i) p[i] = _M_start[i];
        p[size] = x;
        std::free(_M_start);
        _M_start = p; _M_finish = p + size + 1; _M_end_of_storage = p + len;
    }
};
class CAnimateThing { public:
    SNode* CombinationAnims;
    void GetAllCombinationAnimsFromType(long type, CVector& out)
    {
        for (SNode* it = CombinationAnims->next; it != CombinationAnims; it = it->next)
            if (it->data.Type == type)
                out.push_back(&it->data);
    }
};

int main()
{
    SNode head; head.next = &head; head.prev = &head;
    SNode nodes[5];
    const unsigned char types[5] = { 3, 7, 3, 3, 1 };
    for (int i = 0; i < 5; ++i) {
        nodes[i].data.Type = types[i]; nodes[i].data.id = i;
        nodes[i].next = &head; nodes[i].prev = head.prev; head.prev->next = &nodes[i]; head.prev = &nodes[i];
    }
    CAnimateThing thing; thing.CombinationAnims = &head;
    CVector out; out._M_start = 0; out._M_finish = 0; out._M_end_of_storage = 0;
    thing.GetAllCombinationAnimsFromType(3, out);
    bool ok = out._M_finish - out._M_start == 3 && out._M_start[0] == &nodes[0].data && out._M_start[1] == &nodes[2].data && out._M_start[2] == &nodes[3].data;
    thing.GetAllCombinationAnimsFromType(9, out);
    ok = ok && out._M_finish - out._M_start == 3;
    thing.GetAllCombinationAnimsFromType(1, out);
    ok = ok && out._M_finish - out._M_start == 4 && out._M_start[3]->id == 4;
    if (!ok) { std::printf("0070d0a0_TEST FAIL\n"); return 1; }
    std::printf("0070d0a0_TEST PASS\n");
    return 0;
}