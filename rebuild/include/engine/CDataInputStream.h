#include "fable_compiler.h"
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
#include "engine/CDataStream.h"
#include <string.h>
#pragma intrinsic(memcpy)
class CCharString;
class CWideString;
class CRGBColour;
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READDATEANDTIME
class CDateAndTime;
#endif
class C3DVector;
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR
class C2DVector;
#endif
// Retail opt-in (Codex pass 11 consolidation): polymorphic CDataStream base (vptr +0), original CDataInputStream fields
// +4,+8,+c,+10,+14, size 0x18; Read is the inline bounds-checked chunk copy (0x00B3EFA0, 0x00A4FE60).
class CDataInputStream : public CDataStream {
public:
    CDataInputStream();
#ifdef FABLE_RETAIL_ENTITY_LOAD_BINARY_PASS30
    virtual ~CDataInputStream();
#endif

    // retry11a: no user-declared destructor in retail -- every CMemoryDataInputStream / CDataInputStream local is
    // destroyed by a direct call to ~CDataStream 0x00993B10 (stores the CDataStream vtable 0x0129A69C, then jumps to
    // ~CBaseClassNonCopyable 0x0099A300) with no derived vtable store first (0x00520BFB, 0x00520C3F): both derived
    // destructors are compiler-generated.
    virtual void Close();
    unsigned long GetLength() const { return StreamSize; }
    virtual unsigned long GetPosition() const { return StreamPos; }
    virtual void SetPosition(unsigned long);
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READPIXELFORMAT
    // NF205 opt-in (q11815, row 0x009FD160 CGraphicDataBank::CGraphicHeader::Load): Ego_r.pdb / FableWin.pdb
    // ?ReadPixelFormat@CDataInputStream@@QAE?AVCPixelFormat@@XZ (public; out of line in retail at 0x00994670, the CPixelFormat
    // result by hidden pointer). Declaration only; no layout change.
    class CPixelFormat ReadPixelFormat();                                  // 0x00994670
#endif
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
    // Retail opt-in (Codex pass22): original FableWin module548 Read locals dst,
    // no_bytes and overflow_check; donor01876B85 guards the whole body with >0,
    // and01876CCF exits only the invalid-position arm. Updates01876F43..6D.
    FABLE_FORCEINLINE void Read(void* dst,long no_bytes) {
        if(no_bytes>0) {
            __int64 overflow_check=(__int64)(StreamPos+no_bytes);
            if(overflow_check<0 || overflow_check>0x7fffffff)return;
            if(no_bytes<=SourceChunkBytesRemaining) {
                memcpy(dst,CurrentSourcePtr,no_bytes);
                CurrentSourcePtr+=no_bytes;
                SourceChunkBytesRemaining-=no_bytes;
                StreamPos+=no_bytes;
            } else ReadWithSrcChunkOverflow((unsigned char*)dst,no_bytes);
        }
    }
#else
    FABLE_FORCEINLINE void Read(void* buffer,long size) {
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
#endif
    template<class T> FABLE_FORCEINLINE T ReadType() { T val; Read(&val,sizeof(T)); return val; }
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_LOADGAME_PASS24
    // Retail opt-in (Codex pass 24): FableWin ReadUHUGE and ReadType<unsigned __int64>; two dword reads in 0x005253F0.
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_UID_EXPAND_PASS24
    // Inline-budget: native 00525667 expands the UID reader through ReadType, leaving only the chunk-overflow slow path.
    FABLE_FORCEINLINE
#endif
    unsigned __int64 ReadUHUGE() { return ReadType<unsigned __int64>(); }
    CCharString ReadPresizedString(); // Both PDBs and the call to 0x00996390.
#endif
    FABLE_FORCEINLINE unsigned long ReadULONG() { return ReadType<unsigned long>(); }
    FABLE_FORCEINLINE long ReadSLONG() { return ReadType<long>(); }
    // Chunk 779 (row 0x00AAD650 CWaveLoader::LoadWaveFile): FableWin.pdb ?ReadUWORD@CDataInputStream@@QAEGXZ, ?ReadSWORD@..@@QAEFXZ and
    // ?ReadUBYTE@..@@QAEEXZ (Ego_r keeps only the ReadType<G/F/E> instances they wrap), expanded in retail like ReadULONG / ReadSLONG.
    FABLE_FORCEINLINE unsigned short ReadUWORD() { return ReadType<unsigned short>(); }
    FABLE_FORCEINLINE short ReadSWORD() { return ReadType<short>(); }
    FABLE_FORCEINLINE unsigned char ReadUBYTE() { return ReadType<unsigned char>(); }
    // Chunk 737 (CPSCLight / CPSCSingleSprite::ReadBinary 0x00B193D0 / 0x00B068F0): FableWin.pdb ?ReadFloat@CDataInputStream@@QAEMXZ
    // and ?ReadEBOOL@CDataInputStream@@QAE_NXZ, expanded in retail (a 4-byte read stored as a float; a 1-byte read, `test ; setne`,
    // as the landed CPSCAttractor::ReadBinary 0x00B18390 model's ReadBool); ?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ
    // stays out of line (0x009945E0, hidden result pointer).
    FABLE_FORCEINLINE float ReadFloat() { return ReadType<float>(); }
    FABLE_FORCEINLINE bool ReadEBOOL() { return ReadType<unsigned char>() != 0; }
    CRGBColour ReadColour();
    // Chunk 739 (CPSCUpdateNormal::ReadBinary 0x00B0B860): FableWin.pdb ?Read3DVector@CDataInputStream@@QAE?AVC3DVector@@XZ, out of
    // line in retail (0x009950F0, hidden result pointer; the landed CDataInputStream_Read3DVector_009950f0 row).
    C3DVector Read3DVector();
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READDATEANDTIME
    // Nested opt-in (row 0x00A60D40 CChecksumCacheFile::Open): Ego_r.pdb / FableWin.pdb ?ReadDateAndTime@CDataInputStream@@QAE?AVCDateAndTime@@XZ,
    // out of line in retail (0x00995770, hidden result pointer; the landed CDataInputStream_ReadDateAndTime_00995770 row).
    CDateAndTime ReadDateAndTime();                                  // 0x00995770
#endif
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR
    // Nested opt-in (chunk 1018, row 0x00475DFA GFSerialiseVectorBinaryIn<C2DVector>): Ego_r.pdb / FableWin.pdb
    // ?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ, out of line in retail (0x00995030, hidden result pointer:
    // `lea eax,[ebp-10h] ; push eax ; mov ecx,ebx ; call 0x00995030` at 0x00475E70).
    C2DVector Read2DVector();
#endif
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
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_READWHITESPACE
    // Opt-in (chunk 2001, row 0x00AF4900): Ego_r.pdb ?ReadWhiteSpace@CDataInputStream@@QAE?AVCCharString@@XZ (0x00994F60; the component
    // flag reader drops the returned run of white space). Declaration only.
    CCharString ReadWhiteSpace();                                    // 0x00994F60
#endif
    bool IsEOF() const { return StreamPos == GetLength(); }
#endif
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_COPYTO
    // Opt-in (wfA6, row 0x00AE3C30 CParticleProgram::ReadFromFile): Ego_r.pdb ?CopyTo@CDataInputStream@@QAEXAAVCDataOutputStream@@K@Z,
    // out of line in retail (0x009964E0; the reader passes its Constraints memory stream and the byte count just read). Decl only.
    void CopyTo(class CDataOutputStream& out, unsigned long length);  // 0x009964E0
#endif
    virtual void ReadCompressed(void*,long);
    virtual void ReadZlibCompressed(void*,long);
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString();
protected:
#ifdef FABLE_RETAIL_CDATAINPUTSTREAM_CHUNKSEEK
    // Retail opt-in (Codex pass17): PDB protected helpers; donor03080e90/03080b20.
    bool CanSeekToPosWithinSourceChunk(long pos) {
        return (unsigned long)pos>=SourceChunkStreamPos && (unsigned long)pos<=StreamPos+SourceChunkBytesRemaining;
    }
    void SetPositionAssumeWithinSourceChunk(unsigned long pos) {
        CurrentSourcePtr+=(long)(pos-StreamPos);
        SourceChunkBytesRemaining+=(long)(StreamPos-pos);
        StreamPos=pos;
    }
#else
    void SetPositionAssumeWithinSourceChunk(unsigned long);
#endif
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
FABLE_STATIC_ASSERT_AT(121, sizeof(CDataInputStream)==0x18);
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

FABLE_STATIC_ASSERT_AT(151, sizeof(CDataInputStream) == 0x18);
FABLE_STATIC_ASSERT_AT(152, offsetof(CDataInputStream, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(153, offsetof(CDataInputStream, StreamPos) == 0x4);
FABLE_STATIC_ASSERT_AT(154, offsetof(CDataInputStream, StreamSize) == 0x8);
FABLE_STATIC_ASSERT_AT(155, offsetof(CDataInputStream, CurrentSourcePtr) == 0xc);
FABLE_STATIC_ASSERT_AT(156, offsetof(CDataInputStream, SourceChunkStreamPos) == 0x10);
FABLE_STATIC_ASSERT_AT(157, offsetof(CDataInputStream, SourceChunkBytesRemaining) == 0x14);

#endif // FABLE_ENGINE_CDATAINPUTSTREAM_H

#endif
