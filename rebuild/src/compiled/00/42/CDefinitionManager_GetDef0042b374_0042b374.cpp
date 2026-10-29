// CDefinitionManager::GetDef<T>(long, CDefPointer&) -- GetPDefFromGlobalIndex @ 0x009AD9E0 returns the def pointer through the hidden pointer homed in the dead key slot; on a hit the out pointer is reassigned (old def released, new one addref'd) and the temporary released; true, else false; frame-pointer size TU
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042B374, oracle length 98 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; cmp dword ptr [index],0 ; jle F ; push [index] ; lea eax,[index] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[def] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: and dword ptr [esi],0 ; mov eax,[def] ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne R ; mov eax,[ecx] ; call [eax+4] ; R: mov al,1 ; pop esi ; jmp X ; F: xor al,al ; X: pop ebp ; ret 8
// Definition lookup into a counted def pointer: GetPDefFromGlobalIndex @ 0x009AD9E0 returns a CDefPointer by
// value through the hidden pointer, which VC homes in the dead key argument slot; the
// temporary is re-read from that slot at every use. A null def fails (the false path sits at
// the end: the success branch is the `if` body). Otherwise `out = def` is inlined -- unless
// the out pointer already holds the def, its old def is released through CDefBase::Release
// (`this` in ecx, `dec [ecx+4] ; jne`, virtual slot 1 destroys at zero) and nulled, the new one
// stored and addref'd -- and the temporary's destructor releases the def through the
// ref-count base's two inline accessors (decrement, then test the count): the adjusted base
// pointer is materialised once for both (`add eax,4`), the stored value is forwarded to the
// test (re-read in the size TU: `dec [eax] ; cmp [eax],0`) and the def is kept in ecx for the virtual call. Frame pointer and size optimisation from the TU's pragmas. The manifest
// label names one instantiation; the def type is opaque. One of 13 bodies of this shape.
#pragma optimize("y", off)
#pragma optimize("s", on)

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
template <class T> class CDefPointer { public:
    CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    __forceinline CDefPointer& operator=(const CDefPointer& other)   // forced: the size TU would call it out of line
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
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (hidden result pointer)
    bool GetDef_0042b374(long index, CDefPointer<const CDefClassBase>& out);
};

bool CDefinitionManager::GetDef_0042b374(long index, CDefPointer<const CDefClassBase>& out)
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.m_p) {
        out = def;
        return true;
    }
    return false;
}