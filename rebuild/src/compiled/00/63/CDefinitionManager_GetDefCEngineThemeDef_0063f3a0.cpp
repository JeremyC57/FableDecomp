// CDefinitionManager::GetDef<CEngineThemeDef>(long, CDefPointer<const CEngineThemeDef>*) const -- GetPDefFromGlobalIndex @ 0x009AD9E0 returns the def pointer through the hidden pointer homed in the dead key slot; on a hit the out pointer is reassigned (old def released, new one addref'd) and the temporary released; true, else false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063F3A0, oracle length 108 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[index] ; test eax,eax ; jle F ; push eax ; lea eax,[esp+8] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[esp+4] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: mov eax,[def] ; mov dword ptr [esi],0 ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi ; jne R ; mov edx,[ecx] ; call [edx+4] ; R: mov al,1 ; pop esi ; ret 8 ; F: xor al,al ; ret 8
// Definition lookup into a counted def pointer: GetPDefFromGlobalIndex @ 0x009AD9E0 returns a CDefPointer by
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
// Naming: Ego_r.pdb / FableWin.pdb define ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z
//   (the long-index overload; the out pointer is a CDefPointer<const T>*).
//   Callers read the def's +0x8C (CEngineThemeDef::MaterialDef, Ego_r.pdb +0x90; retail defs sit 4 below the donor)
//   and feed it to GetDef<CMaterialDef> 0x0066C340 (IsThingAbleToDig 0x007F034C -> 0x007F0370); the theme callers
//   are CWorldMap::DestroyThemeAt, CMap::HasWaterAt / GetWaterSizeZOffsetAt / InitPassabilityFromThemesIfNecessary.
//   The def classes are opaque here; CDefPointer<T> keeps the landed counted-pointer model.
// Leaf: ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z
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

template <class T> class CDefPointer {
public:
    CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    template <class U> void SetDynamicCast(const CDefPointer<U>& other)   // retail: the plain counted assignment
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
    }
};

class CDefClassBase;
class CEngineThemeDef;

class CDefinitionManager {
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (hidden result pointer)
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        if (index <= 0)
            return false;
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (def.m_p) {
            out->SetDynamicCast(def);
            return true;
        }
        return false;
    }
};

template bool CDefinitionManager::GetDef<CEngineThemeDef>(long, CDefPointer<const CEngineThemeDef>*) const;
