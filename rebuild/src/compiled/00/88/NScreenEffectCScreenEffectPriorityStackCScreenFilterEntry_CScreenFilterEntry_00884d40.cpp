// NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::CScreenFilterEntry(const CScreenFilterEntry&) -- the implicit copy constructor of a screen-filter stack entry
// Leaf: ??0CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAE@ABU012@@Z
// Retail: Fable.exe @ 0x00884D40, 72 bytes, thiscall `ret 4`, this in eax.
// Identity (proven): the body stores vtable 0x01277FF0 = NScreenEffect::CScreenEffectPriorityStack::CStackEntry (RTTI), copies
// the base's three members, then stores 0x01278060 = CScreenFilterEntry (RTTI), block-copies 32 bytes and calls the map copy
// constructor 0x00884A40 on +0x30 -- exactly Ego_r.pdb's CStackEntry { curdurationcount +4, durationcount +8, fadingout +0xC }
// and CScreenFilterEntry { CScreenFilterParams param +0x10 (32 bytes), map<CThing*, CThingGraphicByPassFlagState> reset +0x30 }.
// FableWin.pdb has ??0CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAE@ABU012@@Z; its callers 0x00884D95 /
// 0x00884DB9 / 0x00884DE9 / 0x00884E2C build list<CScreenFilterEntry> nodes (malloc 0x44 = 8 + 0x3C).
// Label correction: catalog "_gapscan::sub_00884d40".
// Callees: 0x00884A40 std::map<CThing*, NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::CThingGraphicByPassFlagState>
// ::map(const map&) (landed).
// Layout: as above (no STL member before +0x30, so the PDB offsets are retail's); CScreenFilterParams = Alpha, Saturation,
// Contrast, Brightness, CRGBFloatColour Tint (Ego_r.pdb), trivially copyable (`rep movsd`, 8 dwords).
// Shape: compiler-generated copy constructors: base vptr, base members, derived vptr, members in declaration order.
class CThing;

class CRGBFloatColour {
public:
    float R, G, B, A;
};

class CScreenFilterParams {
public:
    float Alpha;                                            // +0x00
    float Saturation;                                       // +0x04
    float Contrast;                                         // +0x08
    float Brightness;                                       // +0x0C
    CRGBFloatColour Tint;                                   // +0x10
};

namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    map(const map<_Key, _Tp, _Compare, _Alloc>& __x);       // out of line (0x00884A40)
    unsigned char _M_t[0xC];                                // STLport _Rb_tree: header, node count, compare
};
}

namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CStackEntry {
        CStackEntry(long duration);
        virtual ~CStackEntry();
        long curdurationcount;                              // +0x04
        long durationcount;                                 // +0x08
        bool fadingout;                                     // +0x0C
    };
    struct CScreenFilterEntry : public CStackEntry {
        class CThingGraphicByPassFlagState;
        CScreenFilterEntry(long duration);
        virtual ~CScreenFilterEntry();
        CScreenFilterParams param;                          // +0x10
        std::map<CThing*, CThingGraphicByPassFlagState> reset;   // +0x30
    };
};

class CScreenEffectPriorityStack::CScreenFilterEntry::CThingGraphicByPassFlagState {
public:
    unsigned char Data[9];                                  // CIntelligentPointer<CThing> thing, bool bypassReset (Ego_r.pdb)
};

CScreenEffectPriorityStack::CScreenFilterEntry* CopyScreenFilterEntry_Emit(void* p, const CScreenEffectPriorityStack::CScreenFilterEntry& x);
}

// The implicit copy constructor is emitted out of line by one use (as retail's list-node constructors use it).
inline void* operator new(unsigned int, void* p) { return p; }
NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry*
NScreenEffect::CopyScreenFilterEntry_Emit(void* p, const CScreenEffectPriorityStack::CScreenFilterEntry& x)
{
    return new (p) CScreenEffectPriorityStack::CScreenFilterEntry(x);
}