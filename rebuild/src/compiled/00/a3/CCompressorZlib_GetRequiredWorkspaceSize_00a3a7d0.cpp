// CCompressorZlib::GetRequiredWorkspaceSize() -- static: zlib needs no caller-provided workspace, so 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3A7D0, 3 bytes: `xor eax,eax ; ret` (static __fastcall, no arguments).
// Identity: Ego_r.pdb ?GetRequiredWorkspaceSize@CCompressorZlib@@SIKXZ directly before ?HasStreamInput@CCompressorZlib@@
//   QBE_NXZ and ?CompressStream@CCompressorZlib@@QAEKPAXK@Z = retail 0x00A3A7D0 / 0x00A3A7E0 / 0x00A3A7F0 (the last landed
//   as CCompressorZlib::CompressStream); the callers are the zlib stream users (CDataInputStream::ReadZlibCompressed,
//   CDataOutputStream::WriteZlibCompressed, CCompressedMemoryDataInputStream ctor / OpenMemorySource, the save-data
//   persistence paths), each passing the returned size on (`call ; push eax ; ... call`). Not "AddChildPrimitive" (a
//   propagated label) or a generic ReturnZero.
// Leaf: ?GetRequiredWorkspaceSize@CCompressorZlib@@SIKXZ
class CCompressorZlib {
public:
    static unsigned long __fastcall GetRequiredWorkspaceSize();
};

unsigned long __fastcall CCompressorZlib::GetRequiredWorkspaceSize()
{
    return 0;
}
