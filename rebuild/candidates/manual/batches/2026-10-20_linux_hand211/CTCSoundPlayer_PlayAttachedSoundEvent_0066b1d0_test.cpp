// Behaviour test for CTCSoundPlayer::PlayAttachedSoundEvent (0x0066B1D0): with an entity sound def the played name is
// "<identifier>;<event>", without one just the event; the bank, the three trailing arguments and -1 are forwarded, the
// player's result is returned and the sub def reference is released.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    char buf[128];
    CCharString() { buf[0] = 0; }
    CCharString(const char* t) { std::strcpy(buf, t); }
    ~CCharString() { }
    CCharString& operator+=(const CCharString& other) { std::strcat(buf, other.buf); return *this; }
    CCharString& operator+=(const char* text) { std::strcat(buf, text); return *this; }
};
struct CDefBase {
    virtual void v00() { }
    virtual void Destroy() { ++destroyed; }
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
    int destroyed;
};
template <class T> struct CDefOut {                     // the out slot (released by the holder below)
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CEntitySoundDef : CDefBase {
    CCharString SoundIdentifier;                        // +0x48
};
struct CDefClassBase : CDefBase {
    CEntitySoundDef* sub;
    bool GetSubDef_0065f604(CDefOut<CEntitySoundDef>& out) { if (!sub) return false; ++sub->RefCount; out.Object = sub; return true; }
};
struct CASoundBank;
struct CSoundInit;
struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
    CASoundBank* GetSoundBank_004c7a80() { return (CASoundBank*)0x5678; }
};
struct CTCSoundPlayer {
    virtual void v00() { }
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CASoundBank* lastBank; char lastName[128]; CSoundInit* lastInit; long lastA, lastB, lastIndex;
    unsigned long PlayAttachedSampleByName_0066b120(CASoundBank* bank, const CCharString& name, CSoundInit* init, long a, long b,
                                                     long index)
    {
        lastBank = bank; std::strcpy(lastName, name.buf); lastInit = init; lastA = a; lastB = b; lastIndex = index;
        return 42;
    }
    unsigned long PlayAttachedSoundEvent(const CCharString& eventName, CSoundInit* init, long a, long b);
};

unsigned long CTCSoundPlayer::PlayAttachedSoundEvent(const CCharString& eventName, CSoundInit* init, long a, long b)
{
    CCharString name;
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefOut<CEntitySoundDef> soundDefOut;
    bool found = thingDef->GetSubDef_0065f604(soundDefOut);
    CDefHolder<CEntitySoundDef> soundDef(soundDefOut.Object);
    if (found) {
        name += soundDef->SoundIdentifier;
        name += ";";
    }
    name += eventName;
    CThing* thing = ParentThing;
    return PlayAttachedSampleByName_0066b120(thing->GetSoundBank_004c7a80(), name, init, a, b, -1);
}

int main()
{
    int bad = 0;
    CEntitySoundDef sd; sd.RefCount = 1; sd.destroyed = 0; std::strcpy(sd.SoundIdentifier.buf, "SND_DOOR");
    CDefClassBase td; td.sub = &sd;
    CThing thing; thing.PDef = &td;
    CTCSoundPlayer p; p.ParentThing = &thing;
    CCharString ev("OPEN");
    unsigned long r = p.PlayAttachedSoundEvent(ev, (CSoundInit*)0x11, 2, 3);
    if (r != 42 || std::strcmp(p.lastName, "SND_DOOR;OPEN") || p.lastBank != (CASoundBank*)0x5678) bad |= 1;
    if (p.lastInit != (CSoundInit*)0x11 || p.lastA != 2 || p.lastB != 3 || p.lastIndex != -1) bad |= 2;
    if (sd.RefCount != 1 || sd.destroyed) bad |= 4;
    td.sub = 0;
    p.PlayAttachedSoundEvent(ev, 0, 0, 0);
    if (std::strcmp(p.lastName, "OPEN")) bad |= 8;
    if (bad) { std::printf("0066b1d0_TEST FAIL\n"); return 1; }
    std::printf("0066b1d0_TEST PASS\n");
    return 0;
}
