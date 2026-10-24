// CEngineSubPrimitiveRenderer::SortSingleRadix(const CSortElement* source, CSortElement* destination, unsigned long count, unsigned long radix) -- one 10-bit counting-sort pass of the radix sort: histogram digit `radix` of every SortValue, prefix-sum the 1024 buckets, then scatter the elements into destination in stable order
// Grade: DIFFER(221v232) -- register allocation only: retail keeps source in edx, count in edi and the loop-1 index in esi (temp eax); ours source edi, count esi, index eax (temp edx); the 11-byte gap is retail's loop-1 alignment jmp + lea esp,[esp] pad. Loop 3 (named post-increment index) and the and esp,-8 frame match.
// Leaf: ?SortSingleRadix@CEngineSubPrimitiveRenderer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B84B20, 232 bytes. __thiscall (this unused), `ret 10h`; an
//   `and esp,-8` frame with _chkstk @ 0x00BFEA30 for the two 4 KB bucket arrays.
// Callers: none -- RadixSort 0x00B847D0 inlines the three passes; the out-of-line body is emitted because the member is
//   defined (not inline) in the TU.
// Identity: FableWin.pdb `?SortSingleRadix@CEngineSubPrimitiveRenderer@@AAEXPBVCSortElement@1@PAV21@KK@Z`; the manifest
//   label "RadixSort_CountingPass_b84b20" is descriptive.
// Shape: counts[1024] zeroed (`rep stosd`), `++counts[(source[i].SortValue >> (radix * 10)) & 0x3FF]` (the shift is
//   recomputed per loop: `lea ecx,[ebx+ebx*4] ; shl ecx,1`; the in-place `inc [mem]` leaves a dead `lea`), offsets[0] = 0
//   and `offsets[i] = offsets[i - 1] + counts[i - 1]` (unrolled 3x by VC7.1), then a named `index = offsets[digit]++`
//   before `destination[index] = source[i]` (the address / old / new triple that spills the down-counter to [esp+0Ch]).
class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
    };
private:
    void SortSingleRadix(const CSortElement* source, CSortElement* destination, unsigned long count, unsigned long radix);
};

void CEngineSubPrimitiveRenderer::SortSingleRadix(const CSortElement* source, CSortElement* destination,
                                                  unsigned long count, unsigned long radix)
{
    unsigned long offsets[1024];
    unsigned long counts[1024];
    unsigned long i;

    for (i = 0; i < 1024; ++i)
        counts[i] = 0;

    for (i = 0; i < count; ++i)
        ++counts[(source[i].SortValue >> (radix * 10)) & 0x3FF];

    offsets[0] = 0;
    for (i = 1; i < 1024; ++i)
        offsets[i] = offsets[i - 1] + counts[i - 1];

    for (i = 0; i < count; ++i) {
        unsigned long index = offsets[(source[i].SortValue >> (radix * 10)) & 0x3FF]++;
        destination[index] = source[i];
    }
}
