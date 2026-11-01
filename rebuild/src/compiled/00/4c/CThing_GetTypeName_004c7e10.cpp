// Naming wave 18: the global stand-in g_thingTypeManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
struct CThingTypeEntry { int a,b,c,d; char name[4]; };   // 0x14 bytes
class CThingManager { public: int pad; CThingTypeEntry *entries; };
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
private: public: char pad[0x10]; unsigned char typeIndex; };
char * __fastcall GetTypeName(CThing *self)
{
    unsigned int idx = self->typeIndex;
    return CThing::GetPCurrentThingManager()->entries[idx].name;
}