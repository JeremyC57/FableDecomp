// _STL::__uninitialized_copy(CGraphicInfoFrame*, CGraphicInfoFrame*, CGraphicInfoFrame*, const __false_type&) -- copy-construct [first, last) into raw storage at result and return the end
// Leaf: __uninitialized_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A00750, 74 bytes, __fastcall (first/last in ecx/edx), `ret 8` (result + tag).
// Callers: 0x00A00CD9, 0x00A00D41, 0x00A00FCD, 0x00A01021 (vector<CGraphicInfoFrame> copy/insert paths). The chunk label
//   "_Copy_impl<CGameMapCell*,CGameMapCell*>" is propagated and wrong.
// Shape: STLport 4.x `for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first); return __cur;`.
// Element: CGraphicInfoFrame (Ego_r.pdb, 0x10 bytes = retail; vtable 0x0129C878 from rtti-vtables.tsv): base CBaseClass
//   (ctor 0x0099A2D0 / dtor 0x0099A2E0), FrameNum ushort +0x04, SmallMipmapLevel schar +0x06, Data
//   CIVCountedPointer<CGraphicInfoFrame::CFrameData> +0x08 (intrusive RefCount at +4, virtual slot 1 frees), GraphicInfo +0x0C.
//   The implicit copy constructor stores only the derived vtable; the implicit assignment copies FrameNum, SmallMipmapLevel,
//   Data through CIVCountedPointer::operator= (self test, release + zero, assign, AddRef) and GraphicInfo.
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }

class CBaseClass {                                  // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                   // 0x0099A2D0
    ~CBaseClass();                                  // 0x0099A2E0
};

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

namespace _STL {
struct __false_type { __false_type() {} };
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
}

namespace _STL {
CGraphicInfoFrame* __fastcall __uninitialized_copy(CGraphicInfoFrame* __first, CGraphicInfoFrame* __last, CGraphicInfoFrame* __result,
                                                  const __false_type&)
{
    CGraphicInfoFrame* __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}