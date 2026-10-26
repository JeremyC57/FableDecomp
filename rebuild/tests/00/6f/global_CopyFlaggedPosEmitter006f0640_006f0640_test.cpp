// Behaviour test for 0x006F0640 Copy_FlaggedPosEmitter_006f0640: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CBaseObject;

struct CBaseIntelligentPointer {             // 8 bytes: vtable + item
    virtual void v00();
    void* PData;                             // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }                          // test-only: arrays
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct SFlaggedPos {                         // 13 bytes, byte-packed in retail (the vector sits at +1)
    bool      Flag;                          // +0x00
    C3DVector Pos;                           // +0x01
    SFlaggedPos& operator=(const SFlaggedPos& other)
    {
        Flag = other.Flag;
        Pos = other.Pos;
        return *this;
    }
};

class CThing;

struct SFlaggedPosEmitter {                  // 0x1C bytes
    SFlaggedPos                 Where;       // +0x00
    long                        Value;       // +0x10
    CIntelligentPointer<CThing> PEmitter;    // +0x14
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SFlaggedPosEmitter* __fastcall Copy_FlaggedPosEmitter_006f0640(SFlaggedPosEmitter* first, SFlaggedPosEmitter* last, SFlaggedPosEmitter* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

void CBaseIntelligentPointer::v00() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = item; }

typedef SFlaggedPosEmitter T;
#define KEY(E) (E).Value
#define INIT(E) (E).Where.Flag = true; (E).PEmitter.PData = &(E)
#define CALL(f, l, r) Copy_FlaggedPosEmitter_006f0640(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[4];
    T b[4];
    for (i = 0; i < 4; ++i) { INIT(a[i]); INIT(b[i]); KEY(a[i]) = i + 1; KEY(b[i]) = 0; }
    T* r = CALL(a, a + 3, b);
    CHECK(r == b + 3);
    CHECK(KEY(b[0]) == 1 && KEY(b[1]) == 2 && KEY(b[2]) == 3 && KEY(b[3]) == 0);
    r = CALL(a, a, b + 1);
    CHECK(r == b + 1);
    CHECK(b[2].Where.Flag && b[2].PEmitter.PData == &a[2]);
    if (g_fail) { std::printf("006f0640_TEST FAIL\n"); return 1; }
    std::printf("006f0640_TEST PASS\n");
    return 0;
}