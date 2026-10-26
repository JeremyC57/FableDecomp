// CDefinitionManager::GetDef<CThingObjectDef>(long index, CDefPointer<const CThingObjectDef>* out) const -- the def at a global index into a
// typed counted pointer: false for a non-positive index or a missing def
// Leaf: ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B040, 98 bytes, thiscall `ret 8`, frame-pointer size TU.
// Identity (sweep3; was the address-named CDefinitionManager::GetDef_0042b040): Ego_r.pdb / FableWin.pdb name set
//   `??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z`;
//   The def type is CThingObjectDef: the landed callers bind it as GetDef<CThingObjectDef> (0x706370 binds this exact
//   PDB symbol; CThingObject::ConstructFromParams 0x703050 and the CTCHeroAttachableAppearanceModifiers paths fetch the
//   object def by the thing's def index). FINDINGS chunk 481 said GetDef<CThingDef>, which is not in either PDB name
//   set.
//   The manager call @ 0x009AD9E0 is the public const Ego_r ?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z
//   (a const member can only call the const overload; Ego_r / FableWin list it 0x30 before the protected one, as retail).
// Retail bytes:  push ebp ; mov ebp,esp ; cmp dword ptr [index],0 ; jle F ; push [index] ; lea eax,[index] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[def] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: and dword ptr [esi],0 ; mov eax,[def] ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne R ; mov eax,[ecx] ; call [eax+4] ; R: mov al,1 ; pop esi ; jmp X ; F: xor al,al ; X: pop ebp ; ret 8
// Shape (unchanged from the landed model): the manager returns its CDefPointer<const CDefClassBase> by value through the
//   hidden pointer, homed in the dead index slot and re-read at every use; on a def, `*out = def` is the typed pointer's
//   inlined converting assignment (release the old def, store, addref) and the temporary's destructor releases through
//   the ref-count base's two inline accessors. Frame pointer and size optimisation from the TU's pragmas (the assignment is forced inline).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;          // slot 1
};

class CDefBase : public CDefVirtual, public CRefCounted {
public:
    void Release() const                   // inlined into CDefPointer<const T>::operator=
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary() const          // inlined into ~CDefPointer<const CDefClassBase>
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CDefClassBase;
class CThingObjectDef : public CDefBase { };

template <class T> class CDefPointer;

template <> class CDefPointer<const CDefClassBase> {   // the manager's return type
public:
    const CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};

template <class T> class CDefPointer {
public:
    T* m_p;
    __forceinline CDefPointer& operator=(const CDefPointer<const CDefClassBase>& other)   // forced: the size TU would call it out of line
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = static_cast<T*>(other.m_p);
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

class CDefinitionManager {
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (hidden result pointer)
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;
};

template <>
bool CDefinitionManager::GetDef<CThingObjectDef>(long index, CDefPointer<const CThingObjectDef>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.m_p) {
        *out = def;
        return true;
    }
    return false;
}
