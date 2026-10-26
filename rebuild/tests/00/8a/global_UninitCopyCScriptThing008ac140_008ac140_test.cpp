// Behaviour test for 008ac140: placement-copy 12-byte CScriptThing handles (vptr set by the implicit copy ctor, the counted implementation pointer add-ref'd)
#include <cstdio>
#include <new>

class CCPPointerInfo { public:
    unsigned int RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
class CScriptThing {
public:
    CScriptThing() { }
    virtual ~CScriptThing() { }
    virtual int Kind() const { return 0x5C; }
    CCountedPointer<CScriptThing> PImp;
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

CScriptThing* UninitCopy(const CScriptThing* first, const CScriptThing* last, CScriptThing* result, const FalseType&)
{
    CScriptThing* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    CCPPointerInfo info = { 3, 0, 0 };
    CScriptThing imp;
    CScriptThing src[3];
    src[0].PImp.Data = &imp; src[0].PImp.Info = &info;
    src[2].PImp.Data = &imp; src[2].PImp.Info = &info;
    void* raw = ::operator new(3 * sizeof(CScriptThing));
    CScriptThing* dst = static_cast<CScriptThing*>(raw);
    CScriptThing* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && info.RefCount == 5;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Kind() == 0x5C && dst[i].PImp.Data == src[i].PImp.Data && dst[i].PImp.Info == src[i].PImp.Info;
    for (int i = 0; i < 3; ++i)
        dst[i].~CScriptThing();
    ::operator delete(raw);
    if (!ok) { std::printf("008ac140_TEST FAIL\n"); return 1; }
    std::printf("008ac140_TEST PASS\n");
    return 0;
}