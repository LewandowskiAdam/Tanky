#include "application.hpp"

UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  (void)byte_pool;
  return ret;
}

void MX_ThreadX_Init(void) {
    tx_kernel_enter();
}