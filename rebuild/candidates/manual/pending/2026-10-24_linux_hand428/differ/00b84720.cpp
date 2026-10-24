// CEngineSubPrimitiveRenderer::AddSubPrimitive(ESubPrimitiveType type, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager) -- queue a sub-primitive in its type's sort list: integer sort key from the clamped scaled distance, push_back of the (key, sub-primitive) sort element, and the manager recorded on the sub-primitive
// Grade: DIFFER(142v142) -- retail_asm candidate: retail converts the scaled distance with a bare `fistp dword [esp]` after storing it into the `distance` slot and reloading it (`fstp [esp+14h]; fld [esp+14h]; fistp [esp]`), the signature of an inline-asm float-to-long helper; VC7.1 C++ emits `call __ftol` for the cast. 1 grade.
// Leaf: ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B84720, 142 bytes. __thiscall, `ret 10h`.
// Identity: FableWin.pdb `?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@PAVCEngineSubPrimitiveBase@@MPAVCEnginePrimitiveManager@@@Z`;
//   fields from Ego_r.pdb: MaxSortDistance +0x10, SortDistanceMultiplier +0x14, SubPrimitiveLists[15] +0x18 (retail
//   24-byte lists, the GetCount 0x00B842B0 model), CEngineSubPrimitiveBase::Manager +0x14. The catalog label
//   "Engine_AddClockVertex" is descriptive.
// Callees: vector<CSortElement>::_M_insert_overflow(pos, x, const __false_type&, 1, true) 0x00B85EC0 (push_back's grow).
// Shape: `distance = (MaxSortDistance - distance) * SortDistanceMultiplier`, the key clamped to [0, 0x3FFFFFFF], STLport
//   push_back (placement new with the null test, `+8` on the finish), then `subPrimitive->Manager = manager`.
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

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {
    public:
        unsigned long            SortValue;             // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* subPrimitive)
            : SortValue(sortValue), SubPrimitive(subPrimitive) { }
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
    long key = (long)distance;                          // retail: inline-asm fistp
    if (key < 0)
        key = 0;
    else if (key > 0x3FFFFFFF)
        key = 0x3FFFFFFF;
    SubPrimitiveLists[type].Elements.push_back(CSortElement(key, subPrimitive));
    subPrimitive->Manager = manager;
}
