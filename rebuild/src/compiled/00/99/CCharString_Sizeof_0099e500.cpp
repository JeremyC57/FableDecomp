// CCharString::Sizeof @ 0x0099e500
// Leaf: ?CCharString_Sizeof@@YIKPAVCCharString@@@Z
// Reads [ecx] (data header pointer). If null -> 4. Else (header->len & 0x7fffffff) + 0x15.

struct StringHeader {
    unsigned long refcount;   // +0
    unsigned long field4;     // +4
    unsigned long length;     // +8
};

class CCharString { public:
    StringHeader* header;     // +0
};

unsigned long __fastcall CCharString_Sizeof(CCharString* self) {
    StringHeader* h = self->header;
    unsigned long result = 4;
    if (h != 0)
        result = (h->length & 0x7fffffff) + 0x15;
    return result;
}