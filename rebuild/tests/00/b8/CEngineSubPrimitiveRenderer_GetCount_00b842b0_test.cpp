#include <cstdio>
// Behaviour test for CEngineSubPrimitiveRenderer::GetCount @ 0x00B842B0: the element count of list[type].
class CEngineSubPrimitiveBase;
enum ESubPrimitiveType { ESUBPRIMITIVE_TYPE_FIRST = 0 };
template <class _Tp> class CArray {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement { public: unsigned long SortValue; CEngineSubPrimitiveBase* SubPrimitive; };
    class CSubPrimitiveList {
    public:
        CArray<CSortElement> Elements; long RenderPass; long PreRenderPass;
        bool BoundingVolumePass; bool RequiresStableSort; bool Sorted; bool Enable;
    };
    void* __vftable; unsigned char _pad_0x04[0x0C]; float MaxSortDistance; float SortDistanceMultiplier;
    CSubPrimitiveList SubPrimitiveLists[15];
    unsigned long GetCount(ESubPrimitiveType type) const { return SubPrimitiveLists[type].Elements.size(); }
};
int main()
{
    static CEngineSubPrimitiveRenderer r;
    static CEngineSubPrimitiveRenderer::CSortElement buf[10];
    for (int i = 0; i < 15; ++i) {
        r.SubPrimitiveLists[i].Elements._M_start = buf;
        r.SubPrimitiveLists[i].Elements._M_finish = buf + (i % 10);
        r.SubPrimitiveLists[i].Elements._M_end_of_storage = buf + 10;
    }
    for (int i = 0; i < 15; ++i)
        if (r.GetCount((ESubPrimitiveType)i) != (unsigned long)(i % 10)) { std::printf("00b842b0_TEST FAIL\n"); return 1; }
    std::printf("00b842b0_TEST PASS\n");
    return 0;
}