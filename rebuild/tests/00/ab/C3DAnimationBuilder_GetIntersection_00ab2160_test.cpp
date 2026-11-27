#include <vector>
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_ASSERT(x) ((void)0)
template<class T> class CArray:public std::vector<T>{
public:CArray(){} CArray(unsigned int n,const T& v):std::vector<T>(n,v){}
 T& At(unsigned int n){return (*this)[n];}};
template<class T>struct CCountedPointer{T* ptr;CCountedPointer(T* p=0):ptr(p){}T* operator->()const{return ptr;}T& operator*()const{return *ptr;}T* Get()const{return ptr;}};
struct C3DAnimFileSequenceChunk{int parent,sets;long GetParentIndex()const{return parent;}void SetParentIndex(long idx){++sets;parent=idx;}};
class C3DAnimationBuilder {public:
 struct CBoneInfo{CCountedPointer<CBoneInfo> AnimSource;unsigned long Index;bool ExtensionChild,ExtensionParent;};CBoneInfo bones[2][8];unsigned int calls,returned;
 typedef CArray<CCountedPointer<C3DAnimFileSequenceChunk> > Seq;
 CCountedPointer<CBoneInfo> BuildBoneInfoTree(const Seq& anim,CArray<CCountedPointer<CBoneInfo> >& out){int pass=calls++;for(unsigned int i=0;i<anim.size();++i)out.push_back(&bones[pass][i]);return &bones[pass][0];}
 unsigned long AssignAnimation(CBoneInfo& root,const CCountedPointer<CBoneInfo>& other,bool flag){if(&root!=bones[0]||other.Get()!=bones[1]||!flag)exit(1);return returned;}
 unsigned long GetIntersection(Seq&,Seq&);};
unsigned long C3DAnimationBuilder::GetIntersection(CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& anim1,CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& anim2)
{
    CArray<CCountedPointer<C3DAnimFileSequenceChunk> > new_anim1,new_anim2;
    CArray<CCountedPointer<CBoneInfo> > bone_list1,bone_list2;
    CCountedPointer<CBoneInfo> tree1=BuildBoneInfoTree(anim1,bone_list1);
    CCountedPointer<CBoneInfo> tree2=BuildBoneInfoTree(anim2,bone_list2);
    unsigned long matches=AssignAnimation(*tree1,tree2,true);
    CArray<long> new_indices(anim1.size(),-1);
    for(unsigned long c_bone=0;c_bone<anim1.size();++c_bone) {
        CBoneInfo& bone1=*bone_list1.At(c_bone);
        bool extension=bone1.ExtensionChild||bone1.ExtensionParent;
        if(bone1.AnimSource.Get()||extension) {
            CCountedPointer<C3DAnimFileSequenceChunk> sequence1=anim1[c_bone];
            long parent_index=sequence1->GetParentIndex();
            if(parent_index!=-1) {
                parent_index=new_indices.At(parent_index);
                FABLE_RETAIL_ASSERT(parent_index>=0);
                sequence1->SetParentIndex(parent_index);
            }
            sequence1->SetParentIndex(parent_index);
            new_indices[c_bone]=new_anim1.size();
            new_anim1.push_back(sequence1);
            if(extension) {
                CCountedPointer<C3DAnimFileSequenceChunk> null_sequence;
                new_anim2.push_back(null_sequence);
            } else {
                CBoneInfo& bone2=*bone1.AnimSource;
                CCountedPointer<C3DAnimFileSequenceChunk> sequence2=anim2[bone2.Index];
                new_anim2.push_back(sequence2);
            }
        }
    }
    anim1=new_anim1;
    anim2=new_anim2;
    return matches;
}
int main(){
 for(unsigned int num=0;num<=7;++num)for(unsigned int matched=0;matched<128;++matched)for(unsigned int flags=0;flags<8;++flags){
 C3DAnimationBuilder b;b.calls=0;b.returned=num*19+matched;C3DAnimationBuilder::Seq a,c;C3DAnimFileSequenceChunk f[2][8];unsigned int count=0,expected[8],mapped[8];int parents[8];
 for(unsigned int i=0;i<num;++i){
 b.bones[0][i].ExtensionChild=(i==flags&&i%2==0);b.bones[0][i].ExtensionParent=(i==flags&&i%2!=0);b.bones[0][i].AnimSource=(matched&(1<<i))?&b.bones[1][num-1-i]:0;b.bones[1][i].Index=i;
 parents[i]=i?int((i-1)/2):-1;f[0][i].parent=parents[i];f[0][i].sets=0;f[1][i].parent=17;f[1][i].sets=0;
 a.push_back(&f[0][i]);c.push_back(&f[1][i]);mapped[i]=~0u;if(i==flags||(matched&(1<<i))){mapped[i]=count;expected[count++]=i;}
 }
 if(b.GetIntersection(a,c)!=b.returned||a.size()!=count||c.size()!=count||b.calls!=2)return 1;
 for(unsigned int k=0;k<count;++k){unsigned int i=expected[k];int parent=parents[i]<0?-1:int(mapped[parents[i]]);
 if(a[k].Get()!=&f[0][i]||a[k]->parent!=parent||a[k]->sets!=(parents[i]<0?1:2))return 2;
 if(c[k].Get()!=(i==flags?0:&f[1][num-1-i]))return 3;}
 for(unsigned int i=0;i<num;++i)if(f[1][i].sets||(mapped[i]==~0u&&f[0][i].sets))return 4;
 }puts("00ab2160_TEST PASS");return 0;}