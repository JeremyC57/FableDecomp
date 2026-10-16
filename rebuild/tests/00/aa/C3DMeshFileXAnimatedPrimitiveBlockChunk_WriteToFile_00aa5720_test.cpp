#include <cstdio>
struct Out { unsigned int words[32]; int nw; Out() : nw(0) {} void WriteULong(unsigned int v) { words[nw++] = v; } };
struct W { unsigned int BoneIndex, Weight; };
int main() {
    unsigned int groups[2] = { 7, 8 }; unsigned int verts[1] = { 5 }; W w[2] = { { 1, 100 }, { 2, 155 } };
    unsigned int bonesPerVertex = 2; Out o;
    o.WriteULong(2); for (unsigned int i = 0; i < 2; ++i) o.WriteULong(groups[i]);
    o.WriteULong(bonesPerVertex); o.WriteULong(1); o.WriteULong(verts[0]);
    for (unsigned int k = 0; k < bonesPerVertex * 1; ++k) { o.WriteULong(w[k].BoneIndex); o.WriteULong((unsigned int)(long)(float)w[k].Weight); }
    if (o.nw != 10 || o.words[2] != 8 || o.words[9] != 155 || o.words[3] != 2) { std::printf("00aa5720_TEST FAIL\n"); return 1; }
    std::printf("00aa5720_TEST PASS\n"); return 0; }