// CEngineSubPrimitiveRenderer::AddSubPrimitive(ESubPrimitiveType type, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager) -- queue a sub-primitive in the list of its type with a sort key from its distance (scaled, clamped to [0, 0x3FFFFFFF]) and record its manager
// Leaf: ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@
// Retail-asm: 0x00B84731 `fld dword ptr [esp+14h]` ; 0x00B84735 `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the scaled distance is stored back to its own argument slot (the helper's by-value parameter, aliased to the dead `distance`) and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). The clamp, the push_back and the manager store are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B84720, 142 bytes (+5 int3), __thiscall, `ret 10h`. Callee: the STLport
//   vector<CSortElement>::_M_insert_overflow 0x00B85EC0 on a full list. Layout: MaxSortDistance +0x10, SortDistanceMultiplier
//   +0x14, SubPrimitiveLists[15] of 24-byte CSubPrimitiveList (vector + passes + flags) from +0x18. The manifest label
//   "Engine_AddClockVertex" is a propagated guess; the Ego_r.pdb name is ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@.
inline void* operator new(unsigned int, void* p) throw() { return p; }
class CEnginePrimitiveManager;
class CEngineInternalPrimitiveBase;
class CEngineSubPrimitiveBase {
public:
    virtual void v00();
    unsigned long                 RenderFlags;          // +0x04
    unsigned char                 Alpha;                // +0x08
    CEngineInternalPrimitiveBase* Parent;               // +0x0C
    CEngineSubPrimitiveBase*      NextSubPrimitive;     // +0x10
    CEnginePrimitiveManager*      Manager;              // +0x14
};
enum ESubPrimitiveType { ESUBPRIMITIVE_TYPE_FIRST = 0 };
struct __false_type { __false_type() { } };
template <class _Tp> class CArray {                     // retail: an STLport vector<_Tp> (12 bytes)
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);              // 0x00B85EC0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {
    public:
        unsigned long            SortValue;             // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* subPrimitive)
            : SortValue(sortValue), SubPrimitive(subPrimitive) { }
        CSortElement(const CSortElement& other) : SortValue(other.SortValue), SubPrimitive(other.SubPrimitive) { }
    };
    class CSubPrimitiveList {                           // 24 bytes in retail
    public:
        CArray<CSortElement> Elements;                  // +0x00
        long                 RenderPass;                // +0x0C
        long                 PreRenderPass;             // +0x10
        bool                 BoundingVolumePass;        // +0x14
        bool                 RequiresStableSort;        // +0x15
        bool                 Sorted;                    // +0x16
        bool                 Enable;                    // +0x17
    };
    void*             __vftable_slot0;                  // +0x00 (bases: 16 bytes)
    unsigned char     _pad04[0x0C];                     // +0x04
    float             MaxSortDistance;                  // +0x10
    float             SortDistanceMultiplier;           // +0x14
    CSubPrimitiveList SubPrimitiveLists[15];            // +0x18
    void AddSubPrimitive(ESubPrimitiveType type, CEngineSubPrimitiveBase* subPrimitive, float distance,
                         CEnginePrimitiveManager* manager);
};
void CEngineSubPrimitiveRenderer::AddSubPrimitive(ESubPrimitiveType type, CEngineSubPrimitiveBase* subPrimitive,
                                                  float distance, CEnginePrimitiveManager* manager)
{
    distance = (MaxSortDistance - distance) * SortDistanceMultiplier;
    long key = FloatToLong(distance);
    if (key < 0)
        key = 0;
    else if (key > 0x3FFFFFFF)
        key = 0x3FFFFFFF;
    SubPrimitiveLists[type].Elements.push_back(CSortElement(key, subPrimitive));
    subPrimitive->Manager = manager;
}
