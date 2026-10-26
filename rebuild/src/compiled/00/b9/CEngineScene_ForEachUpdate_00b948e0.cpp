// CEngineScene::ForEachUpdate -- visit nested scene maps in sorted order and Update each. Retail00b948e0 complete179bytes. No scene/scene-map headers exist. VC6 tree node colour+0,parent+4,left+8,right+c,key+10,value+14. Scene MapList tree header+4; inner header at outer node+14. Callee00b947e0 CEngineSceneMap::Update (verified body uses GridLevels+18).
class CEngineSceneMap { public:void Update();}; //00b947e0
struct CMapNode {unsigned long Colour;CMapNode *Parent,*Left,*Right;long Key;CEngineSceneMap* Data;void* Info;};
struct CMapTree {CMapNode* Head;unsigned long Count;};
struct COuterNode {unsigned long Colour;COuterNode *Parent,*Left,*Right;long Key;CMapTree Maps;};
template<class T> static T* Next(T* node){
 if(node->Right){node=node->Right;while(node->Left)node=node->Left;}
 else {T* parent=node->Parent;while(node==parent->Right){node=parent;parent=parent->Parent;}if(node->Right!=parent)node=parent;}
 return node;
}
class CEngineScene { public:unsigned long Base;COuterNode* MapListHead;unsigned long MapListCount;void ForEachUpdate();};
void CEngineScene::ForEachUpdate(){
 for(COuterNode* outer=MapListHead->Left;outer!=MapListHead;outer=Next(outer))
  for(CMapNode* inner=outer->Maps.Head->Left;inner!=outer->Maps.Head;inner=Next(inner))inner->Data->Update();
}