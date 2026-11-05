// CCountedPointer<CSpeechFrame>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the speech frame (free its frequency array storage) and free the object
// Leaf: ?DeleteData@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4A910, length 33 bytes, `ret` (static `__fastcall`, data in ecx).
// Referenced (absolute) at 0x00A4ABB0 by CCountedPointer<CSpeechFrame>::Reset 0x00A4AB60 (landed), which stores it as the
//   count block's DeleteFunc; that Reset is called from 0x00A4958B with a fresh 0x1C-byte CSpeechFrame (three zero words =
//   the array) whose ReadFromFile 0x00A49240 follows. Ego_r.pdb names the template member
//   ?DeleteData@?$CCountedPointer@...@@SIXPAX@Z (public static __fastcall void (void*)).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the array block), operator delete 0x00BFE9BC.
// Label correction: catalogued as `FreeListNode_Destroy`.
// Layout (retail, as the landed CSpeechFrame::ReadFromFile): Frequencies CArray<CComplexNumber> +0 (STLport vector: start
//   +0, finish +4, end of storage +8), Amplitude +0x0C, MedianFrequency +0x10, BaseFrequency +0x14, FrequencyVariance
//   +0x18 (0x1C bytes; the PDB's 16-byte CArray puts it at 0x20).
// Shape: `delete (T*)data` -- the null test, the inline (forced) destructor chain of a POD-element vector (`_Destroy` of
//   trivially destructible elements is empty; `_Vector_base`'s destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                 // 0x00BFEA14
void __cdecl operator delete(void* p);               // 0x00BFE9BC

#include "engine/CArray.h"

class CComplexNumber {                               // 8 bytes, trivially destructible
public:
    float Real, Imaginary;
};


class CSpeechFrame {                                 // 0x1C bytes in retail
public:
    CArray<CComplexNumber> Frequencies;              // +0x00
    float Amplitude;                                 // +0x0C
    float MedianFrequency;                           // +0x10
    float BaseFrequency;                             // +0x14
    float FrequencyVariance;                         // +0x18
};

#include "engine/CCountedPointer.h"

template class CCountedPointer<CSpeechFrame>;