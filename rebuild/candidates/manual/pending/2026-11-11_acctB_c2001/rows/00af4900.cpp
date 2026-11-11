// NParticleEngine::CParticleSystem::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- persist a particle system's database text: its name, Enabled, the X/Y/Z scale and ScaleParticles, then every BEGIN_COMPONENT block (type and name, the DISABLED / VISIBLE flags line, the component script up to END_COMPONENT) added through AddComponent and serialised by the new component
// Leaf: ?DatabaseSerialise@CParticleSystem@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF4900, 1282 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?DatabaseSerialise@CParticleSystem@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z;
//   both calls (0x00AEF619 / 0x00AEF91E) are in the landed CParticleEmitter::DatabaseSerialise 0x00AEF1F0, which binds this spelling.
//   The catalog label "SerializeComponents" is descriptive and wrong.
// Callees: CPersistContext::Transfer<CCharString> 0x004109A0 / <bool> 0x004045C0 / <float> 0x00410620, CCharString() 0x0099E4B0,
//   CCharString(const CCharString&) 0x0099EC30, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, operator= 0x0099EFB0,
//   CStringParser::SkipPastString 0x009B9BA0 / ReadNextItemAsQuotedString 0x009BA1D0 / ReadLineAsString 0x009BA390 /
//   ReadAsStringUntilString 0x009BA330 / Init(const char*, ..) 0x009B9530, CMemoryDataInputStream(const CCharString&) 0x00995F50,
//   CDataInputStream::ReadWhiteSpace 0x00994F60 / ReadUntilWhiteSpace 0x00994E90, CDataStream::~CDataStream 0x00993B10,
//   CParticleSystem::AddComponent 0x00AF4530, CBaseClass() 0x0099A2D0 / ~ 0x0099A2E0, CPersistContext(CStringParser&, ...) 0x009BADD0,
//   malloc / free 0x00BFEA0E / 0x00BFEA14 (the parser's comment list), the component's DatabaseSerialise (CPSCBase vtable slot 17).
// Data: "SystemName" 0x0129F4D8, "Enabled" 0x0126D58C, "ScaleX/Y/Z" 0x0129F4D0/C8/C0, "ScaleParticles" 0x0129F4B0, "BEGIN_COMPONENT"
//   0x0129F4A0, "DISABLED" 0x0129F494, "VISIBLE" 0x0129F48C, "END_COMPONENT" 0x0129F47C, "" 0x0122D70E, NullChar 0x0129AAF4, the CRC-32
//   table 0x0129A168.
// Layout: retail CParticleSystem (engine/CParticleSystem.h: Enabled +0x50 bit 1, ScaleParticles bit 7, Scale +0x60, Name +0x6C);
//   CStringParser (vtable 0x0122D874, Filename +0xC, TempCharString +0x10, Comments +0x14), CMemoryDataInputStream (0x1C).
// Shape: the name goes through the two-argument Transfer (GetDefaultVal<CCharString> builds a local and copies it out); the bool
//   members through a named bool copy with a `true` default; the component name's CRC is the inline CCharString::GetCRC with the CCRC
//   body; the flag words are compared as CCharString temporaries (the inline CCharString / CBasicString<char> operator== and Compare);
//   the component parser's constructor is the inline CStringParser(const CCharString&) body (retail calls it out of line elsewhere).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLESYSTEM_ADDCOMPONENT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN
#define FABLE_RETAIL_CCRC_CALC_BODY
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_INLINE_CTOR
#define FABLE_RETAIL_CSTRINGPARSER_READQUOTED
#define FABLE_RETAIL_CSTRINGPARSER_READLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM_READWHITESPACE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_CHARSTRING
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCRC.h"
#include "engine/CParticleEmitter.h"
#include "engine/CParticleSystem.h"
#include "engine/CPSCBase.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CMemoryDataInputStream.h"

// GetDefaultVal<CCharString> (Ego_r.pdb ??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ), inline in retail.
template <> inline CCharString FABLE_FASTCALL CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
// CBasicString<char>::Compare (Ego_r.pdb / FableWin.pdb ?Compare@?$CBasicString@D@@KIJPBD0@Z, out of line at 0x00411570): its body is
// visible in this TU and expanded inside both flag compares (0x00AF4B60..0x00AF4B94).
template <> inline long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

namespace NParticleEngine {
void CParticleSystem::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    context.Transfer("SystemName", Name);
    {bool temp = Enabled; context.Transfer("Enabled", temp, true); Enabled = temp;}
    context.Transfer("ScaleX", Scale.X, 1.0f);
    context.Transfer("ScaleY", Scale.Y, 1.0f);
    context.Transfer("ScaleZ", Scale.Z, 1.0f);
    {bool temp = ScaleParticles; context.Transfer("ScaleParticles", temp, true); ScaleParticles = temp;}
    CStringParser& parser = *context.GetPParser();
    while (parser.SkipPastString("BEGIN_COMPONENT")) {
        CCharString type;
        parser.ReadNextItemAsQuotedString(type);
        CCharString name;
        parser.ReadNextItemAsQuotedString(name);
        unsigned long nameCRC = name.GetCRC();
        CCharString flags = parser.ReadLineAsString();
        CMemoryDataInputStream flagStream(flags);
        bool enabled = true;
        bool visible = false;
        while (!flagStream.IsEOF()) {
            flagStream.ReadWhiteSpace();
            CCharString flag = flagStream.ReadUntilWhiteSpace();
            if (flag == static_cast<const CCharString&>("DISABLED"))
                enabled = false;
            if (flag == static_cast<const CCharString&>("VISIBLE"))
                visible = true;
        }
        CCharString script = parser.ReadAsStringUntilString("END_COMPONENT");
        CPSCBase& component = AddComponent(type, nameCRC, enabled, visible);
        CStringParser componentParser(script);
        CPersistContext componentContext(componentParser, 0, true, 0, 0, &script);
        component.DatabaseSerialise(componentContext, info);
    }
}
}
