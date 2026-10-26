// _global::Copy_GenerationPos_00722c90(CTCSmokeGenerator::CGenerationPos*, CTCSmokeGenerator::CGenerationPos*, CTCSmokeGenerator::CGenerationPos*, const random_access_iterator_tag&, int*) -- SGI __copy over the smoke generator's emitter positions
// Leaf: Copy_GenerationPos_00722c90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722C90, length 96 bytes.
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the advanced result is returned.
// Element (Ego_r.pdb): CTCSmokeGenerator::CGenerationPos, 0x14 bytes = C3DVector Pos +0, CIntelligentPointer<CThing>
// PEmitter +0xC. The implicit operator= copies Pos through pointer registers (`mov eax,esi ; mov ecx,edi`: a C3DVector
// with a user copy constructor and no operator=, GOTCHAS wave 25 chunk 185), then PEmitter = other.PEmitter as
// SetPItem(other.GetPItem()) (CBaseIntelligentPointer @ 0x00A01B50 / 0x00A01B90, modelled as the landed 0x0080AA60).
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