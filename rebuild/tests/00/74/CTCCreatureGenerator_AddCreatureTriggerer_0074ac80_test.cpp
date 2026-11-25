// Behaviour test for 0x0074AC80 CTCCreatureGenerator::AddCreatureTriggerer(CThingCreatureBase&) (NF446): links the row object and calls it by
// its own symbol. The class text below is the row's prelude verbatim (no new class body); the out-of-line callees it binds (the intelligent
// pointer, STLport __find 0x0074CEE0, vector::push_back 0x0074DA40) are defined here and record their calls. CreatureTriggerers is +0x1C.
class CBaseObject;
class CThingCreatureBase;

class CBaseIntelligentPointer {                 // 8 bytes: vptr + the pointed-to item
public:
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class T>
RandomAccessIter __fastcall __find(RandomAccessIter first, RandomAccessIter last, const T& val,
                                   const random_access_iterator_tag&);                     // 0x0074CEE0

template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last, const T& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}

template <class T> class vector {
public:
    T* _M_start;                                 // +0x00
    T* _M_finish;                                // +0x04
    T* _M_end_of_storage;                        // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void push_back(const T& x);                  // 0x0074DA40
};
}

#define CTCCreatureGenerator CTCCreatureGenerator_HeaderLayout
#include "engine/CTCCreatureGenerator.h"
#undef CTCCreatureGenerator
class CTCCreatureGenerator : public CTCCreatureGenerator_HeaderLayout {
public:
    void AddCreatureTriggerer(CThingCreatureBase& thing);
};


#include <stdio.h>
#include <string.h>
#include <new>
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = item; }
static int g_finds = 0, g_pushes = 0;
typedef CIntelligentPointer<CThingCreatureBase> TP;
namespace std {
template <> TP* __fastcall __find<TP*, TP>(TP* first, TP* last, const TP& val, const random_access_iterator_tag&)
{
    ++g_finds;
    for (; first != last; ++first)
        if (first->m_pItem == val.m_pItem)
            return first;
    return last;
}
template <> void vector<TP>::push_back(const TP& x) { ++g_pushes; _M_finish->m_pItem = x.m_pItem; ++_M_finish; }
}
static unsigned char g_gen[0x100];
static TP* g_store;
static int add(CThingCreatureBase* c) { ((CTCCreatureGenerator*)g_gen)->CTCCreatureGenerator::AddCreatureTriggerer(*c); return 1; }
int main(void)
{
    static unsigned char raw[sizeof(TP) * 8];
    memset(g_gen, 0, sizeof g_gen);
    g_store = (TP*)raw;
    TP** v = (TP**)(g_gen + 0x1C);
    v[0] = v[1] = g_store; v[2] = g_store + 8;
    CThingCreatureBase* a = (CThingCreatureBase*)0x1000; CThingCreatureBase* b = (CThingCreatureBase*)0x2000;
    add(a); add(b); add(a);
    int ok = g_finds == 3 && g_pushes == 2 && v[1] == g_store + 2 && g_store[0].m_pItem == (CBaseObject*)a && g_store[1].m_pItem == (CBaseObject*)b;
    if (!ok) { printf("0074ac80_TEST FAIL\n"); return 1; }
    printf("0074ac80_TEST PASS\n"); return 0;
}
