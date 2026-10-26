#include <cstdio>
// Behaviour test for COpinionDeedEffectsDef::COpinionDeedEffectsDef (0x00456D69): base first, the list
// gets a self-linked sentinel, both vectors start empty.
static int g_baseBuilt = 0;
class CParentDefClassBase { public: virtual void v00() { } CParentDefClassBase() { ++g_baseBuilt; } };
struct SNode { SNode* next; SNode* prev; };
template <class T> struct allocator { allocator() { } };
template <class T> struct list {
    SNode* _M_node; SNode sentinel;
    list(const allocator<T>& = allocator<T>()) { _M_node = &sentinel; sentinel.next = sentinel.prev = &sentinel; }
};
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { } };
class COpinionDeedEffectsDef : public CParentDefClassBase { public:
    long KnowledgeLifeInSeconds; list<int> Effects; long ReactionPriority; vector<int> Reaction; long Layer; vector<int> WarningText;
    COpinionDeedEffectsDef() { }
};
int main()
{
    COpinionDeedEffectsDef d;
    if (g_baseBuilt != 1 || d.Effects._M_node->next != d.Effects._M_node || d.Reaction._M_start != 0 || d.Reaction._M_finish != 0 ||
        d.Reaction._M_end_of_storage != 0 || d.WarningText._M_start != 0 || d.WarningText._M_end_of_storage != 0) { std::printf("00456d69_TEST FAIL\n"); return 1; }
    std::printf("00456d69_TEST PASS\n");
    return 0;
}