// Naming wave 18: the global stand-in g_typeTable_4c7e30 (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Leaf: ?IsLocal@CThing@@QBE_NXZ
// NF238 (2026-11-22, q12670): renamed from the propagated catalog label CThing::GetTypeName (self leaf ?GetTypeName@CThing@@QAEDXZ) to
//   CThing::IsLocal, const, bool. Evidence: (1) Ego_r.pdb thing.obj puts ?GetTypeName@CThing@@QBEABVCCharString@@XZ at 0x004C9820 and
//   ?IsLocal@CThing@@QBE_NXZ right after it at 0x004C9840 (both 27 bytes); retail has GetTypeName at 0x004C7E10 (engine/CThing.h) and this
//   body 0x20 after it; FableWin.pdb thing.obj has the same pair (0x01CF8970 / 0x01CF89D0). (2) The bodies: retail 0x004C7E10 returns the
//   address of the CCharString at +0x10 of the 20-byte thing-type record indexed by the type byte at this+0x10; this body reads the byte
//   at +9 of the same record into al (a bool). (3) Both landed binders, NThingMap::CThingMapManager::PlaceThing 0x00637FC0 and
//   DestroyMapwhoNode 0x00637A10, call it as ?IsLocal@CThing@@QBE_NXZ. In-place respell of the HEAD local CThing model (same class token).
#include "rebuild_abi.h"
// CThing::IsLocal @ 0x004c7e30
// idx = byte[this+0x10]; table = (*g).[+4]; return (bool)byte[table + idx*20 + 9]
class CThingManager { public: char pad[4]; char *m_data; };
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
private: public: char pad[0x10]; unsigned char m_type; bool IsLocal() const; };
bool CThing::IsLocal() const {
    unsigned idx = m_type;
    char *t = CThing::GetPCurrentThingManager()->m_data;
    return *reinterpret_cast<const bool*>(t + idx*20 + 9);
}
