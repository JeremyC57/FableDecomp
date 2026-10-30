// std::fill(CGraphicInfoFrame*, CGraphicInfoFrame*, const CGraphicInfoFrame&) -- assign val to every element of [first, last)
// Leaf: fill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A00A90, 91 bytes, __fastcall (first/last in ecx/edx), `ret 4` (val).
// Caller: 0x00A0103A (vector<CGraphicInfoFrame>::_M_fill_insert). The chunk label
//   "_Copy_backward<CDefPointer<CThingBaseDef const>*,...>" is propagated and wrong.
// Shape: STLport 4.x `for (; __first != __last; ++__first) *__first = __val;` through the implicit CGraphicInfoFrame::operator=.
// Element: CGraphicInfoFrame (Ego_r.pdb, 0x10 bytes = retail; vtable 0x0129C878 from rtti-vtables.tsv): base CBaseClass
//   (ctor 0x0099A2D0 / dtor 0x0099A2E0), FrameNum ushort +0x04, SmallMipmapLevel schar +0x06, Data
//   CIVCountedPointer<CGraphicInfoFrame::CFrameData> +0x08 (intrusive RefCount at +4, virtual slot 1 frees), GraphicInfo +0x0C.
//   The implicit copy constructor stores only the derived vtable; the implicit assignment copies FrameNum, SmallMipmapLevel,
//   Data through CIVCountedPointer::operator= (self test, release + zero, assign, AddRef) and GraphicInfo.
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

template <class T>
class CIVCountedPointer {                           // intrusive: the object's RefCount at +4, virtual slot 1 frees it
public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
};

class CGraphicInfo;

class CGraphicInfoFrame : public CBaseClass {       // vtable 0x0129C878, 0x10 bytes
public:
    class CFrameData {
    public:
        virtual void v000();
        virtual void DeleteThis();                  // slot 1 (+0x04)
        long RefCount;                              // +0x04
        void AddRef() { ++RefCount; }
        void Release()
        {
            if (--RefCount == 0)
                DeleteThis();
        }
    };
    unsigned short FrameNum;                        // +0x04
    signed char SmallMipmapLevel;                   // +0x06
    CIVCountedPointer<CFrameData> Data;             // +0x08
    CGraphicInfo* GraphicInfo;                      // +0x0C
    virtual void v001();
};

namespace std {
struct __false_type { __false_type() {} };
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
}

namespace std {
void __fastcall fill(CGraphicInfoFrame* __first, CGraphicInfoFrame* __last, const CGraphicInfoFrame& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}