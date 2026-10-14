// CDefClassBase::GetSubDef<T>(CDefPointer<T>&) -- the sub-def index for 'CCreatureGeneratorDef' through virtual slot 14 (a temporary CCharString), the def fetched from the manager at +0x1C (GetPDefFromGlobalIndex @ 0x009ADA10) into a typed counted pointer (converting copy, the manager's temporary released), assigned to the out pointer; false when the name has no index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074D160, oracle length 182 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push esi ; push edi ; push -1 ; mov esi,ecx ; push offset 'CCreatureGeneratorDef' ; lea ecx,[name] ; call CCharString::CCharString ; mov eax,[esi] ; lea ecx,[name] ; push ecx ; mov ecx,esi ; call [eax+38h] ; lea ecx,[name] ; mov edi,eax ; call CCharString::~CCharString ; test edi,edi ; je F ; mov edx,[edi] ; mov ecx,[esi+1Ch] ; push edx ; lea eax,[temp] ; push eax ; call GetPDefFromGlobalIndex ; mov esi,[eax] ; test esi,esi ; je C ; inc dword ptr [esi+4] ; C: mov eax,[temp] ; test eax,eax ; je A ; mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi ; mov edx,edi ; test edx,edx ; mov [eax],edi ; jne N ; mov edx,[ecx] ; call [edx+4] ; N: mov dword ptr [temp],0 ; A: mov edi,[out] ; mov ecx,[edi] ; cmp ecx,esi ; je K ; test ecx,ecx ; je B ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: mov dword ptr [edi],0 ; B: test esi,esi ; mov [edi],esi ; je R ; inc dword ptr [esi+4] ; K: test esi,esi ; je R ; dec dword ptr [esi+4] ; jne R ; mov e?x,[esi] ; mov ecx,esi ; call [e?x+4] ; R: pop edi ; mov al,1 ; pop esi ; pop ecx ; ret 4 ; F: pop edi ; xor al,al ; pop esi ; pop ecx ; ret 4
// Typed sub-definition lookup: a block-scoped CCharString named 'CCreatureGeneratorDef' (constructed with the
// explicit -1 length, destroyed at the end of its block, right after the virtual lookup) is
// passed to the virtual GetSubDefIndex (slot 14), which returns a pointer to the global index
// or null (the false path sits at the end: the success branch is the `if` body). The manager
// at +0x1C returns its CDefPointer<CDefBase> through the hidden pointer -- homed in the dead
// string slot -- and the typed CDefPointer<T> is converting-copied from it (addref; no copy
// elision across the two types), then the manager's temporary is destroyed: its destructor
// releases through the ref-count base's two inline accessors (adjusted base pointer
// materialised once, stored value forwarded to the test) and nulls the
// field (the store survives because code follows). `out = result` is the typed pointer's
// inlined assignment (release the old def through CDefBase::Release, store, addref) and the
// typed local's destructor releases through Release (`dec [esi+4] ; jne`, the null store
// elided for the register-held field). The manifest label names one instantiation;
// T is opaque. One of 87 bodies of this shape.
#include "fable_string.h"

struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {
    void Release()                         // CDefPointer<T>: destructor and assignment
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()                // CDefPointer<CDefBase>: the manager's temporary
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CDefPointerBase {                   // CDefPointer<CDefBase>: the manager's return type
    CDefBase* m_p;
    CDefPointerBase() { }
    ~CDefPointerBase()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

struct CSubDef : CDefBase { };              // T

struct CDefPointer {                       // CDefPointer<T>
    CSubDef* m_p;
    CDefPointer() { }
    CDefPointer(const CDefPointerBase& other) : m_p((CSubDef*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

struct CDefinitionManager {
    CDefPointerBase GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};

struct CDefClassBase {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad[0x1c - 4];
    CDefinitionManager* m_pManager;        // +0x1C
    bool GetSubDef_0074d160(CDefPointer& out);
};

bool CDefClassBase::GetSubDef_0074d160(CDefPointer& out)
{
    long* pIndex;
    {
        CCharString name("CCreatureGeneratorDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer result(m_pManager->GetPDefFromGlobalIndex(*pIndex));
        out = result;
        return true;
    }
    return false;
}
