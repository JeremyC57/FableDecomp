# h24_crc.py: naming wave 24, family crc -- the chunk 691 CCRCSymbolMap rows (labels_chunk691; the landed LoadFromFile 0x00A39010 binds
# them): ParseBuffer 0x00A38E50 ?ParseBuffer@CCRCSymbolMap@@IAE_NPBDABVCCharString@@@Z, ParseEnum 0x00A38670
# ?ParseEnum@CCRCSymbolMap@@IAE_NAAVCStringParser@@ABVCCharString@@@Z, Error 0x00A381D0 ?Error@CCRCSymbolMap@@IAAXABVCWideString@@JPBDZZ
# (all three protected in Ego_r.pdb / FableWin.pdb), the static CCRCSymbolMap::CurrentFilename 0x013D27E8 (Ego_r dynamic initializer) and
# CVectorMap<unsigned long, long, CKeyPairCompareLess<...> >::DeallocateMemory 0x00A39900 (Ego_r; called by CCRCSymbolMap::Clear 0x00A38550).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he

# ---- 0x00A381D0 CCRCSymbolMap::Error (protected cdecl varargs member on CWideString; was on a CRetailWideString view) ----
he.put('00a381d0', 'src', '''// CCRCSymbolMap::Error(const CWideString&,long,const char*,...) -- build and release a compiler-style diagnostic
// Leaf: ?Error@CCRCSymbolMap@@IAAXABVCWideString@@JPBDZZ
// Retail0x00A381D0,263 bytes. Native member varargs ABI puts this on stack. Buffers are line[12] and message[256].
// Literal0x0129C14C="(%d)",0x0129C138=" : error C2065 : ",0x0122D70E="". /GS cookie is present.
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?Error@CCRCSymbolMap@@IAAXABVCWideString@@JPBDZZ (protected; labels_chunk691) on the
//   engine CWideString, whose 0x0099B2C0 is ?ToCharString@CWideString@@QBE?AVCCharString@@XZ (both PDBs; DECISIONS wave 1). The row had
//   modelled the file name as a CRetailWideString view with a ConvertToCharString member. fable_string.h's CWideString does not declare
//   ToCharString, so the two string classes are declared here (same members as fable_string.h).
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / operator= 0x0099EFB0, CWideString::ToCharString
//   0x0099B2C0, operator+(const CCharString&, const CCharString&) 0x0099F570 / (const CCharString&, const char*) 0x0099F600 (__fastcall),
//   sprintf / vsprintf.
// Flags: /GS
#include <stdio.h>
#include <stdarg.h>
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
private:
    void* storage_;
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);         // 0x0099F600
class CWideString {
public:
    CCharString ToCharString() const;                   // 0x0099B2C0
private:
    void* storage_;
};
class CCRCSymbolMap {
protected:
    void Error(const CWideString& file, long line, const char* format, ...);
};
void CCRCSymbolMap::Error(const CWideString& file,long line,const char* format,...) {
    CCharString result;
    char lineText[12]; char message[256];
    sprintf(lineText,"(%d)",line+1);
    va_list args; va_start(args,format); vsprintf(message,format,args);va_end(args);
    result=CCharString("",-1)+file.ToCharString()+lineText+" : error C2065 : "+message;
}''')

# ---- 0x00A38670 CCRCSymbolMap::ParseEnum(CStringParser&, const CCharString&) ----
A = '00a38670'
he.edit(A, 'src', '// CCRCSymbolMap::ParseEnum(CStringParser* parser, const CCharString& wantedName) -- parse', '// CCRCSymbolMap::ParseEnum(CStringParser& parser, const CCharString& wantedName) -- parse')
he.edit(A, 'src', '// Leaf: ParseEnum\n', '''// Leaf: ?ParseEnum@CCRCSymbolMap@@IAE_NAAVCStringParser@@ABVCCharString@@@Z
// Naming (naming wave 24, labels_chunk691): Ego_r.pdb / FableWin.pdb ?ParseEnum@CCRCSymbolMap@@IAE_NAAVCStringParser@@ABVCCharString@@@Z
//   (protected, the parser by reference); Error is ?Error@CCRCSymbolMap@@IAAXABVCWideString@@JPBDZZ (protected) and the file name the
//   static CCRCSymbolMap::CurrentFilename 0x013D27E8 (Ego_r dynamic initializer; LoadFromFile 0x00A39010 writes it). Was a public
//   pointer-parameter ParseEnum with a CRetailWideString g_CRCSymbolFileName.
''')
he.edit(A, 'src', '''struct CRetailWideString { void* PStringData; };
extern CRetailWideString g_CRCSymbolFileName;          // 0x013D27E8
''', '''class CWideString { public: void* PStringData; };
''')
he.edit(A, 'src', '''class CCRCSymbolMap { public:
    void Error(const CRetailWideString& file, long line, const char* format, ...);   // 0x00A381D0
    void AddSymbolDirty(const CCharString& name, long value);                       // 0x00A385A0
    bool ParseEnum(CStringParser* parser, const CCharString& wantedName);
};

bool CCRCSymbolMap::ParseEnum(CStringParser* parser, const CCharString& wantedName)''', '''class CCRCSymbolMap { public:
    static CWideString CurrentFilename;                                             // 0x013D27E8
    void AddSymbolDirty(const CCharString& name, long value);                       // 0x00A385A0
protected:
    void Error(const CWideString& file, long line, const char* format, ...);        // 0x00A381D0
    bool ParseEnum(CStringParser& parser, const CCharString& wantedName);
};

bool CCRCSymbolMap::ParseEnum(CStringParser& parser, const CCharString& wantedName)''')
he.edit_all(A, 'src', 'parser->', 'parser.')
he.edit(A, 'src', 'CPersistContext context(parser, 0, true, 0, 0, 0);', 'CPersistContext context(&parser, 0, true, 0, 0, 0);')
he.edit(A, 'src', 'TransferInLong_009e2590(&context, parser, &read);', 'TransferInLong_009e2590(&context, &parser, &read);')
he.edit_all(A, 'src', 'Error(g_CRCSymbolFileName,', 'Error(CurrentFilename,')

# ---- 0x00A38E50 CCRCSymbolMap::ParseBuffer(const char*, const CCharString&) ----
A = '00a38e50'
he.edit(A, 'src', '// CCRCSymbolMap::ParseBuffer(char* buffer, long context) -- parse', '// CCRCSymbolMap::ParseBuffer(const char* buffer, const CCharString& wantedName) -- parse')
he.edit(A, 'src', '// Leaf: ParseBuffer\n', '''// Leaf: ?ParseBuffer@CCRCSymbolMap@@IAE_NPBDABVCCharString@@@Z
// Naming (naming wave 24, labels_chunk691): Ego_r.pdb / FableWin.pdb ?ParseBuffer@CCRCSymbolMap@@IAE_NPBDABVCCharString@@@Z (protected;
//   the second argument, the 4-byte value kept in esi, is the wanted enum name handed on to ParseEnum) and its callee
//   ?ParseEnum@CCRCSymbolMap@@IAE_NAAVCStringParser@@ABVCCharString@@@Z (the parser by reference). The parser text is const char*
//   (the CStringParser::Init spelling the landed 0x009ADE90 already binds: PBD).
''')
he.edit(A, 'src', '''    CStringParser(char* text, TErrorCallback callback, const CCharString& filename, long baseLine)''', '''    CStringParser(const char* text, TErrorCallback callback, const CCharString& filename, long baseLine)''')
he.edit(A, 'src', '''    void Init(char* text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530''', '''    void Init(const char* text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530''')
he.edit(A, 'src', '''class CCRCSymbolMap { public:
    bool ParseEnum(CStringParser* parser, long context);   // 0x00A38670
    bool ParseBuffer(char* buffer, long context);
};

bool CCRCSymbolMap::ParseBuffer(char* buffer, long context)''', '''class CCRCSymbolMap {
protected:
    bool ParseEnum(CStringParser& parser, const CCharString& wantedName);   // 0x00A38670
    bool ParseBuffer(const char* buffer, const CCharString& wantedName);
};

bool CCRCSymbolMap::ParseBuffer(const char* buffer, const CCharString& wantedName)''')
he.edit(A, 'src', '        if (!ParseEnum(&parser, context))', '        if (!ParseEnum(parser, wantedName))')

# ---- 0x00A38550 CCRCSymbolMap::Clear: the vector-map clear is CVectorMap::DeallocateMemory ----
A = '00a38550'
he.edit(A, 'src', '// Callees: CVectorMap::Clear @ 0x00A39900 (out of line)', '// Callees: CVectorMap::DeallocateMemory @ 0x00A39900 (out of line; Ego_r ?DeallocateMemory@?$CVectorMap@KJV?$CKeyPairCompareLess@KJ@@@@QAEXXZ,\n//   labels_chunk691 -- naming wave 24, was a CVectorMap::Clear not in the PDB)')
he.edit(A, 'src', '    void Clear();                                        // out of line\n};\ntemplate <class K, class V> class CKeyPairCompareLess', '    void DeallocateMemory();                             // out of line\n};\ntemplate <class K, class V> class CKeyPairCompareLess')
he.edit(A, 'src', '    SLONGMap.Clear();\n', '    SLONGMap.DeallocateMemory();\n')
