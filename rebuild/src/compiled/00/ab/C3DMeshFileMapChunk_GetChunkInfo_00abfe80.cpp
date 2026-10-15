// C3DMeshFileMapChunk::GetChunkInfo() const -- "Description: " + Name + ", Filename: " + Filename
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABFE80, 153 bytes. Callees: CCharString copy constructor 0x0099EC30,
// operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600,
// operator+(const CCharString&, const CCharString&) 0x0099F570 (all __fastcall: hidden result ecx, left edx, right
// on the stack), ~CCharString 0x0099EAE0.
// Layout of the landed C3DMeshFileMapChunk::WriteToFile (0x00ABFF70): Name +0x30, Filename +0x34. The two strings
// come from by-value inline getters (copies evaluated right to left: Filename first), the sum is built into a
// named local that is copied into the hidden result (no NRVO) after the four temporaries die. The mangled name
// returns `const CCharString` (`?B`), and CCharString is a `class`.
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                                            // 0x0099EC30
    ~CCharString();                                                                   // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);         // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);         // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);  // 0x0099F570

struct C3DMeshFileMapChunk {
    virtual ~C3DMeshFileMapChunk();
    unsigned char _base_body[0x28];     // +0x04 .. +0x2B (C3DMeshFileChunk, retail size)
    long        MapIndex;               // +0x2c
    CCharString Name;                   // +0x30
    CCharString Filename;               // +0x34
    long        Unused;                 // +0x38
    CCharString GetName() const { return Name; }
    CCharString GetFilename() const { return Filename; }
    virtual const CCharString GetChunkInfo() const;
};

const CCharString C3DMeshFileMapChunk::GetChunkInfo() const
{
    CCharString info = "Description: " + GetName() + ", Filename: " + GetFilename();
    return info;
}