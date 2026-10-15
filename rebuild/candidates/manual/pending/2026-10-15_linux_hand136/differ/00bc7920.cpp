// Grade: DIFFER(ours v retail) -- 122v122: the second pushed value (0x27) is homed by retail in the dead vector-argument slot it shares with the empty tag temporary (`mov [esp+0Ch],27h` ; `lea ecx,[esp+14h]` and `lea edx,[esp+18h]` both address it), the lane keeps it in the `push ecx` frame word (`mov [esp+4],27h`, `lea edx,[esp+10h]`); tried literal temporaries (best) and named locals (8-byte frame)
// CEnginePrimitiveManagerText::GetSupportedTypes(std::vector<EEnginePrimitiveType>& types) -- appends the two primitive types this manager creates: 5 (3D text) and 0x27 (2D text)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7920, length 122 bytes.
// Callees: 0x00440990 the STLport vector's reallocating insert (`_M_insert_overflow(pos, const T&, __true_type,
// count, atend)`, landed PushBackConstant family) when the vector is full. `this` is unused (`ret 4`).
// Shape: two inlined STLport `push_back(const T&)` calls (null-tested placement store + `add [last],4`, else the
// overflow insert with the empty `__true_type` tag homed in the dead argument slot); each value is a literal
// bound to the const reference, so VC materialises it in a stack word: the first in the `push ecx` frame word,
// the second in the vector argument's slot (dead once the pointer sits in esi).
#include <new>

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_long {                        // STLport vector<long>: first, last, end_of_storage
    long* first;
    long* last;
    long* end;
    void InsertOverflow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertOverflow(last, value, SScalarTag(), 1, true);
    }
};

struct CEnginePrimitiveManagerText {
    void GetSupportedTypes(CRetailVector_long& types);
};

void CEnginePrimitiveManagerText::GetSupportedTypes(CRetailVector_long& types)
{
    types.push_back(5);                            // ENGINE_PRIMITIVE_TEXT
    types.push_back(0x27);                         // ENGINE_PRIMITIVE_2D_TEXT
}
