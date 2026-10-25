// CVertexShader::CVertexShader() -- the default constructor: install the vtable (0x0129B3D0) and clear the shader data pointer
// Leaf: ??0CVertexShader@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62FB0, 16 bytes, thiscall, returns this.
// Identity: the only vtable stored is 0x0129B3D0, whose RTTI type descriptor is .?AVCVertexShader@@ (rtti-vtables.tsv
//   CVertexShader slots 0 / 2 = 0x009C1AE0 / 0x009BE210); Ego_r.pdb CVertexShader is 8 bytes {vptr, CVertexShaderData* Data
//   +4} and both PDBs list the public default constructor ??0CVertexShader@@QAE@XZ (virtual destructor
//   ??1CVertexShader@@UAE@XZ in FableWin.pdb). Was catalogued CBaseIntelligentPointer::CBaseIntelligentPointer (a
//   generated tiny-body label shared with 0x0099A380).
// Shape: `mov eax,ecx ; mov [eax],vtable ; mov [eax+4],0 ; ret` -- the member initialiser after the vptr store.
class CVertexShaderData;

class CVertexShader {
public:
    CVertexShader();
    virtual ~CVertexShader();
    CVertexShaderData* Data;                    // +0x04
};

CVertexShader::CVertexShader()
    : Data(0)
{
}
