// CEnginePrimitiveManagerRepeatedStaticMeshes::ClipSubSectionsSIMD(const NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement*, unsigned long, CClipListElement*&) -- clip four lanes of a subsection tree and append visible runs.
// Grade: RELOCATION_MATCH (retail_asm) --664/664bytes,200instructions,exact self-relocation; native/portable fullbody and IEEE tests pass.
// Retail: Fable.exe00B98E10..00B990A8exclusive,664bytes,ret0xC; selfcallB9907B is recursion.
// Leaf: ?ClipSubSectionsSIMD@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXPBUCSubsectionElement@NEnginePrimitiveManagerRepeatedStaticMeshes@@KAAPAUCClipListElement@1@@Z
// Flags: none
// Retail-asm: 0x00B98E5B..0x00B99026, original 459-byte hand-SSE kernel (`movaps` / `mulps` / `movmskps`) in shared PDB-named macro; 30 C++ / 10 intrinsic site spellings recorded.
#define FABLE_RETAIL_PASS22_REPEATED_MESH_CLIP_SIMD
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include <string.h>
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshesClipSubSectionsSIMD.h"

void CEnginePrimitiveManagerRepeatedStaticMeshes::ClipSubSectionsSIMD(const NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement* element,unsigned long clip_flags,CClipListElement*& clip_list)
{
    unsigned long reject_mask=0;
    if(!element->ObjectCount[0])reject_mask=1;
    if(!element->ObjectCount[1])reject_mask|=2;
    if(!element->ObjectCount[2])reject_mask|=4;
    if(!element->ObjectCount[3])reject_mask|=8;
    unsigned long sub_clip_flags=clip_flags;
    FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_CLIPSUBSECTIONSSIMD

    if(reject_mask!=15) {
        for(long i=0;i<4;++i) {
            if(!(reject_mask&(1UL<<i))) {
                unsigned long pass_mask=0x111110UL<<i;
                if(element->SubSectionOffset[i] && (sub_clip_flags&pass_mask)!=pass_mask)
                    ClipSubSectionsSIMD(element+element->SubSectionOffset[i],((sub_clip_flags>>i)&0x111110UL)*15,clip_list);
                else {
                    clip_list->Start=element->StartIndex[i];
                    clip_list->Size=element->ObjectCount[i];
                    ++clip_list;
                }
            }
        }
    }
}