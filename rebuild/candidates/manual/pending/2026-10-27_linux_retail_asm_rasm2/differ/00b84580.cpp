// CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags, CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain) -- queue a sub-primitive for sorted rendering: alpha sub-primitives go into list 1 keyed by the reversed sort distance and chain through LastAlpha, opaque ones into the layer's list (11/12 for the overlay flags) keyed by the scaled distance and chain through LastOpaque
// Leaf: ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@AAVCEngineSubPrimitiveChain@@@Z
// Retail-asm: 0x00B845A2 `fld dword ptr [esp + 0x20]` ; 0x00B845A6 `fistp dword ptr [esp + 0x28]` and 0x00B84644 `fld dword ptr [esp + 0x20]` ; 0x00B84648 `fistp dword ptr [esp + 0x28]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84580; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Both sort keys are rounded by the helper through the dead distance slot.
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
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
