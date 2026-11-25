#ifndef FABLE_ENGINE_C3DANIMFILEROOTCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEROOTCHUNK_H
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileRootChunk:public CChunkedFileSuperChunk{public:
C3DAnimFileRootChunk(const C3DAnimFileRootChunk&);
#ifdef FABLE_RETAIL_C3DANIMFILEROOTCHUNK_INLINE_CTOR
// Per-TU opt-in (NF422, lib_3d_anim_file.obj unit 0x00A98E60): retail expands the default constructor in C3DAnimFile::C3DAnimFile()
// (0x00A98E77: push "ANRT" 0x0129DFBC ; call ??0CChunkedFileSuperChunk@@IAE@PBD@Z 0x00AA8360 ; store ??_7C3DAnimFileRootChunk 0x0129DFF0;
// no member stores), like C3DMeshFileRootChunk.h's "3DRT" one. Ego_r.pdb 0x00B97C60 / FableWin.pdb 0x03204D60 ??0C3DAnimFileRootChunk@@QAE@XZ.
// The destructor is the implicit one (still virtual through the base): ??_GC3DAnimFileRootChunk 0x00A9A2E0 calls
// ??1CChunkedFileSuperChunk@@UAE@XZ 0x00AA8490 directly with no vtable store, the body VC7.1 gives only an implicit destructor
// (both PDBs list no ~C3DAnimFileRootChunk proc in the release lib_3d_anim_file.obj).
C3DAnimFileRootChunk() : CChunkedFileSuperChunk("ANRT") { }
#else
C3DAnimFileRootChunk();
#endif
virtual const CCharString GetChunkDescription() const;
CCountedPointer<C3DAnimFileObjectChunk const > FindFirstObject() const;
CCountedPointer<C3DAnimFileObjectChunk> FindFirstObject();
bool HasHelpers() const;
CCountedPointer<C3DAnimFileHelpersChunk const > GetHelpers() const;
CCountedPointer<C3DAnimFileHelpersChunk> GetHelpers();
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
#ifndef FABLE_RETAIL_C3DANIMFILEROOTCHUNK_INLINE_CTOR
virtual ~C3DAnimFileRootChunk();
#endif
private:bool LoopFlag;float Duration;};
FABLE_STATIC_ASSERT_AT(21, sizeof(C3DAnimFileRootChunk)==0x34);
#endif
