// CCreatureGenerationFamilyDef::operator=(const CCreatureGenerationFamilyDef&) -- memberwise copy: CParentDefClassBase part, the Creatures vector, two dwords, the GenerationTypes vector; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C614, length 57 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886, vector<ECreatureGeneratorGenerateType>::operator= @ 0x0045C64D
// (manifest "CActiveFile::operator=" / "CActiveFile_AssignUndefinedVector4", propagated).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CCreatureGenerationFamilyDef's vtable 0x0123390C is the 5-byte `jmp 0x0045C614` right before this body, and
// the PDB layout of CCreatureGenerationFamilyDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x40.. -> retail +0x3c..; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
// the last member's source address is formed with `add edi,50h` (edi is dead afterwards) and the
// +0x4c store sinks below it -- plain source order under optimize("s").
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally as a packed polymorphic base
// whose operator= is out of line; the derived members follow at their natural alignment.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CGenerateTypeVector {            // retail std::vector<ECreatureGeneratorGenerateType>
    long* m_first;
    long* m_last;
    long* m_end;
    CGenerateTypeVector& operator=(const CGenerateTypeVector& other);   // 0x0045C64D
};

struct CCreatureGenerationFamilyDef : CParentDefClassBase { // vtable 0x0123390C
    CLongVector          Creatures;                   // +0x3c
    long                 DifficultyLevel;             // +0x48
    long                 GeneratorObject;             // +0x4c
    CGenerateTypeVector  GenerationTypes;             // +0x50
    CCreatureGenerationFamilyDef& operator=(const CCreatureGenerationFamilyDef& other);
};

CCreatureGenerationFamilyDef& CCreatureGenerationFamilyDef::operator=(const CCreatureGenerationFamilyDef& other)
{
    CParentDefClassBase::operator=(other);
    Creatures = other.Creatures;
    DifficultyLevel = other.DifficultyLevel;
    GeneratorObject = other.GeneratorObject;
    GenerationTypes = other.GenerationTypes;
    return *this;
}
