# h25_cfl.py: naming wave 25, family cfl -- 0x00AA7540 ~CChunkedFileLoader (c692 queue).
#  The own row was landed as a generic `struct T` (Leaf "??0CBase@", defining ??1T@@QAE@XZ); retail is `mov [ecx],0x0129E464 ; ret`,
#  the tail of ~C3DMeshFileLoader 0x00A9A900 / ~C3DAnimFileLoader 0x00A996D0 after their members (both bind ??1CChunkedFileLoader@@UAE@XZ
#  here), and Ego_r.pdb's CChunkedFileLoader (0x10: vptr, ReadUnknownChunkTypes, SupportedFileType[5]) has a virtual dtor.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('00aa7540', 'src', '''// CChunkedFileLoader::~CChunkedFileLoader() -- the chunked-file loader base has no members to release: the destructor only re-installs its
//   own vtable 0x0129E464
// Leaf: ??1CChunkedFileLoader@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7540, 7 bytes: `mov dword ptr [ecx],0x0129E464 ; ret` (the vtable a data relocation).
// Naming wave 25 (c692 queue): the base destructor ~C3DMeshFileLoader 0x00A9A900 and ~C3DAnimFileLoader 0x00A996D0 tail-jump to (both
//   bind ??1CChunkedFileLoader@@UAE@XZ here; vland RELOC_NAME); Ego_r.pdb CChunkedFileLoader (0x10 bytes: vptr, ReadUnknownChunkTypes +4,
//   SupportedFileType[5] +8) declares a virtual destructor. Was landed on a generic `struct T` (??1T@@QAE@XZ); the catalog's
//   "CChunkedFileLoader::CChunkedFileLoader" label is the destructor. No engine/CChunkedFileLoader.h exists.
class CChunkedFileLoader {                                   // vtable 0x0129E464, 0x10 bytes
public:
    virtual ~CChunkedFileLoader();
    long ReadUnknownChunkTypes;                              // +0x04
    char SupportedFileType[5];                               // +0x08
};

CChunkedFileLoader::~CChunkedFileLoader()
{
}
''')
