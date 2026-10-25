// NScreenEffect::CScreenEffectPriorityStack::DeleteScreenFilterEntry(void* handle, float fade_time) -- when the handle is a live screen-filter key, mark it fading out, freeze its parameters at the blend of every entry up to it (in priority order), and restart its duration as a fade over fade_time seconds
// Grade: DIFFER(366v410) -- erase/fade/LerpTo loop/end caching match; left: the default-params zero stores (retail Brightness, Contrast, Alpha; ours Contrast, Brightness, Alpha) and the found-entry copy (retail memberwise mov pairs, 4 scalars then Tint through `add eax,20h`; ours `rep movsd`)
// Leaf: ?DeleteScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008818A0, 416 bytes incl. int3 padding, thiscall, `ret 8`.
// Identity: the catalog's "CEnvironmentSpellEffectInterface_UpdateRadialBlurParams" is descriptive. Ego_r/FableWin PDB
//   `?DeleteScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEXPAXM@Z` (Ego_r 0x008C2430, between
//   DeleteDisplacementEntry and InsertRadialBlurEntry; the landed siblings DeleteRadialBlurEntry 0x00881820 and
//   DeleteDisplacementEntry 0x00881860 sit right before this row). Single caller: the interpolator at 0x006B8400.
// Callees: set<CStackEntry*>::erase(const key&) 0x00884710 (count erased), CScreenFilterParams::LerpTo 0x00882FE0 (same
//   TU, declared only), __ftol2 0x00BFEA70. Global: g_framesPerSecond 0x01375550.
// Layout (PDB with retail STLport containers): ScreenFilterParamStack list<CScreenFilterEntry>[3] +0x94,
//   ScreenFilterKeys +0xA0; CStackEntry {vptr, curdurationcount +4, durationcount +8, fadingout +0xC};
//   CScreenFilterEntry param +0x10 (CScreenFilterParams, 0x20 bytes; PDB user ctor + SetToDefault), reset map +0x30.
// Shape: erase(key) through a CStackEntry* conversion temporary (a local slot: the handle stays live), fadingout set,
//   default parameters, then every entry of the three priority lists blended in with its clamped
//   curdurationcount / durationcount progress (1 when the duration is not positive) until the handle's own entry,
//   which takes the blend (`goto` past both loops); then curdurationcount = 0 and durationcount = fade_time * fps.
extern long g_framesPerSecond;                              // 0x01375550

class CRGBFloatColour {
public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};

class CEnvironmentSpellEffectInterface {
public:
    struct CScreenFilterParams {                            // PDB, 0x20 bytes
        float           Alpha;                              // +0x00
        float           Saturation;                         // +0x04
        float           Contrast;                           // +0x08
        float           Brightness;                         // +0x0C
        CRGBFloatColour Tint;                               // +0x10
        CScreenFilterParams();
        CScreenFilterParams LerpTo(const CScreenFilterParams& to, float t);    // 0x00882FE0
    };
};

inline CEnvironmentSpellEffectInterface::CScreenFilterParams::CScreenFilterParams()
    : Alpha(0.0f), Saturation(1.0f), Contrast(0.0f), Brightness(0.0f), Tint(1.0f, 1.0f, 1.0f, 1.0f)
{
}

namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node<_Tp>* _M_node;                               // the sentinel
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
} // namespace std

namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CStackEntry {
        virtual ~CStackEntry();
        long curdurationcount;                              // +0x04
        long durationcount;                                 // +0x08
        bool fadingout;                                     // +0x0C
    };
    struct CScreenFilterEntry : public CStackEntry {
        CEnvironmentSpellEffectInterface::CScreenFilterParams param;    // +0x10
        unsigned char reset[0x0C];                          // +0x30 map<CThing*, CThingGraphicByPassFlagState>
    };
    struct CKeySet {                                        // STLport set<CStackEntry*> (12 bytes)
        void*         _M_header;
        unsigned long _M_node_count;
        unsigned long _M_key_compare;
        unsigned long erase(CStackEntry* const& key);       // 0x00884710
    };

    virtual ~CScreenEffectPriorityStack();
    unsigned char                     _pad04[0x94 - 0x04];
    std::list<CScreenFilterEntry>     ScreenFilterParamStack[3];    // +0x94
    CKeySet                           ScreenFilterKeys;             // +0xA0
    CEnvironmentSpellEffectInterface::CScreenFilterParams CurrentScreenFilterParam;  // +0xAC

    void DeleteScreenFilterEntry(void* handle, float fade_time);
};
}

void NScreenEffect::CScreenEffectPriorityStack::DeleteScreenFilterEntry(void* handle, float fade_time)
{
    if (ScreenFilterKeys.erase(static_cast<CStackEntry*>(handle)) > 0) {
        CScreenFilterEntry* entry = static_cast<CScreenFilterEntry*>(handle);
        entry->fadingout = true;
        CEnvironmentSpellEffectInterface::CScreenFilterParams params;
        for (int i = 0; i < 3; ++i) {
            std::list<CScreenFilterEntry>::iterator end = ScreenFilterParamStack[i].end();
            for (std::list<CScreenFilterEntry>::iterator it = ScreenFilterParamStack[i].begin(); it != end; ++it) {
                CScreenFilterEntry& current = *it;
                float t = 1.0f;
                if (current.durationcount > 0) {
                    t = (float)current.curdurationcount / current.durationcount;
                    if (t < 0.0f)
                        t = 0.0f;
                    else if (t > 1.0f)
                        t = 1.0f;
                }
                params = params.LerpTo(current.param, t);
                if (&current == entry) {
                    entry->param = params;
                    goto done;
                }
            }
        }
done:
        entry->curdurationcount = 0;
        entry->durationcount = (long)(fade_time * g_framesPerSecond);
    }
}
