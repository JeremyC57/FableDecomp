// CEnginePrimitiveManager2DParticleGroup::GetSupportedTypes(CRetailVector_dword& types) -- types.push_back(0x20) with the retail push_back inlined (InsertGrow @ 0x00440990 when full)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE1A80, length 62 bytes.
// Callees: the reallocating insert @ 0x00440990. Shape: the same body as the landed PushBackConstant family (@ 0x00BBAC20 ...): the value is a named local (it must be
// addressable for the insert's reference argument), stored in place when there is room (`mov [eax],20h ; add [ecx+4],4`), else handed to the insert with the empty
// tag temporary homed in the dead argument slot; `this` is unused (`ret 4`).
#include <new>

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_dword {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

class CEnginePrimitiveManager2DParticleGroup { public:
    void GetSupportedTypes(CRetailVector_dword& types);
};

void CEnginePrimitiveManager2DParticleGroup::GetSupportedTypes(CRetailVector_dword& types)
{
    long value = 0x20;
    types.push_back(value);
}