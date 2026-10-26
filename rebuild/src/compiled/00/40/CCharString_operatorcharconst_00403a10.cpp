// CCharString::operator const char*() const -- the string's characters, or the shared empty string when it has no representation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403A10, 15 bytes. Callees: none (reads g_EmptyCharString_0129AAF4).
//
// The conversion every C-API call on a CCharString goes through. The object holds one pointer to its
// shared representation block; when that is null the string is empty and the address of the global
// empty-string sentinel is returned so callers always get a valid, terminated buffer. Otherwise the
// block's character storage is returned directly (it is the block's first member).
//
// Matching: `if (r == 0) return sentinel;` keeps the null path as the fall-through (`test; jne` then
// `mov eax,imm32; ret`), and the sentinel is returned as the address of a global, not a load. The oracle
// slice is longer than the function; only the leading 15 bytes up to the first `ret` are this routine.

// Representation block: character storage first, so `rep->data` is a single load.
struct StringRep { char* data; long len; };
class CCharString { public: StringRep* rep; };     // null rep == empty string

// Shared empty-string sentinel.
extern char g_EmptyCharString_0129AAF4[1];

const char* __fastcall CCharString_operator_char_const(const CCharString* self)
{
    StringRep* r = self->rep;
    if (r == 0)
        return g_EmptyCharString_0129AAF4;
    return r->data;
}
