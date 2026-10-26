// CHeightMap::SetSizeZAt(const SCell* cell, float size) -- clamp the size into [0, 2048 - 1e-4] and store it as the first float of the cell record at ((rows - cell->y) * (columns + 1) + cell->x) in the 12-byte cell array at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00825440, length 96 bytes.
// Floats 0x0122DEDC (0.0f), 0x012724C0 (2048.0f), 0x0129BA3C (1e-4f). Shape: `fcomp ; test ah,5 ; jp` for the zero test, the limit left on the stack for `fcomp st(1) ; test ah,41h ; je`, the index by `imul` and `lea eax,[eax+eax*2]`, `ret 8`.
extern const float g_zero_0122dedc;               // 0x0122DEDC (0.0f)
extern const float g_maxSize_012724c0;            // 0x012724C0 (2048.0f)
extern const float g_epsilon_0129ba3c;            // 0x0129BA3C (1e-4f)
struct SCellIndex_00825440 { long m_x; long m_y; };
struct SCell_00825440 { float m_sizeZ; float m_1; float m_2; };   // 12 bytes
class CHeightMap { public:
    unsigned char    _pad[0x10];
    SCell_00825440*  m_pCells;                     // +0x10
    unsigned char    _pad14[0x1f0];
    long             m_columns;                    // +0x204
    long             m_rows;                       // +0x208
    void SetSizeZAt(const SCellIndex_00825440* cell, float size);
};

void CHeightMap::SetSizeZAt(const SCellIndex_00825440* cell, float size)
{
    float value;
    if (size < g_zero_0122dedc) {
        value = g_zero_0122dedc;
    } else {
        float limit = g_maxSize_012724c0 - g_epsilon_0129ba3c;
        value = size > limit ? limit : size;
    }
    m_pCells[(m_rows - cell->m_y) * (m_columns + 1) + cell->m_x].m_sizeZ = value;
}