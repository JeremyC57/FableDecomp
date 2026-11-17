#ifndef FABLE_ENGINE_CVERTEXSHADERINPUT_H
#define FABLE_ENGINE_CVERTEXSHADERINPUT_H
// New header (egod e07, row 0x00AB3870): Ego_d.pdb class record -- CBaseClass base, private DataStreams CArray<CStreamInfo> +4,
// D3DDeclaration CArray<_D3DVERTEXELEMENT9> +0x10 (retail STLport 12-byte arrays; sizeof 0x1C), the private static const long
// D3DTypes[] (?D3DTypes@CVertexShaderInput@@0QBJB, retail 0x0129E9CC), nested CStreamInfo {StreamIndex +0, StreamFormat +4} (20
// bytes). Ego_r.pdb members: ?AddStream@CVertexShaderInput@@QAEXKABVCVertexStreamFormat@@_N@Z (0x00AB3B40),
// ?GenerateD3DDeclaration@CVertexShaderInput@@AAEXXZ (private, 0x00AB3870). Needs FABLE_RETAIL_D3DVERTEXELEMENT9.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/CArray.h"
#include "engine/Direct3D9Types.h"
#include "engine/CVertexStreamFormat.h"
class CVertexShaderInput : public CBaseClass {
public:
    class CStreamInfo {
    public:
        CStreamInfo(unsigned long stream_index, const CVertexStreamFormat& format) : StreamIndex(stream_index), StreamFormat(format) { }
        unsigned long       StreamIndex;                                 // +0x00
        CVertexStreamFormat StreamFormat;                                // +0x04
    };
    CVertexShaderInput();                                                // 0x00AB3B10
    void AddStream(unsigned long stream_index, const CVertexStreamFormat& format, bool generate_declaration);   // 0x00AB3B40
private:
    void GenerateD3DDeclaration();                                       // 0x00AB3870
    CArray<CStreamInfo>        DataStreams;                              // +0x04
    CArray<_D3DVERTEXELEMENT9> D3DDeclaration;                           // +0x10
    static const long          D3DTypes[];                               // 0x0129E9CC
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderInput) == 0x1c);
#endif
