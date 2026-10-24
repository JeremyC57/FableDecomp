// _global::_Push_heap(CIntelligentPointer<CThing>*, long, long, CIntelligentPointer<CThing>, CDoNotUseBinaryPredicate) -- heap sift-up of intelligent thing pointers under the coin-flip predicate (random shuffle helper)
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806990, 147 bytes, `ret 0x10`.
// Callees: 0x00497890 GFROR13 (landed, __fastcall), GetPItem 0x00A01B50, SetPItem 0x00A01B90,
// ~CBaseIntelligentPointer 0x00A01C10 (model of the landed sibling _Adjust_heap 0x00807520, which
// calls this body).  Global: the world @ 0x013B86A0, random seed at +0x1613c.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 8-byte pointer
// by value and the 4-byte predicate on the stack (4 + 8 + 4 = 0x10); the callee destroys its own
// by-value pointer at the end (`lea ecx,[esp+14h] ; call 0x00A01C10`).
// CDoNotUseBinaryPredicate (anonymous namespace) ignores both things: it advances the world seed
// (`seed = ROR13(seed * 0x24a1 + 0x24df)`) and answers "low bit clear" (`test al,1 ; jne`).
// The predicate is written `if (Random() & 1) return false; return true;`: `(Random() & 1) == 0`
// materialises the bool (`not al ; test al,1 ; je`) and the sibling's const-ref coin spills it to
// a frame dword (`push ecx`, `and eax,1 ; mov [esp+10h],eax`) -- both graded DIFFER here.
// Element moves are CIntelligentPointer::operator= = `SetPItem(other.GetPItem())`.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(...))`).
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

void __fastcall _Push_heap(ThingPointer* first, long hole, long top, ThingPointer value, CDoNotUseBinaryPredicate comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}