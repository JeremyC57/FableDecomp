#include <cstdio>
#include <cstring>
// behaviour test: UploadPlayerScores -- in task state 100/101 it opens the request with the score string and returns busy
// (1001) if the open reports busy; otherwise it steps the transaction: success (1000) with a decrypted reply whose first
// line is "SID:OK" returns busy (1001) and keeps the connection; any other success drops the connection and returns 1000;
// non-success results pass through
struct Dummy {
    int TaskState; int OpenResult; int TransactionResult; int LineCount; const char* FirstLine;
    bool HttpOpen; char Sent[32];
    int Open(const char* scores) { std::strcpy(Sent, scores); return OpenResult; }
    int UploadPlayerScores()
    {
        if (TaskState == 100 || TaskState == 101) {
            bool busy = Open("scores") == 1001;
            if (busy) return 1001;
        }
        int result = TransactionResult;
        if (result == 1000) {
            if (LineCount > 0 && std::strcmp(FirstLine, "SID:OK") == 0) return 1001;
            HttpOpen = false;
        }
        return result;
    }
};
int main()
{
    Dummy d; std::memset(&d, 0, sizeof d);
    d.TaskState = 100; d.OpenResult = 1001; d.HttpOpen = true; d.TransactionResult = 1000;
    bool ok = d.UploadPlayerScores() == 1001 && std::strcmp(d.Sent, "scores") == 0 && d.HttpOpen;
    d.OpenResult = 1000; d.LineCount = 1; d.FirstLine = "SID:OK";
    ok = ok && d.UploadPlayerScores() == 1001 && d.HttpOpen;
    d.FirstLine = "SID:BAD";
    ok = ok && d.UploadPlayerScores() == 1000 && !d.HttpOpen;
    d.HttpOpen = true; d.TaskState = 104; d.Sent[0] = 0; d.TransactionResult = 1002;
    ok = ok && d.UploadPlayerScores() == 1002 && d.HttpOpen && d.Sent[0] == 0;
    d.TransactionResult = 1000; d.LineCount = 0;
    ok = ok && d.UploadPlayerScores() == 1000 && !d.HttpOpen;
    if (!ok) { std::printf("00561450_TEST FAIL\n"); return 1; }
    std::printf("00561450_TEST PASS\n");
    return 0;
}
