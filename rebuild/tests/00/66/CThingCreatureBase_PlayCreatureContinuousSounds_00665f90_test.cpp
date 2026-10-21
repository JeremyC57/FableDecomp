// Behaviour test for 00665f90: src/00665f90.cpp copied verbatim; PlayCreatureContinuousSounds, when the creature def has
// the sound sub-def and the thing has interface 0x10, sets the sound component's channel to "VOICE<n+1>" (n the
// round-robin counter, which advances modulo 8) and plays SE_CREATURE_CONTINUOUS; the sub-def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// CThingCreatureBase::PlayCreatureContinuousSounds() -- when the creature def has the sound sub-def and the thing a sound component, play "SE_CREATURE_CONTINUOUS" on the next of eight round-robin "VOICE<n>" channels
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
// Shape: the def pointer is a CDefPointer local filled by GetSubDef, a same-TU noinline stand-in body (retail's LTCG saw it
//   keep no reference to the slot), so its inline Release at the single exit is `mov ecx,[slot] ; dec [ecx+4] ; jne`; the
//   GetSubDef result and the component test are one `&&`; the channel name is a named local built from the counter read
//   before the counter is stored back, inside its own block after the component lookup (so the lookup key, a local of
//   the inlined GetTC, shares the voice local's frame word, not the int-string temporary's).
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
void* Find_CThingCreatureDef_GetSubDef(const void* owner);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CThingCreatureDef::GetSubDef(CDefPointer<CCreatureSoundSubDef>* out)
{
    void* def = Find_CThingCreatureDef_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CCreatureSoundSubDef*)def;
    return true;
}
#pragma auto_inline(on)
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
        {
            CCharString voice = "VOICE" + GFIntToCharString(g_CreatureContinuousSoundVoice + 1);
            g_CreatureContinuousSoundVoice = (g_CreatureContinuousSoundVoice + 1) % 8;
            sound->SetVoiceChannel(voice);
            sound->PlaySound("SE_CREATURE_CONTINUOUS", 0, 0, 0, -1);
        }
    }
}
// ---- harness ----
struct CCharStringData { char text[64]; };
static CCharStringData g_pool[32]; static int g_used = 0; static int g_strings = 0;
static CCharStringData* NewData(const char* a, const char* b)
{ CCharStringData* d = &g_pool[g_used++ % 32]; strcpy(d->text, a); strcat(d->text, b); ++g_strings; return d; }
CCharString::CCharString(const char* text, long length) { (void)length; PData = NewData(text, ""); }
CCharString::~CCharString() { --g_strings; }
CCharString __fastcall GFIntToCharString(long value) { char buf[16]; sprintf(buf, "%ld", value); return CCharString(buf); }
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ CCharString r(left); strcat(r.PData->text, right.PData->text); return r; }
long g_CreatureContinuousSoundVoice = 0;
static int g_destroyed = 0; static void* g_subDef = 0; static const void* g_subOwner = 0;
static char g_channel[64]; static char g_played[64]; static int g_plays = 0; static long g_args[4];
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
void* Find_CThingCreatureDef_GetSubDef(const void* owner)
{ g_subOwner = owner; if (g_subDef) ++((CDefPointeeBase*)g_subDef)->m_refCount; return g_subDef; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CTCCreatureSound::SetVoiceChannel(const CCharString& channel) { strcpy(g_channel, channel.PData->text); }
void CTCCreatureSound::PlaySound(const CCharString& name, long a, long b, long c, long d)
{ ++g_plays; strcpy(g_played, name.PData->text); g_args[0] = a; g_args[1] = b; g_args[2] = c; g_args[3] = d; }
int main()
{
    static unsigned char thingBuf[sizeof(CThingCreatureBase)]; CThingCreatureBase& thing = *(CThingCreatureBase*)thingBuf;
    static CThingCreatureDef creatureDef; thing.PDef = &creatureDef;
    static unsigned char soundBuf[8]; CTCCreatureSound* sound = (CTCCreatureSound*)soundBuf;
    static CInterfaceMap::SEntry entries[3] = { { 0x05, 0 }, { 0x10, (CTCBase*)soundBuf }, { 0x20, 0 } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    static CCreatureSoundSubDef subDef; subDef.m_refCount = 1;
    (void)sound;

    // no sub-def: nothing played
    g_subDef = 0; thing.m_presentBits[0] = 0x10000;
    thing.PlayCreatureContinuousSounds();
    CHECK(g_plays == 0 && g_subOwner == &creatureDef && g_CreatureContinuousSoundVoice == 0);

    // sub-def but no sound component bit: nothing played, sub-def released
    g_subDef = &subDef; thing.m_presentBits[0] = 0;
    thing.PlayCreatureContinuousSounds();
    CHECK(g_plays == 0 && subDef.m_refCount == 1 && g_destroyed == 0);

    // both: VOICE1, counter 1
    thing.m_presentBits[0] = 0x10000;
    thing.PlayCreatureContinuousSounds();
    CHECK(g_plays == 1 && strcmp(g_channel, "VOICE1") == 0 && strcmp(g_played, "SE_CREATURE_CONTINUOUS") == 0);
    CHECK(g_args[0] == 0 && g_args[1] == 0 && g_args[2] == 0 && g_args[3] == -1);
    CHECK(g_CreatureContinuousSoundVoice == 1 && subDef.m_refCount == 1 && g_destroyed == 0);

    // wraps after 8: counter 7 -> VOICE8, counter 0
    g_CreatureContinuousSoundVoice = 7;
    thing.PlayCreatureContinuousSounds();
    CHECK(strcmp(g_channel, "VOICE8") == 0 && g_CreatureContinuousSoundVoice == 0);

    // last reference to the sub-def: destroyed
    subDef.m_refCount = 0;
    thing.PlayCreatureContinuousSounds();
    CHECK(g_plays == 3 && g_destroyed == 1 && strcmp(g_channel, "VOICE1") == 0);

    if (fails == 0) printf("00665f90_TEST PASS\n");
    return fails;
}