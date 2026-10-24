// _global::Copy_FlaggedPosEmitter_006f0640(SFlaggedPosEmitter*, SFlaggedPosEmitter*, SFlaggedPosEmitter*, const random_access_iterator_tag&, int*) -- SGI __copy over 0x1C-byte (flag, packed position, word, emitter pointer) records
// Leaf: Copy_FlaggedPosEmitter_006f0640
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F0640, length 112 bytes.
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / 0x1C (`imul 92492493h ; add ; sar 4`), counted down; the advanced result is returned.
// Element: 0x1C bytes = a byte +0 and a C3DVector at the UNALIGNED offset +1 (`lea ecx,[esi+1]` / `lea edx,[edi+1]`
// pointer-register copy: a C3DVector with a user copy constructor and no operator=), so the two sit in a 1-byte-packed
// 13-byte block with a memberwise user operator= (its implicit one block-copies 13 bytes: 3 dwords + 1 byte); then a long at +0x10 (the outer struct realigns) and a CIntelligentPointer at +0x14
// (SetPItem(other.GetPItem()), CBaseIntelligentPointer @ 0x00A01B50 / 0x00A01B90). The manifest's
// `CTCSmokeGenerator::CGenerationPos` label is propagated (that record is 0x14 bytes, see 0x00722C90).
struct CBaseObject;

struct CBaseIntelligentPointer {             // 8 bytes: vtable + item
    virtual void v00();
    void* PData;                             // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

#pragma pack(push, 1)
struct SFlaggedPos {                         // 13 bytes, byte-packed in retail (the vector sits at +1)
    bool      Flag;                          // +0x00
    C3DVector Pos;                           // +0x01
    SFlaggedPos& operator=(const SFlaggedPos& other)
    {
        Flag = other.Flag;
        Pos = other.Pos;
        return *this;
    }
};
#pragma pack(pop)

struct CThing;

struct SFlaggedPosEmitter {                  // 0x1C bytes
    SFlaggedPos                 Where;       // +0x00
    long                        Value;       // +0x10
    CIntelligentPointer<CThing> PEmitter;    // +0x14
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SFlaggedPosEmitter* __fastcall Copy_FlaggedPosEmitter_006f0640(SFlaggedPosEmitter* first, SFlaggedPosEmitter* last, SFlaggedPosEmitter* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}