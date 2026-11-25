#ifndef FABLE_ENGINE_C3DANIMFILE_H
#define FABLE_ENGINE_C3DANIMFILE_H
#include "rebuild_abi.h"
#include "engine/CChunkedFile.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFile:public CChunkedFile{public:
C3DAnimFile(const C3DAnimFile&);
#ifdef FABLE_RETAIL_C3DANIMFILE_LOADER_FRIEND
// Per-TU opt-in (NF422, lib_3d_anim_file.obj unit 0x00A98E60): C3DAnimFileLoader::LoadAnimFile 0x00A99420 calls the PRIVATE root-chunk
// constructor (0x00A99478 `call 0x00A98F40`; Ego_r.pdb 0x00B980A0 / FableWin.pdb 0x03204CD0 spell it AAE), so the loader is a friend.
// Access only: no layout or code change.
friend class C3DAnimFileLoader;
#endif
private:
C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>&);
public:
C3DAnimFile();
CCountedPointer<C3DAnimFileRootChunk const > GetRoot() const;
CCountedPointer<C3DAnimFileRootChunk> GetRoot();
#ifdef FABLE_RETAIL_C3DANIMFILE_IMPLICIT_DESTRUCTOR
// Per-TU opt-in (NF419, rows 0x00A98F40 / 0x00A9A730, lib_3d_anim_file.obj): the destructor is the implicit one (still virtual through
// CChunkedFile). Retail has no ~C3DAnimFile body (no call or jump reaches ~CChunkedFile 0x00AA7DF0 except from the two ??_G at
// 0x00A9A733 / 0x00A9D2A3), and ??_GC3DAnimFile 0x00A9A730 calls ~CChunkedFile directly with no vtable store: VC7.1 emits that
// 30-byte body only for the implicit destructor (a user-written empty one stores the vtable first: 36 bytes, measured).
#else
virtual ~C3DAnimFile();
#endif
};
FABLE_STATIC_ASSERT_AT(24, sizeof(C3DAnimFile)==0x14);
#endif
