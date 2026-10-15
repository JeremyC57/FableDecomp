// CLayer::SetAllPolys(const CMatrix44&) -- set this 4x4 matrix to the inverse of the given one (copied element by element into a D3DX matrix, inverted by D3DXMatrixInverse, copied back)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5CB00, length 144 bytes.
// Callees: D3DXMatrixInverse @ 0x00BFEFE0 (`__stdcall`, statically linked d3dx9).
// Shape: both copies are column-outer / row-inner double loops over 4x4 dword elements (`dec ecx` inner,
// `dec ebx`/`dec edi` outer, the source addressed as `[edx+eax]` with edx = source - destination); the
// determinant out-value is a zeroed float local. The catalog label (CLayer::SetAllPolys) is propagated: the
// body is a 64-byte matrix's "set to inverse of" member, kept under the catalog name.
struct D3DXMATRIX { float m[4][4]; };
D3DXMATRIX* __stdcall D3DXMatrixInverse(D3DXMATRIX* out, float* determinant, const D3DXMATRIX* in);   // 0x00BFEFE0
struct CLayer {                                    // modelled as the 4x4 float matrix the body writes
    float m[4][4];
    void SetAllPolys(const CLayer& source);
};

void CLayer::SetAllPolys(const CLayer& source)
{
    D3DXMATRIX in;
    for (long column = 0; column < 4; ++column)
        for (long row = 0; row < 4; ++row)
            in.m[row][column] = source.m[row][column];
    float determinant = 0.0f;
    D3DXMATRIX out;
    D3DXMatrixInverse(&out, &determinant, &in);
    for (long column = 0; column < 4; ++column)
        for (long row = 0; row < 4; ++row)
            m[row][column] = out.m[row][column];
}
