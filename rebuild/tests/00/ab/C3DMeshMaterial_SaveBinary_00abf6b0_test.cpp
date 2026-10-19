// Behaviour test for retail 0x00abf6b0: the candidate source below is embedded verbatim (from src/00abf6b0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// C3DMeshMaterial::SaveBinary(CDataOutputStream&) -- serialize all material fields and optional filenames.
// Retail00ABF6B0,250 bytes. No material header exists; offsets follow the landed00ABF7B0 constructor.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABF6B0, 250 bytes.
// The stream uses its shared storage header with a method-only view. Two byte-identical 4-byte writers exist:
// WriteLong(long) 0x00993EB0 (the two signed fields) and WriteULong(unsigned long) 0x00993EA0 (the bank indices, flags
// and self-illumination; its propagated label says WriteCBYTE) -- the landed lane names; 0x00993E30 writes a normalized
// boolean byte, 0x00995AE0 writes characters plus NUL.
struct CCharString { void* m_pStringData; };
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamMethods : CDataOutputStream {
    void WriteLong(long value); // 0x00993EB0
    void WriteULong(unsigned long value); // 0x00993EA0
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
    stream.WriteULong(DiffuseMapBankIndex);
    stream.WriteULong(BumpMapBankIndex);
    stream.WriteULong(ReflectionMapBankIndex);
    stream.WriteULong(IlluminationMapBankIndex);
    stream.WriteULong(MapFlags);
    stream.WriteULong(SelfIllumination);
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
// ===== harness =====
#include <cstdio>
#include <cstring>
static unsigned char outputBytes[512];
static unsigned long outputLength=0;
static int g_signed, g_unsigned;
struct CCharStringFixtureData { const char* text; long length; };
static CDataOutputStreamMethods* expectedStream;
static bool wrongStream;
void CBaseClass::v00() {}
void CDataOutputStreamMethods::WriteLong(long value)
{
    if(this!=expectedStream) wrongStream=true;
    ++g_signed;
    unsigned long v=(unsigned long)value;
    outputBytes[outputLength++]=(unsigned char)v;
    outputBytes[outputLength++]=(unsigned char)(v>>8);
    outputBytes[outputLength++]=(unsigned char)(v>>16);
    outputBytes[outputLength++]=(unsigned char)(v>>24);
}

void CDataOutputStreamMethods::WriteULong(unsigned long v)
{
    if(this!=expectedStream) wrongStream=true;
    ++g_unsigned;
    outputBytes[outputLength++]=(unsigned char)v;
    outputBytes[outputLength++]=(unsigned char)(v>>8);
    outputBytes[outputLength++]=(unsigned char)(v>>16);
    outputBytes[outputLength++]=(unsigned char)(v>>24);
}
void CDataOutputStreamMethods::WriteEBOOL(bool value)
{
    if(this!=expectedStream) wrongStream=true;
    outputBytes[outputLength++]=(unsigned char)(value?1:0);
}
void CDataOutputStreamMethods::WriteNullTerminatedString(const CCharString& value)
{
    if(this!=expectedStream) wrongStream=true;
    const CCharStringFixtureData* d=(const CCharStringFixtureData*)value.m_pStringData;
    const char* chars=d?d->text:""; long length=d?d->length:0;
    for(long i=0;i<=length;++i) outputBytes[outputLength++]=(unsigned char)chars[i];
}
static int fail(const char* name) { std::printf("00abf6b0_TEST FAIL: %s\n",name); return 1; }
static void expectedLong(unsigned char* b,unsigned long& n,unsigned long v)
{
    b[n++]=(unsigned char)v; b[n++]=(unsigned char)(v>>8); b[n++]=(unsigned char)(v>>16); b[n++]=(unsigned char)(v>>24);
}
static void expectedByte(unsigned char* b,unsigned long& n,unsigned char v) { b[n++]=v; }
static void expectedString(unsigned char* b,unsigned long& n,const char* s)
{
    if (!s) s="";
    do { b[n++]=(unsigned char)*s; } while (*s++);
}
int main()
{
    CDataOutputStreamMethods stream;
    expectedStream=&stream; wrongStream=false;
    CCharStringFixtureData text[5]={{"body",4},{"diffuse",7},{"bump",4},{"reflect",7},{"illum",5}};
    C3DMeshMaterial material;
    material.MaterialIndex=-3;
    material.Name.m_pStringData=&text[0];
    material.DecalID=13;
    material.DiffuseMapBankIndex=0x11223344;
    material.BumpMapBankIndex=0x55667788;
    material.ReflectionMapBankIndex=0x10203040;
    material.IlluminationMapBankIndex=0x50607080;
    material.MapFlags=0xabcdef01;
    material.SelfIllumination=0x01020304;
    material.TwoSided=true; material.Transparent=false;
    material.BooleanAlpha=true; material.DegenerateTriangles=true;
    CMaterialFilenames names;
    names.Diffuse.m_pStringData=&text[1];
    names.Bump.m_pStringData=&text[2];
    names.Reflection.m_pStringData=&text[3];
    names.Illumination.m_pStringData=&text[4];
    material.Filenames=&names;
    outputLength=0;
    material.SaveBinary(stream);
    unsigned char expected[512]; unsigned long n=0;
    expectedLong(expected,n,(unsigned long)-3);
    expectedString(expected,n,"body");
    expectedLong(expected,n,13);
    expectedLong(expected,n,0x11223344); expectedLong(expected,n,0x55667788);
    expectedLong(expected,n,0x10203040); expectedLong(expected,n,0x50607080);
    expectedLong(expected,n,0xabcdef01); expectedLong(expected,n,0x01020304);
    expectedByte(expected,n,1); expectedByte(expected,n,0);
    expectedByte(expected,n,1); expectedByte(expected,n,1); expectedByte(expected,n,1);
    expectedString(expected,n,"diffuse"); expectedString(expected,n,"bump");
    expectedString(expected,n,"reflect"); expectedString(expected,n,"illum");
    if (wrongStream || outputLength!=n || std::memcmp(outputBytes,expected,n)!=0) return fail("present filename serialization order");
    material.Filenames=0;
    outputLength=0;
    material.SaveBinary(stream);
    n=0;
    expectedLong(expected,n,(unsigned long)-3);
    expectedString(expected,n,"body");
    expectedLong(expected,n,13);
    expectedLong(expected,n,0x11223344); expectedLong(expected,n,0x55667788);
    expectedLong(expected,n,0x10203040); expectedLong(expected,n,0x50607080);
    expectedLong(expected,n,0xabcdef01); expectedLong(expected,n,0x01020304);
    expectedByte(expected,n,1); expectedByte(expected,n,0);
    expectedByte(expected,n,1); expectedByte(expected,n,1); expectedByte(expected,n,0);
    if (wrongStream || outputLength!=n || std::memcmp(outputBytes,expected,n)!=0) return fail("absent filename branch");
    material.Name.m_pStringData=0;
    material.TwoSided=false; material.Transparent=true; material.BooleanAlpha=false; material.DegenerateTriangles=false;
    outputLength=0; material.SaveBinary(stream);
    // The original four-character name shrinks to a single NUL; remaining scalar fields stay in order.
    unsigned char compact[512]; unsigned long compactLength=n-4;
    std::memcpy(compact,expected,4); compact[4]=0;
    std::memcpy(compact+5,expected+9,n-9);
    compact[compactLength-5]=0;compact[compactLength-4]=1;compact[compactLength-3]=0;compact[compactLength-2]=0;
    if(wrongStream || outputLength!=compactLength || std::memcmp(outputBytes,compact,compactLength)) return fail("null name/opposite flags");
    if (g_signed != 6 || g_unsigned != 18) return fail("signed/unsigned writer split (2 + 6 per call)");
    std::printf("00abf6b0_TEST PASS\n");
    return 0;
}