// C3DMeshFileEngineMaterialChunk::CMapFilenames::CMapFilenames() -- the chunk record's name set: the dword at +4 set to 1 (the inline constructor of a `novtable` polymorphic base, so the store precedes this class's vtable store and the first string's `lea`), this class's vtable (0x0129EFBC), then the four CCharStrings at +8..+0x14 default-constructed (0x0099E4B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC5190, length 64 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0. Vtable 0x0129EFBC names the class (rtti-vtables.tsv). Shape: `this` in esi, the +4 store scheduled before the vtable store, `ret`.
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

struct __declspec(novtable) SCountedRecord_00ac5190 {   // a polymorphic base whose own vtable store is folded away: its constructor runs before the derived vtable store
    virtual void v00();
    long m_count;                                  // +0x04
    SCountedRecord_00ac5190() : m_count(1) { }
};

namespace C3DMeshFileEngineMaterialChunk {
struct CMapFilenames : SCountedRecord_00ac5190 {  // vtable 0x0129EFBC
    CCharString m_diffuse;                         // +0x08
    CCharString m_bump;                            // +0x0c
    CCharString m_specular;                        // +0x10
    CCharString m_detail;                          // +0x14
    CMapFilenames();
};
}

C3DMeshFileEngineMaterialChunk::CMapFilenames::CMapFilenames()
    : SCountedRecord_00ac5190(), m_diffuse(), m_bump(), m_specular(), m_detail()
{
}