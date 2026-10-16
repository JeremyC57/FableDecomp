#include <cstdio>
// Behaviour model of CTCTavernGameCardBlackjack::ReorderCards (0x007DA810): every card of every hand gets a deal sound and is moved
// to the position for (hand index, card index).
int main()
{
    int handSizes[3] = { 2, 0, 3 };
    int sounds = 0, moves = 0, lastHand = -1, lastCard = -1, sum = 0;
    long handIndex = 0;
    for (int* h = handSizes; h != handSizes + 3; ++handIndex, ++h)
        for (unsigned int i = 0; i < (unsigned int)*h; ++i) {
            sounds++; moves++; lastHand = (int)handIndex; lastCard = (int)i; sum += (int)handIndex * 10 + (int)i;
        }
    if (sounds != 5 || moves != 5 || lastHand != 2 || lastCard != 2 || sum != 0 + 1 + 20 + 21 + 22) { std::printf("007da810_TEST FAIL\n"); return 1; }
    std::printf("007da810_TEST PASS\n");
    return 0;
}