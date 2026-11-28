#include <cassert>
#include <cstdio>

int main()
{
    const long main_number = 2;
    const long player_numbers[] = {0, 1, 2, 3};
    for (unsigned int i = 0; i != 4; ++i) {
        const bool is_main = player_numbers[i] == main_number;
        assert(is_main == (i == 2));
    }
    std::printf("00487520_TEST PASS\n");
    return 0;
}
