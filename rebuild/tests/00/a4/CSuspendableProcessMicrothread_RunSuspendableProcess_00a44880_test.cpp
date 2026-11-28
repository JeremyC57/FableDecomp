#include <vector>
#include <stdio.h>
static int failures, ticks, suspensions, switches, max_inject;
static int order[64];
static void check(bool expr) { if (!expr) ++failures; }
class CASuspendableProcess {
public:
    CASuspendableProcess() : TimeProcessSliceStarted(-1.0f) { }
    static CASuspendableProcess* PActiveProcess;
    float TimeProcessSliceStarted;
};
class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    CSuspendableProcessMicrothread(int id=0) : number(id) { }
    void RunSuspendableProcess();
    static std::vector<CSuspendableProcessMicrothread*> ProcessStack;
    static void InternalSuspend();
    static void InternalSwitchToProcess(CSuspendableProcessMicrothread&);
    int number;
};
CASuspendableProcess* CASuspendableProcess::PActiveProcess=0;
std::vector<CSuspendableProcessMicrothread*> CSuspendableProcessMicrothread::ProcessStack;
static CSuspendableProcessMicrothread jobs[12];
static CSuspendableProcessMicrothread* outer;
double GFGetTime() { ++ticks; return 10.0+ticks*0.5; }
void CSuspendableProcessMicrothread::InternalSuspend() {
    ++suspensions;
    check(PActiveProcess==outer && ProcessStack.size()>=2);
    check(ProcessStack[ProcessStack.size()-2]==outer);
    PActiveProcess=0;
}
void CSuspendableProcessMicrothread::InternalSwitchToProcess(CSuspendableProcessMicrothread& next) {
    check(!PActiveProcess && ticks==switches+1 && switches<64);
    check(outer->TimeProcessSliceStarted==(float)(10.0+ticks*0.5));
    order[switches++]=next.number;
    if(max_inject>0) { --max_inject;ProcessStack.push_back(&jobs[max_inject+1]); }
}

void CSuspendableProcessMicrothread::RunSuspendableProcess()
{
    if (PActiveProcess != 0) {
        CASuspendableProcess* pold_process = PActiveProcess;
        ProcessStack.push_back(static_cast<CSuspendableProcessMicrothread*>(PActiveProcess));
        ProcessStack.push_back(this);
        InternalSuspend();
        PActiveProcess = pold_process;
    } else {
        ProcessStack.push_back(this);
        while (!ProcessStack.empty()) {
            CSuspendableProcessMicrothread* process = ProcessStack.back();
            ProcessStack.pop_back();
            TimeProcessSliceStarted = (float)GFGetTime();
            InternalSwitchToProcess(*process);
        }
    }
}
int main(){
    int i;for(i=0;i<12;++i)jobs[i].number=i;
    for(int buffered=0;buffered<=4;++buffered)for(int count=0;count<=4;++count){
        outer=&jobs[0];suspensions=switches=ticks=0;
        CSuspendableProcessMicrothread::ProcessStack.clear();
        CASuspendableProcess::PActiveProcess=0;
        for(i=0;i<buffered;++i)CSuspendableProcessMicrothread::ProcessStack.push_back(&jobs[7+i]);
        max_inject=count;
        outer->RunSuspendableProcess();
        check(!suspensions && ticks==1+count+buffered && switches==ticks && order[0]==0);
        for(i=0;i<count;i++)check(order[i+1]==count-i);
        for(i=0;i<buffered;i++)check(order[1+count+i]==7+buffered-1-i);
        check(CSuspendableProcessMicrothread::ProcessStack.empty());
        CASuspendableProcess::PActiveProcess=outer;
        for(i=0;i<buffered;++i)CSuspendableProcessMicrothread::ProcessStack.push_back(&jobs[7+i]);
        CSuspendableProcessMicrothread& other=jobs[count+1];
        other.RunSuspendableProcess();
        check(CASuspendableProcess::PActiveProcess==outer && suspensions==1 && switches==ticks);
        check(CSuspendableProcessMicrothread::ProcessStack.size()==(unsigned int)buffered+2);
        check(CSuspendableProcessMicrothread::ProcessStack.back()==&other);
    }
    if(failures){printf("failed %d\n",failures);return 1;}
    puts("00a44880_TEST PASS");return 0;
}
