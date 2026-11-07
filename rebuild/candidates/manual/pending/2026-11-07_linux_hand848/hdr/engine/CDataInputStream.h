#ifdef FABLE_RETAIL_CDATAINPUTSTREAM
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
#include "engine/CDataStream.h"
#include <string.h>
#pragma intrinsic(memcpy)
class CCharString;
class CWideString;
class CRGBColour;
class C3DVector;
// Retail opt-in (Codex pass 11 consolidation): polymorphic CDataStream base (vptr +0), original CDataInputStream fields
// +4,+8,+c,+10,+14, size 0x18; Read is the inline bounds-checked chunk copy (0x00B3EFA0, 0x00A4FE60).
class CDataInputStream : public CDataStream {
public:
    CDataInputStream();
    // retry11a: no user-declared destructor in retail -- every CMemoryDataInputStream / CDataInputStream local is
    // destroyed by a direct call to ~CDataStream 0x00993B10 (stores the CDataStream vtable 0x0129A69C, then jumps to
    // ~CBaseClassNonCopyable 0x0099A300) with no derived vtable store first (0x00520BFB, 0x00520C3F): both derived
    // destructors are compiler-generated.
    virtual void Close();
    unsigned long GetLength() const { return StreamSize; }
    virtual unsigned long GetPosition() const { return StreamPos; }
    virtual void SetPosition(unsigned long);
    __forceinline void Read(void* buffer,long size) {
        if(size<=0)return;
        __int64 newPosition=(__int64)(StreamPos+size);
        if(newPosition<0 || newPosition>0x7fffffff)return;
        if(size<=SourceChunkBytesRemaining) {
            memcpy(buffer,CurrentSourcePtr,size);
            CurrentSourcePtr+=size;
            SourceChunkBytesRemaining-=size;
            StreamPos+=size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer,size);
    }
    template<class T> __forceinline T ReadType() { T val; Read(&val,sizeof(T)); return val; }
    __forceinline unsigned long ReadULONG() { return ReadType<unsigned long>(); }
    __forceinline long ReadSLONG() { return ReadType<long>(); }
    // Chunk 779 (row 0x00AAD650 CWaveLoader::LoadWaveFile): FableWin.pdb ?ReadUWORD@CDataInputStream@@QAEGXZ, ?ReadSWORD@..@@QAEFXZ and
    // ?ReadUBYTE@..@@QAEEXZ (Ego_r keeps only the ReadType<G/F/E> instances they wrap), expanded in retail like ReadULONG / ReadSLONG.
    __forceinline unsigned short ReadUWORD() { return ReadType<unsigned short>(); }
    __forceinline short ReadSWORD() { return ReadType<short>(); }
    __forceinline unsigned char ReadUBYTE() { return ReadType<unsigned char>(); }
    // Chunk 737 (CPSCLight / CPSCSingleSprite::ReadBinary 0x00B193D0 / 0x00B068F0): FableWin.pdb ?ReadFloat@CDataInputStream@@QAEMXZ
    // and ?ReadEBOOL@CDataInputStream@@QAE_NXZ, expanded in retail (a 4-byte read stored as a float; a 1-byte read, `test ; setne`,
    // as the landed CPSCAttractor::ReadBinary 0x00B18390 model's ReadBool); ?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ
    // stays out of line (0x009945E0, hidden result pointer).
    __forceinline float ReadFloat() { return ReadType<float>(); }
    __forceinline bool ReadEBOOL() { return ReadType<unsigned char>() != 0; }
    CRGBColour ReadColour();
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READPRESIZED
    // Nested opt-in (chunk 848, row 0x005253F0 CThingManager::LoadGameState): Ego_r.pdb ?ReadPresizedString@CDataInputStream@@QAE?AVCCharString@@XZ
    // (retail 0x00996390, hidden result pointer), out of line. Declaration only.
    CCharString ReadPresizedString();                                // 0x00996390
#endif
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READUHUGE
    // Nested opt-in (chunk 848, row 0x005253F0 CThingManager::LoadGameState): FableWin.pdb ?ReadUHUGE@CDataInputStream@@QAE_KXZ
    // (0x01A627C0 in FableWin) wraps ??$ReadType@_K@CDataInputStream@@QAE_KXZ; expanded in retail like ReadULONG (an 8-byte read).
    __forceinline unsigned __int64 ReadUHUGE() { return ReadType<unsigned __int64>(); }
#endif
    // Chunk 739 (CPSCUpdateNormal::ReadBinary 0x00B0B860): FableWin.pdb ?Read3DVector@CDataInputStream@@QAE?AVC3DVector@@XZ, out of
    // line in retail (0x009950F0, hidden result pointer; the landed CDataInputStream_Read3DVector_009950f0 row).
    C3DVector Read3DVector();
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READSTRING
    // Nested opt-in (chunk 779, row 0x00AAD650 CWaveLoader::LoadWaveFile): Ego_r.pdb ?ReadString@CDataInputStream@@QAE?AVCCharString@@K@Z
    // (retail 0x00996BE0, hidden result pointer; bound by the landed 0x005DBF50), out of line in retail.
    CCharString ReadString(unsigned long length);                    // 0x00996BE0
#endif
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
    // Nested opt-in (chunk 775, row 0x009A8170 CBankFileManager::OpenIniFile): Ego_r.pdb ?ReadLine@CDataInputStream@@QAE?AVCCharString@@XZ
    // (0x00994B50), ?ReadUntilCharacter@CDataInputStream@@QAE?AVCCharString@@DAA_N@Z (0x00994DC0) and
    // ?ReadUntilWhiteSpace@CDataInputStream@@QAE?AVCCharString@@XZ (0x00994E90), all out of line. FableWin.pdb
    // ?IsEOF@CDataInputStream@@QBE_NXZ is inline: the fields compared in place (0x009A823A: StreamPos +4 against StreamSize +8);
    // going through the virtual GetPosition() keeps a vtable call there.
    CCharString ReadLine();                                          // 0x00994B50
    CCharString ReadUntilCharacter(char character, bool& found);     // 0x00994DC0
    CCharString ReadUntilWhiteSpace();                               // 0x00994E90
    bool IsEOF() const { return StreamPos == GetLength(); }
#endif
    virtual void ReadCompressed(void*,long);
    virtual void ReadZlibCompressed(void*,long);
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString();
protected:
    void SetPositionAssumeWithinSourceChunk(unsigned long);
private:
    void ReadWithSrcChunkOverflow(unsigned char*,long);
    virtual void InternalReadNextSourceChunk(const unsigned char*&,long&);
    virtual bool NeedsBufferedRead(long);
    virtual void InternalReadUnbuffered(unsigned char*,long);
    unsigned long StreamPos;
    long StreamSize;
    const unsigned char* CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long SourceChunkBytesRemaining;
};
FABLE_STATIC_ASSERT(sizeof(CDataInputStream)==0x18);
#endif
#else
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDataInputStream = 0x18.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CDataInputStream {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long StreamPos;                                 // +0x04
    long          StreamSize;                                // +0x08
    void*         CurrentSourcePtr;                          // +0x0c uchar *
    unsigned long SourceChunkStreamPos;                      // +0x10
    long          SourceChunkBytesRemaining;                 // +0x14
#ifdef FABLE_CDATAINPUTSTREAM_FRIEND
    friend FABLE_CDATAINPUTSTREAM_FRIEND;                   // the including row's inline-helper class
#endif
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size); // 0x00993CA0 ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CDataInputStream) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, StreamPos) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, StreamSize) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, CurrentSourcePtr) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SourceChunkStreamPos) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SourceChunkBytesRemaining) == 0x14);

#endif // FABLE_ENGINE_CDATAINPUTSTREAM_H

#endif
