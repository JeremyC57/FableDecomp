// CLipSyncSampleData::WriteToFile(CDataOutputStream& os) const -- write the phoneme symbol table, the frame rate and every frame's phoneme ids and byte weights
// Leaf: ?WriteToFile@CLipSyncSampleData@@QBEXAAVCDataOutputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A48BA0, 875 bytes, `ret 4`, no frame pointer (`sub esp, 2Ch`), no EH frame.
// Retail-asm: 0x00A48E25 `fld dword ptr [esp+20h]` + 0x00A48E29 `fistp dword ptr [esp+10h]` -- the engine's inline-assembly
//   float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h, FABLE_PORTABLE twin there): the product is stored to
//   the helper's by-value parameter slot (0x00A48E21 `fstp dword ptr [esp+20h]`), reloaded and stored with a bare dword fistp. This
//   row carries no asm of its own.
// Identity: Ego_r.pdb / Ego_d.pdb / FableWin.pdb ?WriteToFile@CLipSyncSampleData@@QBEXAAVCDataOutputStream@@@Z (the shared header's
//   declaration; the sibling of the landed ReadFromFile 0x00A495F0, same original TU lib_speech_analyser). Parameter and locals
//   exactly as Ego_d.pdb (debug length 1651): os; phoneme_map, itr; { c_frame { frame { c_phoneme { phoneme } } } }; { phoneme_id };
//   { c_frame { frame, phoneme_count { c_phoneme { phoneme, byte_weight } } } }.
// Shape: Ego_d.pdb's C13 line table (ref/00a48ba0/egod_lines.tsv, source lines 92..130; Ego_d is a VC10 /Od build with `this` at
//   [ebp-0D8h]: +3 per `this` use, +1 per unsigned char argument, +4 / +7 per EH state store, +5 on every `for` line, checked
//   iterators; lines 109 and 119 are debug-only asserts of 349 bytes each):
//   94 the map (15 = our 8 + EH state); 95 / 115 the frame loops (47 = our 35 + this + long jump + 5); 97 / 117 the frame reference
//   (18 = our 15 + this); 98 the first phoneme loop, the count read in the condition (40 v our 31); 100 / 123 the phoneme copy
//   (25 = our 21 + EH state); 101 the map assignment through GetID() / GetSymbol() (83 v our 51: two EH states, the VC10 map);
//   104 the symbol count (17 = ours); 105 the iterator declared on its own line (12 = our 8 + EH state); 106 the iterator loop;
//   108 phoneme_id (13 = ours); 110 its byte (13 = our 12 + 1); 111 the string (20 = ours); 113 the frame rate, converted to the
//   unsigned long parameter (66 v our 20: VC10 converts float to unsigned in line; retail is `fld ; call __ftol2`, 0x00A48D5D);
//   114 the frame count (20 = our 17 + this); 118 phoneme_count (11 = ours); 120 its byte (13 = 12 + 1); 121 the second phoneme
//   loop on the local count (39 v our 26); 124 the id byte (17 = ours); 125 byte_weight through GFFloatToLongNear (38 v our 26:
//   float argument, float return spill); 126 the limit, ONE line of 31 bytes = the if / else-if text below exactly (measured:
//   GFLimit(byte_weight, 0, 255) is 29 at /Od and gives the same retail bytes); 127 the weight byte (13 = 12 + 1).
//   Same retail bytes either way, text chosen by the simpler form: `++itr` v `itr++`, the implicit float conversion v a (long) cast.
// Inline helpers expanded by retail: STLport map constructor / begin / end / size / iterator ++ / destructor with one level of the
//   recursive tree erase in place (0x00A48EB0..0x00A48ED5; the sibling ReadFromFile calls the erase, so the shared map header keeps
//   it noinline for FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ and this row opts out with the NEW overlay opt-in
//   FABLE_RETAIL_STLP_TREE_MERASE_PASS22_INLINE), CArray size / operator[], CLipSyncFrameData::GetPhonemeCount / GetPhoneme,
//   CLipSyncPhoneme::GetID / GetWeight / GetSymbol (NEW overlay opt-in FABLE_RETAIL_CLIPSYNCPHONEME_GETSYMBOL_INLINE), the implicit
//   CLipSyncPhoneme copy constructor / destructor.
// Callees: 0x00BFEA0E malloc (the tree header), 0x0099EC30 CCharString copy constructor (three sites), 0x00A4A670
//   map<unsigned long, CCharString>::operator[] (landed), 0x0099EFB0 CCharString::operator=, 0x0099EAE0 CCharString destructor (four
//   sites), 0x00993EA0 CDataOutputStream::WriteULONG (three), 0x00993E50 WriteUBYTE (four), 0x00995AE0 WriteNullTerminatedString,
//   0x00BFEA70 __ftol2, 0x00630D90 the tree's _M_erase (the right subtree), 0x00BFEA14 free (the nodes, the header).
// Globals: 0x01230014 the float 255.0.
// The insert-body opt-ins (FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY, FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY) are the sibling
//   TU's: they only give this object the operator[] COMDAT's callee body (the row's own bytes are the same without them).
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CLIPSYNCPHONEME_GETSYMBOL_INLINE
#define FABLE_RETAIL_STLP_TREE_MERASE_PASS22_INLINE
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITEUBYTE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include "fable_string.h"
#include "engine/CDataOutputStream.h"
#include "engine/CLipSyncSampleData.h"
#include "stlport/fable_map.h"
#include "engine/GFFloatToLongNear.h"

void CLipSyncSampleData::WriteToFile(CDataOutputStream& os) const
{
    std::map<unsigned long, CCharString> phoneme_map;
    for (unsigned long c_frame = 0; c_frame < FrameData.size(); c_frame++)
    {
        const CLipSyncFrameData& frame = FrameData[c_frame];
        for (unsigned long c_phoneme = 0; c_phoneme < frame.GetPhonemeCount(); c_phoneme++)
        {
            CLipSyncPhoneme phoneme = frame.GetPhoneme(c_phoneme);
            phoneme_map[phoneme.GetID()] = phoneme.GetSymbol();
        }
    }
    os.WriteULONG(phoneme_map.size());
    std::map<unsigned long, CCharString>::iterator itr;
    for (itr = phoneme_map.begin(); itr != phoneme_map.end(); ++itr)
    {
        unsigned long phoneme_id = itr->first;
        os.WriteUBYTE(phoneme_id);
        os.WriteNullTerminatedString(itr->second);
    }
    os.WriteULONG(FramesPerSecond);
    os.WriteULONG(FrameData.size());
    for (unsigned long c_frame = 0; c_frame < FrameData.size(); c_frame++)
    {
        const CLipSyncFrameData& frame = FrameData[c_frame];
        unsigned long phoneme_count = frame.GetPhonemeCount();
        os.WriteUBYTE(phoneme_count);
        for (unsigned long c_phoneme = 0; c_phoneme < phoneme_count; c_phoneme++)
        {
            CLipSyncPhoneme phoneme = frame.GetPhoneme(c_phoneme);
            os.WriteUBYTE(phoneme.GetID());
            long byte_weight = GFFloatToLongNear(phoneme.GetWeight() * 255.0f);
            if (byte_weight < 0) byte_weight = 0; else if (byte_weight > 255) byte_weight = 255;
            os.WriteUBYTE(byte_weight);
        }
    }
}