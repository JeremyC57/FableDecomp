// test for 0x0098A9A0
#ifndef _MSC_VER
#define __stdcall
#define __declspec(x)
#endif
#include <new>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
struct IDirect3DPixelShader9;
struct IDirect3DDevice9;
struct IDirect3DDevice9Vtbl {
    void* _slots[106];
    long (__stdcall *CreatePixelShader)(IDirect3DDevice9* self, const unsigned long* function, IDirect3DPixelShader9** shader);   // slot 106 (+0x1a8)
};
struct IDirect3DDevice9 { IDirect3DDevice9Vtbl* lpVtbl; };
static const unsigned long* g_code = 0;
static long __stdcall FakeCreate(IDirect3DDevice9*, const unsigned long* f, IDirect3DPixelShader9** out) { g_code = f; *out = (IDirect3DPixelShader9*)0x77; return 0; }

struct CCharString {
    void* PStringData;
    CCharString(const char* s) : PStringData((void*)s) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { }
    ~CCharString() { }
};

template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long Size() const { return (unsigned long)(last - first); }
    const T& operator[](unsigned long i) const { return first[i]; }
    const T* GetData() const { return Size() > 0 ? first : (const T*)this; }
};

struct SScalarTag { SScalarTag() { } };

struct CLongArray {                             // CArray<long>
    long* first; long* last; long* end_of_storage;
    CLongArray() : first(0), last(0), end_of_storage(0) { }
    ~CLongArray() { if (first) free(first); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end_of_storage) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for the STLport growth insert 0x00440990 (_M_insert_overflow): VC7.1 sees it never reads the tag.
__declspec(noinline) void CLongArray::InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    unsigned long size = last - first;
    unsigned long len = size + (size > count ? size : count);
    long* start = (long*)operator new(len * sizeof(long));
    long* finish = start;
    for (long* p = first; p != where; ++p, ++finish)
        *finish = *p;
    for (unsigned long n = 0; n < count; ++n, ++finish)
        *finish = value;
    if (!noTail)
        for (long* p = where; p != last; ++p, ++finish)
            *finish = *p;
    if (first)
        free(first);
    first = start;
    last = finish;
    end_of_storage = start + len;
}
#pragma auto_inline(on)

struct CNameIndexNode {                          // STLport _Rb_tree_node<pair<const CCharString, long>>
    long            color;
    CNameIndexNode* parent;
    CNameIndexNode* left;
    CNameIndexNode* right;
    CCharString     key;                         // +0x10
    long            index;                       // +0x14
};

struct CNameIndexMap {                           // map<CCharString, long>
    CNameIndexNode* header;                      // +0x00 end()
    unsigned long   count;
    unsigned long   compare;
    CNameIndexNode* find(const CCharString& name)
    {
        for (CNameIndexNode* n = header->left; n != header; n = n->right)
            if (!strcmp((const char*)n->key.PStringData, (const char*)name.PStringData)) return n;
        return header;
    }
    long GetIndex(const CCharString& name)
    {
        CNameIndexNode* it = find(name);
        if (it != header)
            return it->index;
        return -1;
    }
};

struct CPixelShaderData {
    virtual ~CPixelShaderData() { ++s_deleted; }
    static int s_deleted;
    IDirect3DPixelShader9* PixelShader;
    unsigned long          ReferenceCount;      // +0x08
    void DecRef() { --ReferenceCount; }
    unsigned long GetRefCount() const { return ReferenceCount; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            delete this;
    }
};

struct CShaderBase {                            // vtable 0x0129A030
    virtual ~CShaderBase() { }
};

struct CPixelShader : CShaderBase {             // vtable 0x0129A054
    CPixelShaderData* Data;
    CPixelShader(IDirect3DPixelShader9* shader, const CLongArray& constIndices, const CCharString& name);
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    virtual ~CPixelShader()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
};

struct CShaderRenderManager {
    unsigned char     _pad000[0x1d4];
    IDirect3DDevice9* D3DDevice;                // +0x1d4
    unsigned char     _pad1d8[0x1e8];
    CNameIndexMap     PSConstIndexMap;          // +0x3c0 (PSConstNames +0x3b4, its map at +0xc)
    CPixelShader CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name);
};

CPixelShader CShaderRenderManager::CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name)
{
    IDirect3DPixelShader9* shader = 0;
    D3DDevice->lpVtbl->CreatePixelShader(D3DDevice, code.GetData(), &shader);
    CLongArray indices;
    for (unsigned long i = 0; i < constNames.Size(); ++i) {
        CCharString constName(constNames[i]);
        long index = PSConstIndexMap.GetIndex(constName);
        if (index != -1)
            indices.push_back(index);
    }
    CPixelShader result(shader, indices, name);
    return result;
}

int CPixelShaderData::s_deleted = 0;
struct CFakeData : CPixelShaderData { long ConstCount; long Consts[8]; };
CPixelShader::CPixelShader(IDirect3DPixelShader9* shader, const CLongArray& constIndices, const CCharString& name)
{
    CFakeData* d = new CFakeData; d->PixelShader = shader; d->ReferenceCount = 1;
    d->ConstCount = constIndices.last - constIndices.first;
    for (long i = 0; i < d->ConstCount; ++i) d->Consts[i] = constIndices.first[i];
    Data = d;
}
int main()
{
    static IDirect3DDevice9Vtbl vt; vt.CreatePixelShader = FakeCreate;
    static IDirect3DDevice9 dev; dev.lpVtbl = &vt;
    static CShaderRenderManager mgr; mgr.D3DDevice = &dev;
    static void* hb[6]; CNameIndexNode* header = (CNameIndexNode*)hb;
    static void* nb1[6]; CNameIndexNode* n1 = (CNameIndexNode*)nb1;
    static void* nb2[6]; CNameIndexNode* n2 = (CNameIndexNode*)nb2;
    n1->key.PStringData = (void*)"fog"; n1->index = 4; n1->right = n2;
    n2->key.PStringData = (void*)"tint"; n2->index = 9; n2->right = header;
    header->left = n1; mgr.PSConstIndexMap.header = header;
    unsigned long code[3] = { 1, 2, 3 };
    CArray<unsigned long> codeArr = { code, code + 3, code + 3 };
    CCharString names[4] = { CCharString("tint"), CCharString("nope"), CCharString("fog"), CCharString("tint") };
    CArray<CCharString> nameArr = { names, names + 4, names + 4 };
    int fails = 0;
    {
        CPixelShader ps = mgr.CreatePixelShader(codeArr, nameArr, CCharString("ps"));
        CFakeData* d = (CFakeData*)ps.Data;
        if (g_code != code) { printf("FAIL code\n"); ++fails; }
        if (d->PixelShader != (IDirect3DPixelShader9*)0x77 || d->ReferenceCount != 1) { printf("FAIL ref %lu\n", d->ReferenceCount); ++fails; }
        if (d->ConstCount != 3 || d->Consts[0] != 9 || d->Consts[1] != 4 || d->Consts[2] != 9) { printf("FAIL consts\n"); ++fails; }
    }
    if (CPixelShaderData::s_deleted != 1) { printf("FAIL delete\n"); ++fails; }
    CArray<unsigned long> empty = { 0, 0, 0 };
    CArray<CCharString> noNames = { 0, 0, 0 };
    { CPixelShader ps = mgr.CreatePixelShader(empty, noNames, CCharString("e")); if (g_code != (const unsigned long*)&empty) { printf("FAIL empty\n"); ++fails; } }
    if (!fails) printf("0098a9a0_TEST PASS\n");
    return fails;
}