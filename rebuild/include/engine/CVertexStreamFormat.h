#ifndef FABLE_ENGINE_CVERTEXSTREAMFORMAT_H
#define FABLE_ENGINE_CVERTEXSTREAMFORMAT_H
// New header (egod e07, row 0x00AB3870): Ego_d.pdb class record (CBaseClass base, private DataElements CArray<CDataElement> +4, nested
// CDataElement {Register +0, DataType +4}; retail sizeof 0x10, vtable 0x0129D578) and EVertexStreamDataType (Ego_d enumerators).
// Members (Ego_r / FableWin / Ego_d): the inline accessors ?GetElementCount@CVertexStreamFormat@@QBEKXZ, ?GetElementRegister@..@@QBEKK@Z,
// ?GetElementDataType@..@@QBE?AW4EVertexStreamDataType@@K@Z (FableWin 0x03387190.., inline in retail) and the static __fastcall
// ?GetDataTypeSize@CVertexStreamFormat@@SIKW4EVertexStreamDataType@@@Z (body of its landed out-of-line copy 0x00AB3740; expanded by
// GenerateD3DDeclaration as the jump table at 0x00AB3AD4). Landed rows of this TU still use local models.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/CArray.h"
enum EVertexStreamDataType {
    VERTEX_STREAM_DATA_TYPE_FLOAT1 = 0, VERTEX_STREAM_DATA_TYPE_FLOAT2 = 1, VERTEX_STREAM_DATA_TYPE_FLOAT3 = 2,
    VERTEX_STREAM_DATA_TYPE_FLOAT4 = 3, VERTEX_STREAM_DATA_TYPE_D3DCOLOR = 4, VERTEX_STREAM_DATA_TYPE_SHORT2 = 5,
    VERTEX_STREAM_DATA_TYPE_SHORT4 = 6, VERTEX_STREAM_DATA_TYPE_UVSHORT2 = 7, VERTEX_STREAM_DATA_TYPE_NORMSHORT1 = 8,
    VERTEX_STREAM_DATA_TYPE_NORMSHORT2 = 9, VERTEX_STREAM_DATA_TYPE_NORMSHORT3 = 10, VERTEX_STREAM_DATA_TYPE_NORMSHORT4 = 11,
    VERTEX_STREAM_DATA_TYPE_NORMPACKED3 = 12, VERTEX_STREAM_DATA_TYPE_POSPACKED3 = 13
};
class CVertexStreamFormat : public CBaseClass {
public:
    class CDataElement {
    public:
        CDataElement(unsigned long reg, EVertexStreamDataType type) : Register(reg), DataType(type) { }
        unsigned long         Register;                                  // +0x00
        EVertexStreamDataType DataType;                                  // +0x04
    };
    void AddDataElement(unsigned long reg, EVertexStreamDataType type);  // 0x00AB3810
    unsigned long GetElementCount() const { return DataElements.size(); }
    unsigned long GetElementRegister(unsigned long index) const { return DataElements[index].Register; }
    EVertexStreamDataType GetElementDataType(unsigned long index) const { return DataElements[index].DataType; }
    unsigned long CalculateVertexSize() const;                           // 0x00AB37B0
    static unsigned long FABLE_FASTCALL GetDataTypeSize(EVertexStreamDataType type)
    {
        switch (type) {
        case VERTEX_STREAM_DATA_TYPE_FLOAT1:       return 4;
        case VERTEX_STREAM_DATA_TYPE_D3DCOLOR:     return 4;
        case VERTEX_STREAM_DATA_TYPE_SHORT2:       return 4;
        case VERTEX_STREAM_DATA_TYPE_UVSHORT2:     return 4;
        case VERTEX_STREAM_DATA_TYPE_NORMSHORT2:   return 4;
        case VERTEX_STREAM_DATA_TYPE_NORMPACKED3:  return 4;
        case VERTEX_STREAM_DATA_TYPE_POSPACKED3:   return 4;
        case VERTEX_STREAM_DATA_TYPE_FLOAT2:       return 8;
        case VERTEX_STREAM_DATA_TYPE_SHORT4:       return 8;
        case VERTEX_STREAM_DATA_TYPE_NORMSHORT4:   return 8;
        case VERTEX_STREAM_DATA_TYPE_FLOAT3:       return 12;
        case VERTEX_STREAM_DATA_TYPE_FLOAT4:       return 16;
        case VERTEX_STREAM_DATA_TYPE_NORMSHORT1:   return 2;
        case VERTEX_STREAM_DATA_TYPE_NORMSHORT3:   return 6;
        }
        return 0;
    }
private:
    CArray<CDataElement> DataElements;                                   // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CVertexStreamFormat) == 0x10);
#endif
