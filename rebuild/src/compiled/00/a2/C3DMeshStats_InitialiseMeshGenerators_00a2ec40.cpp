// C3DMeshStats::InitialiseMeshGenerators(CMeshDataBank* bank, const CCharString&) -- resolve each generator's object in the bank, dropping generators the bank does not have
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2EC40, oracle length 95 bytes.
// Callees: CMeshDataBank lookup by name @ 0x009CCDC0 (thiscall(const CCharString&) returning the bank index, 0 = none;
//          the manifest's "CQuestManager::OnQuestStartScreenDisplayed" label is propagated), the SGI-style counted
//          element copy @ 0x00A32170 (__fastcall(first, last, dest, const random_access_iterator_tag&, long*): 13 dwords
//          rep-movsd + CCharString::operator= per 64-byte C3DGeneratorObject; manifest label `_Copy_impl<...>` propagated),
//          CCharString::~CCharString @ 0x0099EAE0 (manifest `_Cons_val<...>` label propagated).
//
// Retail bytes:  push ebx ; mov ebx,ecx ; mov eax,[ebx+6Ch] ; push esi ; mov esi,[ebx+68h] ; cmp esi,eax ; je E ; push ebp ; mov ebp,[esp+10h] ;
//                push edi ; L: lea eax,[esi+34h] ; push eax ; mov ecx,ebp ; lea edi,[esi+40h] ; call bank lookup ; test eax,eax ; mov [esi+38h],eax ;
//                jne N ; mov edx,[ebx+6Ch] ; cmp edi,edx ; je P ; push eax ; lea ecx,[esp+18h] ; push ecx ; push esi ; mov ecx,edi ; call copy ;
//                P: mov eax,[ebx+6Ch] ; add eax,-40h ; lea ecx,[eax+34h] ; mov [ebx+6Ch],eax ; call CCharString::~CCharString ; mov edi,esi ;
//                N: cmp edi,[ebx+6Ch] ; mov esi,edi ; jne L ; pop edi ; pop ebp ; E: pop esi ; pop ebx ; ret 8
// Retail layout (see the constructor @ 0x00A2F210): +0x68 MeshGenerators = 12-byte CArray<C3DGeneratorObject>
// {first, last, end}; C3DGeneratorObject (PDB, 64 bytes): 52 bytes of transform data, +0x34 ObjectName
// (CCharString), +0x38 BankIndex (ulong), +0x3C UseLocalOrigin. The loop is `it = found ? it + 1 :
// MeshGenerators.erase(it)` with the SGI vector erase inlined: copy [it+1, last) down to it when there is
// a tail, step `last` back one element and destroy the vacated element (its CCharString). The copy's
// iterator-tag temporary is built in the dead first argument's home slot and the distance-type pointer is null.
class CCharString { public:
    char* m_pString;
    ~CCharString();                        // 0x0099EAE0
};

class CBankFile
{
public:
    unsigned long FindIndexBySymbol(const CCharString&) const;   // 0x009CCDC0
};
class CBankFileAsync : public CBankFile
{
public:
};

class CMeshDataBank : public CBankFileAsync { public:
};

class C3DGeneratorObject { public:
    unsigned char m_transform[0x34];       // +0x00: generator placement (trivially copied)
    CCharString   ObjectName;              // +0x34
    unsigned long BankIndex;               // +0x38
    bool          UseLocalOrigin;          // +0x3C
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };   // SGI iterator category (user constructor: no zero-init of the temporary, which is built in a dead argument slot)

// SGI `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` instantiated for C3DGeneratorObject*
C3DGeneratorObject* __fastcall CopyGeneratorObjects(C3DGeneratorObject* first, C3DGeneratorObject* last, C3DGeneratorObject* result,
                                                    const random_access_iterator_tag&, long*);   // 0x00A32170

struct CGeneratorArray {                   // retail CArray<C3DGeneratorObject>: {first, last, end}
    C3DGeneratorObject* m_pFirst;
    C3DGeneratorObject* m_pLast;
    C3DGeneratorObject* m_pEnd;
    C3DGeneratorObject* Begin() { return m_pFirst; }
    C3DGeneratorObject* End() { return m_pLast; }
    C3DGeneratorObject* Erase(C3DGeneratorObject* position)
    {
        if (position + 1 != m_pLast)
            CopyGeneratorObjects(position + 1, m_pLast, position, random_access_iterator_tag(), (long*)0);
        --m_pLast;
        m_pLast->~C3DGeneratorObject();
        return position;
    }
};

class C3DMeshStats { public:
    virtual void v00();
    unsigned char   _pad_0x04[0x68 - 0x04];
    CGeneratorArray MeshGenerators;        // +0x68
    void InitialiseMeshGenerators(CMeshDataBank* bank, const CCharString& name);
};

void C3DMeshStats::InitialiseMeshGenerators(CMeshDataBank* bank, const CCharString& /*name*/)
{
    C3DGeneratorObject* it = MeshGenerators.Begin();
    while (it != MeshGenerators.End()) {
        C3DGeneratorObject* next = it + 1;
        it->BankIndex = bank->FindIndexBySymbol(it->ObjectName);
        if (!it->BankIndex)
            next = MeshGenerators.Erase(it);
        it = next;
    }
}