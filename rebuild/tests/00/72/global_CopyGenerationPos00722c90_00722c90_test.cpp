// Behaviour test for 0x00722C90 Copy_GenerationPos_00722c90: a copy of the source model with working stand-ins for the out-of-line callees.
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

class CThing;

struct CGenerationPos {                      // CTCSmokeGenerator::CGenerationPos, 0x14 bytes
    C3DVector                   Pos;         // +0x00
    CIntelligentPointer<CThing> PEmitter;    // +0x0C
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CGenerationPos* __fastcall Copy_GenerationPos_00722c90(CGenerationPos* first, CGenerationPos* last, CGenerationPos* result, const random_access_iterator_tag& tag, int* distance)
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

typedef CGenerationPos T;
#define KEY(E) (E).Pos.X
#define INIT(E) (E).PEmitter.PData = &(E); (E).Pos.Z = 3.0f
#define CALL(f, l, r) Copy_GenerationPos_00722c90(f, l, r, tag, 0)

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
    CHECK(b[2].PEmitter.PData == &a[2] && b[2].Pos.Z == 3.0f);
    if (g_fail) { std::printf("00722c90_TEST FAIL\n"); return 1; }
    std::printf("00722c90_TEST PASS\n");
    return 0;
}