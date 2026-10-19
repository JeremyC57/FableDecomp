// CEnginePrimitiveManagerStaticMeshesBase::GetSupportedTypes(CRetailVector_dword& types) -- append the three static-mesh primitive types (8, 10, 12) through the inlined push_back
// Grade: DIFFER(176v176) -- frame slots only: retail keeps the first value temporary at [esp+4] but homes the second and third in
//        the dead `types` argument slot ([esp+0Ch], shared with the tag); ours reuses [esp+4] for all three
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA6EF0, length 176 bytes (ret 4; `this` unused).
// Callees: the reallocating insert @ 0x00440990 (STLport _M_insert_overflow), three times.
// Shape: the landed PushBackConstant family (0x00B9BFA0) three times over; tried named locals, one reused local, an enum
// element type, a by-value push_back and an inline helper -- the value temporaries never move to the argument slot.
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
struct CEnginePrimitiveManagerStaticMeshesBase {
    void GetSupportedTypes(CRetailVector_dword& types);
};
void CEnginePrimitiveManagerStaticMeshesBase::GetSupportedTypes(CRetailVector_dword& types)
{
    types.push_back(8);
    types.push_back(10);
    types.push_back(12);
}
