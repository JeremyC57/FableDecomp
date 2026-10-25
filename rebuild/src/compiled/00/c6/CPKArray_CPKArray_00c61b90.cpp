// CPKArray::CPKArray(unsigned long capacity) -- fixed-capacity array of 32-bit entries: allocate the storage, start empty
// Leaf: ??0CPKArray@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C61B90, 37 bytes (thiscall constructor, `ret 4`, returns this).
// Callees: MSVCR71 `operator new[]` (thunk 0x00BFEB22; capacity * 4 bytes, no overflow check in VC7.1).
// Identity: the manifest's "CArrayBase_Initialize" is a propagated label. Its one caller (0x00C44F3A) constructs it as
//   the member at +0x80 of a libpk object inside that object's constructor (EH state 3 -> 4 around it); the same
//   layout (+4 storage, +8 count) is read by CPKArray::GetRandom 0x00C61C00. Descriptive class and member names.
// Shape: body assignments in the order capacity, count, storage (an initialiser list stores the count after the call).
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22 (without it VC7.1 calls scalar new 0x00BFEA1A)

class CPKArray {
public:
    unsigned long  m_capacity;                     // +0
    unsigned long* m_pData;                        // +4
    unsigned long  m_count;                        // +8
    CPKArray(unsigned long capacity);
};

CPKArray::CPKArray(unsigned long capacity)
{
    m_capacity = capacity;
    m_count = 0;
    m_pData = new unsigned long[capacity];
}