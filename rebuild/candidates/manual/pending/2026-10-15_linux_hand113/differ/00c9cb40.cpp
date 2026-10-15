// Grade: DIFFER(ours v retail) -- retail takes the grid in eax (an LTCG custom calling convention: eax = grid, edx = column, one stack argument = row); no VC7.1 source convention passes a pointer in eax, so the fastcall form loads the grid from ecx
// CTopologySizeGrid::Hora(long, long) -- returns the grid cell at (column, row): data[row * width + column]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C9CB40, length 18 bytes (+ int3 padding).
// Layout: data pointer at +0x00, row width at +0x10.
struct CTopologySizeGrid {
    long*         m_pData;                         // +0x00
    unsigned char _pad_0x04[0x0c];
    long          m_width;                         // +0x10
};

long __fastcall Hora(CTopologySizeGrid* grid, long column, long row)
{
    return grid->m_pData[row * grid->m_width + column];
}
