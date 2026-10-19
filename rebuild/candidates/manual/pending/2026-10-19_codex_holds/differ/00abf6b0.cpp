// Grade: HELD(bytes match retail) -- WriteLong bound to both 0x993ea0 and 0x993eb0 | Codex source 00/ab/C3DMeshMaterial_SaveBinary_00abf6b0.cpp, flags , pass pattern 00abf6b0_TEST PASS
// C3DMeshMaterial::SaveBinary(CDataOutputStream&) -- serialize all material fields and optional filenames.
// Retail00ABF6B0,250 bytes. No material header exists; offsets follow the landed00ABF7B0 constructor.
// The stream uses its shared storage header with a method-only view.00993EA0 writes4 bytes despite its
// propagated WriteCBYTE label;00993E30 writes a normalized boolean byte,00995AE0 writes characters plus NUL.
struct CCharString { void* m_pStringData; };
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamMethods : CDataOutputStream {
    void WriteLong(long value); // 0x00993EA0
    void WriteEBOOL(bool value); // 0x00993E30
    void WriteNullTerminatedString(const CCharString& value); // 0x00995AE0
};
struct CMaterialFilenames {
    void* __vftable; // +0: counted-object vptr, established by 00ABEF60
    long references; // +4: reference count
    CCharString Diffuse;
    CCharString Bump;
    CCharString Reflection;
    CCharString Illumination;
};
struct CBaseClass { virtual void v00(); };
struct C3DMeshMaterial : CBaseClass {
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
    stream.WriteLong(MaterialIndex);
    stream.WriteNullTerminatedString(Name);
    stream.WriteLong(DecalID);
    stream.WriteLong(DiffuseMapBankIndex);
    stream.WriteLong(BumpMapBankIndex);
    stream.WriteLong(ReflectionMapBankIndex);
    stream.WriteLong(IlluminationMapBankIndex);
    stream.WriteLong(MapFlags);
    stream.WriteLong(SelfIllumination);
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