// CTCBase::EndSerialising(bool textMode, CCharString& out) -- in text mode append "End" + GetClassTypeName() + ";\r\n" to the serialisation text
// Leaf: ?EndSerialising@CTCBase@@AAEX_NAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006869E0, length 98 bytes. Private (`AAE`); `ret 8`.
// Callees: GetClassTypeName (virtual slot 22, +0x58, CCharString by hidden pointer), operator+(const char*, const CCharString&)
// @ 0x0099F690 and operator+(const CCharString&, const char*) @ 0x0099F600 (both `__fastcall`, hidden result in ecx; strings
// "End" @ 0x0125BE7C, ";\r\n" @ 0x012353B8), CCharString::~CCharString @ 0x0099EAE0 (x3), CCharString::operator+= @ 0x0099F0A0.
// Shape: the ";\r\n" push precedes the class-name call (arguments right to left); the two inner temporaries die before the
// append, the final sum is a named local (inner temporaries die at its declaration) living in the dead `textMode` argument slot (esp+0xC after `sub esp,8`).
class CCharString { public:
    void* PString;
    ~CCharString();                             // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassTypeName() const;   // slot 22 (+0x58)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
private:
    void EndSerialising(bool textMode, CCharString& out);
};

void CTCBase::EndSerialising(bool textMode, CCharString& out)
{
    if (textMode) {
        CCharString text = "End" + GetClassTypeName() + ";\r\n";
        out += text;
    }
}