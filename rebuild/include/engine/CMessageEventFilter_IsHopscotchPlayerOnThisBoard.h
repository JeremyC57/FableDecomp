#ifndef FABLE_ENGINE_HOPSCOTCH_PLAYER_FILTER_H
#define FABLE_ENGINE_HOPSCOTCH_PLAYER_FILTER_H
#include "engine/CMessageEvent.h"
#include "engine/EMessageEventType.h"
class CMessageEventFilter_IsHopscotchPlayerOnThisBoard {
public:
    CMessageEventFilter_IsHopscotchPlayerOnThisBoard(const CThing& board) : HopscotchUID(board.GetUniqueID()) {}
    bool operator()(const CMessageEvent& event) const {
        if (event.Type == MESSAGE_EVENT_HOPSCOTCH_PLAYING) return event.TargetUID == HopscotchUID;
        return false;
    }
private:
    unsigned __int64 HopscotchUID;
};
#endif
