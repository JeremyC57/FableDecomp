#include <stdio.h>

class CContainmentVolume { public:
  int dummy0;
  char* data;
};

static void RelocateModel(CContainmentVolume* volume, void* oldBase, void* newBase) {
  if (volume->data) {
    volume->data = volume->data - (char*)oldBase + (char*)newBase;
  }
}

int main() {
  char buf[64];
  char buf2[64];
  CContainmentVolume v;
  v.dummy0 = 123;
  // non-null branch
  v.data = buf + 10;
  RelocateModel(&v, buf, buf2);
  if (v.data != buf2 + 10) { printf("FAIL reloc\n"); return 1; }
  // null branch: unchanged
  v.data = 0;
  RelocateModel(&v, buf, buf2);
  if (v.data != 0) { printf("FAIL null\n"); return 2; }
  printf("OK_00a755a0\n");
  return 0;
}