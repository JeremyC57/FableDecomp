// CAIStateGroupBase::AIAction_WaitForMessage(EMessageEventType, CThingCreatureBase*, long) -- yield until stop, a matching event, or strict timeout; retail 0x0097d360, 293 bytes. Callees: IsStopping 0x008fd010, GetPWorld 0x008fcfa0, GetGameTick 0x0049d870 (global 0x013b89bc), Yield through CAIBrain slot 2, originator filter constructor 0x006af290, type predicate 0x006af1c0, FindMostRecentMessage 0x008d46d0.
#include <new>
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"

#if defined(_MSC_VER)
#define FABLE_NATIVE_THISCALL __fastcall
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_NATIVE_THISCALL
#define FABLE_FASTCALL
#endif

enum EMessageEventType {
    E_MESSAGE_EVENT_TYPE_OPAQUE = 0,
    E_MESSAGE_EVENT_TYPE_TEST = 7,
    E_MESSAGE_EVENT_TYPE_OTHER = 8,
    E_MESSAGE_EVENT_TYPE_OTHER2 = 9
};
enum EAIActionReturn {
    AI_ACTION_RETURN_ZERO = 0,
    AI_ACTION_RETURN_ONE = 1,
    AI_ACTION_RETURN_TWO = 2,
    AI_ACTION_RETURN_THREE = 3
};

struct CMessageEvent {
    fable_u8 _header[8];
    fable_i32 Type;                                            // +0x08
    fable_u8 _opaque_0c[0x1c];
    fable_u32 OriginatorUID[2];                                // +0x28,+0x2c
    fable_u8 _opaque_0x30[0x28];
};
struct SMessageNode {
    SMessageNode* next;
    SMessageNode* prev;
    CMessageEvent Event;                                       // +0x08
};
struct CMessageEventFilter_IsOfType {
    fable_i32 Type;
    bool operator()(CMessageEvent* event) const;               // 0x006af1c0
};
struct CMessageEventFilter_IsOriginatedByThing {
    fable_u32 UID[2];
    CMessageEventFilter_IsOriginatedByThing(const CThingCreatureBase* thing); // 0x006af290
    bool operator()(CMessageEvent* event) const;
};
struct COwnMessageFilter {
    const CMessageEventFilter_IsOfType* TypeFilter;
    const CMessageEventFilter_IsOriginatedByThing* OriginatorFilter;

};
struct CMessageEventManager {
    CWorld* World;
    SMessageNode* Messages;                                    // +0x04 sentinel
    CMessageEvent* FindMostRecentMessage(const COwnMessageFilter& filter); // 0x008d46d0
};

typedef void (FABLE_NATIVE_THISCALL *CAIBrainYieldFunction)(CAIBrain*);
long FABLE_FASTCALL GetGameTick();                        // 0x0049d870, global 0x013b89bc

struct CAIStateGroupBaseAccess : CAIStateGroupBase {
    bool IsStopping();                                          // 0x008fd010
    CWorld* GetPWorld();                                        // 0x008fcfa0
    EAIActionReturn AIAction_WaitForMessage(EMessageEventType messageType,
                                             CThingCreatureBase* originator,
                                             long timeout);
};

EAIActionReturn CAIStateGroupBaseAccess::AIAction_WaitForMessage(
    EMessageEventType messageType, CThingCreatureBase* originator, long timeout)
{
    if (IsStopping())
        return AI_ACTION_RETURN_ONE;

    GetPWorld();
    long deadline = GetGameTick() + timeout;
    while ((GetPWorld(), GetGameTick()) <= deadline) {
        CAIBrain* brain = PBrain;
        void** slots = (void**)brain->__vftable;
        ((CAIBrainYieldFunction)slots[2])(brain);
        if (IsStopping())
            return AI_ACTION_RETURN_ONE;

        if (originator) {
            CMessageEventFilter_IsOfType typeFilter = { (fable_i32)messageType };
            COwnMessageFilter filter;
            filter.TypeFilter = &typeFilter;
            // The native constructor returns its stack object in EAX. Placement
            // construction in aligned automatic storage expresses that result in
            // valid C++98; the filter is trivial and dies before the next iteration.
            union OriginatorStorage {
                fable_u32 Alignment;
                char Bytes[sizeof(CMessageEventFilter_IsOriginatedByThing)];
            } originatorStorage;
            filter.OriginatorFilter = new (originatorStorage.Bytes) CMessageEventFilter_IsOriginatedByThing(originator);
            CMessageEventManager* boundManager = GetPWorld()->PMessageEventManager_Data;
            if (boundManager->FindMostRecentMessage(filter))
                return AI_ACTION_RETURN_TWO;
        } else {
            CMessageEventFilter_IsOfType typeFilter = { (fable_i32)messageType };
            CMessageEventManager* manager = GetPWorld()->PMessageEventManager_Data;
            CMessageEvent* found = 0;
            SMessageNode* head = manager->Messages;
            SMessageNode* node = head->next;
            while (node != head) {
                CMessageEvent* event = &node->Event;
                if (typeFilter(event)) {
                    found = event;
                    break;
                }
                node = node->next;
                head = manager->Messages;
            }
            if (found) return AI_ACTION_RETURN_TWO;
        }

    }
    return AI_ACTION_RETURN_THREE;
}

#if defined(_MSC_VER)
typedef char fable_abi_assert_base_brain[(offsetof(CAIStateGroupBase, PBrain) == 0x04) ? 1 : -1];
typedef char fable_abi_assert_brain_size[(sizeof(CAIBrain) == 0xb0) ? 1 : -1];
typedef char fable_abi_assert_creature_message_id[(offsetof(CThingCreatureBase, LastMessageEventICreatedID) == 0xe8) ? 1 : -1];
typedef char fable_abi_assert_world_manager[(offsetof(CWorld, PMessageEventManager_Data) == 0x60) ? 1 : -1];
typedef char fable_abi_assert_event_type[(offsetof(CMessageEvent, Type) == 0x08) ? 1 : -1];
typedef char fable_abi_assert_event_originator[(offsetof(CMessageEvent, OriginatorUID) == 0x28) ? 1 : -1];
typedef char fable_abi_assert_node_event[(offsetof(SMessageNode, Event) == 0x08) ? 1 : -1];
typedef char fable_abi_assert_manager_size[(sizeof(CMessageEventManager) == 0x08) ? 1 : -1];
#endif