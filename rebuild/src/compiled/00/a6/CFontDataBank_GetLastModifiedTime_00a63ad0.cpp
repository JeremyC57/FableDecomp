// CFontDataBank::GetLastModifiedTime(const CCharString&) -- the fixed date 1/1/1 1:1:00 as a CDateAndTime by value (font banks carry no modification time)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63AD0, length 32 bytes.
// Callees: CDateAndTime::CDateAndTime @ 0x009E9A70 (seven arguments: 1, 1, 1, 1, 1, 0, 0) on the hidden return slot. Shape: the hidden pointer in esi, returned in eax,
// `ret 8` (hidden pointer + the string reference). Protected virtual (the mangled name in the manifest).
// Leaf: ?GetLastModifiedTime@CFontDataBank@@MAE?AVCDateAndTime@@ABVCCharString@@@Z
class CCharString;
class CDateAndTime { public:
    unsigned char _body[16];
    CDateAndTime(long year, long month, long day, long hour, long minute, long second, long millisecond);   // 0x009E9A70
};

class CFontDataBank { public:
    virtual void v00();
protected:
    virtual CDateAndTime GetLastModifiedTime(const CCharString& name);
};

CDateAndTime CFontDataBank::GetLastModifiedTime(const CCharString& name)
{
    return CDateAndTime(1, 1, 1, 1, 1, 0, 0);
}