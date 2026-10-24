// CTCSpecialAbilities::CreateParticleEffect(long particleDef, const C3DVector& position) -- spawn a particle-emitter thing at the position and, when it is alive, mark its emitter component always-updating; return the new thing
// Grade: DIFFER(90v88) -- register choice only: retail keeps the null-defaulted emitter in ecx (`xor ecx,ecx ; test ah,8` on a dword loaded into eax, `mov ecx,[eax+4]` straight into the SetAlwaysUpdate this); ours holds it in eax (`mov ecx,[esi+28h] ; xor eax,eax ; test ch,8 ... mov eax,[eax+4] ; mov ecx,eax`, +2 bytes).
// Leaf: CreateParticleEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072AB70, 88 bytes (96 with padding), `ret` (static __fastcall).
// Label: catalog "CreateParticleEffect" (module _global); Ego_r.pdb name set has
//   ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (static, __fastcall); ~74 callers in
//   the 0x0078C000..0x0079C000 spell components.
// Callees: CTCDParticleEmitter::Create(long, const C3DVector&, bool) @ 0x006E0880 (static __fastcall; ecx/edx passed
//   straight through, `push 0`), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//   CTCDParticleEmitter::SetAlwaysUpdate(bool) @ 0x006E0590 (landed).
// Layout: CThing interface-presence bitset +0x20 (interface 0x4B = bit 11 of the word at +0x28: `test ah,8`),
//   interface map +0x44 (first/last), dead flag bit 0 of +0x91.
// Shape: the emitter lookup is GetTC with a null default (`xor ecx,ecx` before the presence test); the component is
//   used unconditionally (ecx may be null on the call, as in retail).
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 (bit 0: dead)
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            result = (T*)it->component;
        }
        return result;
    }
};
struct CTCDParticleEmitter {
    static CThing* __fastcall Create(long particleDef, const C3DVector& position, bool attach);   // 0x006E0880
    void SetAlwaysUpdate(bool alwaysUpdate);                                                    // 0x006E0590
};
struct CTCSpecialAbilities {
    static CThing* __fastcall CreateParticleEffect(long particleDef, const C3DVector& position);
};

CThing* __fastcall CTCSpecialAbilities::CreateParticleEffect(long particleDef, const C3DVector& position)
{
    CThing* effect = CTCDParticleEmitter::Create(particleDef, position, false);
    if (effect && !(effect->m_flags91 & 1))
        effect->GetTC<CTCDParticleEmitter>(0x4B)->SetAlwaysUpdate(true);
    return effect;
}
