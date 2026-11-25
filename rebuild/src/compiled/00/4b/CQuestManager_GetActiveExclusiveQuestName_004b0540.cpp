// CQuestManager::GetActiveExclusiveQuestName  @0x004b0540
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// module: CQuestManager  __fastcall  returns CCharString*

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCharString;

struct SubNode {
    int              key;    // +0x00
    SubNode*         next;   // +0x04
    char             pad[0x28 - 8];
    CCharString*     name;   // +0x28
};

class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in Sub (?Find@Sub@@QAEPAUSubNode@@PAH@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): the quest data thing's Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE spelling at 0x004365B0.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Sub;

struct QData {
    char             pad0[0x2c];
    unsigned int     flags;  // +0x2c
    char             pad1[0x44 - 0x2c - 4];
    Sub              sub;     // +0x44
};

struct TNode {
    TNode*           left;   // +0x00
    char             pad0[4];
    char             keybuf[1]; // +0x08
};

struct QManager {
    char             pad0[0x5c];
    TNode*           treehdr; // +0x5c
};

extern bool __fastcall SubCheck(SubNode* self);      // 0x251d60
extern CCharString g_defaultQuestName;               // object @ 0x13bd804

CCharString* __fastcall CQuestManager_GetActiveExclusiveQuestName(QManager* self)
{
    union { ETCInterfaceType scratch; SubNode* found; } u; // single local (the push ecx slot)
    TNode* node = *(TNode**)self->treehdr;
    if (node != self->treehdr) {
        SubNode* found = u.found; // preheader read (once); loop preserves esi
        do {
            QData* q = ((QData*)(((CBaseIntelligentPointer*)((TNode*)((char*)node + 8)))->GetPItem()));
            if (q) {
                if (q->flags & 0x1000) {
                    const Sub* sub = &q->sub;
                    u.scratch = (ETCInterfaceType)0x6c;
                    const Sub::value_type* r = sub->LowerBound(u.scratch);
                    const Sub::value_type* end = sub->end();
                    if (r == end || r->first > 0x6c)
                        r = end;
                    found = (SubNode*)r->second;
                }
                if (found) {
                    if (SubCheck(found))
                        return (CCharString*)&found->name;
                }
            }
            node = node->left;
        } while (node != self->treehdr);
    }
    return &g_defaultQuestName;
}