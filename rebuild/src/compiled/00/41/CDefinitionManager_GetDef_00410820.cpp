// CDefinitionManager::GetDef<CControlsDef>(const CCharString&, CDefPointer<const CControlsDef>*) const -- GetPDefFromInstantiationName @ 0x009ADA40 returns the def pointer through the hidden pointer homed in the dead key slot; on a hit the out pointer is reassigned (old def released, new one addref'd) and the temporary released; true, else false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00410820, oracle length 104 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[name] ; push eax ; lea edx,[esp+8] ; push edx ; call GetPDefFromInstantiationName ; mov eax,[esp+4] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: mov eax,[def] ; mov dword ptr [esi],0 ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi ; jne R ; mov edx,[ecx] ; call [edx+4] ; R: mov al,1 ; pop esi ; ret 8 ; F: xor al,al ; ret 8
// Definition lookup into a counted def pointer: GetPDefFromInstantiationName @ 0x009ADA40 returns a CDefPointer by
// value through the hidden pointer, which VC homes in the dead key argument slot; the
// temporary is re-read from that slot at every use. A null def fails (the false path sits at
// the end: the success branch is the `if` body). Otherwise `out = def` is inlined -- unless
// the out pointer already holds the def, its old def is released through CDefBase::Release
// (`this` in ecx, `dec [ecx+4] ; jne`, virtual slot 1 destroys at zero) and nulled, the new one
// stored and addref'd -- and the temporary's destructor releases the def through the
// ref-count base's two inline accessors (decrement, then test the count): the adjusted base
// pointer is materialised once for both (`add eax,4`), the stored value is forwarded to the
// test and the def is kept in ecx for the virtual call. The manifest
// label names one instantiation; the def type is opaque. One of 14 bodies of this shape.
// Name fix (agentNF6 2026-11-10): CDefinitionManager::GetPDefFromInstantiationName 0x009ADA40 is ?GetPDefFromInstantiationName@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@@Z (Ego_r.pdb / FableWin.pdb; const, returns the class template CDefPointer<const CDefClassBase>). The row's own GetDef<T> identity (one of the 42 PDB GetDef-by-name instantiations) is still unresolved.
// Leaf: ??$GetDef@VCControlsDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCControlsDef@@@@@Z
// Naming (agentNF31, 2026-11-12): was CDefinitionManager::GetDef_00410820(const CCharString&, CDefPointer<const CDefClassBase>&).
//   FableWin.pdb _fablewin_api.tsv 0x017F2709 / 0x01897270 ??$GetDef@VCControlsDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCControlsDef@@@@@Z
//   (const member template, out handle by pointer); the landed caller ResetAssignedInputs 0x004085F0 binds this symbol, passing the
//   FABLE_PC_CONTROL_SCHEME_GDD* names and reading CControlsDef::Controls from the out handle. The row keeps its pre-existing
//   local CDefPointer model (engine/CDefPointer.h's release shapes differ, NF30), with the converting assignment the template
//   body needs (same inline body as the same-type one).
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
    void Release()                         // inlined into CDefPointer::operator=
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()                // inlined into ~CDefPointer
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CDefClassBase;
template <class T> class CDefPointer { public:   // CDefPointer<T> (Ego_r.pdb / FableWin.pdb class template)
    CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CDefPointer& operator=(const CDefPointer& other)   // inlined by the speed TU
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
    template <class U> CDefPointer& operator=(const CDefPointer<U>& other)   // converting form of the same inline body
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

class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromInstantiationName(const CCharString& name) const;   // 0x009ADA40 (hidden result pointer)
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out) const;
};

template <class T> bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out) const
{
    CDefPointer<const CDefClassBase> def = GetPDefFromInstantiationName(name);
    if (def.m_p) {
        *out = def;
        return true;
    }
    return false;
}

class CControlsDef;
template bool CDefinitionManager::GetDef<CControlsDef>(const CCharString& name, CDefPointer<const CControlsDef>* out) const;
