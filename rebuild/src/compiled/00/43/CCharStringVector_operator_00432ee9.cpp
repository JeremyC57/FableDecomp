// CCharStringVector::operator=(const CCharStringVector&) -- STLport vector<CCharString> copy assignment (size-optimised TU)
// Leaf: ??4CCharStringVector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00432EE9, 263 bytes; 52 callers (def Copy bodies 0x45C315..,
// script 0xCBFDBE.., 0xE54C6B..). The catalog label "CCombatWheel::operator=" is a propagated name.
// Callees: _M_allocate_and_copy 0x00432FF0, _M_clear 0x00433040, CCharString::operator= 0x0099EFB0,
//          CCharString copy ctor 0x0099EC30, _Destroy(first, last) range helper 0x00414E00 (__fastcall).
// Shape: STLport `vector::operator=` verbatim -- (1) more than capacity: allocate-and-copy, clear, adopt;
// (2) shrinking: element-assign loop (counted `n = last - first; n > 0`) then destroy the tail;
// (3) growing within capacity: assign over the live part, placement-copy-construct the rest (the
// `cmp [p],0 ; je` is the placement-new null test); finally `finish = start + n`. The TU is
// size-optimised with frame pointers (`push ebp ; mov ebp,esp`, `leave`), as for the landed
// CParticleAttachmentInfoVector::operator= 0x004E29BA.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
private:
    void* m_pData;
};

void __fastcall DestroyRange(CCharString* first, CCharString* last);   // 0x00414E00

static __forceinline CCharString* CopyRange(const CCharString* first, const CCharString* last, CCharString* out)
{
    for (int n = int(last - first); n > 0; --n, ++first, ++out)
        *out = *first;
    return out;
}
static __forceinline CCharString* UninitializedCopy(const CCharString* first, const CCharString* last, CCharString* out)
{
    CCharString* current = out;
    for (; first != last; ++first, ++current)
        new (current) CCharString(*first);
    return current;
}

struct CCharStringVector {
    CCharString* Start;
    CCharString* Finish;
    CCharString* End;
    unsigned int size() const { return Finish - Start; }
    unsigned int capacity() const { return End - Start; }
    CCharString* AllocateAndCopy(unsigned int n, const CCharString* first, const CCharString* last);  // 0x00432FF0
    void Clear();                                                                                    // 0x00433040
    CCharStringVector& operator=(const CCharStringVector& other);
};

CCharStringVector& CCharStringVector::operator=(const CCharStringVector& other)
{
    if (&other != this) {
        unsigned int n = other.size();
        if (n > capacity()) {
            CCharString* fresh = AllocateAndCopy(n, other.Start, other.Finish);
            Clear();
            Start = fresh;
            End = Start + n;
        } else if (size() >= n) {
            CCharString* i = CopyRange(other.Start, other.Finish, Start);
            DestroyRange(i, Finish);
        } else {
            CopyRange(other.Start, other.Start + size(), Start);
            UninitializedCopy(other.Start + size(), other.Finish, Finish);
        }
        Finish = Start + n;
    }
    return *this;
}