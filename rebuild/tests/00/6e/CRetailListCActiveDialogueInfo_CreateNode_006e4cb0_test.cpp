#include <cstdio>
#include <cstdlib>
#include <new>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; } void SetPItem(CBaseObject* item) { m_pItem = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); } };
struct CActiveDialogueInfo { CIntelligentPointer ThingSpeaking; unsigned long SoundID; long Layer; unsigned char IsLayerHead; long OutputGain; };
struct CRetailList_CActiveDialogueInfo { struct node { node* next; node* prev; CActiveDialogueInfo value; }; node* head; node* CreateNode(const CActiveDialogueInfo& value); };
CRetailList_CActiveDialogueInfo::node* CRetailList_CActiveDialogueInfo::CreateNode(const CActiveDialogueInfo& value)
{ node* p = (node*)std::malloc(sizeof(node)); new (&p->value) CActiveDialogueInfo(value); return p; }
int main()
{
    CBaseObject thing; CActiveDialogueInfo info; info.ThingSpeaking.SetPItem(&thing); info.SoundID = 5; info.Layer = 2; info.IsLayerHead = 1; info.OutputGain = 0x3f800000;
    CRetailList_CActiveDialogueInfo list; list.head = 0;
    CRetailList_CActiveDialogueInfo::node* n = list.CreateNode(info);
    if (n == 0 || sizeof(*n) != 0x20 || sizeof(CActiveDialogueInfo) != 0x18 || n->value.ThingSpeaking.GetPItem() != &thing || n->value.SoundID != 5 || n->value.Layer != 2
        || n->value.IsLayerHead != 1 || n->value.OutputGain != 0x3f800000 || (char*)&n->value - (char*)n != 8) { std::printf("006e4cb0_TEST FAIL\n"); return 1; }
    std::free(n);
    std::printf("006e4cb0_TEST PASS\n");
    return 0;
}