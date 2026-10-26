#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// CIMEFont::GetFontNameAndScale(CCharString&,float&) const -- select Arial atlas and scale by unsigned pixel height
// Retail 00404970, 226 bytes. Native PDB Height +16; true const method (fablewin PDB018bcc20).
// Calls CCharString::operator=(const char*) 0099EFE0; narrow append 0099F100.
// Actual fable_string.h has no narrow += declaration: field-free NativeCharString adapter
// declares the missing nonvirtual member, with identical storage and zero this adjustment.
// PDB ??YCCharString@@QAEABV0@PBD@Z confirms const CCharString& result.
#pragma once

#include <stddef.h>

#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif


class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    operator const char*() const;
    CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

struct NativeCharString : CCharString {
    const CCharString& Append(const char*); // 0x0099F100
};
FABLE_STATIC_ASSERT(sizeof(NativeCharString)==sizeof(CCharString));
// Native RTTI0122DCBC: CIMEFont derives CImeUiFont_Base at0 (donor size4).
// Base has exactly five genuine virtual interface slots, no virtual destructor:
// native slot0 SetHeight,1 SetColor,2 SetPosition,3 GetTextExtent,4 DrawTextW.
class CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int)=0;
    virtual void SetColor(unsigned long)=0;
    virtual void SetPosition(int,int)=0;
    virtual void GetTextExtent(const wchar_t*,unsigned long*,unsigned long*)=0;
    virtual void DrawTextW(const wchar_t*)=0;
};
namespace NUISystem {
class CIMEFont : public CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int);
    virtual void SetColor(unsigned long);
    virtual void SetPosition(int,int);
    virtual void GetTextExtent(const wchar_t*,unsigned long*,unsigned long*);
    virtual void DrawTextW(const wchar_t*);
    fable_u32 Colour;
    float Position[2];
    fable_u32 Height;
    void GetFontNameAndScale(CCharString&,float&) const;
};
void CIMEFont::GetFontNameAndScale(CCharString& name,float& scale) const {
    float height = (float)Height;
    name = "ENG_ARIAL_";
    if (Height < 16) {
        static_cast<NativeCharString&>(name).Append("12");
        scale = height * (1.0f/12.0f);
    } else if (Height < 18) {
        static_cast<NativeCharString&>(name).Append("16");
        scale = height * (1.0f/16.0f);
    } else if (Height < 24) {
        static_cast<NativeCharString&>(name).Append("18");
        scale = height * (1.0f/18.0f);
    } else if (Height < 28) {
        static_cast<NativeCharString&>(name).Append("24");
        scale = height * (1.0f/24.0f);
    } else {
        static_cast<NativeCharString&>(name).Append("28");
        scale = height * (1.0f/28.0f);
    }
}
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
static int live;
CCharString::CCharString() : storage_(0) { ++live; }
CCharString::~CCharString() { if(storage_) { free(storage_->text); free(storage_); } --live; }
const CCharString& CCharString::operator=(const char* s) {
 if(storage_) { free(storage_->text); free(storage_); }
 storage_=(CCharStringData*)malloc(sizeof(CCharStringData));
 storage_->text=(char*)malloc(strlen(s)+1); strcpy(storage_->text,s);storage_->owners=1;
 return *this;
}
CCharString::operator const char*() const { return storage_?storage_->text:""; }
const CCharString& NativeCharString::Append(const char* suffix) {
 char b[64];strcpy(b,(const char*)*this);strcat(b,suffix);
 return static_cast<CCharString&>(*this)=b;
}
namespace NUISystem {
void CIMEFont::SetHeight(unsigned int h) { Height=h; }
void CIMEFont::SetColor(unsigned long c) { Colour=(fable_u32)c; }
void CIMEFont::SetPosition(int x,int y) { Position[0]=(float)x;Position[1]=(float)y; }
void CIMEFont::GetTextExtent(const wchar_t*,unsigned long*,unsigned long*) {}
void CIMEFont::DrawTextW(const wchar_t*) {}
}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NUISystem::CIMEFont)==20);
#endif
int main() {
 const fable_u32 heights[]={0,1,15,16,17,18,23,24,27,28,100,0x80000000u,0xffffffffu};
 const char* suffix[]={"12","12","12","16","16","18","18","24","24","28","28","28","28"};
 const float factors[]={1.f/12,1.f/12,1.f/12,1.f/16,1.f/16,1.f/18,1.f/18,1.f/24,1.f/24,1.f/28,1.f/28,1.f/28,1.f/28};
 { NativeCharString name; NUISystem::CIMEFont font;
 assert((void*)&name==(void*)static_cast<CCharString*>(&name));
 CImeUiFont_Base* fontBase=&font;
 assert((void*)fontBase==(void*)&font);
 fontBase->SetHeight(7);fontBase->SetColor(0x123456);fontBase->SetPosition(10,20);
 assert(font.Height==7 && font.Colour==0x123456 && font.Position[0]==10 && font.Position[1]==20);
 for(unsigned int i=0;i<sizeof(heights)/sizeof(heights[0]);++i) {
  font.Height=heights[i];font.Colour=0x12345678; font.Position[0]=2; font.Position[1]=3;
  const NUISystem::CIMEFont& f=font;float scale=-1;
  static_cast<CCharString&>(name)="stale";
  f.GetFontNameAndScale(name,scale);
  char expected[64];strcpy(expected,"ENG_ARIAL_");strcat(expected,suffix[i]);
  assert(strcmp((const char*)name,expected)==0);
  double expectedScale=(double)(float)heights[i]*(double)factors[i];
  double error=(double)scale-expectedScale;if(error<0)error=-error;
  assert(error<=expectedScale*0.000001+0.000001);
  assert(font.Height==heights[i] && font.Colour==0x12345678 && font.Position[0]==2 && font.Position[1]==3);
 }
 assert(live==1);
 } assert(live==0);
 puts("00404970_TEST PASS");return 0;
}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CImeUiFont_Base)==4);
typedef void (CImeUiFont_Base::*ImeHeightMember)(unsigned int);
FABLE_STATIC_ASSERT(sizeof(ImeHeightMember)==4);
#endif