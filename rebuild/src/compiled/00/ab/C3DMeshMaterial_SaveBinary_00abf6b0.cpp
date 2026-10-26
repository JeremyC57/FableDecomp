// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: WriteLong bound to both 0x993ea0 and 0x993eb0)
// C3DMeshMaterial::SaveBinary(CDataOutputStream&) -- serialize all material fields and optional filenames.
// Retail00ABF6B0,250 bytes. No material header exists; offsets follow the landed00ABF7B0 constructor.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABF6B0, 250 bytes.
// The stream uses its shared storage header with a method-only view. Two byte-identical 4-byte writers exist:
// WriteLong(long) 0x00993EB0 (the two signed fields) and WriteULong(unsigned long) 0x00993EA0 (the bank indices, flags
// and self-illumination; its propagated label says WriteCBYTE) -- the landed lane names; 0x00993E30 writes a normalized
// boolean byte, 0x00995AE0 writes characters plus NUL.
class CCharString { public: void* m_pStringData; };
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteEBOOL(bool);   // 0x00993E30
    void WriteSLONG(long);   // 0x00993EB0
    void WriteULONG(unsigned long);   // 0x00993EA0
    void WriteNullTerminatedString(const CCharString&);   // 0x00995AE0
};
struct CDataOutputStreamMethods : CDataOutputStream {
};
struct CMaterialFilenames {
    void* __vftable; // +0: counted-object vptr, established by 00ABEF60
    long references; // +4: reference count
    CCharString Diffuse;
    CCharString Bump;
    CCharString Reflection;
    CCharString Illumination;
};
class CBaseClass { public: virtual void v00(); };
class C3DMeshMaterial : public CBaseClass { public:
    long MaterialIndex;
    CCharString Name;
    long DecalID;
    unsigned long DiffuseMapBankIndex;
    unsigned long BumpMapBankIndex;
    unsigned long ReflectionMapBankIndex;
    unsigned long IlluminationMapBankIndex;
    unsigned long MapFlags;
    unsigned long SelfIllumination;
    bool TwoSided;
    bool Transparent;
    bool BooleanAlpha;
    bool DegenerateTriangles;
    CMaterialFilenames* Filenames;
    void SaveBinary(CDataOutputStream& stream);
};
void C3DMeshMaterial::SaveBinary(CDataOutputStream& destination)
{
    CDataOutputStreamMethods& stream=(CDataOutputStreamMethods&)destination;
    stream.WriteSLONG(MaterialIndex);
    stream.WriteNullTerminatedString(Name);
    stream.WriteSLONG(DecalID);
    stream.WriteULONG(DiffuseMapBankIndex);
    stream.WriteULONG(BumpMapBankIndex);
    stream.WriteULONG(ReflectionMapBankIndex);
    stream.WriteULONG(IlluminationMapBankIndex);
    stream.WriteULONG(MapFlags);
    stream.WriteULONG(SelfIllumination);
    stream.WriteEBOOL(TwoSided);
    stream.WriteEBOOL(Transparent);
    stream.WriteEBOOL(BooleanAlpha);
    stream.WriteEBOOL(DegenerateTriangles);
    bool hasFilenames = Filenames != 0;
    stream.WriteEBOOL(hasFilenames);
    if (hasFilenames) {
        stream.WriteNullTerminatedString(Filenames->Diffuse);
        stream.WriteNullTerminatedString(Filenames->Bump);
        stream.WriteNullTerminatedString(Filenames->Reflection);
        stream.WriteNullTerminatedString(Filenames->Illumination);
    }
}