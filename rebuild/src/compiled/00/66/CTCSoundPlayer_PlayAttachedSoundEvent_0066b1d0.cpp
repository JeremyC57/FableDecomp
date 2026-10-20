// CTCSoundPlayer::PlayAttachedSoundEvent(const CCharString& eventName, CSoundInit* init, long a, long b) -- play the attached sound named "<entity sound identifier>;<event>" (just the event when the thing has no entity sound def)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066B1D0, 163 bytes (`ret 10h`).
// Callees: CCharString() @ 0x0099E4B0 / ~CCharString @ 0x0099EAE0, CDefClassBase::GetSubDef_0065f604(CDefPointer&) @ 0x0065F604
// (the 'CEntitySoundDef' instantiation), CCharString::operator+=(const CCharString&) @ 0x0099F0A0, operator+=(const char*)
// @ 0x0099F100 (";" @ 0x0123542C), the thing's sound-bank getter @ 0x004C7A80 (`this` unused: [[0x013B8A1C]+0x1c]+0x10;
// manifest "CAnimateThing::GetMeshBank" is propagated), the by-name attached-sample player @ 0x0066B120 (bank, name, then the
// three forwarded arguments and -1; manifest name propagated) -- both named descriptively; the def's virtual slot 1.
// Layout: CThing PDef +0x70; CEntitySoundDef SoundIdentifier (CCharString) +0x48 (Ego_r.pdb +0x50 less the 8 bytes of the
// sound map's retail containers).
// Shape: the thing def is read into a local before the out slot is declared (its zero store sinks below the push); the
// sub def is read once from its out slot into a holder (edi) released before the name; the three trailing arguments
// are forwarded as dwords; the owner is read into a local before the player call (ecx set before the pushes), whose
// arguments are pushed before the bank getter call.
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    CCharString& operator+=(const char* text);          // 0x0099F100
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
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
    unsigned char _pad08[0x48 - 0x08];
    CCharString SoundIdentifier;                        // +0x48
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_0065f604(CDefOut<CEntitySoundDef>& out);   // 0x0065F604
};
struct CASoundBank;
struct CSoundInit;
struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
    CASoundBank* GetSoundBank_004c7a80();               // 0x004C7A80
};
struct CTCSoundPlayer {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned long PlayAttachedSampleByName_0066b120(CASoundBank* bank, const CCharString& name, CSoundInit* init, long a, long b,
                                                     long index);   // 0x0066B120
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