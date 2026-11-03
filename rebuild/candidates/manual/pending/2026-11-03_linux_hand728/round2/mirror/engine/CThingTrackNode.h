#ifndef FABLE_ENGINE_CTHINGTRACKNODE_H
#define FABLE_ENGINE_CTHINGTRACKNODE_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// NextNode, PreviousNode, NextNodeUID, PreviousNodeUID, TrackName, IsStart, IsEnd; native allocation527360.
class CThingTrackNode:public CThingGameObject {
private:
    union { CThingTrackNode* NextNode; long NextNodeThingFileID; }; // +d0, ctor837ca7
    union { CThingTrackNode* PreviousNode; long PreviousNodeThingFileID; }; // +d4, ctor837cad
    unsigned __int64 NextNodeUID; // +d8, ctor837cb3/837cb9
    unsigned __int64 PreviousNodeUID; // +e0, ctor837cbf/837cc5
    CCharString TrackName; // +e8, ctor837c94
    bool IsStart; // +ec, ctor837ccb
    bool IsEnd; // +ed, ctor837cd1
    unsigned char _pad_0xee[2];
public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingTrackNode)==0xf0);
#endif
