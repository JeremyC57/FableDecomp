// CTCAIScratchpad::OnSerialise(CPersistContext& context, bool) -- persists the five static gossip tables when the parent thing is the hero: the two flag maps through TransferMap, then each category -> phrases map as a count, a key per entry and a phrase vector per key
// Leaf: ?OnSerialise@CTCAIScratchpad@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ABBD0, 2828 bytes, thiscall `ret 8`, virtual: CTCAIScratchpad vtable 0x0126D07C slot 1
//   (the CTCBase OnSerialise slot; CTCHeroOnlineScoreboard::OnSerialise 0x00560720 is slot 1 of its vtable too).
// Identity: Ego_r.pdb ?OnSerialise@CTCAIScratchpad@@MAEXAAVCPersistContext@@_N@Z (donor 0x0054BB30, right after the ctor 0x0054BAE0
//   as retail 0x007ABBD0 follows the ctor 0x007ABB80); FableWin.pdb 0x01E81B80 (protected virtual). The catalog's vfunc_1 is a placeholder.
// Statics (Ego_r.pdb `dynamic initializer for 'CTCAIScratchpad::...'`, FableWin ?GetGossip@CTCAIScratchpad@@SIABV?$map@VCCharString@@V?$vector@...):
//   ActiveGossipCategories 0x013BAE2C, IsGossipForPlayer 0x013BAE38 (map<CCharString, bool>), Gossip 0x013BAE44, GossipVillages
//   0x013BAE50, GossipFactions 0x013BAE5C (map<CCharString, vector<CCharString> >; STLport tree: header +0, node count +4).
// Callees: CPersistContext::TransferMap<bool, CCharString>(name, map&, const bool&) 0x007AE3D0, Transfer<long> 0x00410BE0,
//   Transfer<CCharString> 0x004109A0, TransferVector<CCharString> 0x0049B8D0, map<CCharString, vector<CCharString> >::operator[]
//   0x007AEBE0, GFIntToCharString 0x0099F830, operator+(const CCharString&, const CCharString&) 0x0099F570, CCharString members
//   (default 0x0099E4B0, (const char*, long) 0x0099EBF0, copy 0x0099EC30, operator const char* 0x0099E4C0, dtor 0x0099EAE0).
// Shape: the hero test reads ParentThing->UsingComponents bit TCI_HERO_STATS (`test byte [thing+20h],10h`). Every name is rebuilt
//   from its literal at each use ("Gossip" + "_num_vectors", "Gossip" + i, "_" + "Gossip" + i); the two-argument Transfer<T> is
//   inline (GetDefaultVal<T>: 0 for long; a CCharString local copied out for CCharString).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "stlport/fable_map.h"
#include "engine/CPersistContext.h"

CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

template <> inline long __fastcall CPersistContext::GetDefaultVal<long>() { return 0; }
template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }

class CTCAIScratchpad : public CTCBase {
public:
    typedef std::map<CCharString, bool> CFlagMap;
    typedef std::map<CCharString, std::vector<CCharString> > CGossipMap;
    static CFlagMap ActiveGossipCategories;                                            // 0x013BAE2C
    static CFlagMap IsGossipForPlayer;                                                 // 0x013BAE38
    static CGossipMap Gossip;                                                          // 0x013BAE44
    static CGossipMap GossipVillages;                                                  // 0x013BAE50
    static CGossipMap GossipFactions;                                                  // 0x013BAE5C
protected:
    virtual void OnSerialise(CPersistContext& context, bool);
};

void CTCAIScratchpad::OnSerialise(CPersistContext& context, bool)
{
    if (GetParentThing().HasTC(TCI_HERO_STATS)) {
        long num;
        context.TransferMap("ActiveGossipCategories", ActiveGossipCategories, false);
        context.TransferMap("IsGossipForPlayer", IsGossipForPlayer, false);
        if (context.IsSaving()) {
            {
                num = Gossip.size();
                context.Transfer(CCharString("Gossip") + CCharString("_num_vectors"), num);
                long i = 0;
                for (CGossipMap::iterator it = Gossip.begin(); it != Gossip.end(); ++it, ++i) {
                    CCharString key = (*it).first;
                    context.Transfer(CCharString("Gossip") + GFIntToCharString(i), key);
                    context.TransferVector(CCharString("_") + CCharString("Gossip") + GFIntToCharString(i), (*it).second);
                }
            }
            {
                num = GossipVillages.size();
                context.Transfer(CCharString("GossipVillages") + CCharString("_num_vectors"), num);
                long i = 0;
                for (CGossipMap::iterator it = GossipVillages.begin(); it != GossipVillages.end(); ++it, ++i) {
                    CCharString key = (*it).first;
                    context.Transfer(CCharString("GossipVillages") + GFIntToCharString(i), key);
                    context.TransferVector(CCharString("_") + CCharString("GossipVillages") + GFIntToCharString(i), (*it).second);
                }
            }
            {
                num = GossipFactions.size();
                context.Transfer(CCharString("GossipFactions") + CCharString("_num_vectors"), num);
                long i = 0;
                for (CGossipMap::iterator it = GossipFactions.begin(); it != GossipFactions.end(); ++it, ++i) {
                    CCharString key = (*it).first;
                    context.Transfer(CCharString("GossipFactions") + GFIntToCharString(i), key);
                    context.TransferVector(CCharString("_") + CCharString("GossipFactions") + GFIntToCharString(i), (*it).second);
                }
            }
        }
        else {
            {
                context.Transfer(CCharString("Gossip") + CCharString("_num_vectors"), num);
                for (long i = 0; i < num; ++i) {
                    CCharString key;
                    context.Transfer(CCharString("Gossip") + GFIntToCharString(i), key);
                    context.TransferVector(CCharString("_") + CCharString("Gossip") + GFIntToCharString(i), Gossip[key]);
                }
            }
            {
                context.Transfer(CCharString("GossipVillages") + CCharString("_num_vectors"), num);
                for (long i = 0; i < num; ++i) {
                    CCharString key;
                    context.Transfer(CCharString("GossipVillages") + GFIntToCharString(i), key);
                    context.TransferVector(CCharString("_") + CCharString("GossipVillages") + GFIntToCharString(i), GossipVillages[key]);
                }
            }
            {
                context.Transfer(CCharString("GossipFactions") + CCharString("_num_vectors"), num);
                for (long i = 0; i < num; ++i) {
                    CCharString key;
                    context.Transfer(CCharString("GossipFactions") + GFIntToCharString(i), key);
                    context.TransferVector(CCharString("_") + CCharString("GossipFactions") + GFIntToCharString(i), GossipFactions[key]);
                }
            }
        }
    }
}