// CPersistContextReadSection::CPersistContextReadSection() -- default-constructs the section's input stream, parser and text
// Leaf: ??0CPersistContextReadSection@@QAE@XZ
// Retail: Fable.exe @ 0x00485CD2, 31 bytes, returns this. Ego_r.pdb members: Context (CPersistContext, +0x00, no ctor code),
// InputStream (CMemoryDataInputStream, +0x2c, ctor 0x00485400), Parser (CStringParser, +0x48, ctor 0x00485C90; retail
// 0x28 bytes, Ego_r.pdb 48), Text (CCharString, +0x70 retail, ctor 0x0099E4B0; Ego_r.pdb +0x78).
// Layout: no engine header for this class; modelled from Ego_r.pdb with the retail CStringParser size.
class CPersistContext {
public:
    unsigned char Data[0x2c];                            // Ego_r.pdb 44 bytes, no constructor code here
};
class CMemoryDataInputStream {
public:
    unsigned char Data[0x1c];
    CMemoryDataInputStream();                            // 0x00485400
};
class CStringParser {
public:
    unsigned char Data[0x28];
    CStringParser();                                     // 0x00485C90
};
class CCharString {
public:
    void* PData;
    CCharString();                                       // 0x0099E4B0
};
class CPersistContextReadSection {
public:
    CPersistContext        Context;                      // +0x00
    CMemoryDataInputStream InputStream;                  // +0x2c
    CStringParser          Parser;                       // +0x48
    CCharString            Text;                         // +0x70
    CPersistContextReadSection();
};
CPersistContextReadSection::CPersistContextReadSection()
{
}