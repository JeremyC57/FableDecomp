// Behaviour test for the mailbox queue PostMessage (00c26cd0): lock, append the message at end() through insert(P, 1, x), release the semaphore once.
#include <stdio.h>
static int g_locks = 0, g_unlocks = 0, g_releases = 0;
struct Msg { long id; };
struct Vec {
    Msg* data[8]; unsigned int n;
    Msg** begin() { return data; }
    Msg** end() { return data + n; }
    void insert(Msg** p, unsigned int count, Msg* const& x) { unsigned int at = (unsigned int)(p - data); for (unsigned int i = n; i > at; --i) data[i + count - 1] = data[i - 1]; for (unsigned int k = 0; k < count; ++k) data[at + k] = x; n += count; }
    Msg** insert(Msg** p, Msg* const& x) { unsigned int off = (unsigned int)(p - begin()); insert(p, 1, x); return begin() + off; }
};
struct Queue;
struct Lock { Queue* q; Lock(Queue* qq) : q(qq) { ++g_locks; } ~Lock() { ++g_unlocks; } };
struct Queue {
    Vec pending;
    void Release() { ++g_releases; }
    void PostMessage(Msg* m) { Lock lock(this); pending.insert(pending.end(), m); Release(); }
};
int main()
{
    Queue q; q.pending.n = 0; Msg a = { 1 }, b = { 2 };
    q.PostMessage(&a); q.PostMessage(&b);
    if (q.pending.n != 2 || q.pending.data[0] != &a || q.pending.data[1] != &b || g_locks != 2 || g_unlocks != 2 || g_releases != 2)
    { printf("00c26cd0_TEST FAIL\n"); return 1; }
    printf("00c26cd0_TEST PASS\n"); return 0;
}