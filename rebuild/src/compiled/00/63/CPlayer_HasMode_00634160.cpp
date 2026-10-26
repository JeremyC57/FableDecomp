// CPlayer::HasMode(EPlayerMode mode) const -- whether the mode is on the player's mode list (retail +0x210)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634160, length 39 bytes.
// Identity: catalog label CPlayer::HasMode; PDB `PlayerModes` list<EPlayerMode> (PDB +0x210, 12 bytes;
// the retail list is the 4-byte head pointer, which is why every later CPlayer member sits 8 bytes
// lower: AggressiveMode +0x217, JoystickDeviceNumber +0x228 in this chunk).
// Callees: none.
// Shape: an inlined linear find over the list nodes (next +0, prev +4, value +8) with the head
// (end) kept in ecx, then `found != end` as a bool through a full register (`xor edx,edx ; setne dl
// ; mov al,dl`), `ret 4`. Retail-offset view of CPlayer (engine/CPlayer.h has the PDB layout, which
// disagrees from +0x210 on).
enum EPlayerMode { PLAYER_MODE_NULL = 0 };

struct SModeNode {                                 // list node
    SModeNode*  m_pNext;                           // +0x00
    SModeNode*  m_pPrev;                           // +0x04
    EPlayerMode m_value;                           // +0x08
};

class CPlayer { public:
    unsigned char _pad00[0x210];
    SModeNode*    PlayerModes;                     // +0x210 list<EPlayerMode> head (the sentinel)
    bool HasMode(EPlayerMode mode) const;
};

bool CPlayer::HasMode(EPlayerMode mode) const
{
    SModeNode* head = PlayerModes;
    SModeNode* node = head->m_pNext;
    while (node != head && node->m_value != mode)
        node = node->m_pNext;
    return node != head;
}