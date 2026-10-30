// Grade: IMPROVED(349v340) retry1 -- retail_asm helper + binding spelling of _M_insert_overflow; residue: escaped-temp reload in push_back
// CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags, CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain) -- queue a sub-primitive for sorted rendering: alpha sub-primitives go into list 1 keyed by the reversed sort distance and chain through LastAlpha, opaque ones into the layer's list (11/12 for the overlay flags) keyed by the scaled distance and chain through LastOpaque
// Retail-asm: 0x00B845A2 `fld dword ptr [esp+20h]` + 0x00B845A6 `fistp dword ptr [esp+28h]` (and 0x00B84648) -- the engine inline-assembly float-to-long helper; VC7.1 C++ (long)x calls __ftol2.
// Leaf: ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@AAVCEngineSubPrimitiveChain@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B84580, length 352 bytes (body 0x154), `ret 18h`.
// Callees: the sort-list vector's _M_insert_overflow @ 0x00B85EC0.
// Layout: engine/CEngineSubPrimitiveRenderer.h (MaxSortDistance +0x10, SortDistanceMultiplier +0x14) with 0x18-byte retail
//   CSubPrimitiveList records from +0x18 (the sort vector first); CEngineSubPrimitiveBase (PDB): RenderFlags +4, Alpha +8,
//   NextSubPrimitive +0x10, Manager +0x14; CEngineSubPrimitiveChain (PDB): LastOpaque +0, LastAlpha +4.
// Note: retail converts the sort key with a bare `fld ; fistp` pair through a frame float (the engine's inline-asm
//   FloatToLong rounding helper, asm_bake family); the cast here goes through __ftol2.
struct CEngineSubPrimitiveBase;
class CEnginePrimitiveManager;
struct CEngineSubPrimitiveBase {
    void*                    _vptr;
    unsigned long            RenderFlags;           // +0x04
    unsigned char            Alpha;                 // +0x08
    void*                    Parent;                // +0x0c
    CEngineSubPrimitiveBase* NextSubPrimitive;      // +0x10
    CEnginePrimitiveManager* Manager;               // +0x14
};
struct CEngineSubPrimitiveChain {
    CEngineSubPrimitiveBase* LastOpaque;
    CEngineSubPrimitiveBase* LastAlpha;
};
namespace std {
struct __false_type { __false_type() {} };
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {      // STLport 4.6 vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
protected:
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int n, bool atEnd);   // 0x00B85EC0
};
}
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {
    public:
        long                     Key;
        CEngineSubPrimitiveBase* SubPrimitive;
    };
    struct CSortList {                                      // 24 bytes in retail
        std::vector<CSortElement> Elements;
        unsigned char _pad[0x0c];
        void push_back(const CSortElement& x) { Elements.push_back(x); }
    };
    void*     _vptr;
    unsigned char _pad04[0x0c];
    float     MaxSortDistance;                      // +0x10
    float     SortDistanceMultiplier;               // +0x14
    CSortList SubPrimitiveLists[15];                // +0x18
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags,
                         CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain);
};
// the engine's inline-assembly float-to-long helper: expanded in place, reads the homed value, writes a frame word
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static inline long ClampKey(long key) { return key < 0 ? 0 : (key > 0x3fffffff ? 0x3fffffff : key); }
void CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha,
                                                  unsigned long flags, CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain)
{
    if (flags & 1) {
        distance = (MaxSortDistance - distance) * SortDistanceMultiplier;
        long key = ClampKey(FloatToLong(distance));
        if (chain.LastAlpha) {
            chain.LastAlpha->NextSubPrimitive = subPrimitive;
            chain.LastAlpha = subPrimitive;
        }
        else {
            CSortElement element = { key, subPrimitive };
            SubPrimitiveLists[1].push_back(element);
            chain.LastAlpha = subPrimitive;
        }
    }
    else {
        long list = 0;
        if (flags & 0x80)
            list = 11;
        if (flags & 0x100)
            list = 12;
        distance = distance * SortDistanceMultiplier;
        long key = ClampKey(FloatToLong(distance));
        if (chain.LastOpaque)
            chain.LastOpaque->NextSubPrimitive = subPrimitive;
        else {
            CSortElement element = { key, subPrimitive };
            SubPrimitiveLists[list + 1].push_back(element);
        }
        chain.LastOpaque = subPrimitive;
    }
    subPrimitive->RenderFlags = flags;
    subPrimitive->Alpha = alpha;
    subPrimitive->Manager = manager;
}
