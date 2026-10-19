#include <cstdio>
struct Node { int side[4]; Node(){ for(int i=0;i<4;i++) side[i]=-1; } };
static void setup(Node* n, int which, int leaf) {
    if(which==0){ n[1].side[2]=leaf; n[3].side[2]=leaf; }
    else if(which==1){ n[0].side[3]=leaf; n[2].side[3]=leaf; }
    else if(which==2){ n[0].side[1]=leaf; n[1].side[1]=leaf; }
    else if(which==3){ n[2].side[0]=leaf; n[3].side[0]=leaf; }
}
int main(){ Node n[4]; setup(n,0,7); setup(n,1,8); setup(n,2,9); setup(n,3,10);
 if(n[1].side[2]!=7||n[3].side[2]!=7||n[0].side[3]!=8||n[2].side[3]!=8||n[0].side[1]!=9||n[1].side[1]!=9||n[2].side[0]!=10||n[3].side[0]!=10){std::printf("00a75f10_TEST FAIL\n");return 1;} std::printf("00a75f10_TEST PASS\n");return 0;}
