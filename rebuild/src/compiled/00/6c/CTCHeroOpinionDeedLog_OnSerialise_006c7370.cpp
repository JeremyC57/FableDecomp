// CTCHeroOpinionDeedLog::OnSerialise(CPersistContext& context, bool in_game) -- save or restore the hero's deed log: the next handle, the two crime switches, the crime village and the opinion-deed map (flattened into a vector of pairs to save, re-inserted pair by pair on load)
// Flags: none
// Leaf: ?OnSerialise@CTCHeroOpinionDeedLog@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C7370, 342 bytes, `ret 8` (8-byte aligned frame); CTCHeroOpinionDeedLog vtable
//   0x0125EA48 slot 1 (RTTI .?AVCTCHeroOpinionDeedLog@@; the CTCBase OnSerialise slot, Ego_r.pdb ?OnSerialise@CTCHeroOpinionDeedLog@@
//   UAEXAAVCPersistContext@@_N@Z 0x006FEAA0). Every transfer name is the PDB member it addresses.
// Callees: CPersistContext::Transfer<long> @ 0x00410BE0 ("NextHandle", default 0), Transfer<bool> @ 0x004045C0 ("WeaponOutCrimeEnabled"
//   default true, "GuardsIgnoreCrimes" default false), Transfer<CIntelligentPointer<CThing> >(const char*, T&) @ 0x006C9C60
//   ("VillageForCrimes"), vector<pair<long, COpinionDeed*> >::reserve @ 0x006C8560 (FableWin.pdb spelling), the STLport input-iterator
//   __copy(first, last, back_insert_iterator, const input_iterator_tag&, int*) @ 0x006C9B70 (__fastcall; reached through the inline
//   std::copy and __copy_aux), TransferVector<pair<long, COpinionDeed*> >(const char*, vector<T>&) @ 0x006C9EE0 (the named vector transfer: Ego_r.pdb / FableWin.pdb spell all 40 named vector transfers ??$TransferVector@..QAEXPBDAAV?$vector@, none ??$Transfer@) ("OpinionDeedMap"), the map's
//   insert_unique(const value_type&) @ 0x006C8400, free @ 0x00BFEA14.
// Layout: engine/CTCHeroOpinionDeedLog.h (DEEDS + COMPLETE: Ego_r.pdb members at their retail STLport offsets).
// Shape: a save (mode 1 or 3) flattens the map into a reserved local vector before the transfer; a load transfers into an empty local
//   vector and inserts each pair into the map.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_ONSERIALISE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_COPY_GENERIC_INLINE
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
#define FABLE_RETAIL_STLP_COPY_INPUT_NOINLINE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_TREE_ITERATOR_TRAITS
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CTCBase.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CPersistContext.h"

void CTCHeroOpinionDeedLog::OnSerialise(CPersistContext& context, bool in_game)
{
    context.Transfer("NextHandle", NextHandle, 0L);
    context.Transfer("WeaponOutCrimeEnabled", WeaponOutCrimeEnabled, true);
    context.Transfer("GuardsIgnoreCrimes", GuardsIgnoreCrimes, false);
    context.Transfer("VillageForCrimes", VillageForCrimes);
    if (context.IsSaving())
    {
        std::vector<std::pair<long, COpinionDeed*> > deeds;
        deeds.reserve(OpinionDeedMap.size());
        std::copy(OpinionDeedMap.begin(), OpinionDeedMap.end(), std::back_inserter(deeds));
        context.TransferVector("OpinionDeedMap", deeds);
    }
    else
    {
        std::vector<std::pair<long, COpinionDeed*> > deeds;
        context.TransferVector("OpinionDeedMap", deeds);
        std::vector<std::pair<long, COpinionDeed*> >::iterator end = deeds.end();
        for (std::vector<std::pair<long, COpinionDeed*> >::iterator it = deeds.begin(); it != end; ++it)
            OpinionDeedMap.insert(*it);
    }
}
