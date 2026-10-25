// CPKArray::GetRandom() -- return a uniformly chosen entry of the array (rand() scaled by 1/32767, clamped to the last)
// Leaf: ?GetRandom@CPKArray@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C61C00, 78 bytes (thiscall, plain `ret`).
// Callees: MSVCR71 `rand` (import 0x01440124), `_ftol2` 0x00BFEA70; constants 4294967296.0f (0x0122DCB4, the unsigned
//   fild fix-up) and 3.0518509e-05f = 1/32767 (0x0129C81C).
// Identity: the manifest's "CArrayBase_GetRandomElement" is right in substance; the array is the one built by
//   CPKArray::CPKArray 0x00C61B90 (+4 storage, +8 count). Its caller 0x00C61E43 passes the entry to a lookup 0x00C36C70.
//   Descriptive name.
// Shape: the count is read once into a local; `rand() * count / 32767.0f` compiles to the same reciprocal multiply.
extern "C" __declspec(dllimport) int __cdecl rand(void);

class CPKArray {
public:
    unsigned long  m_capacity;                     // +0
    unsigned long* m_pData;                        // +4
    unsigned long  m_count;                        // +8
    unsigned long GetRandom();
};

unsigned long CPKArray::GetRandom()
{
    unsigned long count = m_count;
    unsigned long index = (unsigned long)((float)rand() * (float)count * (1.0f / 32767.0f));
    if (index == count)
        index--;
    return m_pData[index];
}