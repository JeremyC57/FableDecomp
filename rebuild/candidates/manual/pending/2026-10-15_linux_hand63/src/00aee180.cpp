// CParticleEmitter::GetPParticleSystemFromName(const CCharString& name) -- returns the first particle system whose Name equals `name` (inline CCharString ==: same data block, or equal lengths and Compare == 0), else null
// Leaf: GetPParticleSystemFromName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEE180, length 86 bytes (`ret 4`).
// Overloads: this is the non-const lookup (called on `this` by CParticleEmitter::DatabaseCopy
// 0x00AEF00D); the byte-identical const twin 0x00AEE1E0 is called on the source emitter (0x00AEF059).
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, strcmp-style).
// Flags: /Oa
// Build: the module assumes no aliasing (/Oa, in the lander's flag sweep): the name's data block is
// loaded once before the loop (esi) and `last` is cached in ebx across the Compare calls; without
// /Oa both are re-read every pass.
// Layout: ParticleSystems (retail 12-byte vector of 8-byte counted pointers) at +0x08 of the emitter;
// CParticleSystem::Name (CCharString) at +0x6c (PDB +0x88; the retail system is 0x1c narrower in
// front of it: two 4-byte lists, a 16-byte CVectorMap and a 0x24-byte parameter block). The
// generated engine/CParticleEmitter.h keeps ParticleSystems opaque (16-byte PDB vector), so the
// emitter is modelled locally.
// Shape: the system's name is taken as a named pointer (`&system->GetName()`) and compared with a
// FREE inline operator==(const CCharString&, const CCharString&) that re-reads `left.m_pData` /
// `right.m_pData` at every use -- that keeps the name address in eax (`add eax,6Ch ; mov eax,[eax]`)
// and the length compare in ecx; the member operator== with block locals (the landed 0x0081E9F0
// form), a base-class accessor, a counted-pointer `operator->` and the pointer local alone all fold
// the address into `mov eax,[ecx+6Ch]` (tried). `neg ; sbb al,al ; inc al` for Compare == 0; a hit
// returns the re-read object pointer, a miss `xor eax,eax`.

struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};

long __fastcall Compare_00411570(const char* a, const char* b);           // 0x00411570

struct CCharString {
    CCharStringData* m_pData;
};

inline bool operator==(const CCharString& left, const CCharString& right)
{
    if (left.m_pData == right.m_pData)
        return true;
    if (left.m_pData && right.m_pData && left.m_pData->m_length == right.m_pData->m_length)
        return Compare_00411570(left.m_pData->m_pData, right.m_pData->m_pData) == 0;
    return false;
}

struct CParticleSystem {
    unsigned char _state[0x6c];
    CCharString   Name;                                                   // +0x6c
    const CCharString& GetName() const { return Name; }
};

struct CParticleSystemPtr {                                               // CCountedPointer<CParticleSystem> (8 bytes)
    CParticleSystem* m_pObject;
    void*            m_pCount;
};

struct CParticleSystemPtrVector {                                         // retail vector<CCountedPointer<CParticleSystem> > (12 bytes)
    CParticleSystemPtr* m_first;
    CParticleSystemPtr* m_last;
    CParticleSystemPtr* m_end;
};

struct CParticleEmitter {
    virtual void v00();
    unsigned char            _base4[4];                                   // +0x04
    CParticleSystemPtrVector ParticleSystems;                             // +0x08
    CParticleSystem* GetPParticleSystemFromName(const CCharString& name);
};

CParticleSystem* CParticleEmitter::GetPParticleSystemFromName(const CCharString& name)
{
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        const CCharString* systemName = &it->m_pObject->GetName();
        if (*systemName == name)
            return it->m_pObject;
    }
    return 0;
}
