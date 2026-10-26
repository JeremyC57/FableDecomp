#pragma optimize("s",on)

void* __cdecl memmove(void* destination, const void* source, unsigned int count);
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71)

void* __fastcall Copy_MemoryStepped(
    void* begin, void* end, void* destination, void* unused)
{
    if (end != begin) {
        unsigned int count = (unsigned char*)end - (unsigned char*)begin;
        destination = memmove(destination, begin, count);
        destination = (unsigned char*)destination + count;
    }
    return destination;
}

void* __fastcall CActiveFile_CopyRange(
    void* begin, void* end, void* destination, void* unused)
{
    if (end != begin) {
        unsigned int count = (unsigned char*)end - (unsigned char*)begin;
        destination = memmove(destination, begin, count);
        destination = (unsigned char*)destination + count;
    }
    return destination;
}

struct DoubleListNode
{
    DoubleListNode* previous;
    DoubleListNode* next;
    unsigned long value;
};

DoubleListNode** __fastcall Std_DoubleLinkedList_CreateNode(
    DoubleListNode** output, void* unused_edx, void* unused_stack)
{
    *output = 0;
    DoubleListNode* node = (DoubleListNode*)malloc(12);
    node->previous = node;
    node->next = node;
    *output = node;
    return output;
}