// std::__median(const CIntelligentPointer<CThing>& a, const CIntelligentPointer<CThing>& b, const CIntelligentPointer<CThing>& c, CDoNotUseBinaryPredicate comp) -- SGI median-of-three for the introsort pivot of a thing-pointer array under the coin-flip predicate: each comparison advances the world's random seed and answers "low bit clear", so the pick is random
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00805FE0, 220 bytes, `ret 8` (__fastcall: a in ecx, b in edx, c and the
// predicate's slot on the stack -- the SGI/STLport algorithm TUs are /Gr). Callee: GFROR13 @ 0x00497890 (landed,
// __fastcall). Global: the world @ 0x013B86A0, its random seed at +0x1613C (the landed 0x00807520 model).
// Shape: STLport `if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; } else if
// (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;` -- the predicate ignores both pointers:
// `seed = ROR13(seed * 0x24A1 + 0x24DF); if (seed & 1) return false; return true;` inlined at every call -- the
// if/return form is what gives retail's bare `test al,1` (`return (x & 1) == 0` / `!(x & 1)` / `?:` all compile to
// `not al ; test al,1`; the same form also matches the sibling __unguarded_linear_insert @ 0x00806910). VC7.1 hoists the
// next `seed * 0x24A1 + 0x24DF` store above each branch, since both arms call it again; the result is the address in eax.
// Label: catalogued as Dinkumware `_Med3<CIntelligentPointer<CThing>*, CDoNotUseBinaryPredicate>`; the body returns a
// reference and never swaps, i.e. SGI's `__median`.
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

class CWorld { public:
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
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
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

const ThingPointer& __fastcall __median(const ThingPointer& a, const ThingPointer& b, const ThingPointer& c, CDoNotUseBinaryPredicate comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}