#ifndef FABLE_ENGINE_CHITLOCATIONSDEF_H
#define FABLE_ENGINE_CHITLOCATIONSDEF_H
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
// Retail RTTI 01232CB4 identifies CSubDefClassBase; original HitLocations vector at +40.
class CHitLocationsDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<long> HitLocations;
};
FABLE_STATIC_ASSERT(sizeof(CHitLocationsDef)==52);
#endif
