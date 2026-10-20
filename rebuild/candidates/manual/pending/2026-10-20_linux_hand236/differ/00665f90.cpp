// CThingCreatureBase::PlayCreatureContinuousSounds() -- when the creature def has the sound sub-def and the thing a sound component, play "SE_CREATURE_CONTINUOUS" on the next of eight round-robin "VOICE<n>" channels
// Grade: DIFFER(ours v retail) -- 258v246: the lookup key takes the frame word of the int-string temporary (retail shares the voice local's word), and the final def release comes out as `mov esi,[eax+4] ; add eax,4 ; dec esi ; mov [eax],esi ; test` where retail has `dec dword ptr [ecx+4] ; jne` (four release forms tried)
// Leaf: ?PlayCreatureContinuousSounds@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665F90, 246 bytes (+10 int3), `ret`.
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x0065F604 (manifest names a propagated CAbilityDef
//   instantiation; the sub-def type is not recovered), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the
//   inlined GetInterface for interface type 0x10 = bit 16 of the bitset word at +0x20), GFIntToCharString(long) @ 0x0099F830
//   and operator+(const char*, const CCharString&) @ 0x0099F690 (both `__fastcall`, hidden result in ecx; "VOICE"
//   @ 0x0125ABFC), the sound component's channel setter @ 0x0066A9D0 and CSoundInfo::PlaySound-shaped player @ 0x0066BD00
//   ("SE_CREATURE_CONTINUOUS" @ 0x0125ABE4, then 0, 0, 0, -1), CCharString(const char*, long) @ 0x0099EBF0, ~CCharString
//   @ 0x0099EAE0, the def's virtual slot 1 (+4) at a zero count.
// Globals: the round-robin voice counter 0x013BA874 (`(n + 1) % 8`, signed).
// Layout: retail CThingCreatureBase PDef +0xE0 (the landed ctor 0x00666090); CThing interface bitset +0x20, map +0x44.
// Shape: the def pointer is a CDefPointer local (its address escapes into the out-of-line GetSubDef, so the release
//   re-reads it at the single exit; the def's own inline Release gives `dec [ecx+4] ; jne`); the GetSubDef result and the component test are one `&&`; the channel
//   name is a named local built from the counter read before the counter is stored back.
struct CCharStringData;
struct CCharString {
    CCharStringData* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                              // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);      // 0x0099F690
extern long g_CreatureContinuousSoundVoice;                                        // 0x013BA874

struct CDefPointeeBase {                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
};
struct CCreatureSoundSubDef : CDefPointeeBase { };          // the sub-def queried by 0x0065F604 (type unproven)
struct CThingCreatureDef {
    bool GetSubDef(CDefPointer<CCreatureSoundSubDef>* out);   // 0x0065F604
};
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CTCCreatureSound {                   // interface type 0x10
    void SetVoiceChannel(const CCharString& channel);                       // 0x0066A9D0
    void PlaySound(const CCharString& name, long a, long b, long c, long d); // 0x0066BD00
};
struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0xe0 - 0x4c];
    CThingCreatureDef* PDef;                // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void PlayCreatureContinuousSounds();
};

void CThingCreatureBase::PlayCreatureContinuousSounds()
{
    CDefPointer<CCreatureSoundSubDef> def;
    if (PDef->GetSubDef(&def) && HasTC(0x10)) {
        CTCCreatureSound* sound = (CTCCreatureSound*)GetTC(0x10);
        CCharString voice = "VOICE" + GFIntToCharString(g_CreatureContinuousSoundVoice + 1);
        g_CreatureContinuousSoundVoice = (g_CreatureContinuousSoundVoice + 1) % 8;
        sound->SetVoiceChannel(voice);
        sound->PlaySound("SE_CREATURE_CONTINUOUS", 0, 0, 0, -1);
    }
}
