// Behaviour test for 0x006E37D0 NSpeechGainManager::Update: with lines active, the first line is flagged as layer head
// (only it), each line is updated with the frames elapsed since the last update, finished lines are unlinked and their
// layer count drops; the last update frame always becomes the current frame.
#include <cstdio>
#include <cstdlib>

struct Info { int Layer; bool IsLayerHead; int updates; bool finishAfter; long lastElapsed; };
struct Node { Node* next; Node* prev; Info data; };
static Node* g_head;
static short g_counts[3];
static long g_lastFrame, g_frame;

static bool UpdateDialogueElement(Info& info, long elapsed) { ++info.updates; info.lastElapsed = elapsed; return !info.finishAfter; }

static void Update()
{
    if (g_head->next != g_head) {
        long elapsed = g_frame - g_lastFrame;
        Node* it = g_head->next; Node* next = it;
        while (it != g_head) {
            next = next->next;
            it->data.IsLayerHead = it == g_head->next;
            if (!UpdateDialogueElement(it->data, elapsed)) {
                --g_counts[it->data.Layer];
                Node* n = it->next; Node* p = it->prev; p->next = n; n->prev = p; std::free(it); next = n;
            }
            it = next;
        }
    }
    g_lastFrame = g_frame;
}

static Node* Push(int layer, bool finish)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->data.Layer = layer; n->data.IsLayerHead = false; n->data.updates = 0; n->data.finishAfter = finish; n->data.lastElapsed = 0;
    n->prev = g_head->prev; n->next = g_head; g_head->prev->next = n; g_head->prev = n; ++g_counts[layer];
    return n;
}

#define FAIL { std::printf("006e37d0_TEST FAIL\n"); return 1; }
int main()
{
    Node sentinel; sentinel.next = sentinel.prev = &sentinel; g_head = &sentinel;
    g_frame = 10; g_lastFrame = 4; Update();
    if (g_lastFrame != 10) FAIL;
    Node* a = Push(0, true); Node* b = Push(1, false); Node* c = Push(1, false);
    g_frame = 13; Update();
    if (g_counts[0] != 0 || g_counts[1] != 2 || g_head->next != b || b->next != c || c->next != g_head) FAIL;
    if (!b->data.IsLayerHead || c->data.IsLayerHead || b->data.lastElapsed != 3 || c->data.updates != 1 || g_lastFrame != 13) FAIL;
    (void)a;
    std::free(b); std::free(c);
    std::printf("006e37d0_TEST PASS\n");
    return 0;
}