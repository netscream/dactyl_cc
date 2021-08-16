//#include <hal_i2c_lld.h>

#include "quantum.h"
#include "split_util.h"
#include "transport.h"

//#include "lagrange.h"

bool transport_master(matrix_row_t master_matrix[], matrix_row_t slave_matrix[])
{
    //print("Transport Master!\n");
    return false;
}

void transport_slave(matrix_row_t master_matrix[], matrix_row_t slave_matrix[])
{

}

void transport_master_init(void)
{
    print("Initialize master transport\n");
    i2c_lld_init();
}

void transport_slave_init(void)
{
    print("Initialize slave transport\n");
}
