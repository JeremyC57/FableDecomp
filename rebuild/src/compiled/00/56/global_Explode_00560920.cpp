// _global::Explode(std::string& input, const std::string& token, std::vector<std::string>& ret) -- appends every piece of input before a token to ret; returns ret's size
// Flags: /Gr
// Leaf: ?Explode@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00560920, 653 bytes, __fastcall (ecx = input, edx = token, [esp+4] = ret), `ret 4`. Grade: RELOCATION_MATCH.
// Identity: Ego_r.pdb / FableWin.pdb ?Explode@?A0xf010c0d0@@YIHAAV?$basic_string@D..@std@@ABV23@AAV?$vector@..@Z (anonymous namespace;
//   the PDBs spell Dinkumware's char_traits as a struct, retail STLport 4.6 as a class). Ego_d.pdb locals input, token, ret, startpos, findpos, { newstr }.
// Callees: search<const char*,const char*,_Eq_traits> 0x00567120, __uninitialized_copy<string> 0x00568E70, basic_string::_M_range_initialize<char*>
//   0x00567D00, _String_base::_M_throw_out_of_range 0x00565360 / _M_throw_length_error 0x00565A30, malloc / memmove / free / puts / abort.
//   push_back expands _M_insert_overflow in place.
// TU: the scoreboard crypto TU (Ego_d.pdb line-table file id 2880; retail order EncipherMorsel 0x0055E040, DecipherMorsel 0x0055E180,
//   CryptoMatic 0x0055E2C0, Explode 0x00560920, GetDecryptedDocument 0x00560BB0). All five rows share this define block and /Gr: the
//   retail TU is a __fastcall-default one (the anonymous-namespace helpers and the STLport instances it calls -- search 0x00567120,
//   __uninitialized_copy 0x00568E70 -- are __fastcall without a spelling; Explode grades DIFFER(472v653) without /Gr).
// Text: the landed 0x00563480 file's same-TU text with Ego_d.pdb's names; the morsels' declarations split over two lines as Ego_d's
//   line table has them (lines 83 / 84, 95 / 96). See the batch README.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_SEARCH
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_EXPLODE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#define FABLE_RETAIL_LHHTTP2_DOCUMENT
#include "engine/CTCHeroOnlineScoreboard.h"
#include "stlport/fable_string_bodies.h"

void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*) throw();

namespace {
int FABLE_FASTCALL Explode(std::string& input,const std::string& token,std::vector<std::string>& ret)
{
    unsigned int startpos=0;
    unsigned int findpos=0;
    do {
        findpos=input.find(token,startpos);
        if(findpos!=std::string::npos) {
            std::string newstr=input.substr(startpos,findpos-startpos);
            ret.push_back(newstr);
            startpos=findpos+token.length();
        }
    } while(findpos!=std::string::npos);
    return ret.size();
}
}
