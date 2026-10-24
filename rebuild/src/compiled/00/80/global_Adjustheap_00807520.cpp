// _global::_Adjust_heap(CIntelligentPointer<CThing>*, long, long, CIntelligentPointer<CThing>, CDoNotUseBinaryPredicate) -- heap sift-down of intelligent thing pointers under a coin-flip predicate (random shuffle helper), then _Push_heap @ 0x00806990
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807520, 217 bytes.
// Callees: 0x00497890 GFROR13 (landed, __fastcall), CBaseIntelligentPointer ctor 0x00A01B10,
// GetPItem 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10 (the model of the landed sibling
// 0x00926850), 0x00806990 _Push_heap (same instantiation).  Global: the world @ 0x013B86A0, whose
// random generator seed sits at +0x1613c (same hop as the landed 0x0077C490 / 0x008FCFD0).
// Link-time __fastcall: first in ecx, hole in edx, then bottom, the 8-byte pointer by value (built
// in the _Push_heap slot by the copy constructor: base ctor, vtable store, SetPItem(GetPItem())),
// and the 4-byte predicate; the callee destroys its own by-value pointer (`call 0x00A01C10`).
// CDoNotUseBinaryPredicate (anonymous namespace) ignores both things: it advances the world seed
// (`seed = ROR13(seed * 0x24a1 + 0x24df)`) and answers "low bit clear" -- a random ordering.
// The coin is a temporary bound to a const reference: that is what parks `seed & 1` in the second
// frame dword (`and eax,1 ; mov [esp+0x14],eax`) and makes the frame `sub esp,8`; a plain
// `(Random() & 1) == 0` keeps it in eax and the frame shrinks to `push ecx`.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
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
        const unsigned long& bit = g_pWorld->RandomSeed.Random() & 1;
        return bit == 0;
    }
};
}

void __fastcall _Push_heap(ThingPointer* first, long hole, long top, ThingPointer value, CDoNotUseBinaryPredicate pred);   // 0x00806990

void __fastcall _Adjust_heap(ThingPointer* first, long hole, long bottom, ThingPointer value, CDoNotUseBinaryPredicate pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}