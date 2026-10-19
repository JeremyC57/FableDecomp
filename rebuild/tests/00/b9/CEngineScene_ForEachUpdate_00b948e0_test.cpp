// CEngineScene::ForEachUpdate -- visit nested scene maps in sorted order and Update each. Retail00b948e0 complete179bytes. No scene/scene-map headers exist. VC6 tree node colour+0,parent+4,left+8,right+c,key+10,value+14. Scene MapList tree header+4; inner header at outer node+14. Callee00b947e0 CEngineSceneMap::Update (verified body uses GridLevels+18).
struct CEngineSceneMap {void Update();}; //00b947e0
struct CMapNode {unsigned long Colour;CMapNode *Parent,*Left,*Right;long Key;CEngineSceneMap* Data;void* Info;};
struct CMapTree {CMapNode* Head;unsigned long Count;};
struct COuterNode {unsigned long Colour;COuterNode *Parent,*Left,*Right;long Key;CMapTree Maps;};
template<class T> static T* Next(T* node){
 if(node->Right){node=node->Right;while(node->Left)node=node->Left;}
 else {T* parent=node->Parent;while(node==parent->Right){node=parent;parent=parent->Parent;}if(node->Right!=parent)node=parent;}
 return node;
}
struct CEngineScene {unsigned long Base;COuterNode* MapListHead;unsigned long MapListCount;void ForEachUpdate();};
void CEngineScene::ForEachUpdate(){
 for(COuterNode* outer=MapListHead->Left;outer!=MapListHead;outer=Next(outer))
  for(CMapNode* inner=outer->Maps.Head->Left;inner!=outer->Maps.Head;inner=Next(inner))inner->Data->Update();
}
#include <cstdio>
#include <cstddef>
static CEngineSceneMap maps[49];static int seen[80],count;
void CEngineSceneMap::Update(){seen[count++]=int(this-maps);}
template<class T> static void empty(T& h){h.Parent=0;h.Left=h.Right=&h;}
template<class T> static void insert(T& h,T& n,long key){n.Key=key;n.Left=n.Right=0;if(!h.Parent){h.Parent=&n;n.Parent=&h;h.Left=h.Right=&n;return;}T* p=h.Parent;for(;;){if(key<p->Key){if(p->Left)p=p->Left;else{p->Left=&n;n.Parent=p;break;}}else{if(p->Right)p=p->Right;else{p->Right=&n;n.Parent=p;break;}}}if(key<h.Left->Key)h.Left=&n;if(key>h.Right->Key)h.Right=&n;}
static int deepTrees(){
 CEngineScene scene; COuterNode head,outer[7]; CMapNode heads[7],inner[49];
 const int order[7]={3,1,5,0,2,4,6}; empty(head); scene.MapListHead=&head;
 for(int a=0;a<7;++a){empty(heads[a]);outer[a].Maps.Head=&heads[a];
  for(int b=0;b<7;++b){int key=order[b],idx=a*7+key;inner[idx].Data=&maps[idx];insert(heads[a],inner[idx],key);}}
 for(int a=0;a<7;++a)insert(head,outer[order[a]],order[a]);
 count=0;scene.ForEachUpdate();int failures=count!=49;
 for(int a=0;a<count;++a)if(seen[a]!=a)++failures;
 return failures;
}
int main(){int fail=deepTrees();CEngineScene scene;COuterNode oh,on[3];CMapNode ih[3],in[9];empty(oh);scene.MapListHead=&oh;count=0;scene.ForEachUpdate();if(count)++fail;
 for(int shape=0;shape<3;++shape){empty(oh);for(int a=0;a<3;++a){empty(ih[a]);on[a].Maps.Head=&ih[a];for(int b=0;b<3;++b){int idx=a*3+b;in[idx].Data=&maps[idx];insert(ih[a],in[idx],b);} }
 int order[3];order[0]=shape==0?1:(shape==1?0:2);order[1]=shape==0?0:1;order[2]=shape==0?2:(shape==1?2:0);
 for(int a=0;a<3;++a)insert(oh,on[order[a]],order[a]);count=0;scene.ForEachUpdate();if(count!=9)++fail;for(int a=0;a<count;++a)if(seen[a]!=a)++fail;
 empty(ih[1]);count=0;scene.ForEachUpdate();if(count!=6)++fail;for(int a=0;a<count;++a)if(seen[a]!=(a<3?a:a+3))++fail;
 }
#ifdef _MSC_VER
 if(offsetof(COuterNode,Maps)!=0x14||offsetof(CMapNode,Data)!=0x14||offsetof(CEngineScene,MapListHead)!=4)++fail;
#endif
 if(fail){std::printf("00b948e0_TEST FAIL %d\n",fail);return 1;}std::printf("00b948e0_TEST PASS\n");return 0;}