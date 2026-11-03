// C3DMeshFileMaterialListChunk::C3DMeshFileMaterialListChunk() -- base CChunkedFileSuperChunk("MTLS") then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFB90, oracle length 23 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x129e074 ; mov esi,ecx ; call CChunkedFileSuperChunk::CChunkedFileSuperChunk ; mov dword ptr [esi],offset C3DMeshFileMaterialListChunk::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor that passes the constant 19521652 (0x129e074) to the base constructor @ 0x00AA8360
// (an identifier/type code of this class in the base's scheme) and installs this class's vtable
// (0x0129EDE0, named through rtti-vtables.tsv). One of 11 bodies of this shape.
class CChunkedFileSuperChunk { public:        // RTTI direct base (was the stand-in CChunkedFileSuperChunk)
    virtual void v00();
protected:
    CChunkedFileSuperChunk(const char* type);  // 0x00AA8360 (Ego_r / FableWin protected ??0CChunkedFileSuperChunk@@IAE@PBD@Z)
public:
};

class C3DMeshFileMaterialListChunk : public CChunkedFileSuperChunk { public:
    virtual void v00();
    C3DMeshFileMaterialListChunk();
};

C3DMeshFileMaterialListChunk::C3DMeshFileMaterialListChunk()
    : CChunkedFileSuperChunk("MTLS")
{
}