// CFactionManager::SaveGameState(CPersistContext&) const -- persist faction count and per-index faction object.
// Native006B9560 168 bytes;410BE0 transfer,99F830 integer text,99F690 concat,99E4C0 cstr,6BAED0 named faction transfer,99EAE0 dtor.
// PDB Factions vector map pair is key plus two-word counted pointer (12-byte records).
#include "fable_string.h"
CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall operator+(const char*,const CCharString&);
class CFaction;
class CPersistContext { public:
 void Transfer(const char*,long&,const long&);
 void TransferFaction(const char*,CFaction&);
};
struct CountedFaction { CFaction* Data; void* Info; __forceinline CFaction& operator*() const { return *Data; } };
struct FactionEntry { unsigned long Key; CountedFaction Faction; };
class CFactionManager { public:
 FactionEntry *Start,*Finish,*EndOfStorage;
 void SaveGameState(CPersistContext&) const;
};
void CFactionManager::SaveGameState(CPersistContext& ctx) const {
 long n=Finish-Start;
 ctx.Transfer("NumberOfFactions",n,0);
 for(long i=0;i<n;++i) ctx.TransferFaction("Faction"+GFIntToCharString(i),*Start[i].Faction);
}