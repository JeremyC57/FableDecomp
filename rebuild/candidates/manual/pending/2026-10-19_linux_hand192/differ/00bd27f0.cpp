// Grade: DIFFER(176v176) -- frame-slot assignment only: retail keeps the first type value in the `push ecx` word but homes the second and third values in the dead `types` argument slot, the same slot as every __true_type tag (the tag-over-live-slot lane gap); ours keeps all three values in the `push ecx` word (tried three named locals, three block-scoped locals)
// CEnginePrimitiveManagerAnimatedMeshesBase::GetSupportedTypes(CArray<EEnginePrimitiveType>&) -- report the three primitive types this manager handles (9, 0xb, 0xd) by appending them to the caller's type list
// Leaf: GetSupportedTypes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD27F0, 176 bytes (+ int3 padding), `ret 4` (`this` unused).
// Callees: the retail vector<long>::_M_insert_overflow / InsertGrow 0x00440990 (as the landed single-type siblings 0x00B9BFA0
//   and 0x00BE1A80).
// Shape: three inline push_backs of named constant locals (the first homed in the `push ecx` word, the later two in the dead
//   `types` argument slot); the empty __true_type tag passed by const reference to each overflow call.
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

struct CEnginePrimitiveManagerAnimatedMeshesBase {
    void GetSupportedTypes(CRetailVector_dword& types);
};

void CEnginePrimitiveManagerAnimatedMeshesBase::GetSupportedTypes(CRetailVector_dword& types)
{
    {
        long type = 9;
        types.push_back(type);
    }
    {
        long type = 0xb;
        types.push_back(type);
    }
    {
        long type = 0xd;
        types.push_back(type);
    }
}
