// CDrawGuildSeal::DisplayNextMessage() -- HEAD DRAFT (first two blocks of 1182 bytes): clear the alpha, return on an empty
// Grade: DIFFER(263v1182) 345 diff lines (skeleton 1v1182/393): head draft of the first two blocks (queue-empty return, timer start, quest-core OBJECTIVE_UPDATE sound); branch targets line up with retail (jne 0x64aac7); the message display and x87 alpha tail are not drafted (retry wave 2 sec2c)
// message queue, start the display timer (2 s) and play the guild-seal OBJECTIVE_UPDATE sound when the front message is the
// quest-core one and the hero is alive with a sound player; then wait for the timer. The message display / x87 alpha tail
// (0x0064AAD5..0x0064AE5C) is not drafted.
// Leaf: DisplayNextMessage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064A9C8, 1182 bytes; /Os ebp frame (push 10h / pop ebx idiom).
#pragma optimize("s", on)
class CCharString { public: void* PStringData; CCharString(const char*, long = -1); ~CCharString(); };
class CTCBase;
struct CVectorMapTC { struct value_type { long first; CTCBase* second; }; value_type* _M_start; value_type* _M_finish;
    value_type* LowerBound(const long& key); };                          // 0x0040F020
class CTCBitset { public: unsigned long W[9]; bool test(long pos) const; };   // 0x00410DE0
class CThing { public: unsigned char _pad00[0x20]; CTCBitset UsingComponents; CVectorMapTC Components; };
class CThingPlayerCreature : public CThing { };
bool __fastcall IS_THING_ALIVE(CThingPlayerCreature* const& thing);      // 0x0043DBD0
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };               // 0x00449970
class CTCSoundPlayer { public: unsigned long PlaySound(const CCharString&, long, long, long, long); };   // 0x0066BD00
struct SStringMapNode { unsigned char _pad[0x14]; long Value; };
class CStringMap { public: SStringMapNode* Header; SStringMapNode* find(const CCharString& key); };   // 0x0042B467
class CPlayerGuiDef { public: unsigned char _pad[0x934]; CStringMap SoundMap; };
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };   // 0x00438570
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
extern long g_FramesPerSecond;                                           // 0x01375550
struct SGuildSealMessage { long Type; long SoundID; };
struct CMessageQueue {                                                   // deque<SGuildSealMessage*> iterators
    SGuildSealMessage** StartCur; void* StartFirst; void* StartLast; void* StartNode;
    SGuildSealMessage** FinishCur; void* FinishFirst; void* FinishLast; void* FinishNode;
    bool empty() const { return FinishCur == StartCur; }
    SGuildSealMessage* front() const { return *StartCur; }
};
class CDrawGuildSeal {
public:
    unsigned char _pad00[8];
    CPlayerManager* PPlayerManager;                                      // +0x08
    unsigned char _pad0c[0x60 - 0x0c];
    long Alpha;                                                          // +0x60
    long DisplayEndFrame;                                                // +0x64
    CMessageQueue Messages;                                              // +0x68
    void DisplayNextMessage();
};
void CDrawGuildSeal::DisplayNextMessage()
{
    Alpha = 0;
    CMessageQueue* queue = &Messages;
    if (queue->empty())
        return;
    if (DisplayEndFrame == 0) {
        DisplayEndFrame = CWorld::GetFrame() + g_FramesPerSecond * 2;
        CStringMap* sounds = &const_cast<CPlayerGuiDef&>(CPlayerGui::PeekGuiDef()).SoundMap;
        SStringMapNode* it = sounds->find(CCharString("HUD_ORB_QUEST_CORE"));
        if (it != sounds->Header && it->Value == queue->front()->SoundID) {
            CThingPlayerCreature* hero = PPlayerManager->GetMainPlayer().GetPControlledCreature();
            if (IS_THING_ALIVE(hero) && hero->UsingComponents.test(0x10)) {
                long key = 0x10;
                CVectorMapTC::value_type* entry = hero->Components.LowerBound(key);
                if (entry == hero->Components._M_finish || key < entry->first)
                    entry = hero->Components._M_finish;
                ((CTCSoundPlayer*)entry->second)->PlaySound(CCharString("OBJECTIVE_UPDATE"), 1, 0, 0, -1);
            }
        }
    }
    if (CWorld::GetFrame() <= DisplayEndFrame)
        return;
}
