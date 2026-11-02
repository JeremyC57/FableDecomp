#ifndef FABLE_ENGINE_CRUNTIMESCRIPTEDENTITYPERSIST_H
#define FABLE_ENGINE_CRUNTIMESCRIPTEDENTITYPERSIST_H
#include "fable_string.h"
// Nested-type declaration: no enclosing object is instantiated by these rows.
// Layout: Ego_r CPersistInfo sizeof 16/alignment 8, UID +0/PersistScript +8;
// native pair copy cbcb3b..cbcb56 confirms second +8 and its string +16.
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {
    public:
        unsigned __int64 UID;
        CCharString PersistScript;
    };
};
}
#endif
