// CThingManager::PeekPThingWithUID(unsigned __int64 uid) const @ 0x0051ED60 -- the thing mapped to `uid`, or 0 (the const twin of
//   GetPThingWithUID 0x0051ED80)
// Leaf: ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z
// Naming wave 30 (family c699): the own row is Ego_r.pdb / FableWin.pdb ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z (donor order
//   PeekPThingWithUID 0x00528FD0 -> GetPThingWithUID 0x00529000 = retail 0x0051ED60 -> 0x0051ED80). Was the free __fastcall stand-in
//   "CStopWatch::GetTicks" (?GetTicks@@YIIPAUCStopWatch@@PAX_J@Z).
// Shape: thiscall, the 8-byte by-value uid => ret 8; its stack slot is reused as the find iterator's scratch. UIDToThingMap (+0x58,
//   Ego_r map<unsigned __int64, CThing*>) is searched by its find 0x00527D80 (the row's stand-in spelling, as in the twin row), the
//   returned node compared with the head sentinel, node->second (+0x18) returned, else 0.
class CThing;

struct Node {
    Node* left;         // +0
    char  pad4[0x14];
    CThing* value;      // +0x18
};

struct Map {
    Node* head;         // +0
    Node* Find(void* iterOut) const;   // 0x00527D80
};

class CThingManager { public:
    char pad[0x58];
    Map  themap;        // +0x58 UIDToThingMap
    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;
};

const CThing* CThingManager::PeekPThingWithUID(unsigned __int64 uid) const
{
    const Map* m = &this->themap;
    Node* n = m->Find(&uid);
    if (n != m->head)
        return n->value;
    return 0;
}