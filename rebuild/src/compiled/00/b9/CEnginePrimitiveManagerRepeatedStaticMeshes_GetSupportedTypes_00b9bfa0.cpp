// CEnginePrimitiveManagerRepeatedStaticMeshes::GetSupportedTypes(CRetailVector_dword& types) -- types.push_back(0x2a) with the retail push_back inlined (InsertGrow @ 0x00440990 when full)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9BFA0, length 62 bytes.
// Callees: the reallocating insert @ 0x00440990. Shape: the landed PushBackConstant family: a named local value (addressable for the insert's reference argument),
// stored in place when there is room, else handed to the insert with the empty tag temporary homed in the dead argument slot; `this` is unused (`ret 4`).
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

class CEnginePrimitiveManagerRepeatedStaticMeshes { public:
    void GetSupportedTypes(CRetailVector_dword& types);
};

void CEnginePrimitiveManagerRepeatedStaticMeshes::GetSupportedTypes(CRetailVector_dword& types)
{
    long value = 0x2a;
    types.push_back(value);
}