// C3DAnimFileBoneMaskChunk::CreateNewInstance() const -- a new "AMSK" C3DAnimFileBoneMaskChunk passed through InitNewInstance (the chunk prototype's factory)
// Leaf: ?CreateNewInstance@C3DAnimFileBoneMaskChunk@@UBEPAVCChunkedFileChunk@@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB0730, 83 bytes, thiscall, plain `ret`.
// Identity (proven): slot 9 of vtable 0x0129E980 (RTTI .?AVC3DAnimFileBoneMaskChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass), and the
//   object it builds stores that same vtable; FableWin.pdb ?CreateNewInstance@C3DAnimFileBoneMaskChunk@@UBEPAVCChunkedFileChunk@@XZ; Ego_r.pdb member
//   EnableMask (CArray<bool>) at +0x10.
// Callees: operator new 0x00BFEA1A (0x1C bytes), CChunkedFileChunk::CChunkedFileChunk(const char*) 0x00AA7590 (literal 0x0129E8F8
//   "AMSK"), virtual InitNewInstance(CChunkedFileChunk*) const (slot 10, +0x28).
// Shape (family K052, representative 0x00AA46B0): the landed G048 factory 0x00A98AB0 with a CChunkedFileChunk base and the
//   member CArray's inline default constructor (three zero stores after the vtable store).
class CChunkedFileSuperChunk;
class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();                            // slot 0
    virtual void v01(); virtual void v02();
    virtual int IsSuperChunk() const;                        // slot 3
    virtual int IsUnknownType() const;                       // slot 4
    virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual CChunkedFileChunk* CreateNewInstance() const = 0;   // slot 9
protected:
    CChunkedFileChunk(const char* chunkType);                // 0x00AA7590
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;   // slot 10 (+0x28)
    char ChunkType[5];                                       // +0x04
    CChunkedFileSuperChunk* Parent;                          // +0x0C
};
#include "engine/CArray.h"
class C3DVertex2;
class C3DAnimFileBoneMaskChunk : public CChunkedFileChunk {
public:
    C3DAnimFileBoneMaskChunk() : CChunkedFileChunk("AMSK") { }
    virtual CChunkedFileChunk* CreateNewInstance() const;
    CArray<bool> EnableMask;                                     // +0x10
};
CChunkedFileChunk* C3DAnimFileBoneMaskChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileBoneMaskChunk);
}