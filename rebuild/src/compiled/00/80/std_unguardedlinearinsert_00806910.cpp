// std::__unguarded_linear_insert(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>, CDoNotUseBinaryPredicate) -- insertion-sort inner loop: shift intelligent thing pointers right while the coin-flip predicate says the value goes first, then drop the value in
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806910, 113 bytes (128 with padding), `ret 0xc`.
// Label correction: the catalog "_Push_heap<CIntelligentPointer<CThing>*,int,...,CDoNotUseBinaryPredicate>"
// names the template arguments only; the body is SGI/STLport `__unguarded_linear_insert(last, val, comp)`,
// called by the landed `__unguarded_insertion_sort_aux` @ 0x00807320 (which already names it so).
// Callees: 0x00497890 GFROR13 (landed, __fastcall), GetPItem 0x00A01B50, SetPItem 0x00A01B90,
// ~CBaseIntelligentPointer 0x00A01C10 (model of the landed sibling _Adjust_heap 0x00807520; the
// same TU's _Push_heap is 0x00806990).  Global: the world @ 0x013B86A0, random seed at +0x1613c.
// Link-time __fastcall (/Gr algorithm TU): last in ecx (edx unused), the 8-byte pointer by value
// and the 4-byte predicate on the stack (8 + 4 = 0xc); the callee destroys its own by-value
// pointer at the end (`lea ecx,[esp+10h] ; call 0x00A01C10`).
// CDoNotUseBinaryPredicate (anonymous namespace) ignores both things: it advances the world seed
// (`seed = ROR13(seed * 0x24a1 + 0x24df)`) and answers "low bit clear" (`test al,1 ; jne`).
// The predicate is the `if (Random() & 1) return false; return true;` form of the sibling 0x00806990.
// Element moves are CIntelligentPointer::operator= = `SetPItem(other.GetPItem())`.
// Shape: SGI/STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; }`
// -- the pointer pair lives in ebx/edi (`lea edi,[ebx-8]`), one move per iteration.
class CThing;
class CBaseObject;

long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CRandomSeed {
    unsigned long Seed;
    unsigned long Random()
    {
        Seed = Seed * 0x24a1 + 0x24df;
        Seed = GFROR13(Seed);
        return Seed;
    }
};

struct CWorld {
    unsigned char _pad[0x1613c];
    CRandomSeed RandomSeed;                     // +0x1613c
};

extern CWorld* g_pWorld;                        // 0x013B86A0

struct PointerLink { CBaseObject* item; int refs; };
class CBaseIntelligentPointer {
public:
    PointerLink* link;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* p) { CBaseIntelligentPointer::SetPItem((CBaseObject*)p); }
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); return *this; }
};
typedef CIntelligentPointer<CThing> ThingPointer;

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const
    {
        if (g_pWorld->RandomSeed.Random() & 1)
            return false;
        return true;
    }
};
}

namespace std {
void __fastcall __unguarded_linear_insert(ThingPointer* last, ThingPointer value, CDoNotUseBinaryPredicate comp)
{
    ThingPointer* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
}