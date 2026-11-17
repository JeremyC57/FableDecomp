// CVertexShaderInput::GenerateD3DDeclaration() -- rebuild the D3D vertex declaration from the stream descriptions: size it to one entry per element plus one end marker per stream, then one TEXCOORD element per stream element (stream, running byte offset, D3D type from D3DTypes, the element's Register member as usage index) and the D3DDECL_END marker; an element type with no D3D equivalent clears the declaration
// Leaf: ?GenerateD3DDeclaration@CVertexShaderInput@@AAEXXZ
// Retail: 0x00AB3870, 668 bytes with the GetDataTypeSize jump table at 0x00AB3AD4 (code ends at the `ret` 0x00AB3AD3).
// Names: Ego_r / Ego_d; Ego_d locals ele, size, data_offset, dest_pos (function scope), c_stream / stream (first loop), c_stream /
//        stream / no_elem / c_elem and the inner block's reg, ele, d3dtype, type. D3DTypes = ?D3DTypes@CVertexShaderInput@@0QBJB.
// FITTED (chosen by grading; Ego_d gives names / scopes only): `dest_pos` declared after the resize (retail zeroes data_offset first and
//        keeps 0 in ebp), the one-argument resize(size) (STLport fills with _D3DVERTEXELEMENT9(); the two-argument form schedules the
//        zero store one instruction early), the END marker as a late function-scope `ele` initialiser; the shared GetDataTypeSize body
//        (engine/CVertexStreamFormat.h: one `case` per enumerator grouped by size, which also grades RELOCATION_MATCH at its own
//        out-of-line copy 0x00AB3740) -- the fall-through grouped spelling gives a two-level byte-indexed table here.
#define FABLE_RETAIL_D3DVERTEXELEMENT9
#include "engine/CVertexShaderInput.h"

void CVertexShaderInput::GenerateD3DDeclaration()
{
    unsigned long size = 0;
    long data_offset = 0;
    for (unsigned long c_stream = 0; c_stream < DataStreams.size(); ++c_stream)
    {
        const CStreamInfo& stream = DataStreams[c_stream];
        size += stream.StreamFormat.GetElementCount() + 1;
    }
    D3DDeclaration.resize(size);
    unsigned long dest_pos = 0;
    for (unsigned long c_stream = 0; c_stream < DataStreams.size(); ++c_stream)
    {
        const CStreamInfo& stream = DataStreams[c_stream];
        unsigned long no_elem = stream.StreamFormat.GetElementCount();
        for (unsigned long c_elem = 0; c_elem < no_elem; ++c_elem)
        {
            EVertexStreamDataType type = stream.StreamFormat.GetElementDataType(c_elem);
            long d3dtype = D3DTypes[type];
            if (d3dtype == -1)
            {
                D3DDeclaration.clear();
                return;
            }
            unsigned long reg = stream.StreamFormat.GetElementRegister(c_elem);
            _D3DVERTEXELEMENT9 ele;
            ele.Stream = (WORD)c_stream;
            ele.Offset = (WORD)data_offset;
            ele.Type = (BYTE)d3dtype;
            ele.Method = D3DDECLMETHOD_DEFAULT;
            ele.Usage = D3DDECLUSAGE_TEXCOORD;
            ele.UsageIndex = (BYTE)reg;
            D3DDeclaration[dest_pos++] = ele;
            data_offset += CVertexStreamFormat::GetDataTypeSize(type);
        }
    }
    _D3DVERTEXELEMENT9 ele = { 0xff, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 };
    D3DDeclaration[dest_pos] = ele;
}
