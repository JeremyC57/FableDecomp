// CCharString::GetLength() const -- number of characters in the string, 0 when it has no representation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403A00, 11 bytes. Callees: none.
//
// CCharString is the engine's reference-counted narrow string. An object holds a single pointer to a
// shared representation block; a null pointer is the canonical empty string, so the length is read from
// the block when there is one and is otherwise zero.
//
// Matching: the null test is written as an early return so the zero path is the fall-through.
// Representation block: character storage and the cached length.
struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };     // null rep == empty string
long __fastcall CCharString_GetLength(const CCharString* self) {
    StringRep* r = self->rep;
    if (r == 0) return 0;
    return r->len;
}
