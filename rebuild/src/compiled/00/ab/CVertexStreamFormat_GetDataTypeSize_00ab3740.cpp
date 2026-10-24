// CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType type) -- byte size of one vertex-stream element of the given data type (0 for an unknown type)
// Leaf: ?GetDataTypeSize@CVertexStreamFormat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB3740, length 108 bytes (51 code + 1 nop + the 14-entry jump table)
//   (+4 int3), static __fastcall (type in ecx), plain `ret`.
// Identity: Ego_r.pdb `?GetDataTypeSize@CVertexStreamFormat@@SIKW4EVertexStreamDataType@@@Z` (static, __fastcall,
//   unsigned long); callers CVertexStreamFormat::CalculateVertexSize (0x00AB37D5) and the vertex stream converter
//   (0x00AC4E73 / 0x00AC4E84). The manifest's "GetDataTypeSize" leaf is right.
// Table (verified entry by entry against retail 0x00AB3774, relocation masking cannot check it):
//   0 -> 4, 1 -> 8, 2 -> 12, 3 -> 16, 4 -> 4, 5 -> 4, 6 -> 8, 7 -> 4, 8 -> 2, 9 -> 4, 10 -> 6, 11 -> 8, 12 -> 4, 13 -> 4;
//   anything above 13 -> 0. The enumerator names of the engine's EVertexStreamDataType are not in the PDB, so they are
//   given here by element size (descriptive).
// Shape: one dense switch with a `return` per case in enumerator order except the three 8-byte types, which share one
//   case group in second place; VC7.1 cross-jumps the identical `return 4` blocks into the first one (block order 4, 8,
//   12, 16, 2, 6, default) and keeps a direct 14-entry jump table. Fully grouped case labels give a byte index table
//   plus a 7-entry jump table (98 v 108); fully separate returns keep the LAST 8-byte block (order 4, 12, 16, 2, 6, 8).
//   The object's table relocations were checked against retail entry by entry ($L299=4, $L300=8, $L301=12, $L302=16,
//   $L306=2, $L308=6).
enum EVertexStreamDataType {
    VSDT_4BYTE_A = 0, VSDT_8BYTE_A = 1, VSDT_12BYTE = 2, VSDT_16BYTE = 3, VSDT_4BYTE_B = 4, VSDT_4BYTE_C = 5,
    VSDT_8BYTE_B = 6, VSDT_4BYTE_D = 7, VSDT_2BYTE = 8, VSDT_4BYTE_E = 9, VSDT_6BYTE = 10, VSDT_8BYTE_C = 11,
    VSDT_4BYTE_F = 12, VSDT_4BYTE_G = 13
};

class CVertexStreamFormat {
public:
    static unsigned long __fastcall GetDataTypeSize(EVertexStreamDataType type);
};

unsigned long __fastcall CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType type)
{
    switch (type) {
    case VSDT_4BYTE_A:  return 4;
    case VSDT_8BYTE_A:
    case VSDT_8BYTE_B:
    case VSDT_8BYTE_C:  return 8;
    case VSDT_12BYTE:   return 12;
    case VSDT_16BYTE:   return 16;
    case VSDT_4BYTE_B:  return 4;
    case VSDT_4BYTE_C:  return 4;
    case VSDT_4BYTE_D:  return 4;
    case VSDT_2BYTE:    return 2;
    case VSDT_4BYTE_E:  return 4;
    case VSDT_6BYTE:    return 6;
    case VSDT_4BYTE_F:  return 4;
    case VSDT_4BYTE_G:  return 4;
    }
    return 0;
}