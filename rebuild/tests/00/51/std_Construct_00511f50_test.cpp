// behaviour test for 00511f50: placement-copy-construct a CIntelligentPointer<T> into raw storage; the
// copy must point at the same thing (registered through SetPItem) and leave the source untouched.
#include <cstdio>
#include <new>

class CThing { public: long m_id; };
class CBaseObject;

static long g_setCalls = 0;

struct CBaseIntelligentPointer {
    virtual long Kind() const { return 1; }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; ++g_setCalls; }
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual long Kind() const { return 2; }
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};

typedef CIntelligentPointer<const CThing> CThingPointer;

static void Cons_val(CThingPointer* ptr, const CThingPointer& value)
{
    new ((void*)ptr) CThingPointer(value);
}

int main()
{
    CThing thing;
    CThingPointer source;
    void* raw = ::operator new(sizeof(CThingPointer));
    CThingPointer* dest = (CThingPointer*)raw;
    int ok = 1;
    thing.m_id = 42;
    source.SetPItem((CBaseObject*)&thing);
    g_setCalls = 0;
    Cons_val(dest, source);
    if (dest->GetPItem() != &thing || dest->GetPItem()->m_id != 42)
        ok = 0;
    if (dest->Kind() != 2 || g_setCalls != 1)
        ok = 0;
    if (source.GetPItem() != &thing)
        ok = 0;
    dest->~CThingPointer();
    ::operator delete(raw);
    if (!ok) {
        std::printf("00511f50_TEST FAIL\n");
        return 1;
    }
    std::printf("00511f50_TEST PASS\n");
    return 0;
}