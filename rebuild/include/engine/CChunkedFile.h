#ifndef FABLE_ENGINE_CCHUNKEDFILE_H
#define FABLE_ENGINE_CCHUNKEDFILE_H
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CChunkedFile:public CBaseClassNonCopyable{public:
CChunkedFile(const CChunkedFile&);
CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&);
virtual ~CChunkedFile();
#ifdef FABLE_RETAIL_CCHUNKEDFILE_SETTERS
// Per-TU opt-in (NF422, the default constructors 0x00A98E60 C3DAnimFile / 0x00A9AD80 C3DMeshFile): FableWin.pdb
// ?SetVersion@CChunkedFile@@QAEXK@Z (0x03204C20) and ?SetFileComment@CChunkedFile@@QAEXABVCCharString@@@Z (0x03204C40), debug COMDATs
// of lib_3d_anim_file.obj next to ??0C3DAnimFile@@QAE@XZ; release expands both (0x00A98F15 `call CCharString::operator=` on +0x10,
// 0x00A98F23 `mov [edi+0Ch],64h`). No layout change.
    void SetVersion(unsigned long version) { Version = version; }
    void SetFileComment(const CCharString& comment) { FileComment = comment; }
#endif
#ifdef FABLE_RETAIL_CCHUNKEDFILE_GETCHUNKEDFILEROOT
    const CCountedPointer<CChunkedFileSuperChunk>& GetChunkedFileRoot() const { return RootChunk; }
#endif
private:CCountedPointer<CChunkedFileSuperChunk> RootChunk;unsigned long Version;CCharString FileComment;};
FABLE_STATIC_ASSERT_AT(14, sizeof(CChunkedFile)==0x14);
#endif
