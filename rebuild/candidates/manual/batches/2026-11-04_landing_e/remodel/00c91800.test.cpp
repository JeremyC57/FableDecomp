// Behaviour test for libogg oggpack_get_buffer (0x00C91800): returns the packer's buffer pointer unchanged.
#include <cstdio>
typedef struct { long endbyte; int endbit; unsigned char *buffer; unsigned char *ptr; long storage; } oggpack_buffer;
unsigned char *oggpack_get_buffer(oggpack_buffer *b){
  return(b->buffer);
}
int main(){
  unsigned char data[8]; oggpack_buffer b; int ok=1;
  b.endbyte=3; b.endbit=5; b.buffer=data; b.ptr=data+3; b.storage=8;
  if(oggpack_get_buffer(&b)!=data) ok=0;
  b.buffer=0; if(oggpack_get_buffer(&b)!=0) ok=0;
  printf("00c91800_TEST %s\n", ok ? "PASS" : "FAIL");
  return ok ? 0 : 1;
}
