# Chunk 903 lessons
- A hidden-result function that only default-constructs its result re-reads the result pointer from [esp+4] after the constructor call only under `#pragma optimize("s", on)`; /Ot parks it in esi (004dec09).
- A plain `switch` on a member compiles to `sub eax,0 ; je ; dec eax ; jne` at /Ot (0061b530); the shared zero register form (`xor edx,edx ; sub eax,edx`) is the /Os variant (chunk 901, 00581ab3).
