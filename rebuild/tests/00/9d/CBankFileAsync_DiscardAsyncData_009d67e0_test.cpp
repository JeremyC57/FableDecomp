// Behaviour test for 009d67e0: CBankFileAsync::DiscardAsyncData -- finished: clear the restart flag; unfinished and not abortable: queue; always release + process.
#include <cstdio>
struct TData { bool m_finished, m_completionPending, abortable; int aborts;
    bool Abort() { ++aborts; return abortable; }
    bool CancelIfFinished() { if (m_finished) { m_completionPending = false; return true; } return false; } };
struct TPtr { TData* Data; int* count; void Reset() { if (count) --*count; count = 0; Data = 0; } };
struct TAsync {
    TData* queued[4]; int nQueued; int processed;
    TAsync() : nQueued(0), processed(0) {}
    void DiscardAsyncData(TPtr& data)
    {
        TData* p = data.Data;
        if (p) {
            if (!p->CancelIfFinished() && !data.Data->Abort()) queued[nQueued++] = data.Data;
            data.Reset();
        }
        ++processed;
    }
};
int main()
{
    int refs = 2;
    TData fin = { true, true, false, 0 }; TData busy = { false, true, false, 0 }; TData done = { false, true, true, 0 };
    TAsync a;
    TPtr p1 = { &fin, &refs }; a.DiscardAsyncData(p1);
    if (fin.m_completionPending || fin.aborts || a.nQueued || p1.Data || refs != 1) { std::printf("009d67e0_TEST FAIL\n"); return 1; }
    TPtr p2 = { &busy, &refs }; a.DiscardAsyncData(p2);
    if (a.nQueued != 1 || a.queued[0] != &busy || busy.aborts != 1 || refs != 0) { std::printf("009d67e0_TEST FAIL\n"); return 1; }
    TPtr p3 = { &done, 0 }; a.DiscardAsyncData(p3);
    TPtr p4 = { 0, 0 }; a.DiscardAsyncData(p4);
    if (a.nQueued != 1 || done.aborts != 1 || a.processed != 4) { std::printf("009d67e0_TEST FAIL\n"); return 1; }
    std::printf("009d67e0_TEST PASS\n");
    return 0;
}