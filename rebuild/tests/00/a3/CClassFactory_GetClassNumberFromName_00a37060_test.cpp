#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// CClassFactory::GetClassNumberFromName(const CCharString& name) -- index of the used class type whose name equals `name`; otherwise builds "Unable to find class with name <name>" and throws CGenericException
// Leaf: GetClassNumberFromName
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A37060, 208 bytes incl. padding (ret 4).
// Callees: Compare(const char*, const char*) @ 0x00411570 (__fastcall, in the inline CCharString ==),
//          CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600,
//          operator+(const CCharString&, const CCharString&) @ 0x0099F570 (both __fastcall, result in ecx),
//          ~CCharString @ 0x0099EAE0, operator const char* @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84
//          (ThrowInfo 0x013692F8 = CGenericException, a 1-byte empty class). Strings "" 0x0122D70E,
//          "Unable to find class with name " 0x0129CF3C.
// Layout: retail CClassFactory (landed ReregisterAllocatedClass 0x00A37300 model; rebuild/include/engine/CClassFactory.h is
//          the Ego_r layout, 4 bytes larger before MaxClassTypeID): ClassTypeInfo vector {first +4}, MaxClassTypeID +0x10;
//          20-byte CClassTypeInfo: Used +8, Name +0x10 (CCharString).
// Shape: the CCharString == of the landed CMapBrushList::GetScriptedMapBrush (same block, or equal lengths and Compare == 0);
//          the message is traced by reference (release build: only the const char* conversion survives); no EH frame.
#include <stddef.h>
struct CCharStringData { const char* m_pData; unsigned long m_length; };
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = m_pData;
        CCharStringData* b = other.m_pData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare_00411570(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

inline void TraceError(const CCharString& text) { (const char*)text; }

struct CGenericException { };

struct CClassTypeInfo_Retail {                  // 20 bytes
    void*         TypeList;                     // +0x00 (retail list: sentinel pointer)
    unsigned long NoAllocated;                  // +0x04
    bool          Used;                         // +0x08
    bool          Local;                        // +0x09
    void*         AllocFunc;                    // +0x0C
    CCharString   Name;                         // +0x10
};

struct CClassFactory {
    void*                  __vftable;           // +0x00
    CClassTypeInfo_Retail* ClassTypeInfo_first; // +0x04
    CClassTypeInfo_Retail* ClassTypeInfo_last;  // +0x08
    CClassTypeInfo_Retail* ClassTypeInfo_end;   // +0x0C
    unsigned long          MaxClassTypeID;      // +0x10
    unsigned long          NoAllocatedClasses;  // +0x14
    unsigned long GetClassNumberFromName(const CCharString& name);
};

unsigned long CClassFactory::GetClassNumberFromName(const CCharString& name)
{
    unsigned long count = MaxClassTypeID;
    unsigned long i = 0;
    if (i < count) {
        const CCharString* n = &ClassTypeInfo_first->Name;
        do {
            const CClassTypeInfo_Retail* info = (const CClassTypeInfo_Retail*)((const char*)n - offsetof(CClassTypeInfo_Retail, Name));
            if (info->Used && name == *n)
                return i;
            ++i;
            n = &info[1].Name;
        } while (i < count);
    }
    CCharString message = CCharString("") + "Unable to find class with name " + name;
    TraceError(message);
    CGenericException exception;
    throw exception;
}

// ---- test stubs ----
long __fastcall Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }
static CCharStringData* mk(const char* s) { CCharStringData* d = (CCharStringData*)malloc(sizeof(CCharStringData)); d->m_pData = s; d->m_length = (unsigned long)strlen(s); return d; }
CCharString::CCharString(const char* text, long) { m_pData = mk(text); }
CCharString::~CCharString() {}
CCharString::operator const char*() const { return m_pData ? m_pData->m_pData : ""; }
CCharString __fastcall operator+(const CCharString& l, const char*) { return CCharString((const char*)l); }
CCharString __fastcall operator+(const CCharString& l, const CCharString&) { return CCharString((const char*)l); }
int main()
{
    static unsigned char raw[5 * sizeof(CClassTypeInfo_Retail)];
    memset(raw, 0, sizeof(raw));
    CClassTypeInfo_Retail* t = (CClassTypeInfo_Retail*)raw;
    const char* names[5] = { "Alpha", "Beta", "Gamma", "Beta", "Delta" };
    bool used[5] = { true, false, true, true, true };
    for (int k = 0; k < 5; ++k) { t[k].Used = used[k]; t[k].Name.m_pData = mk(names[k]); }
    static CClassFactory f; memset(&f, 0, sizeof(f));
    f.ClassTypeInfo_first = t; f.MaxClassTypeID = 5;
    int ok = 1;
    CCharString a("Alpha"), b("Beta"), d("Delta"), g("Gamma");
    if (f.GetClassNumberFromName(a) != 0) ok = 0;
    if (f.GetClassNumberFromName(b) != 3) ok = 0;   // index 1 unused
    if (f.GetClassNumberFromName(g) != 2) ok = 0;
    if (f.GetClassNumberFromName(d) != 4) ok = 0;
    CCharString same(""); same.m_pData = t[2].Name.m_pData;   // pointer-equal path
    if (f.GetClassNumberFromName(same) != 2) ok = 0;
    if (ok) printf("00a37060_TEST PASS\n");
    return ok ? 0 : 1;
}