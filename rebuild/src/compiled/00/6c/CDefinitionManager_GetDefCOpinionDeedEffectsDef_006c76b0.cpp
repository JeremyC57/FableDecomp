// CDefinitionManager::GetDef<COpinionDeedEffectsDef>(long index, CDefPointer<const COpinionDeedEffectsDef>* out) const -- the def at a global index into a
// typed counted pointer: false for a non-positive index or a missing def
// Leaf: ??$GetDef@VCOpinionDeedEffectsDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C76B0, 108 bytes, thiscall `ret 8`, /O2 TU.
// Identity (sweep3; was the address-named CDefinitionManager::GetDef_006c76b0): Ego_r.pdb / FableWin.pdb name set
//   `??$GetDef@VCOpinionDeedEffectsDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@@Z`;
//   its three call sites (0x006C5650 in CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630, 0x006C5BBD and
//   0x006C6FBD further into the CTCHeroOpinionDeedLog TU) all fetch the manager through CTCBase::GetDefinitionManager
//   and pass a ushort from DeedEffectsDefIndices (Ego_r CTCHeroOpinionDeedLog +0x3A, retail +0x2E); Ego_r
//   PeekPDeedEffectsDef returns CDefPointer<const COpinionDeedEffectsDef>
//   The manager call @ 0x009AD9E0 is the public const Ego_r ?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z
//   (a const member can only call the const overload; Ego_r / FableWin list it 0x30 before the protected one, as retail).
// Retail bytes:  mov eax,[index] ; test eax,eax ; jle F ; push eax ; lea eax,[esp+8] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[esp+4] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: mov eax,[def] ; mov dword ptr [esi],0 ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi ; jne R ; mov edx,[ecx] ; call [edx+4] ; R: mov al,1 ; pop esi ; ret 8 ; F: xor al,al ; ret 8
// Shape (unchanged from the landed model): the manager returns its CDefPointer<const CDefClassBase> by value through the
//   hidden pointer, homed in the dead index slot and re-read at every use; on a def, `*out = def` is the typed pointer's
//   inlined converting assignment (release the old def, store, addref) and the temporary's destructor releases through
//   the ref-count base's two inline accessors.
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
class COpinionDeedEffectsDef : public CDefBase { };

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
    CDefPointer& operator=(const CDefPointer<const CDefClassBase>& other)
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
bool CDefinitionManager::GetDef<COpinionDeedEffectsDef>(long index, CDefPointer<const COpinionDeedEffectsDef>* out) const
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
