// Grade: DIFFER(332v340) -- lane gap: asm_bake family: retail converts both sort keys with a bare fld/fistp pair through a frame float (inline-asm FloatToLong helper); compiled C++ goes through __ftol2
// CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags, CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain) -- queue a sub-primitive for sorted rendering: alpha sub-primitives go into list 1 keyed by the reversed sort distance and chain through LastAlpha, opaque ones into the layer's list (11/12 for the overlay flags) keyed by the scaled distance and chain through LastOpaque
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
struct CSortElement { long Key; CEngineSubPrimitiveBase* SubPrimitive; };
struct __true_type { __true_type() {} };
struct CSortList {
    CSortElement* _M_start;
    CSortElement* _M_finish;
    CSortElement* _M_end_of_storage;
    unsigned char _pad[0x0c];
    void _M_insert_overflow(CSortElement* pos, const CSortElement& x, const __true_type&, unsigned int n, bool atEnd);   // 0x00B85EC0
    void push_back(const CSortElement& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
class CEngineSubPrimitiveRenderer {
public:
    void*     _vptr;
    unsigned char _pad04[0x0c];
    float     MaxSortDistance;                      // +0x10
    float     SortDistanceMultiplier;               // +0x14
    CSortList SubPrimitiveLists[15];                // +0x18
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags,
                         CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain);
};
static inline long ClampKey(long key) { return key < 0 ? 0 : (key > 0x3fffffff ? 0x3fffffff : key); }
void CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha,
                                                  unsigned long flags, CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain)
{
    if (flags & 1) {
        distance = (MaxSortDistance - distance) * SortDistanceMultiplier;
        long key = ClampKey((long)distance);
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
        long key = ClampKey((long)distance);
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
