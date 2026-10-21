// Behaviour test for 004c82a0: src/004c82a0.cpp copied verbatim; CCharString::operator!= stubbed over a C string so
// every exit (component 199, game-state persisted, a named serialisation section, the LevelPersistent bit) is observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThing::IsLevelPersistent() const -- whether the thing survives its level: game-state persisted, or serialised by name, else the LevelPersistent bit
// Retry 10b: GameStatePersisted bound to a `const bool&` (a bit-field reference binds a bool TEMPORARY whose address is taken): retail's dead frame byte `mov [esp+7],al` and the separate `mov al,1` exit (`return true`); a plain bool local folds into a test.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C82A0, 104 bytes (+8 int3). __fastcall, returns bool in al.
// Callees: CCharString::operator!=(const char*) const 0x0099E960 (x2: "" 0x0122D70E, "NULL" 0x012393E4).
// Shape: a thing holding component 199 (bitset word 6 bit 7: `mov al,[esi+38h] ; test al,al ; jns`) is never level
//        persistent; GameStatePersisted (bit 5 of +0x92) bound to a const bool& first (`mov [esp+7],al`) returns true;
//        without KillOnLevelUnload (bit 2 of +0x93) a real SerialisationSectionName (not "" / "NULL") returns true;
//        otherwise the LevelPersistent bit (bit 4 of +0x92) as an unsigned char bitfield (`shr al,4 ; and al,1`).
// Layout: retail CThing = Ego_r.pdb - 0x10 here: UsingComponents +0x20, SerialisationSectionName +0x58, flag bytes +0x91..
//         +0x93 with the PDB bit order.
struct CCharString {
    void* PStringData;
    bool operator!=(const char* text) const;     // 0x0099E960
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];            // +0x20 bitset<274>
    unsigned char _pad44[0x58 - 0x44];
    CCharString SerialisationSectionName;        // +0x58 (PDB +0x68)
    unsigned char _pad5c[0x91 - 0x5c];
    unsigned char Dead : 1;                      // +0x91
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;  // +0x93
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char _bits93 : 4;
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    bool IsGameStatePersisted() const { return GameStatePersisted; }
    bool IsLevelPersistent() const;
};
bool CThing::IsLevelPersistent() const
{
    if (HasTC(199))
        return false;
    const bool& persistent = GameStatePersisted;
    if (persistent)
        return true;
    if (!KillOnLevelUnload && SerialisationSectionName != "" && SerialisationSectionName != "NULL")
        return true;
    return LevelPersistent;
}
// ---- end source ----
bool CCharString::operator!=(const char* text) const { return strcmp((const char*)PStringData, text) != 0; }
void CThing::v00() {}
int main()
{
    static unsigned char raw[sizeof(CThing)];
    CThing* t = (CThing*)raw;
    CHECK((char*)&t->SerialisationSectionName - (char*)t == 0x58);
    memset(raw, 0, sizeof(raw)); t->SerialisationSectionName.PStringData = (void*)"";
    CHECK(t->IsLevelPersistent() == false);                       // nothing set
    t->LevelPersistent = 1; CHECK(t->IsLevelPersistent() == true);      // the bit
    t->LevelPersistent = 0; t->GameStatePersisted = 1; CHECK(t->IsLevelPersistent() == true);
    CHECK((raw[0x92] & 0x20) != 0);                               // bit 5 of +0x92
    t->UsingComponents[199 >> 5] |= 1UL << (199 & 31); CHECK(t->IsLevelPersistent() == false);   // component 199 wins
    CHECK(raw[0x38] == 0x80);
    memset(raw, 0, sizeof(raw));
    t->SerialisationSectionName.PStringData = (void*)"Section"; CHECK(t->IsLevelPersistent() == true);
    t->KillOnLevelUnload = 1; CHECK(t->IsLevelPersistent() == false);
    t->KillOnLevelUnload = 0; t->SerialisationSectionName.PStringData = (void*)"NULL"; CHECK(t->IsLevelPersistent() == false);
    t->LevelPersistent = 1; CHECK(t->IsLevelPersistent() == true);
    if (fails == 0) printf("004c82a0_TEST PASS\n"); else printf("004c82a0_TEST FAIL\n");
    return fails;
}