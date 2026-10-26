// Actual GFSerialiseMapBinaryOut<CHeroStatIncreaseData,EDamageAttribute>, not manifest WriteToStream.
// PDB0058CFB0 / fablewin01F0B3F0. ECX outputstream, EDX const outer map. Leaf: WriteStatIncreaseMap
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};
#include "engine/_quarantine/CHeroExperienceDef.h"
#include <stdlib.h>
struct OutputMethods:CDataOutputStream {OutputMethods();}; //defaultPDBAFBE40; WriteSLONG993EB0/PDBAFBF50
// Real enum keys; native storage four bytes. Values and container layouts from CHeroExperienceDef/CHeroStatIncreaseData donor.
#ifdef _MSC_VER
enum EDamageAttributeABI {DamageUnknown=0};enum EHeroExperienceStatCategoryABI {ExperienceUnknown=0};
#else
// VC7.1 enum ABI is signed32; GCC chooses unsigned for a partial enum declaration.
typedef fable_i32 EDamageAttributeABI;typedef fable_i32 EHeroExperienceStatCategoryABI;const EHeroExperienceStatCategoryABI ExperienceUnknown=0;
#endif
struct InnerNode {unsigned char Colour;InnerNode*Parent,*Left,*Right;EHeroExperienceStatCategoryABI Key;float Value;};
struct InnerMap {InnerNode*Head;unsigned long Count;unsigned char Allocator[4];
 InnerMap():Head(0){Head=(InnerNode*)malloc(sizeof(InnerNode));Count=0;Head->Colour=0;Head->Parent=0;Head->Left=Head;Head->Right=Head;}
 void Erase(InnerNode*); //native4DA252 genuine STLport recursive _M_erase, frees right-subtrees then left spine
 ~InnerMap(){if(Count){Erase(Head->Parent);Head->Left=Head;Head->Parent=0;Head->Right=Head;Count=0;}if(Head)free(Head);}
};
struct HeroStatIncreaseData {InnerMap StatIncreases;}; //implicit genuine default constructs map, native12 vs donor16
struct StatTraits {HeroStatIncreaseData DefaultValue;void TransferBinaryOut(OutputMethods&,const HeroStatIncreaseData&);}; //native6DB0F0/PDB58AD90, void thiscall(stream&,constdata&), inner enum keys WriteSLONG and float WriteFloat993EE0
struct OuterNode {unsigned char Colour;OuterNode*Parent,*Left,*Right;EDamageAttributeABI Key;HeroStatIncreaseData Value;};
struct OuterMap {OuterNode*Head;unsigned long Count;unsigned char Allocator[4];};
static __forceinline OuterNode* NextNode(OuterNode*x){if(x->Right){x=x->Right;while(x->Left)x=x->Left;}else{OuterNode*y=x->Parent;while(x==y->Right){x=y;y=y->Parent;}if(x->Right!=y)x=y;}return x;}
void __fastcall WriteStatIncreaseMap(OutputMethods&stream,const OuterMap&map){StatTraits traits;stream.WriteSLONG(map.Count);for(OuterNode*i=map.Head->Left;i!=map.Head;i=NextNode(i)){stream.WriteSLONG(i->Key);traits.TransferBinaryOut(stream,i->Value);}}